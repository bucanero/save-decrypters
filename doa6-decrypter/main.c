/*
*
*	Dead or Alive 6 PS4 Save Decrypter - (c) 2026 by Bucanero - www.bucanero.com.ar
*	based on https://github.com/bucanero/apollo-patches/issues/33
*
*/

#define CBC 1

#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/md5.c"

#define DOA6_NAME_LEN	32


void md5_hash(const u8* in, u32 size, u8* out)
{
	MD5_CTX context;

	MD5Init (&context);
	MD5Update (&context, in, size);
	MD5Final (out, &context);
}

// Key and IV only depend on the save file name (e.g. "SYSTEMDATA")
//   R   = MD5(name, zero padded to 32 bytes)
//   KEY = uppercase hex of R, non A-F chars first, then A-F chars (stable)
//   IV  = bytes of R, odd values first, then even values (stable)
void derive_key_iv(const char* name, u8* key, u8* iv)
{
	char x[DOA6_NAME_LEN + 1] = {0};
	char hex[DOA6_NAME_LEN + 1];
	u8 r[16];
	int i, k = 0, v = 0;

	strncpy(x, name, DOA6_NAME_LEN);
	md5_hash((u8*) x, DOA6_NAME_LEN, r);

	for (i = 0; i < 16; i++)
		snprintf(hex + i*2, 3, "%02X", r[i]);

	for (i = 0; i < DOA6_NAME_LEN; i++)
		if (hex[i] < 'A' || hex[i] > 'F')
			key[k++] = hex[i];
	for (i = 0; i < DOA6_NAME_LEN; i++)
		if (hex[i] >= 'A' && hex[i] <= 'F')
			key[k++] = hex[i];

	for (i = 0; i < 16; i++)
		if (r[i] & 1)
			iv[v++] = r[i];
	for (i = 0; i < 16; i++)
		if (!(r[i] & 1))
			iv[v++] = r[i];

	printf("[*] Save Name: %s\n", x);
	printf("[*] AES Key  : %.32s\n", key);
	printf("[*] AES IV   : ");
	for (i = 0; i < 16; i++)
		printf("%02X", iv[i]);
	printf("\n");
}

int decrypt_data(const char* name, u8* data, size_t* size)
{
	u8 key[32], iv[16], pad;
	struct AES_ctx ctx;

	if (!*size || *size % AES_BLOCKLEN)
	{
		printf("[!] Invalid File Size: %zu bytes (not a multiple of 16)\n", *size);
		return 0;
	}

	derive_key_iv(name, key, iv);
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_decrypt_buffer(&ctx, data, *size);

	// PKCS#7 padding
	pad = data[*size - 1];
	if (pad < 1 || pad > AES_BLOCKLEN)
	{
		printf("[!] Invalid Padding (wrong file name?)\n");
		return 0;
	}
	for (int i = 1; i <= pad; i++)
		if (data[*size - i] != pad)
		{
			printf("[!] Invalid Padding (wrong file name?)\n");
			return 0;
		}

	*size -= pad;
	printf("[*] Total Decrypted Size: 0x%zX (%zu bytes)\n", *size, *size);
	printf("[*] Decrypted File Successfully!\n\n");

	return 1;
}

int encrypt_data(const char* name, u8** data, size_t* size)
{
	u8 key[32], iv[16], pad;
	struct AES_ctx ctx;

	// PKCS#7 padding
	pad = AES_BLOCKLEN - (*size % AES_BLOCKLEN);
	*data = realloc(*data, *size + pad);
	memset(*data + *size, pad, pad);
	*size += pad;

	derive_key_iv(name, key, iv);
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_encrypt_buffer(&ctx, *data, *size);

	printf("[*] Total Encrypted Size: 0x%zX (%zu bytes)\n", *size, *size);
	printf("[*] Encrypted File Successfully!\n\n");

	return 1;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename [savename]\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt File\n");
	printf(" -e            Encrypt File\n\n");
	printf("The save name (default: file name up to the first '.', e.g. SYSTEMDATA)\n");
	printf("is used to derive the AES key and IV.\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	char *opt, *bak, *name, *ptr;
	int ret;

	printf("\nDead or Alive 6 PS4 Save Decrypter 0.1.0 - (c) 2026 by Bucanero\n\n");

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

	if (argc > 2)
		name = strdup(argv[3]);
	else
	{
		ptr = strrchr(argv[2], '/');
		if (!ptr)
			ptr = strrchr(argv[2], '\\');
		name = strdup(ptr ? ptr + 1 : argv[2]);
		if ((ptr = strchr(name, '.')) != NULL)
			*ptr = 0;
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
		ret = decrypt_data(name, data, &len);
	else
		ret = encrypt_data(name, &data, &len);

	if (ret)
		write_buffer(argv[2], data, len);

	free(name);
	free(bak);
	free(data);

	return ret ? 0 : -1;
}
