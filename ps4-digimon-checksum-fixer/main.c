/*
*
*	Digimon World Next Order (PS4) Checksum Fixer - (c) 2025 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the python script by alfazari
*
*/

#include "../common/iofile.c"

uint32_t Add_sum(const u8* data, int size)
{
    uint32_t sum = 0;

    while (size--)
        sum += (unsigned char) *data++;

    return sum;
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

	printf("\nDigimon World Next Order (PS4) checksum fixer 0.1.0 - (c) 2025 by Bucanero\n\n");

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
	printf("[*] Stored Checksum : %08X\n", (*(u32*)(data + 8)));

	memset(data + 8, 0, sizeof(u32));
	csum = Add_sum(data, len);
	memcpy(data + 8, &csum, sizeof(u32));

	printf("[*] New Checksum    : %08X\n", csum);

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
