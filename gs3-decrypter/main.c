/*
*	Goat Simulator 3 Decrypter - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/aes.c"
#include "../common/iofile.c"
#include "../common/miniz.h"

const u8 SECRET_KEY[32] = {
	0x4C, 0x08, 0x54, 0x71, 0x3A, 0x51, 0xA1, 0x04,
	0xFE, 0x9B, 0x1F, 0x75, 0x22, 0x75, 0xD2, 0x36,
	0x4F, 0x60, 0x06, 0x44, 0xB6, 0xDE, 0x4F, 0x54,
	0x73, 0xDB, 0x5B, 0x92, 0x27, 0x3E, 0xC0, 0xAF,
};

#define HEADER_SIZE 16

int decrypt_data(u8 **data, size_t *len)
{
	if (*len < HEADER_SIZE)
		return -1;
	u32 cipher_size = U_U32_LE(*data, 12);
	if (cipher_size % AES_BLOCKLEN != 0)
		return -2;
	if ((u64)HEADER_SIZE + (u64)cipher_size != *len)
		return -3;
	printf("[*] Total decrypted size is %" PRIu32 " bytes.\n", cipher_size);

	struct AES_ctx ctx;
	AES_init_ctx(&ctx, SECRET_KEY);
	for (size_t i = HEADER_SIZE; i < HEADER_SIZE + cipher_size; i += AES_BLOCKLEN)
		AES_ECB_decrypt(&ctx, *data + i);

	u8 p = (*data)[*len - 1];
	if (p == 0 || p > AES_BLOCKLEN)
		return -4;
	if (*len < p)
		return -5;
	size_t l = *len - p;
	for (size_t i = l; i < *len; i++) {
		if ((*data)[i] != p)
			return -6;
	}

	void *tmp = realloc(*data, l);
	if (tmp == NULL)
		return -7;
	*data = tmp;
	*len = l;

	printf("[*] Decrypted file successfully.\n\n");
	return 0;
}

int encrypt_data(u8 **data, size_t *len)
{
	if (*len < HEADER_SIZE)
		return -1;
	u8 p = (AES_BLOCKLEN - ((*len - HEADER_SIZE) % AES_BLOCKLEN));
	if (*len - HEADER_SIZE >= SIZE_MAX - p)
		return -2;
	size_t l = *len - HEADER_SIZE + p;
	if (l > UINT32_MAX)
		return -3;
	P_U32_LE(*data, 12, l);
	printf("[*] Total encrypted size is %zu bytes.\n", l);

	void *tmp = realloc(*data, *len + p);
	if (tmp == NULL)
		return -4;
	memset(*data + *len, p, p);
	*len += p;

	struct AES_ctx ctx;
	AES_init_ctx(&ctx, SECRET_KEY);
	for (size_t i = HEADER_SIZE; i < HEADER_SIZE + l; i += AES_BLOCKLEN)
		AES_ECB_encrypt(&ctx, *data + i);

	printf("[*] Encrypted file successfully.\n\n");
	return 0;
}

int decompress_data(u8 **data, size_t *len)
{
	if (*len < HEADER_SIZE)
		return -1;
	u32 decomp_size = U_U32_LE(*data,  8);
	u32 cipher_size = U_U32_LE(*data, 12);
	void *out;
	size_t out_len;

	out = tinfl_decompress_mem_to_heap(
		*data + HEADER_SIZE, *len - HEADER_SIZE,
		&out_len,
		TINFL_FLAG_PARSE_ZLIB_HEADER
	);
	if (out == NULL)
		return -2;
	if (out_len != decomp_size) {
		free(out);
		return -3;
	}

	void *p = realloc(*data, HEADER_SIZE + out_len);
	if (p == NULL) {
		free(out);
		return -4;
	}
	*data = p;
	*len = HEADER_SIZE + out_len;
	memcpy(*data + HEADER_SIZE, out, out_len);
	free(out);

	printf("[*] Decompressed %" PRIu32 " bytes into %" PRIu32 " bytes.\n", cipher_size, decomp_size);
	return 0;
}

int compress_data(u8 **data, size_t *len)
{
	if (*len < HEADER_SIZE)
		return -1;
	u32 decomp_size = *len - HEADER_SIZE;
	P_U32_LE(*data, 8, decomp_size);

	void *out;
	size_t out_len;
	out = tdefl_compress_mem_to_heap(
		*data + HEADER_SIZE, decomp_size,
		&out_len,
		TDEFL_WRITE_ZLIB_HEADER
	);
	if (out == NULL) {
		return -1;
	}
	if (out_len >= SIZE_MAX - HEADER_SIZE) {
		free(out);
		return -2;
	}

	void *p = realloc(*data, HEADER_SIZE + out_len);
	if (p == NULL) {
		free(out);
		return -3;
	}
	*data = p;
	memcpy(*data + HEADER_SIZE, out, out_len);
	free(out);
	*len = HEADER_SIZE + out_len;

	printf("[*] Compressed %" PRIu32 " bytes into %zu bytes.\n", decomp_size, out_len);
	return 0;
}

void print_usage(const char *argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt File\n");
	printf(" -e            Encrypt File\n\n");
}

int main(int argc, char **argv)
{
	u8 *data;
	size_t len;
	char *opt;
	char *filename;
	char *bak;

	printf("\nGoat Simulator 3 decrypter\n\n");

	if (argc - 1 < 2) {
		print_usage(argv[0]);
		return 1;
	}
	opt = argv[1];
	if (*opt++ != '-' || (*opt != 'd' && *opt != 'e')) {
		print_usage(argv[0]);
		return 1;
	}
	filename = argv[2];

	if (read_buffer(filename, &data, &len) != 0) {
		printf("[*] Could not access the file: (%s).\n", filename);
		return 1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", filename);
	write_buffer(bak, data, len);
	free(bak);

	if (*opt == 'd') {
		int ret = decrypt_data(&data, &len);
		if (ret < 0) {
			free(data);
			if (ret == -7)
				printf("[!] Memory allocation error.\n");
			else
				printf("[!] Failed to decrypt.\n");
			return 1;
		}
		ret = decompress_data(&data, &len);
		if (ret < 0) {
			free(data);
			if (ret == -4)
				printf("[!] Memory allocation error.\n");
			else
				printf("[!] Failed to decompress.\n");
			return 1;
		}
	}
	else {
		int ret = compress_data(&data, &len);
		if (ret < 0) {
			free(data);
			if (ret == -3)
				printf("[!] Memory allocation error.\n");
			else
				printf("[!] Failed to compress.\n");
			return 1;
		}
		ret = encrypt_data(&data, &len);
		if (ret < 0) {
			free(data);
			if (ret == -4)
				printf("[!] Memory allocation error.\n");
			else
				printf("[!] Failed to encrypt.\n");
			return 1;
		}
	}

	if (write_buffer(filename, data, len) != 0) {
		free(data);
		printf("[!] Failed to write the file: %s.\n", filename);
		return 1;
	}
	free(data);

	return 0;
}

