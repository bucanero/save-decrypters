/*
*
*	Max Payne 3 PS3 Save Decrypter - (c) 2025 by Red-EyeX32/Bucanero
*
* This tool is based on the original decrypter by Red-EyeX32
*
*/

#include <stdbool.h>
#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/sha1.c"
#include "../common/hmac-sha1.c"


#define PROFILE_KEY_SIZE 0x32

// Max Payne 3 Save data encryption key
const uint8_t EncAesKey[0x20] = {
	0x1A, 0xB5, 0x6F, 0xED, 0x7E, 0xC3, 0xFF, 0x01, 0x22, 0x7B, 0x69, 0x15, 0x33, 0x97, 0x5D, 0xCE,
	0x47, 0xD7, 0x69, 0x65, 0x3F, 0xF7, 0x75, 0x42, 0x6A, 0x96, 0xCD, 0x6D, 0x53, 0x07, 0x56, 0x5D
};

// Create the save key
// PlayStation: PS35675hA
// XBOX: XEN43156A
// PC: PCgh64rwA

const uint8_t hmacBuffer1[0x10] = { 0x0F, 0xC9, 0x19, 0xE8, 0x9A, 0x17, 0xC4, 0x5F, 0xE7, 0x16, 0xD4, 0x6C, 0x3A, 0x15, 0x9C, 0x75 };
const uint8_t hmacBuffer2[0x10] = { 0xE1, 0x09, 0xA5, 0x42, 0xF6, 0x0A, 0x13, 0x3B, 0x81, 0xAC, 0x02, 0x55, 0xCC, 0x39, 0x40, 0x1B };
const uint8_t hmacBuffer3[0x10] = { 0x15, 0x08, 0xE9, 0x6F, 0x47, 0xB8, 0x47, 0xD1, 0x3A, 0x65, 0x8C, 0x71, 0x00, 0x00, 0x00, 0x00 };


uint8_t* Mp3_HmacSha(const uint8_t* pbKey, int cbKey, const uint8_t* pbInp, int cbInp, int cbOut, int cbRounds, int pbKey_Length)
{
	if (cbOut == 0x00)
		return NULL;

	uint8_t* hash = malloc(cbOut);
	int idx = 1, len = cbOut, offset = 0;

	do
	{
//		uint8_t pbData1[0x04];
//		pbData1.WriteInt32(0, idx);
		uint32_t pbData1 = ES32(idx);
		uint8_t sKey[cbKey];

		if (pbKey_Length < cbKey)
		{
//			sKey = malloc(cbKey);
//			Array.Copy(pbKey, 0, sKey, 0, cbKey);
			memset(sKey, 0, cbKey);
			memcpy(sKey, pbKey, pbKey_Length);
		}
		else
		{
//			sKey = pbKey;
			memcpy(sKey, pbKey, cbKey);
		}
//		var sha = new HMACSHA1(sKey);
//		sha.TransformBlock(pbInp, 0, cbInp, null, 0x00);
//		sha.TransformFinalBlock(pbData1, 0, 0x04);
		uint8_t tmpHash[20]; //= sha.Hash;
		uint8_t digest[20]; //= tmpHash;

		uint8_t buftmp[cbInp + 4];
		memcpy(buftmp, pbInp, cbInp);
		memcpy(buftmp + cbInp, &pbData1, 4);
		hmac_sha1(tmpHash, sKey, cbKey, buftmp, cbInp + 4);
		memcpy(digest, tmpHash, sizeof(digest));

		if (cbRounds > 1)
		{
			int rounds = cbRounds - 1;
			do
			{
				hmac_sha1(digest, sKey, cbKey, digest, 0x14);

				int j = 0;
				for (int x = 0; x < 4; x++)
				{
					int a = digest[j];
					int b = tmpHash[j];
					int c = tmpHash[j + 1];
					int d = digest[j + 1];
					int e = tmpHash[j + 2];
					a ^= b;
					int f = tmpHash[j + 3];
					b = digest[j + 3];
					d ^= c;
					int g = digest[j + 2];
					int h = tmpHash[j + 4];
					b ^= f;
					c = digest[j + 4];
					g ^= e;
					tmpHash[j] = (uint8_t)(a);
					a = b & 0xFF;
					h ^= c;
					tmpHash[j + 1] = (uint8_t)(d);
					int i = g & 0xFF;
					tmpHash[j + 3] = (uint8_t)a;
					d = h & 0xFF;
					tmpHash[j + 2] = (uint8_t)i;
					tmpHash[j + 4] = (uint8_t)d;
					j += 5;
				}

			} while (--rounds != 0);
		}

		int dlen = len < 0x14 ? len : 0x14;

		idx++;
		memcpy(hash + offset, tmpHash, dlen);
		offset += dlen;
		len -= dlen;

	} while (len != 0);

	return hash;
}

uint8_t* Mp3_AesEcb(const uint8_t* key, uint8_t* pbInp, int cbInp, bool encrypt)
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

	return pbInp;
}

uint8_t* Mp3_AesEcb2(const uint8_t* key, uint8_t* pbInp, int cbInp, bool encrypt)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, key);

//	int dataLen = pbInp_Length & ~0x0F;
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
		for (int i = 0; i < 16; i++)
		{
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
	return pbInp;
}

uint8_t* Mp3_AesEcb3(const uint8_t* key, uint8_t* pbInp, int cbInp, bool encrypt)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, key);

//	int dataLen = pbInp_Length & ~0x0F;
	int dataLen = cbInp & ~0x0F;

	if (dataLen > 0)
	{
		for (int i = 0; i < 16; i++)
		{
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
	return pbInp;
}

void decrypt_data(uint8_t* saveBuffer, u32 saveLen, const char* ProfileKey)
{
    printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", saveLen, saveLen);

	uint8_t pbData1[0x2C];
	uint8_t saveData[saveLen + 4];

	u32 pbSaveDataLen = ES32(saveLen); // big endian

	memcpy(pbData1, saveBuffer, 0x2C);
	memcpy(saveData, saveBuffer + 0x2C, saveLen);
	memcpy(saveData + saveLen, &pbSaveDataLen, 4);

	uint8_t digestKey[20];

	hmac_sha1(digestKey, ProfileKey, PROFILE_KEY_SIZE, saveData, saveLen + 4);
//	hmac_sha1(digestKey, ProfileKey, strlen(ProfileKey), saveData, saveLen + 4);

	printf("[*] SHA1 HMAC: " SHA1_FMT(digestKey, "\n"));


	uint8_t* aesKey = Mp3_HmacSha(digestKey, 0x14, hmacBuffer1, 0x10, 0x20, 0x7D0, sizeof(digestKey));

	uint8_t* digestKey2 = Mp3_AesEcb(aesKey, pbData1, 0x2C, false);

	uint8_t* hmacDigest = Mp3_HmacSha(digestKey2, 0x2C, hmacBuffer2, 0x10, 0x20, 0x7D0, sizeof(pbData1)); //sizeof(digestKey2)

	uint8_t* hmacDigest2 = Mp3_AesEcb(EncAesKey, hmacDigest, 0x20, true);

	Mp3_AesEcb2(hmacDigest2, saveData, saveLen, false); //sizeof(saveData)

	// verifying the data
	uint8_t sha1key[20];
	uint8_t tmpbuf[PROFILE_KEY_SIZE + 4];
	memcpy(tmpbuf, ProfileKey, PROFILE_KEY_SIZE);
	memcpy(tmpbuf + PROFILE_KEY_SIZE, &pbSaveDataLen, 4);

	sha1(sha1key, tmpbuf, sizeof(tmpbuf));

	uint8_t hmacDigest3[0x20];
	hmac_sha1(hmacDigest3, sha1key, sizeof(sha1key), saveData, saveLen);
	memcpy(hmacDigest3 + 0x14, digestKey2 + 0x20, 4);
	memcpy(hmacDigest3 + 0x18, digestKey2 + 0x24, 8);

	uint8_t hmacBuffer[0x10];
	memcpy(hmacBuffer, hmacBuffer3, 0x10);
	memcpy(hmacBuffer + 0x0C, digestKey2 + 0x20, 4);

	uint8_t* aesKey2 = Mp3_HmacSha(hmacDigest3, 0x20, hmacBuffer, 0x10, 0x20, 0x7D0, sizeof(hmacDigest3));

	uint8_t pbData2[0x20];
	memcpy(pbData2, digestKey2, 0x20);

	Mp3_AesEcb(EncAesKey, pbData2, 0x20, false);
	Mp3_AesEcb(aesKey2, pbData2, 0x20, false);

	if (memcmp(pbData2, hmacDigest3, 0x20) != 0)
	{
		printf("[!] save data could not be verified.\n");
	}

	free(aesKey);
	free(hmacDigest);
	free(aesKey2);

	memcpy(saveBuffer + 0x2C, saveData, saveLen);

    printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(void* data, u32 size)
{
    printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);


    printf("[*] Encrypted File Successfully!\n\n");
	return;
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
	u32 Version, HeaderLength, saveBufferLen, saveDataLen;
	char *opt, *bak;

	printf("\nmax-payne3-decrypter 0.1.0 - (c) 2025 by Red-EyeX32 & Bucanero\n\n");

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
	
	int storedSum = ES32(*(u32*)(data+0x10));
	int sum = 0;

	Version = ES32(*(u32*)(data+0x14));

	for (int x = 0; x < saveBufferLen; x++)
		sum += data[0x44 + x];

	if (sum != storedSum)
	{
//		MaxPayne3Exception("save has been tampered with!");
		printf("[*] Invalid checksum detected.\n");
		free(data);
		return -1;
	}

	printf("[*] Header Length: %d\n", HeaderLength);
	printf("[*] Save Buffer Length: %d\n", saveBufferLen);
	printf("[*] Save Data Length: %d\n", saveDataLen);
	printf("[*] Version: %d\n", Version);

	// Create the save key
	// PlayStation: PS35675hA
	// XBOX: XEN43156A
	// PC: PCgh64rwA
	char ProfileKey[PROFILE_KEY_SIZE];

	memset(ProfileKey, 0, sizeof(ProfileKey));
	snprintf(ProfileKey, sizeof(ProfileKey), "PS35675hA%02d%d", HeaderLength, Version);

	printf("[*] Profile Key: %s\n", ProfileKey);

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	if (*opt == 'd')
		decrypt_data(data+0x18, saveBufferLen, ProfileKey);
	else
	{
//		encrypt_data(data+8, dsize);
	}

	write_buffer("out.bin", data, len);
//	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
