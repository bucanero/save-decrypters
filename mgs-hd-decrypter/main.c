/*
*
*	MGS PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original XBOX MGS 2/3 - SecFixers by Philymaster
*
*/

#include "../common/iofile.c"

#define MGS2_KEY            "2MetalOfSolidSonsLibertyGear"
#define MGS3_KEY            "3MetalSolidSnakeEaterGear"
#define MGS2_ALPHABET       "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ_.wxyzABCDMNOPQRST"
#define MGS3_ALPHABET       "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ+-wxyzABCDMNOPQRST"

#define CRC_POLY            0xEDB88320

int CalculateCRC32(const u8* data, int size)
{
    int crc = -1;

    while (size--)
    {
        crc = crc ^ *data++ << 24 >> 24;
        for (int j = 0; j < 8; j++)
        {
            int num = crc & 1;
            crc = crc >> 1 & 0x7fffffff;
            if (num != 0)
            {
                crc ^= CRC_POLY;
            }
        }
    }

    return ~crc;
}

void crc32_fill(uint32_t *table, uint32_t poly)
{
    uint8_t index=0, z;
    do
    {
        table[index]=index;
        for(z=8; z; z--)
        	table[index] = (table[index] & 1) ? (table[index] >> 1)^poly : table[index] >> 1;
    } while(++index);
}

u32 crc32(u8* input, u32 len)
{
	u32 crc_table[256];
	u32 crc = 0xFFFFFFFF;

	crc32_fill(crc_table, CRC_POLY);

	for (int i = 0; i < len; i++)
		crc = (crc >> 8 ^ crc_table[((crc ^ (u32)input[i]) & 0xFF)]);

	return ~crc;
}

void Decrypt(const char* key, u8* data, int size)
{
    int keylen = (key[0] ? strlen(key) : 1);

    for (int i = size - 1; i > 0; i--)
        data[i] -= (key[i % keylen] + data[i-1]);
    
    data[0] -= key[0];
    return;
}

void Encrypt(const char* key, u8* data, int size)
{
    int keylen = (key[0] ? strlen(key) : 1);
    data[0] += key[0];

    for (int i = 1; i < size; i++)
        data[i] += (key[i % keylen] + data[i-1]);

    return;
}

void EncodeBase64(u8* data, const char* chars)
{
    int i, j, k;
    u8 tmpArray[28];

    data[20] = 0;
    for (j = 0; j < 20; j++)
        data[20] ^= data[j];

    for (i = 0, j = 0, k = 0; j < 28; j++)
    {
        if (k == 0 || k == 1)
            tmpArray[j] = (u8)(data[i] >> (2-k));

        else if (k == 2)
            tmpArray[j] = (u8)(data[i] & 63);

        else if (k <= 7)
            tmpArray[j] = (u8)(data[i + 1] >> (10-k)) | ((data[i] & ((1 << (8-k)) - 1)) << (k-2));

        k += 6;
        if (k >= 8)
        {
            k -= 8;
            i++;
        }
    }

    data[0] = 68;
//    for (i = 0, j = 0; i < 28; i++, j += (i % 4) ? 0 : 28)
//        data[i + 1] = (u8)chars[(tmpArray[i] + j +  7 * (i % 4)) & 63];

    for (i = 0, j = 0; i < 28; j += 28, i += 4)
    {
        data[i + 1] = (u8)chars[(tmpArray[i] + j) & 63];
        data[i + 2] = (u8)chars[(tmpArray[i + 1] + j +  7) & 63];
        data[i + 3] = (u8)chars[(tmpArray[i + 2] + j + 14) & 63];
        data[i + 4] = (u8)chars[(tmpArray[i + 3] + j + 21) & 63];
    }

    return;
}

void DecodeBase64(u8* data, const char* str)
{
    int i, j, k, m;
    u8 b64_table[0x100];
    u8 tmpArray[0x20];

    memset(tmpArray, 0, sizeof(tmpArray));
    memset(b64_table, 0xFF, sizeof(b64_table));

    for (k = 0; k < 64; k++)
        b64_table[str[k]] = (u8)k;

    for (j = 0, m = 0; j < 196; j += 7, m++)
    {
        k = b64_table[data[1 + m]];

        if (k == 0xff)
            return;

        data[m] = (u8)((k - j) & 63);
    }

    for (j = 0, k = 0, m = 0; m < 21; m++)
    {
        if (j <= 5)
            tmpArray[m] = (u8)(data[k] & 63 >> (j & 31)) << ((2 + j) & 31);

        i = ~j + 7;
        j = 0;
        k++;

        if (i < 2)
        {
            tmpArray[m] |= (u8)(data[k] << ((~i + 3) & 31));
            i += 6;
            k++;
        }
        i -= 2;

        if (i == 0)
        {
            tmpArray[m] |= (u8)(data[k]);
            k++;
            j = 0;
        }
        else if (i <= 5)
        {
            j = 6 - i;
            tmpArray[m] |= (u8)(data[k] >> (i & 31));
        }
    }
    memcpy(data, tmpArray, sizeof(tmpArray));

    return;
}

void mgs2_decrypt_data(u8* data, u32 size)
{
	printf("[*] MGS2 Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

    Decrypt(MGS2_KEY, data + 4, 0x71a6);

    // Decrypt Layer 2
    Decrypt("", data + 4, 0x1598);
    Decrypt("", data + 0x15aa, 0x1c00);
    Decrypt("", data + 0x31aa, 0x4000);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

u32 mgs2_encrypt_data(u8* data, u32 size)
{
	printf("[*] MGS2 Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

    u32 crc = ES32(CalculateCRC32(data + 4, 0x1598) ^ CalculateCRC32(data + 0x15aa, 0x1c00));
    memcpy(data + 0x15a6L, &crc, sizeof(u32));

    // Encrypt Layer 2
    Encrypt("", data + 4, 0x1598);
    Encrypt("", data + 0x15aa, 0x1c00);
    Encrypt("", data + 0x31aa, 0x4000);

    crc = crc32(data + 4, 0x71a6);
    Encrypt(MGS2_KEY, data + 4, 0x71a6);

    printf("[*] New Checksum: %08X\n", crc);
	printf("[*] Encrypted File Successfully!\n\n");
	return (ES32(crc));
}

void mgs3_decrypt_data(u8* data, u32 size)
{
	printf("[*] MGS3 Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

    Decrypt(MGS3_KEY, data, 0x497c);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

u32 mgs3_encrypt_data(u8* data, u32 size)
{
	printf("[*] MGS3 Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

    u32 crc = crc32(data, 0x497c);
    Encrypt(MGS3_KEY, data, 0x497c);

    printf("[*] New Checksum: %08X\n", crc);
	printf("[*] Encrypted File Successfully!\n\n");
	return (ES32(crc));
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
	char *opt, *bak, type;

	printf("\nMetal Gear Solid HD save decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");
	
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
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	if (memcmp(data + 0x71AA, "MGS2", 4) == 0)
		type = 2;

	if (memcmp(data + 0x497C, "MGS3", 4) == 0)
		type = 3;

	if (*opt == 'd' && type == 2)
	{
		mgs2_decrypt_data(data, len);
		write_buffer(argv[2], data, len);
	    free(data);

		if (read_buffer("MASTER.BIN", &data, &len) != 0)
		{
			printf("[*] Could Not Access The File (%s)\n", "MASTER.BIN");
			return -1;
		}

		write_buffer("MASTER.BIN.bak", data, len);

		DecodeBase64(data, MGS2_ALPHABET);
		write_buffer("MASTER.BIN", data, len);

		printf("[*] %s File Decoded\n", "MASTER.BIN");
	}

	else if (*opt == 'e' && type == 2)
	{
		u32 crc = mgs2_encrypt_data(data, len);
		write_buffer(argv[2], data, len);
	    free(data);
	
		if (read_buffer("MASTER.BIN", &data, &len) != 0)
		{
			printf("[*] Could Not Access The File (%s)\n", "MASTER.BIN");
			return -1;
		}

		write_buffer("MASTER.BIN.bak", data, len);
	
	    memcpy(data + 16, &crc, sizeof(u32));
	    EncodeBase64(data, MGS2_ALPHABET);
		write_buffer("MASTER.BIN", data, len);

		printf("[*] %s File Encoded\n", "MASTER.BIN");
	}

	else if (*opt == 'd' && type == 3)
	{
		mgs3_decrypt_data(data, len);
		write_buffer(argv[2], data, len);
	    free(data);

		if (read_buffer("MASTER.BIN", &data, &len) != 0)
		{
			printf("[*] Could Not Access The File (%s)\n", "MASTER.BIN");
			return -1;
		}

		write_buffer("MASTER.BIN.bak", data, len);

		DecodeBase64(data, MGS3_ALPHABET);
		write_buffer("MASTER.BIN", data, len);

		printf("[*] %s File Decoded\n", "MASTER.BIN");
	}

	else if (*opt == 'e' && type == 3)
	{
		u32 crc = mgs3_encrypt_data(data, len);
		write_buffer(argv[2], data, len);
	    free(data);
	
		if (read_buffer("MASTER.BIN", &data, &len) != 0)
		{
			printf("[*] Could Not Access The File (%s)\n", "MASTER.BIN");
			return -1;
		}

		write_buffer("MASTER.BIN.bak", data, len);

	    memcpy(data + 8, &crc, sizeof(u32));
	    memcpy(data + 16, &crc, sizeof(u32));
		EncodeBase64(data, MGS3_ALPHABET);
		data[0] = 0x5F;
		write_buffer("MASTER.BIN", data, len);

		printf("[*] %s File Encoded\n", "MASTER.BIN");
	}

	free(bak);
	free(data);
	
	return 0;
}
