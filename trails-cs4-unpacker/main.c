/*
*
*	The Legend of Heroes: Trails of Cold Steel IV (PS4) Save Unpacker - (c) 2026 by Bucanero - www.bucanero.com.ar
*
* Information about the custom LZ compression method:
*	- https://github.com/AdmiralCurtiss/SenPatcher/blob/master/native/sen/pkg_extract.cpp
*   - https://github.com/AdmiralCurtiss/SenPatcher/blob/master/native/sen/pkg_compress.cpp
*/

#include <stdbool.h>
#include "../common/iofile.c"

#define CRC32_POLY    0xEDB88320

typedef struct {
    size_t Length;
    size_t Position;
} Backref;


void init_crc32_table(uint32_t* crc32_table, uint32_t poly)
{
	for (int b = 0; b < 256; ++b)
	{
		uint32_t r = b;

		for (int i = 0; i < 8; ++i)
			r = (r & 1) ? (r >> 1) ^ poly : (r >> 1);

		crc32_table[b] = r;
	}

	return;
}

u32 calc_crc32(const u8* data, u32 len)
{
	u32 crc32_table[256];
	u32 crc = len;

	init_crc32_table(crc32_table, CRC32_POLY);

	while (len--)
		crc = crc32_table[(crc ^ *data++) & 0xFF] ^ (crc >> 8);

	return crc;
}

// Helper function to write 32-bit value in little endian
static void WriteUInt32LE(char* dest, uint32_t value) {
    dest[0] = (char)(value & 0xFF);
    dest[1] = (char)((value >> 8) & 0xFF);
    dest[2] = (char)((value >> 16) & 0xFF);
    dest[3] = (char)((value >> 24) & 0xFF);
}

// Helper function to read 32-bit value in little endian
uint32_t ReadUInt32(const char* buffer)
{
	return ((uint32_t)(uint8_t)(buffer[0]) << 0) |
		   ((uint32_t)(uint8_t)(buffer[1]) << 8) |
		   ((uint32_t)(uint8_t)(buffer[2]) << 16) |
		   ((uint32_t)(uint8_t)(buffer[3]) << 24);
}

static bool DecompressType1(char* uncompressedDataBuffer,
                            uint32_t uncompressedSize,
                            const char* compressedData,
                            uint32_t compressedSize)
{
    if (compressedSize < 12) {
        return false;
    }

    // decompression algorithm derived from 0x41aa50 in CS2
    // very simple, can only copy verbatim bytes or reference already written bytes
    char* target = uncompressedDataBuffer;
    // the game ignores the target size here...
    const uint32_t targetSize = ReadUInt32(&compressedData[0]);
    const uint32_t sourceSize = ReadUInt32(&compressedData[4]);
    // this is in fact read and compared as 32-bit, very odd
    const uint32_t backrefByte = ReadUInt32(&compressedData[8]);
    uint32_t targetPosition = 0;
    uint32_t sourcePosition = 12;
    while (sourcePosition < sourceSize) {
        if (targetPosition >= uncompressedSize) {
            return false;
        }

        if (sourcePosition >= compressedSize) {
            return false;
        }
        const uint8_t sourceByte = (uint8_t)(compressedData[sourcePosition]);
        ++sourcePosition;
        if (sourceByte == backrefByte) {
            if (sourcePosition >= compressedSize) {
                return false;
            }
            uint32_t backrefOffset = (uint8_t)(compressedData[sourcePosition]);
            ++sourcePosition;
            if (backrefOffset == backrefByte) {
                target[targetPosition] = (char)(backrefByte);
                ++targetPosition;
            } else {
                if (backrefByte < backrefOffset) {
                    --backrefOffset;
                }
                if (backrefOffset == 0) {
                    return false;
                }
                if (sourcePosition >= compressedSize) {
                    return false;
                }
                const uint32_t backrefLength = (uint8_t)(compressedData[sourcePosition]);
                ++sourcePosition;

                if (backrefLength > 0) {
                    if (targetPosition < backrefOffset) {
                        return false;
                    }
                    if (targetPosition > UINT32_MAX - backrefLength) {
                        return false;
                    }
                    if (targetPosition + backrefLength > uncompressedSize) {
                        return false;
                    }

                    // This is technically a behavior difference, the game just does a memcpy here.
                    // This makes a difference when the two ranges here overlap: What exactly memcpy
                    // does in that situation is undefined, so all the game's assets are packed in a
                    // way where that never happens. Copying one byte at a time like this works
                    // correctly even in the overlapping case.
                    for (size_t i = 0; i < backrefLength; ++i) {
                        target[targetPosition + i] = target[(targetPosition - backrefOffset) + i];
                    }
                    targetPosition += backrefLength;
                }
            }
        } else {
            target[targetPosition] = (char)(sourceByte);
            ++targetPosition;
        }
    }
    if (uncompressedSize != targetPosition) {
        return false;
    }

    return true;
}

static void CompressType1CalculateByteCounts(uint32_t countPerByte[256],
                                             const char* uncompressedData,
                                             uint32_t uncompressedLength)
{
    // The way this compression works, all bytes are just verbatim except one byte (specified in the
    // header) which is used as a command for a backref. So to determine which byte is the best
    // choice for this, we check which bytes exist in the file and then pick the one that occurs the
    // least.
    memset(countPerByte, 0, 256 * sizeof(uint32_t));
    for (size_t i = 0; i < uncompressedLength; ++i) {
        countPerByte[(uint8_t)(uncompressedData[i])] += 1;
    }
}

static uint8_t CompressType1GetLeastUsedByte(const uint32_t countPerByte[256])
{
    uint32_t leastUsedByte = 0;
    for (uint32_t i = 1; i < 256; ++i) {
        if (countPerByte[i] < countPerByte[leastUsedByte]) {
            leastUsedByte = i;
        }
    }
    return (uint8_t)(leastUsedByte);
}

static uint64_t CompressType1BoundForBackrefByte(const uint32_t countPerByte[256],
                                                 uint32_t uncompressedLength,
                                                 uint8_t backrefByte)
{
    const uint64_t bound = 12 + uncompressedLength + countPerByte[backrefByte];
    return bound;
}

static Backref find_best_backref(const char* uncompressedData,
                                 uint32_t uncompressedLength,
                                 size_t uncompressedPosition)
{
    static const size_t maxBackrefLength = 255;
    static const size_t maxBackrefOffset = 254;

    Backref bestBackref = {0, 0};
    if (uncompressedPosition == 0) {
        return bestBackref; // no backref possible
    }

    // the game uses memcpy to evaluate the backref, so the backref *must* be completely in the
    // already written bytes. this really inhibits efficient compression of repeated byte
    // patterns, but otherwise we're risking invalid decompression by the game...

    const size_t firstPossibleBackrefPosition =
        uncompressedPosition < maxBackrefOffset ? 0 : (uncompressedPosition - maxBackrefOffset);
    const size_t lastPossibleBackrefPosition = uncompressedPosition - 1;

    size_t currentBackrefTest = lastPossibleBackrefPosition;
    
    while (1) {
        // count_backref_from_here inline implementation
        size_t count = 0;
        const size_t localMaxBackrefLength = uncompressedPosition - currentBackrefTest;
        const size_t allowedBackrefLength =
            (uncompressedLength - uncompressedPosition) >= localMaxBackrefLength
                ? localMaxBackrefLength
                : (uncompressedLength - uncompressedPosition);
        for (size_t i = 0; i < allowedBackrefLength; ++i) {
            if (uncompressedData[currentBackrefTest + i]
                == uncompressedData[uncompressedPosition + i]) {
                ++count;
            } else {
                break;
            }
        }
        
        if (count > bestBackref.Length) {
            bestBackref.Length = count;
            bestBackref.Position = currentBackrefTest;
        }
        if (count == maxBackrefLength) {
            break;
        }

        if (currentBackrefTest == firstPossibleBackrefPosition) {
            break;
        }
        --currentBackrefTest;
    }

    return bestBackref;
}

// compressedData must be at least CompressType1BoundForBackrefByte() bytes long, there is no bounds
// checking for it in this function!
static bool CompressType1(char* compressedData,
                          uint32_t* compressedLength,
                          const char* uncompressedData,
                          uint32_t uncompressedLength,
                          uint8_t backrefByte)
{
    size_t outputPosition = 12;
    size_t pos = 0;
    
    while (pos < uncompressedLength) {
        Backref bestBackref = find_best_backref(uncompressedData, uncompressedLength, pos);
        
        if (bestBackref.Length <= 1) {
            // could not find a (sensible) backref, need to write literal
            const uint8_t byte = (uint8_t)uncompressedData[pos];
            if (byte == backrefByte) {
                compressedData[outputPosition] = (char)byte;
                compressedData[outputPosition + 1] = (char)byte;
                outputPosition += 2;
            } else {
                compressedData[outputPosition] = (char)byte;
                outputPosition += 1;
            }
            pos += 1;
            continue;
        }
        
        if (bestBackref.Length >= 4) {
            compressedData[outputPosition] = (char)backrefByte;
            uint8_t offset = (uint8_t)(pos - bestBackref.Position);
            if (backrefByte <= offset) {
                ++offset;
            }
            compressedData[outputPosition + 1] = (char)offset;
            compressedData[outputPosition + 2] = (char)((uint8_t)(bestBackref.Length));
            outputPosition += 3;
            pos += bestBackref.Length;
            continue;
        }

        // we have a backref of 2 or 3 bytes; depending on what bytes are referenced, it could be
        // shorter to write the backref, or it could be shorter to write the literal bytes.
        // falcom's original compressor did not do this, so in order to get bitexact output with
        // the vanilla game files this is disabled. if you for some reason need to get a few extra
        // bytes out of this compression, feel free to enable it
        {
            const uint8_t byte = (uint8_t)uncompressedData[pos];
            if (byte == backrefByte) {
                compressedData[outputPosition] = (char)byte;
                compressedData[outputPosition + 1] = (char)byte;
                outputPosition += 2;
            } else {
                compressedData[outputPosition] = (char)byte;
                outputPosition += 1;
            }
            pos += 1;
        }
    }

    if (outputPosition > UINT32_MAX) {
        return false;
    }
    const uint32_t compressedLengthLocal = (uint32_t)outputPosition;

    // Write header in little endian
    WriteUInt32LE(&compressedData[0], uncompressedLength);
    WriteUInt32LE(&compressedData[4], compressedLengthLocal);
    WriteUInt32LE(&compressedData[8], (uint32_t)backrefByte);

    *compressedLength = compressedLengthLocal;
    return true;
}


void* decompress_data(void* data, size_t* size)
{
	uint32_t original_size = ReadUInt32((char*)data);

	printf("[*] Total Compressed Size Is %ld bytes\n", *size);
	printf("[*] Original Uncompressed Size Is %d bytes (0x%X)\n", original_size, original_size);

	void* decompressed_data = malloc(original_size);
	if (!decompressed_data)
	{
		printf("[*] Could Not Allocate Memory For Decompression!\n");
		return data;
	}

	if (!DecompressType1(decompressed_data, original_size, data, *size))
	{
		printf("[*] Decompression Failed!\n");
		free(decompressed_data);
		return data;
	}

	free(data);
	*size = original_size;

	int crc = calc_crc32(decompressed_data + 0x10, original_size - 0x10);
	if (memcmp(&crc, decompressed_data + 0x0C, 4) != 0)
		printf("[*] Warning: CRC32 Mismatch After Decompression!\n");
	else
		printf("[*] Stored CRC32 Checksum: %08X\n", crc);

	printf("[*] Decompressed File Successfully!\n\n");

	return decompressed_data;
}

void* compress_data(void* data, size_t* size)
{
	uint32_t countPerByte[256];
	uint32_t compressed_size = 0;

	printf("[*] Total Uncompressed Size Is %ld bytes\n", *size);

	CompressType1CalculateByteCounts(countPerByte, (char*)data, *size);
	uint8_t backrefByte = CompressType1GetLeastUsedByte(countPerByte);
	printf("[*] Selected Backref Byte: 0x%02X\n", backrefByte);

	uint64_t compressed_bound = CompressType1BoundForBackrefByte(countPerByte, *size, backrefByte);

	void* compressed_data = malloc(compressed_bound);
	if (!compressed_data)
	{
		printf("[*] Could Not Allocate Memory For Compression!\n");
		return data;
	}

	if (!CompressType1(compressed_data, &compressed_size, data, *size, backrefByte))
	{
		printf("[*] Compression Failed!\n");
		free(compressed_data);
		return data;
	}

	printf("[*] Encrypted File Successfully!\n\n");
	*size = compressed_size;
	return compressed_data;
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

	printf("\ntrails-cs4-unpacker 0.1.0 - (c) 2026 by Bucanero\n\n");

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

	if (*opt == 'u')
		data = decompress_data(data, &len);
	else
	{
		u32 crc = calc_crc32(data + 0x10, len - 0x10);

		printf("[*] Stored Checksum: %08X\n", *(u32*)&data[12]);
		printf("[*] Update Checksum: %08X\n", crc);
		memcpy(data + 12, &crc, 4);

		data = compress_data(data, &len);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
