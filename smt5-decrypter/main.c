/*
*
*	Shin Megami Tensei 5 PS4 Save Decrypter - (c) 2024 by Bucanero - www.bucanero.com.ar
*
*/

#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/sha1.c"

// https://www.rytek.me/projects/smt5decryptorencryptor/
const u8 SMTV_AES_KEY[32] = {
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
		0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66
	};


void decrypt_data(u8* data, u32 size)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, SMTV_AES_KEY);

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
	AES_init_ctx(&ctx, SMTV_AES_KEY);

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

	printf("\nSMT5 PS4 Save Decrypter 0.1.0 - (c) 2024 by Bucanero\n\n");

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
		decrypt_data(data ,  len);
	else
	{
		sha1(data, data + 0x40, (len - 0x40));
		printf("[*] Updated SHA1: " SHA1_FMT(data, "\n"));

		encrypt_data(data ,  len);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
