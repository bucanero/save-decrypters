/*
*
*	Tomb Raider VI Remaster (PS4) Save Unpacker - (c) 2026 by Bucanero - www.bucanero.com.ar
*
* Based on TRR-SaveMaster by JulianOzelRose
* Information about the custom LZW compression method:
*   - https://github.com/JulianOzelRose/TRR-SaveMaster/blob/master/TRR-SaveMaster/TR6Utilities.cs
*/

#include "../common/iofile.c"

// Constants
#define BASE_SAVEGAME_OFFSET_TR6      0x293C04
#define SAVEGAME_SIZE                 0xA470
#define MAX_SAVEGAMES                 32
#define COMPRESSED_BLOCK_START_OFFSET 0x368
#define COMPRESSED_BLOCK_SIZE_OFFSET  0x360

// LZW Constants
#define MAX_BITS        12
#define INIT_BITS       9
#define MAX_CODE        ((1U << MAX_BITS) - 1)
#define CLEAR_CODE      0x100
#define FIRST_CODE      0x101
#define HASH_SIZE       0x1400
#define MAX_BUFFER_SIZE 0x40000 // 256 KB for decompressed data (adjust as needed)
#define LZW_BUFFER_SIZE 0x1000  // 4096 entries


// Helper macro function to write bits
#define WriteBits(code, width) do { \
    bitBuffer |= ((uint64_t)code << bitCount); \
    bitCount += width; \
    bitTotal += width; \
    while (bitCount >= 8) { \
        if (destPos >= MAX_BUFFER_SIZE) { \
            printf("[PACK] Destination buffer overflow!\n"); \
            break; \
        } \
        destBuffer[destPos++] = (uint8_t)(bitBuffer & 0xFF); \
        bitBuffer >>= 8; \
        bitCount -= 8; \
    } \
} while (0)

// Helper macro: read 'width' bits from the bit stream into 'dest'.
#define ReadBits(dest, width) do { \
    while (bitCount < (width)) { \
        if (srcByte >= srcLen) { \
            printf("[UNPACK] Unexpected end of input!\n"); \
            goto done; \
        } \
        bitBuf   |= ((uint64_t)srcData[srcByte++]) << bitCount; \
        bitCount += 8; \
    } \
    (dest)     = (uint32_t)(bitBuf & ((1U << (width)) - 1)); \
    bitBuf   >>= (width); \
    bitCount  -= (width); \
    bitTotal  += (width); \
} while (0)

// Helper macro: discard 'width' bits (used to skip Pack's padding flush).
#define SkipBits(width) do { \
    int _w = (width); \
    while (_w > 0) { \
        int _chunk = (_w < bitCount) ? _w : bitCount; \
        if (_chunk == 0) { \
            if (srcByte >= srcLen) goto done; \
            bitBuf   |= ((uint64_t)srcData[srcByte++]) << bitCount; \
            bitCount += 8; \
            continue; \
        } \
        uint32_t _mask = (_chunk < 32) ? ((1U << _chunk) - 1) : 0xFFFFFFFFU; \
        bitBuf   >>= _chunk; \
        bitCount  -= _chunk; \
        bitTotal  += _chunk; \
        _w        -= _chunk; \
    } \
} while (0)

// LZW string table entry
typedef struct {
    uint32_t prefix; // predecessor code (or 0xFFFFFFFF if root)
    uint8_t  suffix; // character appended at this entry
} LZWEntry;

// Same clearTable used by Pack to flush padding bits after CLEAR_CODE.
const uint8_t clearTable[8] = { 0x00, 0x3C, 0x18, 0x54, 0x30, 0x0C, 0x48, 0x24 };


// Pack function - compress LZW data
uint8_t* Pack(const uint8_t* rawData, int rawLength, size_t* outSize)
{
    int destPos = 0;
    uint8_t* destBuffer = (uint8_t*)malloc(MAX_BUFFER_SIZE);
    
    if (!destBuffer) {
        printf("[PACK] Memory allocation failed!\n");
        *outSize = 0;
        return NULL;
    }
    
    // Write the 3-byte header.
    destBuffer[destPos++] = 0x1F;
    destBuffer[destPos++] = 0x9D;
    destBuffer[destPos++] = 0x8C;

    // Bit-packing state.
    uint64_t bitBuffer = 0;
    int bitCount = 0;
    // Start after the header: 3 bytes = 24 bits.
    int bitTotal = 24;

    // Dictionary.
    uint32_t* dictionary = (uint32_t*)calloc(HASH_SIZE, sizeof(uint32_t));
    int codeWidth = INIT_BITS;
    uint32_t maxCode = (1U << codeWidth) - 1;
    uint32_t nextCode = FIRST_CODE;

    // Keep track of the bit offset at the beginning of the current dictionary block.
    int blockBase = bitTotal;

    if (rawLength == 0) {
        free(dictionary);
        *outSize = destPos;
        return destBuffer;
    }

    uint32_t currentCode = rawData[0];
    int inputPos = 1;

    // Main Compression Loop.
    while (inputPos < rawLength) {
        uint8_t nextChar = rawData[inputPos++];
        uint32_t combinedCode = (currentCode << 8) | nextChar;
        uint32_t hashIndex = ((uint32_t)nextChar << 4) ^ currentCode;
        hashIndex %= HASH_SIZE;

        int found = 0;
        uint32_t step = (hashIndex == 0) ? 1u : (0x13FFu - hashIndex);

        while (dictionary[hashIndex] != 0) {
            int entry = (int)dictionary[hashIndex];
            int adjusted = entry + ((entry >> 31) & 0xFFF);
            if ((adjusted >> 12) == (int)combinedCode) {
                currentCode = (uint32_t)(entry & 0xFFF);
                found = 1;
                break;
            }

            // Apply probe arithmetic
            int tempIndex = (int)hashIndex - (int)step;
            if (tempIndex < 0) {
                tempIndex += 0x13FF; // wraparound
            }

            hashIndex = (uint32_t)tempIndex;
        }

        if (!found) {
            WriteBits(currentCode, codeWidth);

            if (nextCode > maxCode && codeWidth < MAX_BITS) {
                codeWidth++;
                maxCode = (1U << codeWidth) - 1;
            }

            if (nextCode <= MAX_CODE) {
                dictionary[hashIndex] = (combinedCode << 12) | nextCode;
                nextCode++;
            }
            else {
                // Dictionary full: emit CLEAR code and then flush extra bits
                WriteBits(CLEAR_CODE, codeWidth);
                // Compute how many bits have been output since the start of this dictionary block.
                int bitsSince = bitTotal - blockBase;
                int index = (bitsSince >> 2) & 7;
                int extraBits = clearTable[index]; // extra bits to flush
                WriteBits(0, extraBits);
                
                // Reset dictionary and LZW state
                memset(dictionary, 0, HASH_SIZE * sizeof(uint32_t));
                codeWidth = INIT_BITS;
                maxCode = (1U << codeWidth) - 1;
                nextCode = FIRST_CODE;
                // Reset blockBase to the current bit total
                blockBase = bitTotal;
            }

            currentCode = nextChar;
        }
    }

    // Final write
    WriteBits(currentCode, codeWidth);

	// Helper function to flush remaining bits
    if (bitCount > 0) {
        if (destPos < MAX_BUFFER_SIZE) {
            destBuffer[destPos++] = (uint8_t)(bitBuffer & 0xFF);
        }
        bitBuffer = 0;
        bitCount = 0;
    }

    free(dictionary);
    
    // Create result array of exact size
    uint8_t* result = (uint8_t*)malloc(destPos);
    if (result) {
        memcpy(result, destBuffer, destPos);
        *outSize = destPos;
    }
    else {
        *outSize = 0;
    }
    
    free(destBuffer);
    return result;
}

// Decode a code to a byte string and write it into the output buffer.
// Returns the first (root) character of the string.
static uint8_t DecodeString(uint8_t* outBuf, size_t* outPos,
                             const LZWEntry* table, uint32_t code,
                             size_t maxOut)
{
    // Walk the chain and build the string in reverse on a small stack.
    uint8_t stack[MAX_CODE + 1];
    int top = 0;

    while (code > 0xFF) {
        if (top >= (int)(sizeof(stack))) {
            printf("[UNPACK] Stack overflow in DecodeString!\n");
            break;
        }
        stack[top++] = table[code].suffix;
        code         = table[code].prefix;
    }
    // 'code' is now the root literal character.
    uint8_t firstChar = (uint8_t)code;
    stack[top++] = firstChar;

    // Write the string forward (reverse the stack).
    for (int i = top - 1; i >= 0; i--) {
        if (*outPos >= maxOut) {
            printf("[UNPACK] Output buffer overflow!\n");
            break;
        }
        outBuf[(*outPos)++] = stack[i];
    }
    return firstChar;
}

// Unpack function - decompress LZW data produced by Pack()
// Returns a heap-allocated buffer that the caller must free(), or NULL on error.
uint8_t* Unpack(const uint8_t* srcData, size_t srcLen, size_t* outSize)
{
    *outSize = 0;

    if (!srcData || srcLen < 3) {
        printf("[UNPACK] Input too short!\n");
        return NULL;
    }

    // Validate 3-byte header written by Pack.
    if (srcData[0] != 0x1F || srcData[1] != 0x9D || srcData[2] != 0x8C) {
        printf("[UNPACK] Bad magic bytes: %02X %02X %02X\n",
                    srcData[0], srcData[1], srcData[2]);
        return NULL;
    }

    // Allocate output buffer.
    uint8_t* outBuf = (uint8_t*)malloc(MAX_BUFFER_SIZE);
    if (!outBuf) {
        printf("[UNPACK] Memory allocation failed!\n");
        return NULL;
    }
    size_t outPos = 0;

    // Allocate the string table.
    // Codes 0x000–0x0FF are literal bytes (no table entry needed).
    // Codes 0x101–MAX_CODE are built dynamically.
    LZWEntry* table = (LZWEntry*)malloc((MAX_CODE + 1) * sizeof(LZWEntry));
    if (!table) {
        printf("[UNPACK] Table allocation failed!\n");
        free(outBuf);
        return NULL;
    }

    // -------------------------------------------------------------------------
    // Bit-reading state (LSB-first, matching WriteBits in Pack).
    // We start reading from byte index 3 (past the 3-byte header).
    // bitTotal tracks the total number of bits consumed so we can replicate
    // the blockBase / clearTable index arithmetic used by Pack.
    // -------------------------------------------------------------------------
    size_t   srcByte  = 3;         // next byte to load from srcData
    uint64_t bitBuf   = 0;         // bit accumulator (LSB = next bit to read)
    int      bitCount = 0;         // valid bits currently in bitBuf
    int      bitTotal = 24;        // total bits consumed (header = 3 bytes = 24 bits)

    // -------------------------------------------------------------------------
    // LZW state — reset at start and after every CLEAR_CODE.
    // -------------------------------------------------------------------------
    int      codeWidth = INIT_BITS;
    uint32_t maxCode   = (1U << codeWidth) - 1;
    uint32_t nextCode  = FIRST_CODE;
    int      blockBase = 24; // mirrors Pack's blockBase (starts at 24 = header bits)

    // Read the very first code — it is always a literal.
    uint32_t prevCode;
    ReadBits(prevCode, codeWidth);

    if (prevCode == CLEAR_CODE) {
        // Extremely unlikely for a well-formed stream, but handle it.
        goto done;
    }

    // Output the first literal.
    if (outPos >= MAX_BUFFER_SIZE) goto done;
    outBuf[outPos++] = (uint8_t)prevCode;

    uint8_t firstChar = (uint8_t)prevCode; // root char of the previous string

    // -------------------------------------------------------------------------
    // Main decompression loop.
    // -------------------------------------------------------------------------
    while (srcByte < srcLen) {
        uint32_t code;
        ReadBits(code, codeWidth);

        // ---- CLEAR code: reset dictionary and skip Pack's padding bits ------
        if (code == CLEAR_CODE) {
            // Compute padding bits exactly as Pack does.
            int bitsSince = bitTotal - blockBase;
            int index     = (bitsSince >> 2) & 7;
            int extraBits = clearTable[index];
            SkipBits(extraBits);

            // Reset LZW state.
            codeWidth = INIT_BITS;
            maxCode   = (1U << codeWidth) - 1;
            nextCode  = FIRST_CODE;
            blockBase = bitTotal; // matches Pack's blockBase = bitTotal after flush

            // Read the first code of the new block.
            ReadBits(prevCode, codeWidth);
            if (prevCode == CLEAR_CODE) break; // should not happen in valid stream

            if (outPos >= MAX_BUFFER_SIZE) goto done;
            outBuf[outPos++] = (uint8_t)prevCode;
            firstChar        = (uint8_t)prevCode;
            continue;
        }

        // ---- Decode the current code ----------------------------------------
        uint32_t entry = code;
        uint8_t  newFirstChar;

        if (code < nextCode) {
            // Code is already in the table (or is a literal).
            newFirstChar = DecodeString(outBuf, &outPos, table, code, MAX_BUFFER_SIZE);
        } else if (code == nextCode) {
            // Special case: encoder just added this code; its string is the
            // previous string + its own first character.
            newFirstChar = firstChar;
            // Temporarily emit the previous string, then append firstChar.
            DecodeString(outBuf, &outPos, table, prevCode, MAX_BUFFER_SIZE);
            if (outPos < MAX_BUFFER_SIZE) {
                outBuf[outPos++] = firstChar;
            }
        } else {
            printf("[UNPACK] Bad code %u (nextCode=%u)\n", code, nextCode);
            goto done;
        }

        firstChar = (code == nextCode) ? firstChar : newFirstChar;

        // ---- Add new entry to the string table ------------------------------
        if (nextCode <= MAX_CODE) {
            table[nextCode].prefix = prevCode;
            table[nextCode].suffix = firstChar;
            nextCode++;

            // Widen code width when the next code would exceed the current max
            // (mirrors Pack: widens when nextCode > maxCode after increment).
            if (nextCode > maxCode && codeWidth < MAX_BITS) {
                codeWidth++;
                maxCode = (1U << codeWidth) - 1;
            }
        }

        prevCode = code;
    }

done:
    free(table);

    // Return an exactly-sized copy.
    uint8_t* result = (uint8_t*)malloc(outPos);
    if (result) {
        memcpy(result, outBuf, outPos);
        *outSize = outPos;
    }
    free(outBuf);
    return result;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -u            Unpack File\n");
	printf(" -p            Pack File\n\n");
	return;
}

int main(int argc, char **argv)
{
    size_t len;
    u8* data;
    char *opt, *bak;
    FILE* out;

    printf("\nTomb Raider VI Remaster (PS4) Save Unpacker 0.1.0 - (c) 2026 by Bucanero\n\n");

    if (--argc < 2)
    {
        print_usage(argv[0]);
        return -1;
    }

    opt = argv[1];
    if (*opt++ != '-' || (*opt != 'u' && *opt != 'p'))
    {
        print_usage(argv[0]);
        return -1;
    }

    if (read_buffer(argv[2], &data, &len) != 0)
    {
        printf("[*] Could Not Access The File (%s)\n", argv[2]);
        return -1;
    }
    // Save a file backup
    asprintf(&bak, "%s.bak", argv[2]);
    write_buffer(bak, data, len);

    out = fopen(argv[2], "wb");
    if (!out)    {
        printf("[*] Could Not Open Output File (%s)\n", argv[2]);
        free(data);
        return -1;
    }
    fwrite(data, 1, BASE_SAVEGAME_OFFSET_TR6, out);

    size_t compressedSize, decompressedSize, saveSize;
    uint8_t *savedata, *compressedData, *decompressedData;

    savedata = data + BASE_SAVEGAME_OFFSET_TR6;
    saveSize = len - BASE_SAVEGAME_OFFSET_TR6;

    if (*opt == 'u')
    {
        for (int i = 0; i < MAX_SAVEGAMES; i++)
        {
            fwrite(savedata + (i * SAVEGAME_SIZE), 1, COMPRESSED_BLOCK_SIZE_OFFSET, out);
            compressedData = savedata + (i * SAVEGAME_SIZE) + COMPRESSED_BLOCK_START_OFFSET;
            compressedSize = *(uint32_t*)(savedata + (i * SAVEGAME_SIZE) + COMPRESSED_BLOCK_SIZE_OFFSET);
            if (memcmp(compressedData - COMPRESSED_BLOCK_START_OFFSET, "TOMB", 4) || memcmp(compressedData, "\x1F\x9D\x8C", 3))
            {
                for (int j = COMPRESSED_BLOCK_SIZE_OFFSET; j < MAX_BUFFER_SIZE; j++)
                    fputc('\0', out);

                continue;
            }

            printf("------\nSavegame slot #%d detected, decompressing...\n", i);

            if (compressedSize > saveSize) {
                printf("Compressed block size offset for savegame %d is out of bounds!\n", i);
                continue;
            }
            
            uint8_t* decompressed = Unpack(compressedData, compressedSize, &decompressedSize);
            
            if (!decompressed) {
                printf("Error! Decompression failed for savegame %d!\n", i);
                exit(-1);
            }
            printf("Decompressed %zu bytes -> %zu bytes\n", compressedSize, decompressedSize);

            fputc(decompressedSize & 0xFF, out);
            fputc((decompressedSize >> 8) & 0xFF, out);
            fputc((decompressedSize >> 16) & 0xFF, out);
            fputc((decompressedSize >> 24) & 0xFF, out);
            fwrite("\0\0\0\0", 1, 4, out);

            fwrite(decompressed, 1, decompressedSize, out);
            for (int j = decompressedSize + 8 + COMPRESSED_BLOCK_SIZE_OFFSET; j < MAX_BUFFER_SIZE; j++)
                fputc('\xFF', out);

            free(decompressed);
        }

        fwrite(savedata + (MAX_SAVEGAMES * SAVEGAME_SIZE), 1, saveSize - (MAX_SAVEGAMES * SAVEGAME_SIZE), out);
        printf("[*] Unpacked File Successfully!\n\n");
    }
    else
    {
        for (int i = 0; i < MAX_SAVEGAMES; i++)
        {
            fwrite(savedata + (i * MAX_BUFFER_SIZE), 1, COMPRESSED_BLOCK_SIZE_OFFSET, out);
            decompressedData = savedata + (i * MAX_BUFFER_SIZE) + COMPRESSED_BLOCK_START_OFFSET;
            decompressedSize = *(uint32_t*)(savedata + (i * MAX_BUFFER_SIZE) + COMPRESSED_BLOCK_SIZE_OFFSET);
            if (memcmp(decompressedData - COMPRESSED_BLOCK_START_OFFSET, "TOMB", 4) || memcmp(decompressedData, "TOMB", 4))
            {
                for (int j = COMPRESSED_BLOCK_SIZE_OFFSET; j < SAVEGAME_SIZE; j++)
                    fputc('\0', out);

                continue;
            }

            printf("------\nSavegame slot #%d detected, compressing...\n", i);

            if (decompressedSize > saveSize) {
                printf("Decompressed block size offset for savegame %d is out of bounds!\n", i);
                continue;
            }
            
            uint8_t* compressed = Pack(decompressedData, decompressedSize, &compressedSize);
            
            if (!compressed || compressedSize > SAVEGAME_SIZE) {
                printf("Error! Compression failed for savegame %d!\n", i);
                exit(-1);
            }
            printf("Compressed %zu bytes -> %zu bytes\n", decompressedSize, compressedSize);

            fputc(compressedSize & 0xFF, out);
            fputc((compressedSize >> 8) & 0xFF, out);
            fputc((compressedSize >> 16) & 0xFF, out);
            fputc((compressedSize >> 24) & 0xFF, out);
            fwrite("\0\0\0\0", 1, 4, out);

            fwrite(compressed, 1, compressedSize, out);
            for (int j = compressedSize + 8 + COMPRESSED_BLOCK_SIZE_OFFSET; j < SAVEGAME_SIZE; j++)
                fputc('\0', out);

            free(compressed);
        }        

        fwrite(savedata + (MAX_SAVEGAMES * MAX_BUFFER_SIZE), 1, saveSize - (MAX_SAVEGAMES * MAX_BUFFER_SIZE), out);
        printf("[*] Packed File Successfully!\n\n");
    }

    fclose(out);
    free(data);
    free(bak);

    return 0;
}

