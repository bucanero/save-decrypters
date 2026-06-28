/*
*	Sword Art Online: Fatal Bullet Checksum Fixer - (c) 2026 by hzh
*	https://github.com/hzhreal/
*/

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/iofile.c"
#include "../common/sha1.c"
#include "../common/hmac-sha1.c"

const u8 HMAC_SHA1_KEY[32] = "1FB00CC8D8D94CD0A94C847C2F04A921";

const u8 PREFIX[8] = {
    0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00
};

int find(const u8 *d, size_t d_len,
		 const u8 *t, size_t t_len,
		 size_t *match, size_t start, size_t end)
{
	if (!(start < end))
		return -1;
	if (!(end <= d_len))
		return -2;
	if (!(t_len <= end - start))
		return -3;

	for (size_t i = start; i <= end - t_len; i++) {
		if (memcmp(d + i, t, t_len) == 0) {
			*match = i;
			return 0;
		}
	}
	return -4;
}


int patch_checksum(u8 **data, size_t *len)
{
	size_t match;
	size_t len_;

	if (*len < 0x1000)
		return -1;
	if (find(*data, *len, PREFIX, sizeof(PREFIX), &match, *len - 0x1000, *len) != 0) {
		printf("[*] There is no checksum to patch.\n");
		return 0;
	}

	if (match + sizeof(PREFIX) > SIZE_MAX - HMAC_SHA1_BYTES)
		return -2;
	len_ = match + sizeof(PREFIX) + HMAC_SHA1_BYTES;
	if (*len != len_) {
		void *p = realloc(*data, len_);
		if (p == NULL)
			return -3;
		*data = p;
		*len = len_;
	}

	hmac_sha1(*data + *len - HMAC_SHA1_BYTES, HMAC_SHA1_KEY, sizeof(HMAC_SHA1_KEY),
			  *data, *len - HMAC_SHA1_BYTES - (sizeof(PREFIX) - 4));
	printf("[*] Successfully patched checksum.\n");

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

	printf("\nSword Art Online: Fatal Bullet (PS4) checksum fixer\n\n");

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

	int ret = patch_checksum(&data, &len);
	if (ret != 0) {
		free(data);
		if (ret == -3)
			printf("[!] Memory allocation error.\n");
		else
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

