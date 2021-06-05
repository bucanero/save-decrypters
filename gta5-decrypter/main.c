/*
*
*	GTA 5 PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
*/

#include "../common/iofile.c"
#include "../common/aes.c"

const u8 GTAV_PS3_KEY[32] = {
		0x16, 0x85, 0xFF, 0xA3, 0x8D, 0x01, 0x0F, 0x0D, 0xFE, 0x66, 0x1C, 0xF9, 0xB5, 0x57, 0x2C, 0x50,
		0x0D, 0x80, 0x26, 0x48, 0xDB, 0x37, 0xB9, 0xED, 0x0F, 0x48, 0xC5, 0x73, 0x42, 0xC0, 0x22, 0xF5
	};


void decrypt_data(u8* data, u32 size)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, GTAV_PS3_KEY);

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
	AES_init_ctx(&ctx, GTAV_PS3_KEY);

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

	printf("\nGTA5-ps3save-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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
		decrypt_data(data, len);
	else
		encrypt_data(data, len);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
