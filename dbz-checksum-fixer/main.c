/*
*
*	DBZ Ultimate Tenkaichi (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original dbz_ultimate_tenkaichi_checksum_fixer by Red-EyeX32
*
*/

#include "../common/iofile.c"


u32 wadd_le(const uint16_t* data, u32 len)
{
    u32 checksum = 0;
    len /= 2;

    while (len--)
        checksum += *data++;

    return checksum;
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

	printf("\nDBZ Ultimate Tenkaichi (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

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

	if (len != 150000)
	{
		printf("[*] Not A DBZ File (%s)\n", argv[1]);
		return -1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[1]);
	write_buffer(bak, data, len);

	printf("[*] Stored Checksum : %02X %02X %02X %02X\n", data[1], data[2], data[3], data[7]);
	printf("[*] Calculation Size: %lu bytes\n", len - 8);

	csum = wadd_le((u16*)(data + 8), len - 8);
	printf("[*] New Checksum    : %08X\n", csum);

	memset(data, 0, 8);
	data[1] = 0xFF & (csum >> 0x18);
	data[2] = 0xFF & (csum >> 0x10);
	data[3] = 0xFF & (csum >> 8);
	data[7] = 0xFF & csum;

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
