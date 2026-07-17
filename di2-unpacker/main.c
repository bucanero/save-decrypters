/*
*	Dead Island 2 Unpacker - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../common/iofile.c"
#include "../common/zstd.c"
#include "../common/cityhash.c"

const u8 START_MARKER[4] = {0x01, 0xD9, 0x0E, 0x51};
const u8 END_MARKER  [4] = {0x42, 0xFC, 0x13, 0x5A};

int rfind(const u8 *d, size_t d_len,
		  const u8 *t, size_t t_len,
		  size_t *match, size_t start, size_t end)
{
	if (!(end < start))
		return -1;
	if (!(start <= d_len))
		return -2;
	if (!(start >= t_len))
		return -3;
	if (!(t_len <= start - end))
		return -4;

	for (size_t i = start - t_len; i >= end; i--) {
		if (memcmp(d + i, t, t_len) == 0) {
			*match = i;
			return 0;
		}
		if (i == 0)
			break;
	}
	return -4;
}

int decompress_data(u8 **data, size_t *len)
{
	if (*len < 16)
		return -1;
	if (memcmp(*data + 8, START_MARKER, sizeof(START_MARKER)) != 0)
		return -2;

	u32 length = U_U32_LE(*data, 12);
	size_t start = 12 + sizeof(length) + length;
	if (*len < start)
		return -3;
	size_t end;
	if (rfind(*data, *len, END_MARKER, sizeof(END_MARKER), &end, *len - 8, start) != 0)
		return -4;
	if (end >= SIZE_MAX - 12)
		return -5;
	if (end + 12 > *len)
		return -6;

	void *out = malloc(start);
	if (out == NULL)
		return -7;
	memcpy(out, *data, start);

	size_t out_len = start;
	size_t i = start;
	u32 cs;
	u32 ds;
	size_t actual_ds;
	void *block;
	void *tmp;
	while (i < end) {
		if (i >= SIZE_MAX - 8)
			return -8;
		cs = U_U32_LE(*data, i    );
		if (i >= SIZE_MAX - 8 - cs)
			return -9;
		if (i + 8 + cs > end)
			return -10;
		ds = U_U32_LE(*data, i + 4);
		if (out_len >= SIZE_MAX - ds)
			return -11;
		if (cs == 0 || ds == 0)
			return -12;

		// trusting input
		block = malloc(ds);
		if (block == NULL) {
			free(out);
			return -13;
		}

		actual_ds = ZSTD_decompress(block, ds, *data + i + 8, cs);
		if (ZSTD_isError(actual_ds) || actual_ds != ds) {
			free(out);
			free(block);
			return -14;
		}

		tmp = realloc(out, out_len + ds);
		if (tmp == NULL) {
			free(out);
			free(block);
			return -15;
		}
		out = tmp;
		memcpy(out + out_len, block, ds);
		out_len += ds;
		free(block);

		i += 8 + cs;
	}

	if (out_len >= SIZE_MAX - 12) {
		free(out);
		return -16;
	}
	tmp = realloc(out, out_len + 12);
	if (tmp == NULL) {
		free(out);
		return -17;
	}
	out = tmp;
	memcpy(out + out_len, *data + end, 12);
	out_len += 12;
	free(*data);
	*data = out;
	printf("[*] Decompressed %zu bytes into %zu bytes.", *len, out_len);
	*len = out_len;

	return 0;
}

int compress_data(u8 **data, size_t *len)
{
	u32 length = U_U32_LE(*data, 12);
	size_t start = 12 + sizeof(length) + length;
	size_t end;
	rfind(*data, *len, END_MARKER, sizeof(END_MARKER), &end, *len, 0);

	u8 *out = malloc(start);
	if (out == NULL)
		return -1;
	memcpy(out, *data, start);
	size_t out_len = start;

	u32 bs = 1 << 20; // 1 MiB
	size_t q = (end - start) / bs;
	size_t r = (end - start) % bs;
	size_t t1;
	u32 cs;
	u32 ds = bs;
	void *t2;

	for (size_t i = 0; i < q + 1; i++) {
		if (i == q) {
			if (r == 0) {
				break;
			}
			ds = r;
		}

		t1 = ZSTD_compressBound(ds);
		if (out_len >= SIZE_MAX - sizeof(cs) - sizeof(ds) - t1) {
			free(out);
			return -2;
		}
		if (t1 > UINT32_MAX) {
			free(out);
			return -3;
		}
		t2 = realloc(out, out_len + sizeof(cs) + sizeof(ds) + t1);
		if (t2 == NULL) {
			free(out);
			return -4;
		}
		out = t2;

		t1 = ZSTD_compress(
			out + out_len + sizeof(cs) + sizeof(ds), t1,
			*data + start + i * bs, ds, 1
		);
		if (ZSTD_isError(t1)) {
			free(out);
			return -5;
		}
		cs = t1;
		t2 = realloc(out, out_len + sizeof(cs) + sizeof(ds) + cs);
		if (t2 == NULL) {
			free(out);
			return -6;
		}
		out = t2;
		P_U32_LE(out, out_len, cs);
		P_U32_LE(out, out_len + sizeof(cs), ds);
		out_len += sizeof(cs) + sizeof(ds) + cs;
	}

	if (out_len >= SIZE_MAX - 12) {
		free(out);
		return -7;
	}
	t2 = realloc(out, out_len + 12);
	if (t2 == NULL) {
		free(out);
		return -8;
	}
	out = t2;
	memcpy(out + out_len, *data + end, 12);
	out_len += 12;
	free(*data);
	*data = out;
	printf("[*] Compressed %zu bytes into %zu bytes.", *len, out_len);
	*len = out_len;
	
	return 0;
}

int patch_checksum(u8 *data, size_t len)
{
	if (len < 16)
		return -1;
	if (memcmp(data + 8, START_MARKER, sizeof(START_MARKER)) != 0)
		return -2;

	u32 length = U_U32_LE(data, 12);
	size_t start = 12 + sizeof(length) + length;
	if (len < start)
		return -3;
	size_t end;
	if (rfind(data, len, END_MARKER, sizeof(END_MARKER), &end, len - 8, start) != 0)
		return -4;
	if (end >= SIZE_MAX - 12)
		return -5;
	if (end + 12 > len)
		return -6;

	u64 hash = U_U64_LE(data, end + 4);
	printf("[*] Old checksum: %" PRIX64".\n", ES64(hash));
	hash = cityhash64(data + start, end - start);
	P_U64_LE(data, end + 4, hash);
	printf("[*] New checksum: %" PRIX64".\n", ES64(hash));

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

	printf("\nDead Island 2 (PS4) unpacker\n\n");

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
			if (ret == -7 || ret == -13 || ret == -15 || ret == -17)
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
			if (ret == -1 || ret == -4 || ret == -6 || ret == -8)
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

