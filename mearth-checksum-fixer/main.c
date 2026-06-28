/*
*	Middle-Earth Checksum Fixer - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#include <limits.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../common/iofile.c"

#define CNT(a) (sizeof(a) / sizeof(a[0]))

const u8 MAGIC[4] = "SAVE";
const u32 MAGIC_OFFSETS[2] = {0x00, 0x08};
#define MAGIC_OFFSETS_MAX 0x08

typedef struct
{
	u32 size_offset;
	u32 write_offsets[2];
	u32 write_count;
} MEarth_Checksum_Ctx;

int get_ctx(MEarth_Checksum_Ctx *ctx, const u8 *data, size_t len)
{
	if (MAGIC_OFFSETS_MAX + sizeof(MAGIC) > len)
		return -1;
	u32 m = UINT32_MAX;
	for (u32 i = 0; i < CNT(MAGIC_OFFSETS); i++) {
		if (memcmp(data + MAGIC_OFFSETS[i], MAGIC, sizeof(MAGIC)) == 0) {
			m = MAGIC_OFFSETS[i];
			break;
		}
	}
	if (m == UINT32_MAX)
		return -2;

	switch (m) {
		case 0x00:
			// shadow of war
			ctx->size_offset = 0x10;
			ctx->write_offsets[0] = 0x08;
			ctx->write_count = 1;
			break;
		case 0x08:
			// shadow of mordor
			ctx->size_offset = 0x14;
			ctx->write_offsets[0] = 0x04;
			ctx->write_offsets[1] = 0x10;
			ctx->write_count = 2;
			break;
	}

	return 0;
}

int patch_checksum(u8 *data, size_t len)
{
	MEarth_Checksum_Ctx ctx = {0};
	if (get_ctx(&ctx, data, len) != 0)
		return -1;

	u32 start_off;
	u32 size;
	if (ctx.size_offset + 4 > len)
		return -2;
	size = U_U32_LE(data, ctx.size_offset);
	start_off = ctx.size_offset + 4;
	if (start_off + size > len)
		return -3;
	// checksum is written before the start offset
	// no bounds check needed
	u32 sum;
	printf("[*] Checksum start offset: 0x%" PRIu32 ".\n", start_off);
	printf("[*] Checksum size: %" PRIu32 ".\n", size);
	for (u32 i = 0; i < ctx.write_count; i++) {
		sum = U_U32_BE(data, ctx.write_offsets[i]);
		printf("[*] Old checksum at offset 0x%" PRIX32 ": 0x%" PRIX32 ".\n", ctx.write_offsets[i], sum);
	}

	sum = 0;
	for (size_t i = start_off; i < start_off + size; i++)
		sum += data[i];

	for (u32 i = 0; i < ctx.write_count; i++) {
		P_U32_LE(data, ctx.write_offsets[i], sum);
	}
	printf("[*] New checksum at all offsets: 0x%" PRIX32 ".\n", ES32(sum));

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

	printf("\nMiddle-Earth (PS4) checksum fixer\n\n");

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

