/*
*
* The Legend of Heroes: Trails *... (PS4) Checksum Fixer - (c) 2026 by Bucanero - www.bucanero.com.ar
*
* This checksum tool is based on crc_fixer_gui.py
*
*/

#include "../common/iofile.c"
#include "../common/crc32.c"

void print_usage(const char* argv0)
{
	printf("USAGE: %s filename\n\n", argv0);
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8 *data;
	u32 crc;
	char *opt, *bak;

	printf("\nThe Legend of Heroes: Trails ... (PS4) checksum fixer 0.1.0 - (c) 2026 by Bucanero\n\n");

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

	crc = ~crc32_calculate(data + 12, len - 12, CRC32_POLY, len - 12);

	printf("[*] File Size       : %lu bytes\n", len);
	printf("[*] Stored Checksum : %08X\n", *(u32*)(data + 8));
	printf("[*] New Checksum    : %08X\n\n", crc);

	// Write new checksum to file data
	memcpy(data + 8, &crc, sizeof(u32));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
