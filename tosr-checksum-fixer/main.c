/*
*
*	Tales of Symphonia Remastered (PS4) Checksum Fixer - (c) 2026 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Castlevania LOS Save Editor" by JizzaBeez
*
*/

#include "../common/iofile.c"


u32 leDWadd(const u8* Bytes, u32 Length)
{
	u32 dwadd = 0;

	for (int i = 0; i < Length; i += 4)
		dwadd += (Bytes[i + 3] << 24) | (Bytes[i + 2] << 16) | (Bytes[i + 1] << 8) | Bytes[i];

	return dwadd;
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

	printf("\nTales of Symphonia Remastered (PS4) checksum fixer 0.1.0 - (c) 2026 by Bucanero\n\n");

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
	printf("[*] Stored Checksum : %08X\n", *(u32*)(data));

	csum = leDWadd(data + 4, 0x25F4);
	memcpy(data, &csum, sizeof(u32));
	printf("[*] New Checksum    : %08X\n\n", csum);

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
