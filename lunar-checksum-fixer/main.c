/*
*	LUNAR Remastered Collection checksum fixer - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#define CRC32_POLY 0xEDB88320
#define CRC32_INIT 0xFFFFFFFF

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../common/iofile.c"

void init_crc32_table(u32 *table)
{
	for (int b = 0; b < 256; ++b) {
		u32 r = b;

		for (int i = 0; i < 8; ++i)
			r = (r & 1) ? (r >> 1) ^ CRC32_POLY : (r >> 1);

		table[b] = r;
	}
}

u32 calc_crc32(const u32 *table, const u32 *init, const u8 *data, u32 len)
{
	const u32 *t;
	u32 crc;
	if (init == NULL)
		crc = CRC32_INIT;
	else
		crc = *init;

	if (table == NULL) {
		u32 t_[256];
		init_crc32_table(t_);
		t = t_;
	}
	else
		t = table;

	while (len--)
		crc = t[(crc ^ *data++) & 0xFF] ^ (crc >> 8);

	return ~crc;
}

int patch_checksum_1(u8 *data, size_t len)
{
	u32 sum;
	u32 n;
	
	if (len < 8)
		return -1;

	memcpy(&sum, data + (len - 4), sizeof(sum));
	printf("[*] Old checksum: (%" PRIX32 ").\n", ES32(sum));
	sum = 0;
	for (size_t i = 0; i < (len / 4) - 1; i++) {
		memcpy(&n, data + i * 4, sizeof(n));
		sum += n;
	}
	printf("[*] New checksum: (%" PRIX32 ").\n", ES32(sum));
	memcpy(data + (len - 4), &sum, sizeof(sum));
	return 0;
}

int patch_checksum_2(u8 *data, size_t len)
{
	u32 sum;

	if (len < 0x300)
		return -1;

	memcpy(&sum, data + 0x204, sizeof(sum));
	printf("[*] Old checksum: (%" PRIX32 ").\n", ES32(sum));
	sum = 0;
	for (size_t i = 0x300; i < len; i++)
		sum += data[i];
	printf("[*] New checksum: (%" PRIX32 ").\n", ES32(sum));
	memcpy(data + 0x204, &sum, sizeof(sum));
	return 0;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [type] filename\n\n", argv0);
	printf("TYPE           Explanation:\n");
	printf(" -1            LUNAR 1\n");
	printf(" -2            LUNAR 2\n\n");
}

int main(int argc, char **argv)
{
	u8 *data;
	size_t len;
	char *bak;
	char *filename;
	char *type;

	printf("\nLUNAR Remastered Collection (PS4) checksum fixer\n\n");

	if (--argc < 2) {
		print_usage(argv[0]);
		return 1;
	}
	type = argv[1];
	if (*type++ != '-' && (*type != '1' || *type != '2')) {
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

	if (*type == '1') {
		if (patch_checksum_1(data, len) != 0) {
			free(data);
			printf("[!] Invalid save.\n");
			return 1;
		}
	}
	else {
		if (patch_checksum_2(data, len) != 0) {
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

