/*
*
*	Call of Duty Black Ops PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original black_ops_save_decrypter by Red-EyeX32
*
*/

#define MBEDTLS_DES_C
#define MBEDTLS_CIPHER_MODE_CBC
#define mbedtls_platform_zeroize(b, len)		memset(b, 0, len)

#include "../common/iofile.c"
#include "../common/des.c"

#define COD_DES3_KEY    "Md8ea20lPcftYwsl496q63x9"
#define COD_DES3_IV     "0Peyx825"


void decrypt_data(u8* data, u32 size)
{
    u8 iv[MBEDTLS_DES_KEY_SIZE];
    mbedtls_des3_context ctx3;

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	memcpy(iv, COD_DES3_IV, MBEDTLS_DES_KEY_SIZE);
	mbedtls_des3_init(&ctx3);
	mbedtls_des3_set3key_dec(&ctx3, (u8*) COD_DES3_KEY);

	mbedtls_des3_crypt_cbc(&ctx3, MBEDTLS_DES_DECRYPT, size, iv, data, data);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
    u8 iv[MBEDTLS_DES_KEY_SIZE];
    mbedtls_des3_context ctx3;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	memcpy(iv, COD_DES3_IV, MBEDTLS_DES_KEY_SIZE);
	mbedtls_des3_init(&ctx3);
	mbedtls_des3_set3key_enc(&ctx3, (u8*) COD_DES3_KEY);

	mbedtls_des3_crypt_cbc(&ctx3, MBEDTLS_DES_ENCRYPT, size, iv, data, data);

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

	printf("\ncod-blackops-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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
