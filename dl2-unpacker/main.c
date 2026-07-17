/*
*	Dying Light 2 Unpacker - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../common/iofile.c"
#include "../common/miniz.h"
#include "../common/crc32.c"
#include "../common/crc.c"

#define FHCRC    (1 << 1)
#define FEXTRA   (1 << 2)
#define FNAME    (1 << 3)
#define FCOMMENT (1 << 4)
#define RES1     (1 << 5)
#define RES2     (1 << 6)
#define RES3     (1 << 7)

/*
 * Since miniz doesn't support gzip out of the box
 * we will both parse and construct a minimal gzip header (10 bytes) and trailer;
 * this should be sufficient for savegames from dl2.
 * https://en.wikipedia.org/wiki/Gzip#File_structure
*/

int decompress_data(u8 **data, size_t *len)
{
	if (*len < 10)
		return -1;
	u8 id1 = (*data)[0];
	u8 id2 = (*data)[1];
	u8 cm  = (*data)[2];
	if (id1 != 0x1F || id2 != 0x8B || cm != 0x08)
		return -2;
	u8 flg = (*data)[3];
	if (flg & FHCRC)
		return -3;
	if (flg & FEXTRA)
		return -4;
	if (flg & FNAME)
		return -5;
	if (flg & FCOMMENT)
		return -6;
	if (flg & RES1)
		return -7;
	if (flg & RES2)
		return -8;
	if (flg & RES3)
		return -9;
	if (*len < 10 + 8)
		return -10;

	u32 crc   = U_U32_LE(*data, *len - 8);
	u32 isize = U_U32_LE(*data, *len - 4);
	u32 crc_;
	u32 isize_;

	void *out;
	size_t out_len;

	out = tinfl_decompress_mem_to_heap(
		*data + 10, *len - 10 - 8,
		&out_len,
		0
	);
	if (out == NULL)
		return -11;
	isize_ = out_len;
	if (isize != isize_) {
		free(out);
		return -12;
	}
	crc_ = crc32_calculate(out, out_len, CRC32_POLY, CRC32_INIT);
	if (crc != crc_) {
		free(out);
		return -13;
	}

	void *p = realloc(*data, out_len);
	if (p == NULL) {
		free(out);
		return -14;
	}
	printf("[*] Decompressed %zu bytes into %zu bytes.", *len, out_len);
	*data = p;
	*len = out_len;
	memcpy(*data, out, out_len);
	free(out);

	return 0;
}

int compress_data(u8 **data, size_t *len)
{
	u8 header[10] = {
		0x1F, // id1
		0x8B, // id2
		0x08, // cm: deflate
		0x00, // flg
		0x00, 0x00, 0x00, 0x00, // mtime: no timestamp
		0x00, // xfl: default value
		0x03  // os: unix
	};

	void *out;
	size_t out_len;

	out = tdefl_compress_mem_to_heap(
		*data, *len,
		&out_len,
		0
	);
	if (out == NULL) {
		return -1;
	}
	u32 isize = *len;
	u32 crc   = crc32_calculate(*data, *len, CRC32_POLY, CRC32_INIT);

	if (out_len >= SIZE_MAX - sizeof(header) - sizeof(crc) - sizeof(isize)) {
		free(out);
		return -2;
	}
	size_t l = sizeof(header) + out_len + sizeof(crc) + sizeof(isize);
	void *p = realloc(*data, l);
	if (p == NULL) {
		free(out);
		return -3;
	}
	printf("[*] Compressed %zu bytes into %zu bytes.", *len, l);
	*data = p;
	*len = l;
	memcpy(*data, header, sizeof(header));
	memcpy(*data + sizeof(header), out, out_len);
	P_U32_LE(*data, sizeof(header) + out_len, crc);
	P_U32_LE(*data, sizeof(header) + out_len + sizeof(crc), isize);
	free(out);

	return 0;
}

#define SEG_SIZE 20
typedef struct {
	u32 magic;
	u32 type;
	u64 crc;
	u32 size;
} Seg;
Seg read_seg(u8 *data)
{
	Seg s;
	s.magic = U_U32_LE(data,  0);
	s.type  = U_U32_LE(data,  4);
	s.crc   = U_U64_LE(data,  8);
	s.size  = U_U32_LE(data, 16);
	return s;
}

const u8 MAGIC_SGDS[4] = "SGDS";
const u8 MAGIC_SGDD[4] = "SGDD";

int patch_checksum(u8 *data, size_t len)
{
	if (len < SEG_SIZE)
		return -1;

	custom_crc_t crc_params = {
		.width  = 64,
		.poly   = UINT64_C(0xAD93D23594C935A9),
		.init   = UINT64_C(0xFFFFFFFFFFFFFFFF),
		.refIn  = 1,
		.refOut = 1,
		.xor    = UINT64_C(0xFFFFFFFFFFFFFFFF)
	};
	u64 crc;

	size_t found  = 0;
	size_t fixed  = 0;
	size_t offset = 0;
	size_t seg_start;
	size_t seg_end;
	Seg seg;

	while (offset <= len - SEG_SIZE) {
		bool is_SGDS = memcmp(data + offset, MAGIC_SGDS, sizeof(MAGIC_SGDS)) == 0;
		bool is_SGDD = memcmp(data + offset, MAGIC_SGDD, sizeof(MAGIC_SGDD)) == 0;
		if (!is_SGDS && !is_SGDD) {
			offset++;
			continue;
		}

		seg = read_seg(data + offset);
		seg_start = offset + SEG_SIZE;
		if (seg_start >= SIZE_MAX - seg.size)
			return -2;
		seg_end = seg_start + seg.size;
		if (seg_end > len) {
			offset += 4;
			continue;
		}

		if (seg.type == 2) {
			crc = crc64_hash(data + seg_start, seg_end - seg_start, &crc_params);
			if (crc != seg.crc) {
				P_U64_LE(data, offset + 8, crc);
				fixed++;
			}
		}

		found++;
		offset = seg_end;
	}
	printf("[*] Found %zu segments and fixed the checksum of %zu of them.\n", found, fixed);

	return 0;
}

void print_usage(const char *argv0)
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

	printf("\nDying Light 2 (PS4) unpacker\n\n");

	if (argc - 1 < 2) {
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
		int ret = decompress_data(&data, &len);
		if (ret != 0) {
			free(data);
			if (ret == -14)
				printf("[!] Memory allocation error.\n");
			else
				printf("[!] Failed to decompress save.\n");
			return 1;
		}
	}
	else {
		if (patch_checksum(data, len) != 0) {
			free(data);
			printf("[!] Invalid save.\n");
			return 1;
		}
		int ret = compress_data(&data, &len);
		if (ret != 0) {
			free(data);
			if (ret == -3)
				printf("[!] Memory allocation error.\n");
			else
				printf("[!] Failed to compress.\n");
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

