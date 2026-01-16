/*
*
*	Max Payne 3 PS3 Save Decrypter - (c) 2025 by Bucanero
*
* This tool is based on crypto notes by Vulnavia, and PS3 keys and code by Red-EyeX32
*
*/

#include <stdbool.h>
#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/sha1.c"
#include "../common/hmac-sha1.c"
#include "../common/pbkdf2-sha1.c"

// Max Payne 3 init key
// PlayStation: PS35675hA
// XBOX: XEN43156A
// PC: PCgh64rwA
#define BASE_PROFILE_KEY "PS35675hA"
#define PROFILE_KEY_SIZE 0x32
#define MODE_ENCRYPT 1
#define MODE_DECRYPT 0
#define MAX_DECOMP_BUF_SIZE 0x0C800

//- 0x18~0x37 = Blob_1, HMACSHA1(0x14) [encrypted]
//- 0x38~0x3B = Integer_7, Salt (The manipulated Nanoseconds Part of Time/Date)
//- 0x3C~0x43 = Long_1, Time/Date (Unixtime Integer only the other Part is 0)
typedef struct ProfileHeader {
	uint8_t blob_1[0x20]; // 0x18~0x37
	uint8_t integer_7[0x04]; // 0x38~0x3B
	uint8_t long_1[0x08]; // 0x3C~0x43
} save_header_t;

// Max Payne 3 Save data encryption key
const uint8_t StaticAESKey[0x20] = {
	0x1A, 0xB5, 0x6F, 0xED, 0x7E, 0xC3, 0xFF, 0x01, 0x22, 0x7B, 0x69, 0x15, 0x33, 0x97, 0x5D, 0xCE,
	0x47, 0xD7, 0x69, 0x65, 0x3F, 0xF7, 0x75, 0x42, 0x6A, 0x96, 0xCD, 0x6D, 0x53, 0x07, 0x56, 0x5D
};

const uint8_t pkdfSalt1[0x10] = { 0x0F, 0xC9, 0x19, 0xE8, 0x9A, 0x17, 0xC4, 0x5F, 0xE7, 0x16, 0xD4, 0x6C, 0x3A, 0x15, 0x9C, 0x75 };
const uint8_t pkdfSalt2[0x10] = { 0xE1, 0x09, 0xA5, 0x42, 0xF6, 0x0A, 0x13, 0x3B, 0x81, 0xAC, 0x02, 0x55, 0xCC, 0x39, 0x40, 0x1B };
const uint8_t pkdfSalt3[0x10] = { 0x15, 0x08, 0xE9, 0x6F, 0x47, 0xB8, 0x47, 0xD1, 0x3A, 0x65, 0x8C, 0x71, 0x00, 0x00, 0x00, 0x00 };


void Mp3_AesEcb(const uint8_t* key, uint8_t* pbInp, int cbInp, bool encrypt)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, key);

	int dataLen = cbInp & ~0x0F;

	if (dataLen > 0) {
		for (int i = 0; i < 16; i++) {
			if (encrypt)
			{
				for (int j = 0; j < dataLen; j+= AES_BLOCKLEN)
					AES_ECB_encrypt(&ctx, pbInp + j);
			}
			else
			{
				for (int j = 0; j < dataLen; j+= AES_BLOCKLEN)
					AES_ECB_decrypt(&ctx, pbInp + j);
			}
		}
	}
}

void Mp3_AesEcb2(const uint8_t* key, uint8_t* pbInp, int cbInp, bool encrypt)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, key);

	int dataLen = cbInp & ~0x0F;

	if (dataLen > 0)
	{
		for (int i = 0; i < 16; i++)
		{
			if (encrypt)
			{
				AES_ECB_encrypt(&ctx, pbInp + cbInp - 0x10);
			}
			else
			{
				AES_ECB_decrypt(&ctx, pbInp + cbInp - 0x10);
			}
		}
	}
}

void decrypt_data(uint8_t* saveBuffer, u32 saveLen, const char* ProfileKey)
{
	uint8_t* saveData;
	uint8_t digestKey[20];
	uint8_t PKDF2Key1[0x20];
	uint8_t PKDF2Key2[0x20];
	uint8_t PKDF2Key3[0x20];
	save_header_t headerData;
	u32 beSaveDataLen = ES32(saveLen); // big endian

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", saveLen, saveLen);

	saveData = malloc(saveLen + 4);
	memcpy(&headerData, saveBuffer, sizeof(headerData));
	memcpy(saveData, saveBuffer + 0x2C, saveLen);
	memcpy(saveData + saveLen, &beSaveDataLen, 4);

	// calculate HMACSHA1
	hmac_sha1(digestKey, ProfileKey, PROFILE_KEY_SIZE, saveData, saveLen + 4);
	printf("[*] HMAC-SHA1: " SHA1_FMT(digestKey, "\n"));

	free(saveData);
	saveData = saveBuffer + 0x2C;

	// derive key1
	gc_pbkdf2_sha1(digestKey, sizeof(digestKey), pkdfSalt1, sizeof(pkdfSalt1), 2000, PKDF2Key1, sizeof(PKDF2Key1));

	// decrypt header
	Mp3_AesEcb(PKDF2Key1, (uint8_t*)&headerData, sizeof(headerData), MODE_DECRYPT);

	// derive key2
	gc_pbkdf2_sha1(&headerData, sizeof(headerData), pkdfSalt2, sizeof(pkdfSalt2), 2000, PKDF2Key2, sizeof(PKDF2Key2));

	Mp3_AesEcb(StaticAESKey, PKDF2Key2, sizeof(PKDF2Key2), MODE_ENCRYPT);
	// decrypt save data
	Mp3_AesEcb2(PKDF2Key2, saveData, saveLen, MODE_DECRYPT);
	Mp3_AesEcb(PKDF2Key2, saveData, saveLen, MODE_DECRYPT);

	// verifying the data
	uint8_t sha1key[20];
	uint8_t tmpbuf[PROFILE_KEY_SIZE + 4];
	memcpy(tmpbuf, ProfileKey, PROFILE_KEY_SIZE);
	memcpy(tmpbuf + PROFILE_KEY_SIZE, &beSaveDataLen, 4);

	sha1(sha1key, tmpbuf, sizeof(tmpbuf));

	uint8_t hmacInitKey2[0x20];
	hmac_sha1(hmacInitKey2, sha1key, sizeof(sha1key), saveData, saveLen);
	memcpy(hmacInitKey2 + 0x14, headerData.integer_7, 4);
	memcpy(hmacInitKey2 + 0x18, headerData.long_1, 8);

	uint8_t saltBuffer[0x10];
	memcpy(saltBuffer, pkdfSalt3, 0x10);
	memcpy(saltBuffer + 0x0C, headerData.integer_7, 4);

	// derive key3
	gc_pbkdf2_sha1(hmacInitKey2, sizeof(hmacInitKey2), saltBuffer, sizeof(saltBuffer), 2000, PKDF2Key3, sizeof(PKDF2Key3));

	// decrypt header blob_1 (hmac-sha1)
	Mp3_AesEcb(StaticAESKey, headerData.blob_1, 0x20, MODE_DECRYPT);
	Mp3_AesEcb(PKDF2Key3, headerData.blob_1, 0x20, MODE_DECRYPT);

	memcpy(saveBuffer, &headerData, sizeof(headerData));
	printf("[*] Data Hash: " SHA1_FMT(headerData.blob_1, "\n"));

	if (memcmp(headerData.blob_1, hmacInitKey2, 0x20) != 0)
	{
		printf("[!] Warning: save data could not be verified.\n");
	}

	printf("[*] Decrypted File Successfully!\n");
	return;
}

void encrypt_data(uint8_t* saveBuffer, u32 saveLen, const char* ProfileKey)
{
	uint8_t* saveData;
	uint8_t digestKey[20];
	uint8_t PKDF2Key1[0x20];
	uint8_t PKDF2Key2[0x20];
	uint8_t PKDF2Key3[0x20];
	save_header_t headerData;
	u32 pbSaveDataLen = ES32(saveLen); // big endian

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", saveLen, saveLen);

	saveData = saveBuffer + 0x2C;
	memcpy(&headerData, saveBuffer, sizeof(headerData));

	// calculating HMACSHA1
	uint8_t sha1key[20];
	uint8_t tmpbuf[PROFILE_KEY_SIZE + 4];
	memcpy(tmpbuf, ProfileKey, PROFILE_KEY_SIZE);
	memcpy(tmpbuf + PROFILE_KEY_SIZE, &pbSaveDataLen, 4);

	sha1(sha1key, tmpbuf, sizeof(tmpbuf));

	uint8_t hmacInitKey2[0x20];
	hmac_sha1(hmacInitKey2, sha1key, sizeof(sha1key), saveData, saveLen);
	memcpy(hmacInitKey2 + 0x14, headerData.integer_7, 4);
	memcpy(hmacInitKey2 + 0x18, headerData.long_1, 8);

	uint8_t saltBuffer[0x10];
	memcpy(saltBuffer, pkdfSalt3, 0x10);
	memcpy(saltBuffer + 0x0C, headerData.integer_7, 4);

	// derive key3
	gc_pbkdf2_sha1(hmacInitKey2, sizeof(hmacInitKey2), saltBuffer, sizeof(saltBuffer), 2000, PKDF2Key3, sizeof(PKDF2Key3));
	// store new header blob_1
	printf("[*] Old Hash : " SHA1_FMT(headerData.blob_1, "\n"));
	printf("[*] New Hash : " SHA1_FMT(hmacInitKey2, "\n"));
	memcpy(headerData.blob_1, hmacInitKey2, 0x20);
	// encrypt header blob_1 (hmac-sha1)
	Mp3_AesEcb(PKDF2Key3, headerData.blob_1, 0x20, MODE_ENCRYPT);
	Mp3_AesEcb(StaticAESKey, headerData.blob_1, 0x20, MODE_ENCRYPT);

	// derive key2
	gc_pbkdf2_sha1(&headerData, sizeof(headerData), pkdfSalt2, sizeof(pkdfSalt2), 2000, PKDF2Key2, sizeof(PKDF2Key2));

	Mp3_AesEcb(StaticAESKey, PKDF2Key2, sizeof(PKDF2Key2), MODE_ENCRYPT);
	// encrypt save data
	Mp3_AesEcb(PKDF2Key2, saveData, saveLen, MODE_ENCRYPT);
	Mp3_AesEcb2(PKDF2Key2, saveData, saveLen, MODE_ENCRYPT);

	// calculate HMACSHA1
	saveData = malloc(saveLen + 4);
	memcpy(saveData, saveBuffer + 0x2C, saveLen);
	memcpy(saveData + saveLen, &pbSaveDataLen, 4);

	hmac_sha1(digestKey, ProfileKey, PROFILE_KEY_SIZE, saveData, saveLen + 4);
	printf("[*] HMAC-SHA1: " SHA1_FMT(digestKey, "\n"));
	free(saveData);

	// derive key1
	gc_pbkdf2_sha1(digestKey, sizeof(digestKey), pkdfSalt1, sizeof(pkdfSalt1), 2000, PKDF2Key1, sizeof(PKDF2Key1));

	// encrypt header
	Mp3_AesEcb(PKDF2Key1, (uint8_t*)&headerData, sizeof(headerData), MODE_ENCRYPT);
	memcpy(saveBuffer, &headerData, sizeof(headerData));

	printf("[*] Encrypted File Successfully!\n");
	return;
}

uint8_t* lz77_DecompressData(uint8_t* data, uint32_t in_size, uint32_t *out_size)
{
	uint8_t* outstream;
	uint32_t pos = 2, op_pos = 0;
	int code, off;

	if ((data[0] + (data[1] << 8) + 2) != in_size)
	{
		printf("[!] Decompression error: invalid input size.\n");
		return NULL;
	}

	outstream = malloc(MAX_DECOMP_BUF_SIZE);
	memset(outstream, 0, MAX_DECOMP_BUF_SIZE);

	while (pos < in_size)
	{
		code = data[pos++];

		for (int i = 0; i < 8 && pos < in_size; i++)
		{
			code &= 0xFF;

			if (code & 0x80)
			{
				if (op_pos < MAX_DECOMP_BUF_SIZE)
					outstream[op_pos++] = data[pos++];
			}
			else
			{
				int k = data[pos++];

				off = op_pos - 1;
				off -= (k >> 4) | (data[pos++] << 4);

				if (off < 0)
				{
					printf("[!] Decompression error: invalid offset.\n");
					free(outstream);
					return NULL;
				}

				for (int j = (k & 0x0F) + 3; j && op_pos < MAX_DECOMP_BUF_SIZE; j--)
				{
					outstream[op_pos++] = outstream[off++];
				}
			}

			code <<= 1;
		}
	}

	*out_size = op_pos;

	return outstream;
}

uint8_t* lz77_CompressData(uint8_t* data, uint32_t in_size, uint32_t *out_size)
{
	uint8_t* outstream;
	uint32_t op_pos = 0;
	uint32_t ip_pos = 0;
	// Worst case: all literals (1 byte each + 1 control byte per 8 literals) + 2 byte header
	uint32_t max_out_size = in_size + (in_size / 8) + 16;
	
	outstream = malloc(max_out_size);
	memset(outstream, 0, max_out_size);
	
	// Reserve space for size header (will be written at the end)
	op_pos = 2;
	
	while (ip_pos < in_size)
	{
		uint8_t code = 0;
		uint32_t code_pos = op_pos++;
		
		for (int i = 0; i < 8 && ip_pos < in_size; i++)
		{
			int best_len = 0;
			int best_off = 0;
			
			// Search for matches in the sliding window (up to 4096 bytes back)
			// The offset is stored in 12 bits, allowing values 0-4095, which represents
			// actual offsets of 1-4096 bytes back
			int search_start = (ip_pos > 4096) ? ip_pos - 4096 : 0;
			
			for (int j = search_start; j < (int)ip_pos; j++)
			{
				int match_len = 0;
				
				// Count matching bytes
				while (match_len < 18 && 
				       (ip_pos + match_len) < in_size && 
				       data[j + match_len] == data[ip_pos + match_len])
				{
					match_len++;
				}
				
				// Only consider matches of length 3 or more
				if (match_len >= 3 && match_len > best_len)
				{
					best_len = match_len;
					best_off = ip_pos - j;
				}
			}
			
			if (best_len >= 3 && best_len <= 18)
			{
				// Encode as back-reference (bit = 0)
				// Length: best_len - 3 (stored in low 4 bits)
				// Offset: best_off - 1 (stored in 12 bits across 2 bytes)
				int offset = best_off - 1;
				int length = best_len - 3;
				
				outstream[op_pos++] = ((offset & 0x0F) << 4) | (length & 0x0F);
				outstream[op_pos++] = (offset >> 4) & 0xFF;
				
				ip_pos += best_len;
			}
			else
			{
				// Encode as literal (bit = 1)
				code |= (1 << (7 - i));
				outstream[op_pos++] = data[ip_pos++];
			}
		}
		
		outstream[code_pos] = code;
	}
	
	// Write the size header (compressed size - 2)
	uint32_t compressed_size = op_pos - 2;
	outstream[0] = compressed_size & 0xFF;
	outstream[1] = (compressed_size >> 8) & 0xFF;
	
	*out_size = op_pos;
	
	return outstream;
}

uint32_t addSum(uint8_t* data, u32 len)
{
	uint32_t sum = 0;

	while (len--)
		sum += *data++;

	return sum;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt File\n");
	printf(" -e            Encrypt File\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	u32 Version, HeaderLength, saveBufferLen, saveDataLen, storedSum;
	char ProfileKey[PROFILE_KEY_SIZE];
	char *opt, *bak;

	printf("\nmax-payne3-decrypter 0.2.0 - (c) 2026 by Bucanero\n\n");

	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}
	
	opt = argv[1];
	if (*opt++ != '-' || (*opt != 'd' && *opt != 'e'))
	{
		print_usage(argv[0]);
		return -1;
	}

	if (read_buffer(argv[2], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[2]);
		return -1;
	}

	HeaderLength = ES32(*(u32*)(data+4));
	if (memcmp(data, "mp3\0", 4) != 0 || HeaderLength < 0x1A)
	{
		printf("[*] Invalid header detected.\n");
		free(data);
		return -1;
	}

	saveBufferLen = ES32(*(u32*)(data+8));
	saveDataLen = ES32(*(u32*)(data+0x0C));

	if ((saveBufferLen + HeaderLength) != saveDataLen)
	{
		printf("[*] Invalid save size detected.\n");
		free(data);
		return -1;
	}
	
	storedSum = ES32(*(u32*)(data+0x10));
	Version = ES32(*(u32*)(data+0x14));

	printf("[*] Stored Checksum: %08X\n", storedSum);
	printf("[*] Header Length: %d\n", HeaderLength);
	printf("[*] Save Buffer Length: %d\n", saveBufferLen);
	printf("[*] Save Data Length: %d\n", saveDataLen);
	printf("[*] Version: %d\n", Version);

	// Create the save key
	memset(ProfileKey, 0, sizeof(ProfileKey));
	snprintf(ProfileKey, sizeof(ProfileKey), BASE_PROFILE_KEY "%02d%d", HeaderLength, Version);

	printf("[*] Profile Key: %s\n", ProfileKey);

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	if (*opt == 'd')
	{
		if (addSum(data + 0x44, saveBufferLen) != storedSum)
		{
			printf("[!] Warning: Invalid checksum detected.\n");
		}

		decrypt_data(data+0x18, saveBufferLen, ProfileKey);

		uint32_t decompSize = 0;
		uint8_t* decompressedData = lz77_DecompressData(data + 0x44, saveBufferLen, &decompSize);
		if (!decompressedData)
		{
			printf("[!] ERROR: Decompression failed, can't decrypt data\n");
			return -1;
		}

		printf("[*] Compressed Size: %d bytes\n", saveBufferLen);
		printf("[*] Decompressed Size: %d bytes\n\n", decompSize);
		memcpy(data + 0x44, decompressedData, decompSize);
		free(decompressedData);
		
		// Update saveBufferLen to reflect the decompressed size
		saveBufferLen = decompSize;
		*(u32*)(data+8) = ES32(saveBufferLen);
		
		// Update save data length (saveBufferLen + HeaderLength)
		saveDataLen = saveBufferLen + HeaderLength;
		*(u32*)(data+0x0C) = ES32(saveDataLen);
	}
	else
	{
		// Try to compress the data before encryption
		uint32_t compressedSize = 0;
		uint8_t* compressedData = lz77_CompressData(data + 0x44, saveBufferLen, &compressedSize);
		
		if (!compressedData)
		{
			printf("[!] ERROR: Compression failed, can't encrypt data\n");
			return -1;
		}

		printf("[*] Compressed Size: %d bytes (from %d bytes)\n", compressedSize, saveBufferLen);
			
		// Check if compressed size fits in the allocated buffer
		// The buffer at data + 0x44 has (len - 0x44) bytes available
		if (compressedSize > (len - 0x44))
		{
			printf("[!] ERROR: Compressed data is larger than available file buffer\n");
			return -1;
		}

		// Copy compressed data back
		memcpy(data + 0x44, compressedData, compressedSize);
		free(compressedData);
		
		// Update the save buffer length
		saveBufferLen = compressedSize;
		*(u32*)(data+8) = ES32(saveBufferLen);
		
		// Update save data length (saveBufferLen + HeaderLength)
		saveDataLen = saveBufferLen + HeaderLength;
		*(u32*)(data+0x0C) = ES32(saveDataLen);

		printf("[*] New Save Buffer Length: %d\n", saveBufferLen);
		printf("[*] New Save Data Length: %d\n", saveDataLen);
		encrypt_data(data+0x18, saveBufferLen, ProfileKey);

		storedSum = addSum(data + 0x44, saveBufferLen);
		*(u32*)(data+0x10) = ES32(storedSum);
		printf("[*] New Checksum: %08X\n\n", storedSum);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
