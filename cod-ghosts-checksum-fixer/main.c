/*
*
*	Call of Duty Ghosts MPDATA (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Call of Duty Ghosts Checksum Fixer" by Red-EyeX32
*
*/

#include "../common/iofile.c"


#define CRC32_POLY    0xEDB88320
#define CRC32_INIT    0xFFFFFFFF


void init_crc32_table(uint32_t* crc32_table, uint32_t poly)
{
	for (int b = 0; b < 256; ++b)
	{
		uint32_t r = b;

		for (int i = 0; i < 8; ++i)
			r = (r & 1) ? (r >> 1) ^ poly : (r >> 1);

		crc32_table[b] = r;
	}

	return;
}

u32 calc_crc32(const u8* data, u32 len)
{
	u32 crc32_table[256];
	u32 crc = CRC32_INIT;

	init_crc32_table(crc32_table, CRC32_POLY);

	while (len--)
		crc = crc32_table[(crc ^ *data++) & 0xFF] ^ (crc >> 8);

	// Custom CRC32: add 4 null bytes at the end
	for (int i = 0; i < 4; i++)
		crc = crc32_table[(crc ^ 0) & 0xFF] ^ (crc >> 8);

	return ~crc;
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

	printf("\nCall of Duty Ghosts MPDATA (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

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
	
	if (len != 37884L)
	{
		printf("[*] This is an invalid MPDATA file! (%s)\n", argv[1]);
		return -1;
	}
	
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[1]);
	write_buffer(bak, data, len);

	printf("[*] File Size       : %lu bytes\n", len);
	printf("[*] Stored Checksum : %08X\n", ES32(*(u32*)data));

	csum = calc_crc32(data + 4, len - 4);
	memcpy(data, &csum, sizeof(u32));

	// CRC32 stored as little endian
	printf("[*] New Checksum    : %08X\n", ES32(csum));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
