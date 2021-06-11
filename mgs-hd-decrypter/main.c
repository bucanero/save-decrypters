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

int CalculateCRC32(const u8* data, int offset, int size)
{
    int crc = -1;

    for (int i = offset; i < offset + size; i++)
    {
        crc = crc ^ data[i] << 24 >> 24;
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

u8* Decrypt(const char* key, u8* data, int offset, int size)
{
    u8 num = 0;
    for (int i = offset; i < offset + size; i++)
    {
        u8 num1 = (u8)(data[i] - key[(i - offset) % strlen(key)] - (char)num);
        num = data[i];
        data[i] = num1;
    }
    return data;
}

u8* DecryptLayer2(u8* data, int offset, int size, int salt)
{
    u8 num = 0;
    for (int i = offset; i < offset + size; i++)
    {
//        salt = Routines.GetNextSalt(salt);
        u8 num1 = (u8)(data[i] - salt - num);
        num = data[i];
        data[i] = num1;
    }
    return data;
}

u8* Encrypt(const char* key, u8* data, int offset, int size)
{
    u8 num = 0;
    for (int i = offset; i < offset + size; i++)
    {
        num = (u8)(data[i] + key[(i - offset) % strlen(key)] + (char)num);
        data[i] = num;
    }
    return data;
}

u8* EncryptLayer2(u8* data, int offset, int size, int salt)
{
    u8 num = 0;
    for (int i = offset; i < offset + size; i++)
    {
//        salt = Routines.GetNextSalt(salt);
        num = (u8)(data[i] + salt + num);
        data[i] = num;
    }
    return data;
}

u8* EncodeBase64(u8* data, const char* chars)
{
    int num;
    int num1;
    int j;
    u8* numArray = data;
    u8 num2 = 0;
    for (int i = 0; i < 20; i++)
    {
        num2 = (u8)(num2 ^ data[i]);
    }
    data[20] = num2;
    u8 numArray1[28];
    int num3 = 0;
    int num4 = 0;
    int num5 = 0;
    int num6 = 0;
    for (j = 0; j < 28; j++)
    {
        if (num3 <= 7)
        {
            switch (num3)
            {
                case 0:
                {
                    numArray1[j] = (u8)(numArray[num4] >> 2);
                    break;
                }
                case 1:
                {
                    numArray1[j] = (u8)(numArray[num4] >> 1);
                    break;
                }
                case 2:
                {
                    numArray1[j] = (u8)(numArray[num4] & 63);
                    break;
                }
                case 3:
                {
                    num1 = numArray[num4 + 1];
                    num = numArray[num4];
                    num6 = num1 >> 7;
                    num5 = (num & 31) << 1;
                    numArray1[j] = (u8)(num5 | num6);
                    break;
                }
                case 4:
                {
                    num1 = numArray[num4 + 1];
                    num = numArray[num4];
                    num6 = num1 >> 6;
                    num5 = (num & 15) << 2;
                    numArray1[j] = (u8)(num5 | num6);
                    break;
                }
                case 5:
                {
                    num1 = numArray[num4 + 1];
                    num = numArray[num4];
                    num6 = num1 >> 5;
                    num5 = (num & 7) << 3;
                    numArray1[j] = (u8)(num5 | num6);
                    break;
                }
                case 6:
                {
                    num1 = numArray[num4 + 1];
                    num = numArray[num4];
                    num6 = num1 >> 4;
                    num5 = (num & 3) << 4;
                    numArray1[j] = (u8)(num5 | num6);
                    break;
                }
                case 7:
                {
                    num1 = numArray[num4 + 1];
                    num = numArray[num4];
                    num6 = num1 >> 3;
                    num5 = (num & 1) << 5;
                    numArray1[j] = (u8)(num5 | num6);
                    break;
                }
                case 8:
                {
                    numArray1[j] = (u8)(num5 | num6);
                    break;
                }
            }
        }
        num3 += 6;
        if (num3 >= 8)
        {
            num3 -= 8;
            num4++;
        }
    }
    j = 7;
    num4 = 0;
    j = 0;
    u8* numArray2 = malloc(32);
    numArray2[0] = 68;
    for (int k = 0; k < 7; k++)
    {
        int num7 = j + 21;
        num = numArray1[num4];
        num6 = numArray1[num4 + 1];
        int num8 = num + j;
        num = numArray1[num4 + 2];
        num6 += j;
        num5 = numArray1[num4 + 3];
        num8 &= 63;
        num3 = num + num7;
        num = num6 + 7;
        num6 = num3 - 7;
        num3 = num & 63;
        num = chars[num8];
        num5 += num7;
        num7 = num6 & 63;
        num6 = num5 & 63;
        j += 28;
        numArray2[num4 + 1] = (u8)num;
        num5 = chars[num3];
        numArray2[num4 + 2] = (u8)num5;
        num3 = chars[num7];
        numArray2[num4 + 3] = (u8)num3;
        num7 = chars[num6];
        numArray2[num4 + 4] = (u8)num7;
        num4 += 4;
    }
    return numArray2;
}

u8* DecodeBase64(u8* data, const char* str)
{
    int num;
    int num1;
    int l;
    int num2;
    int num3;
    int num4;
    u8* numArray = data;
/*
    for (int i = 0; i < 28; i++)
    {
        if (numArray[1 + i] == 95)
        {
            numArray[1 + i] = 43;
        }
    }
*/
    u8 numArray1[0x100];
    for (int j = 0; j < 0x100; j++)
    {
        numArray1[j] = 0xff;
    }
//    const char str[] = "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ+-wxyzABCDMNOPQRST";
    for (int k = 0; k < 64; k++)
    {
        numArray1[str[k]] = (u8)k;
    }
    int m = 0;
    u8 numArray2[32];
    for (l = 0; l < 196; l += 7)
    {
        num = numArray1[numArray[1 + m]];
        numArray2[m] = (u8)num;
        if (num == 0xff)
        {
            return NULL;
        }
        num4 = num - l & 63;
        int num5 = m;
        m = num5 + 1;
        numArray2[num5] = (u8)num4;
    }
    u8 numArray3[21];
    l = 0;
    num = 0;
    for (m = 0; m < 21; m++)
    {
        if (l <= 5)
        {
            num1 = numArray2[num];
            num3 = (num1 & 63 >> (l & 31)) << (2 + l & 31);
            numArray3[m] = (u8)num3;
        }
        num1 = ~l + 7;
        l = 0;
        num++;
        if (num1 < 2)
        {
            num4 = numArray2[num];
            int num6 = ~num1 + 3;
            num1 += 6;
            num3 = num4 << (num6 & 31);
            num6 = numArray3[m];
            num++;
            numArray3[m] = (u8)(num3 | num6);
        }
        num1 -= 2;
        if (num1 == 0)
        {
            int num7 = num;
            num = num7 + 1;
            num2 = numArray2[num7];
            l = 0;
            numArray3[m] = (u8)(num2 | numArray3[m]);
        }
        else if (num1 <= 5)
        {
            num2 = numArray2[num];
            l = 6 - num1;
            num3 = (u8)(num2 >> (num1 & 31));
            numArray3[m] = (u8)(num3 | numArray3[m]);
        }
    }
    u8* numArray4 = malloc(sizeof(numArray3));
    memcpy(numArray4, numArray3, sizeof(numArray3));
/*
    num = 0;
    for (int n = 0; n < 20; n++)
    {
        num1 = numArray3[n];
        num2 = num1;
        num = num1 ^ num;
        numArray4[n] = (u8)num1;
    }
*/
    return numArray4;
}

void mgs2_decrypt_data(u8* data, u32 size)
{
	printf("[*] MGS2 Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

    Decrypt(MGS2_KEY, data, 4, 0x71a6);
    DecryptLayer2(data, 4, 0x1598, 0);
    DecryptLayer2(data, 0x15aa, 0x1c00, 0);
    DecryptLayer2(data, 0x31aa, 0x4000, 0);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

u32 mgs2_encrypt_data(u8* data, u32 size)
{
	printf("[*] MGS2 Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

    u32 crc = ES32(CalculateCRC32(data, 4, 0x1598) ^ CalculateCRC32(data, 0x15aa, 0x1c00));
    memcpy(data + 0x15a6L, &crc, sizeof(u32));

    EncryptLayer2(data, 4, 0x1598, 0);
    EncryptLayer2(data, 0x15aa, 0x1c00, 0);
    EncryptLayer2(data, 0x31aa, 0x4000, 0);

    crc = crc32(data + 4, 0x71a6);
    Encrypt(MGS2_KEY, data, 4, 0x71a6);

    printf("[*] New Checksum: %08X\n", crc);
	printf("[*] Encrypted File Successfully!\n\n");
	return (ES32(crc));
}

void mgs3_decrypt_data(u8* data, u32 size)
{
	printf("[*] MGS3 Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

    Decrypt(MGS3_KEY, data, 0, 0x497c);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

u32 mgs3_encrypt_data(u8* data, u32 size)
{
	printf("[*] MGS3 Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

    u32 crc = crc32(data, 0x497c);
    Encrypt(MGS3_KEY, data, 0, 0x497c);

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
	u8* master;
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

		master = DecodeBase64(data, MGS2_ALPHABET);
		write_buffer("MASTER.BIN", master, 21);
		free(master);

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
	    master = EncodeBase64(data, MGS2_ALPHABET);
		write_buffer("MASTER.BIN", master, 32);
		free(master);

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

		master = DecodeBase64(data, MGS3_ALPHABET);
		write_buffer("MASTER.BIN", master, 21);
		free(master);

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
		master = EncodeBase64(data, MGS3_ALPHABET);
		master[0] = 0x5F;
		write_buffer("MASTER.BIN", master, 32);
		free(master);

		printf("[*] %s File Encoded\n", "MASTER.BIN");
	}

	free(bak);
	free(data);
	
	return 0;
}
