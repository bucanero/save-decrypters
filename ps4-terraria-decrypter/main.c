/*
*	Terraria PS4 Decrypter / Unpacker - (c) 2024 by hzh
*	https://github.com/hzhreal/
*/

#define CBC 1
#define AES128 1
#define AES256 0

#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/miniz.h"

const u8 SECRET_KEY[] = {
    0x68, 0x00, 0x33, 0x00, 0x79, 0x00, 0x5F, 0x00,
    0x67, 0x00, 0x55, 0x00, 0x79, 0x00, 0x5A, 0x00
};
const u8 COMP_MAGIC[] = {
	0x7E, 0x22, 0xA2, 0x1A
};
const u8 DEC_MAGIC[] = {
	'r', 'e', 'l', 'o', 'g', 'i', 'c'
};

int decompress_data(u8 **data, size_t *len)
{
	if (*len < sizeof(COMP_MAGIC))
		return -1;
	if ( memcmp(*data, &COMP_MAGIC, sizeof(COMP_MAGIC)) != 0 )
		return 0;

	u32 size;
	int n = sizeof(COMP_MAGIC) + sizeof(size);
	if (*len < n)
		return -2;
	size = U_U32_LE(*data, sizeof(COMP_MAGIC));

	size_t in_len = *len - n;
	size_t out_len;
	void *out = tinfl_decompress_mem_to_heap(
		*data + n, in_len,
		&out_len, TINFL_FLAG_PARSE_ZLIB_HEADER
	);
	if (out == NULL)
		return -3;
	if (out_len != size) {
		free(out);
		return -4;
	}

	void *p = realloc(*data, size);
	if (p == NULL) {
		free(out);
		return -5;
	}
	*data = p;
	*len = size;
	memcpy(*data, out, size);
	free(out);

	printf("[*] Decompressed %zu bytes into %" PRIu32 " bytes.\n", in_len, size);
	return 0;
}

int compress_data(u8 **data, size_t *len)
{
	u32 size = *len;
	if (size != *len)
		return -1;

	size_t out_len;
	void *out = tdefl_compress_mem_to_heap(*data, size, &out_len, TDEFL_WRITE_ZLIB_HEADER);
	int n = sizeof(COMP_MAGIC) + sizeof(size);
	if (out == NULL)
		return -2;
	if (out_len >= SIZE_MAX - n)
		return -3;

	void *p = realloc(*data, n + out_len);
	if (p == NULL) {
		free(out);
		return -4;
	}
	*data = p;
	*len = n + out_len;
	memcpy(*data, &COMP_MAGIC, sizeof(COMP_MAGIC));
	P_U32_LE(*data, sizeof(COMP_MAGIC), size);
	memcpy(*data + n, out, out_len);
	free(out);

	printf("[*] Compressed %" PRIu32 " bytes into %zu bytes.\n", size, out_len);
	return 0;
}

int decrypt_data(u8 *data, size_t size)
{
	if (size < 4 + sizeof(DEC_MAGIC))
		return -1;
	if ( memcmp(data + 4, &DEC_MAGIC, sizeof(DEC_MAGIC)) == 0 )
		return 0;
	if (size % AES_BLOCKLEN != 0)
		return -2;

	size_t i = size;
	u8 pad[AES_BLOCKLEN] = {0};
	for (size_t j = size; j > AES_BLOCKLEN - 1; j -= AES_BLOCKLEN) {
		if ( memcmp(data + (j - AES_BLOCKLEN), pad, AES_BLOCKLEN) != 0 )
			break;
		i -= AES_BLOCKLEN;
	}
	if (i == 0)
		return 0;

	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, SECRET_KEY, SECRET_KEY);

	printf("[*] Total decrypted size is 0x%lX (%zu bytes).\n", i, i);

	AES_CBC_decrypt_buffer(&ctx, data, i);

	printf("[*] Decrypted file successfully.\n\n");
	return 0;
}

int encrypt_data(u8 *data, size_t size)
{
	if (size % AES_BLOCKLEN != 0)
		return -1;

	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, SECRET_KEY, SECRET_KEY);

	printf("[*] Total encrypted size is 0x%lX (%zu bytes).\n", size, size);

	AES_CBC_encrypt_buffer(&ctx, data, size);

	printf("[*] Encrypted file successfully.\n\n");
	return 0;
}

void print_usage(const char *argv0)
{
	printf("USAGE: %s [option] [type] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt/Decompress File\n");
	printf(" -e            Encrypt/Compress File\n\n");
	printf("TYPES          Explanation:\n");
	printf(" 1             .plr\n");
	printf(" 2             .wld\n\n");
}

int main(int argc, char **argv)
{
	u8 *data;
	size_t len;
	char *opt;
	char type;
	char *filename;
	char *bak;

	printf("\nTerraria (PS4) decrypter\n\n");

	if (argc - 1 < 3) {
		print_usage(argv[0]);
		return 1;
	}

	opt = argv[1];
	if (*opt++ != '-' || (*opt != 'd' && *opt != 'e')) {
		print_usage(argv[0]);
        return 1;
	}

	type = *argv[2];
	if (type != '1' && type != '2') {
		print_usage(argv[0]);
		return 1;
	}

	filename = argv[3];
	if (read_buffer(filename, &data, &len) != 0) {
		printf("[!] Could not access the file (%s).\n", filename);
		return 1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", filename);
	write_buffer(bak, data, len);
	free(bak);

	if (*opt == 'd') {
		int ret = decompress_data(&data, &len);
		if (ret < 0) {
			free(data);
			if (ret == -5)
				printf("[!] Memory allocation error.\n");
			else
				printf("[!] Failed to decompress.\n");
			return 1;
		}

		if ( decrypt_data(data, len) < 0 ) {
			free(data);
			printf("[!] Failed to decrypt.\n");
			return 1;
		}

		goto success;
	}

	switch (type) {
		case '1':
			if ( encrypt_data(data, len) < 0 ) {
				free(data);
				printf("[!] Failed to encrypt.\n");
				return 1;
			}
			break;

		case '2': {
			int ret = compress_data(&data, &len);
			if (ret < 0) {
				free(data);
				if (ret == -4)
					printf("[!] Memory allocation error.\n");
				else
					printf("[!] Failed to compress.\n");
				return 1;
			}
			break;
		}
	}

success:
	if ( write_buffer(filename, data, len) != 0 ) {
		free(data);
		printf("[!] Failed to write the file: %s.\n", filename);
		return 1;
	}
	free(data);
	return 0;
}

