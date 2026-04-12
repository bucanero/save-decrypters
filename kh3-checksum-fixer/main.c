/*
*	Kingdom Hearts III checksum fixer - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../common/iofile.c"
#include "../common/crc32.c"

int patch_checksum(u8 *data, size_t len)
{
	u32 sum;

	if (len < 0x10)
		return -1;

	memcpy(&sum, data + 0xC, sizeof(sum));
	printf("[*] Old checksum: (%" PRIX32 ").\n", ES32(sum));
	sum = crc32_calculate(data + 0x10, len - 0x10, CRC32_POLY, CRC32_INIT);
	printf("[*] New checksum: (%" PRIX32 ").\n", ES32(sum));
	memcpy(data + 0xC, &sum, sizeof(sum));
	return 0;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s filename\n\n", argv0);
}

int main(int argc, char **argv)
{
	u8 *data;
	size_t len;
	char *bak;
	char *filename;

	printf("\nKingdom Hearts III (PS4) checksum fixer\n\n");

	if (--argc < 1) {
		print_usage(argv[0]);
		return 1;
	}
	filename = argv[1];

	if (read_buffer(filename, &data, &len) != 0) {
		printf("[!] Could not access the file: (%s).\n", filename);
		return 1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", filename);
	write_buffer(bak, data, len);
	free(bak);

	if (patch_checksum(data, len) != 0) {
		free(data);
		printf("[!] Invalid save.\n");
		return 1;
	}

	if (write_buffer(filename, data, len) != 0) {
		free(data);
		printf("[!] Failed to write file.\n");
		return 1;
	}

	free(data);
	return 0;
}

