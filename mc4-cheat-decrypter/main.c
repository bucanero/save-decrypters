/*
*
*	MC4 Cheat Decrypter - (c) 2023 by Bucanero - www.bucanero.com.ar
*
*/

#define CBC 1

#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/base64.c"

const unsigned char MC4_AES256CBC_KEY[] = "304c6528f659c766110239a51cl5dd9c";
const unsigned char MC4_AES256CBC_IV[]  = "u@}kzW2u[u(8DWar";


u8* decrypt_data(u8* data, size_t* size)
{
	u8* bin_data;
	size_t bin_size;
	struct AES_ctx ctx;

	printf("[*] Base64 Encoded Size: %zu bytes\n", *size);

	bin_data = base64_decode(data, *size, &bin_size);
	if (!bin_data)
	{
		printf("Base64 Error!\n");
		return data;
	}

	*size = bin_size;
	free(data);

	printf("[*] Total Decrypted Size: %zu bytes\n", bin_size);

	AES_init_ctx_iv(&ctx, MC4_AES256CBC_KEY, MC4_AES256CBC_IV);
	AES_CBC_decrypt_buffer(&ctx, bin_data, bin_size);

	printf("[*] Decrypted File Successfully!\n\n");
	return bin_data;
}

u8* encrypt_data(u8* data, size_t* size)
{
	u8* b64_data;
	size_t b64_size;
	struct AES_ctx ctx;

	printf("[*] Total XML Size: %zu bytes\n", *size);

	AES_init_ctx_iv(&ctx, MC4_AES256CBC_KEY, MC4_AES256CBC_IV);
	AES_CBC_encrypt_buffer(&ctx, data, *size);

	b64_data = base64_encode(data, *size, &b64_size);
	if (!b64_data)
	{
		printf("Base64 Error!\n");
		return data;
	}

	*size = b64_size;
	free(data);

	printf("[*] Total Encrypted Size: %zu bytes\n", b64_size);
	printf("[*] Encrypted File Successfully!\n\n");
	return b64_data;
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

	printf("\nMC4 Cheat Decrypter 0.1.0 - (c) 2023 by Bucanero\n\n");

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
		data = decrypt_data(data, &len);
	else
		data = encrypt_data(data, &len);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
