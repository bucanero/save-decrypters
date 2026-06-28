/*
*	Far Cry 5 Checksum Fixer - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#include "../common/iofile.c"
#include "../common/crc32.c"

int patch_checksum(u8 *data, size_t len)
{
	if (len < 0x10)
		return -1;

	u32 sum = U_U32_LE(data, 0xC);
	printf("[*] Old checksum: %" PRIX32".\n", ES32(sum));


	u32 msg_len = U_U32_LE(data, 0x8);
	if (0x10 + msg_len > len)
		return -2;
	sum = crc32_calculate(data + 0x10, msg_len, CRC32_POLY, CRC32_INIT);

	P_U32_LE(data, 0xC, sum);
	printf("[*] New checksum: %" PRIX32".\n", ES32(sum));

	return 0;
}

void print_usage(const char *argv0)
{
	printf("USAGE: %s filename\n\n", argv0);
}

int main(int argc, char **argv)
{
	u8 *data;
	size_t len;
	char *filename;
	char *bak;

	printf("\nFar Cry 5 (PS4) checksum fixer\n\n");

	if (argc - 1 < 1) {
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

