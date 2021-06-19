/*
*
*	LEGO Games (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Security Archive" notes by Philymaster (https://community.wemod.com/t/philymasters-security-archive/3923)
*
*/

#include "../common/iofile.c"

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
int fnv1_hash(const u8* data, int size)
{
    int sum = -1;

    while (size--)
        sum = (sum * 0x1000193) ^ *data++;

    return ~sum;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s filename\n\n", argv0);
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8 *data;
	u32 csum;
	char *opt, *bak;

	printf("\nLEGO Games (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

	if (--argc < 1)
	{
		print_usage(argv[0]);
		return -1;
	}

	if (read_buffer(argv[1], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[1]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[1]);
	write_buffer(bak, data, len);

	printf("[*] File Size       : %lu bytes\n", len);
	printf("[*] Stored Checksum : %08X\n", ES32(*(u32*)(data + 0x0C)));

	csum = ES32(fnv1_hash(data + 0x10, len - 0x10));
	memcpy(data + 0x0C, &csum, sizeof(u32));

	printf("[*] New Checksum    : %08X\n", ES32(csum));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
