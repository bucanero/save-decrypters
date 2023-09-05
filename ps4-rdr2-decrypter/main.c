/*
*
*	RDR2 PS4 Save Decrypter - (c) 2023 by Bucanero - www.bucanero.com.ar
*
*/

#include "../common/iofile.c"
#include "../common/aes.c"

// same as GTA5 PS3 Key
const u8 RDR2_PS4_KEY[32] = {
		0x16, 0x85, 0xFF, 0xA3, 0x8D, 0x01, 0x0F, 0x0D, 0xFE, 0x66, 0x1C, 0xF9, 0xB5, 0x57, 0x2C, 0x50,
		0x0D, 0x80, 0x26, 0x48, 0xDB, 0x37, 0xB9, 0xED, 0x0F, 0x48, 0xC5, 0x73, 0x42, 0xC0, 0x22, 0xF5
	};

int search_data(const u8* data, size_t size, int start, const char* search, int len)
{
	for (size_t i = start; i <= (size-len); i++)
		if (memcmp(data + i, search, len) == 0)
			return i;

    return -1;
}

// https://github.com/Zhaxxy/rdr2_enc_dec/blob/main/rdr2_enc_dec.py#L10
uint32_t rockstar_chks(const char* data, int len)
{
    uint32_t checksum = 0x3FAC7125;

    while (len--)
    {
        checksum = ((checksum + (signed char) *data++) * 0x401) & 0xFFFFFFFF;
        checksum = (checksum >> 6 ^ checksum) & 0xFFFFFFFF;
    }
    checksum = (checksum*9) & 0xFFFFFFFF;
    
    return (((checksum >> 11 ^ checksum) * 0x8001) & 0xFFFFFFFF);
}

void decrypt_data(u8* data, u32 size)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, RDR2_PS4_KEY);

    printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	for (int i = 0; i < size; i+= AES_BLOCKLEN)
	{
		AES_ECB_decrypt(&ctx, data + i);
	}

    printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, RDR2_PS4_KEY);

    printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	for (int i = 0; i < size; i+= AES_BLOCKLEN)
	{
		AES_ECB_encrypt(&ctx, data + i);
	}

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
	char *opt, *bak;

	printf("\nRDR2 PS4 Save Decrypter 0.1.0 - (c) 2023 by Bucanero\n\n");

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

	if (*opt == 'd')
		decrypt_data(data + 0x120, len - 0x120);
	else
	{
		uint32_t chks, chks_len;
		int chks_off = search_data(data, len, 0, "CHKS", 5);
	
		if (chks_off < 0)
		{
			printf("[!] CHKS Header Not Found!\n>>> Aborting... (Not a decrypted RDR2 save?)\n\n");
			return -1;
		}

		while (chks_off > 0)
		{
			chks     = ES32(*(uint32_t*)(data + chks_off + 4));
			chks_len = ES32(*(uint32_t*)(data + chks_off + 8));
		
			if (chks != 0x14)
				printf(" ! CHKS Header Mismatch!\n   > Expected: %08X\n   > Detected: %08X\n\n", 0x14, chks);
	
			printf(" - CHKS Offset : 0x%X\n", chks_off);
			printf(" - CHKS Size   : 0x%X (%d bytes)\n", chks_len, chks_len);
			printf(" - Old Checksum: %08X\n", ES32(*(uint32_t*)(data + chks_off + 0xC)));
		
			memset(data + chks_off + 8, 0, 8);
			chks = rockstar_chks((char*) data + (chks_off - chks_len + 0x14), chks_len);
			printf(" + New Checksum: %08X\n\n", chks);
	
			chks = ES32(chks);
			chks_len = ES32(chks_len);
			memcpy(data + chks_off + 0xC, &chks, sizeof(uint32_t));
			memcpy(data + chks_off + 0x8, &chks_len, sizeof(uint32_t));

			chks_off = search_data(data, len, chks_off+1, "CHKS", 5);
		}

		encrypt_data(data + 0x120, len - 0x120);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
