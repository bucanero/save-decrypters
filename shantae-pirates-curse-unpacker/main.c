/*
*	Shantae and the Pirate's Curse unpacker - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../common/jhash.h"
#include "../common/miniz.h"
#include "../common/iofile.c"

int decompress_data(u8 **data, size_t *len)
{
	void *out;
	size_t out_len;
	u8 tail[4];
	if (*len < 4)
		return -1;
	memcpy(tail, *data, sizeof(tail));

	out = tinfl_decompress_mem_to_heap(
		*data + sizeof(tail), *len - sizeof(tail),
		&out_len,
		0
	);
	if (out == NULL) {
		printf("[!] Failed to decompress.\n");
		return -2;
	}
	printf("[*] Decompressed %zu bytes into %zu bytes.", *len - sizeof(tail), out_len);

	*data = realloc(*data, sizeof(tail) + out_len);
	assert(*data != NULL);
	memcpy(*data, tail, sizeof(tail));
	memcpy(*data + sizeof(tail), out, out_len);
	free(out);

	*len = sizeof(tail) + out_len;
	return 0;
}

int compress_data(u8 **data, size_t *len)
{
	void *out;
	size_t out_len;
	u8 tail[4];
	if (*len < 4)
		return -1;
	memcpy(tail, *data, sizeof(tail));

	out = tdefl_compress_mem_to_heap(
		*data + sizeof(tail), *len - sizeof(tail),
		&out_len,
		0
	);
	if (out == NULL) {
		printf("[!] Failed to compress.\n");
		return -1;
	}
	printf("[*] Compressed %zu bytes into %zu bytes.", *len - sizeof(tail), out_len);

	*data = realloc(*data, sizeof(tail) + out_len);
	assert(*data != NULL);
	memcpy(*data, tail, sizeof(tail));
	memcpy(*data + sizeof(tail), out, out_len);
	free(out);

	*len = sizeof(tail) + out_len;
	return 0;
}

int patch_checksum(u8 *data, size_t len)
{
	u32 sum;

	if (len < 4)
		return -1;

	memcpy(&sum, data, sizeof(sum));
	printf("[*] Old checksum: (%" PRIX32 ").\n", ES32(sum));
	sum = jhash(data + 4, (u32)(len - 4), 0) + 0x4900DC7C;
	printf("[*] New checksum: (%" PRIX32 ").\n", ES32(sum));
	memcpy(data, &sum, sizeof(sum));
	return 0;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -u            Unpack file\n");
	printf(" -p            Pack file\n\n");
}

int main(int argc, char **argv)
{
	u8 *data;
	size_t len;
	char *bak;
	char *filename;
	char *opt;

	printf("\nShantae and the Pirate's Curse (PS4) unpacker\n\n");

	if (--argc < 2) {
		print_usage(argv[0]);
		return 1;
	}
	opt = argv[1];
	if (*opt++ != '-' || (*opt != 'u' && *opt != 'p')) {
		print_usage(argv[0]);
		return 1;
	}
	filename = argv[2];

	if (read_buffer(filename, &data, &len) != 0) {
		printf("[!] Could not access the file: (%s).\n", filename);
		return 1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", filename);
	write_buffer(bak, data, len);
	free(bak);

	if (*opt == 'u') {
		if (decompress_data(&data, &len) != 0) {
			free(data);
			return 1;
		}
	}
	else {
		if (compress_data(&data, &len) != 0) {
			free(data);
			return 1;
		}
		if (patch_checksum(data, len) != 0) {
			free(data);
			printf("[!] Invalid save.\n");
			return 1;
		}
	}

	if (write_buffer(filename, data, len) != 0) {
		free(data);
		printf("[!] Failed to write file.\n");
		return 1;
	}

	free(data);
	return 0;
}

