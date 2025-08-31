/*
*
*	Dark Souls Remastered Decrypter - (c) 2025 by Bucanero - www.bucanero.com.ar
*	based on https://github.com/alfizari/Dark-Souls-Remastered-Save-decrypt-PS4 by alfizari
*/

#define CBC 1
#define AES128 1
#define AES256 0

#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/md5.c"


const unsigned char DSR_AES128CBC_KEY[] = "\x20\xEC\x4B\x75\x19\xC2\xBD\x15\xE7\x0C\x1E\xE4\xB2\x04\xB8\xCB";


void md5_hash(const u8* in, u32 size, u8* out)
{
	MD5_CTX context;

	MD5Init (&context);
	MD5Update (&context, in, size);
	MD5Final (out, &context);
}

void decrypt_data(u8* data, size_t size)
{
	u8 iv[16];
	struct AES_ctx ctx;

	printf("[*] Total Decrypted Size: %zu bytes\n", size);

	md5_hash(data, size - 0x10, iv);
	if (memcmp(data + size - 0x10, iv, 0x10) == 0)
		printf("[*] MD5 Hash OK!\n");
	else
		printf("[*] MD5 Hash Error!\n");

	memcpy(iv, data, 0x10);
	AES_init_ctx_iv(&ctx, DSR_AES128CBC_KEY, iv);
	AES_CBC_decrypt_buffer(&ctx, data + 0x10, size - 0x20);

	printf("[*] Decrypted File Successfully!\n\n");

	return;
}

void encrypt_data(u8* data, size_t size)
{
	u8 iv[16];
	struct AES_ctx ctx;

	printf("[*] Total Encrypted Size: %zu bytes\n", size);

	memcpy(iv, data, 0x10);
	AES_init_ctx_iv(&ctx, DSR_AES128CBC_KEY, iv);
	AES_CBC_encrypt_buffer(&ctx, data + 0x10, size - 0x20);

	printf("[*] Encrypted File Successfully!\n");
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

	printf("\nDark Souls Remastered Decrypter 0.1.0 - (c) 2025 by Bucanero\n\n");

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
	{
		encrypt_data(data, len);
		md5_hash(data, len - 0x10, data + len - 0x10);
		printf("[*] MD5 Hash Updated!\n\n");
	}

//	write_buffer(argv[2], data, len);
	write_buffer("out.bin", data, len);

	free(bak);
	free(data);
	
	return 0;
}
