/*
*
*	Tears to Tiara 2 (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Tears to Tiara 2 checksum fixer" by chaoszage
*
*/

#include "../common/iofile.c"


u32 add(const u8* data, u32 len)
{
	u32 csum = 0x3428;

	while (len--)
		csum += *data++;

	return csum;
}

u32 calc_checksum(const u8* data, u32 len)
{
	u32 crc = 1;
	u32 add = 0x3428;

	while (len--)
	{
		add += *data++;
		crc = (crc * add) + add;
	}

	return (crc);
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

	printf("\nTears to Tiara 2 (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

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
	
	if (*(u32*)(data+0x10) != ES32(0x47414D45))
	{
		printf("[*] This is an invalid Tears to Tiara 2 file! (%s)\n", argv[1]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[1]);
	write_buffer(bak, data, len);

	printf("[*] File Size       : %lu bytes\n", len);
	printf("[*] Stored Checksum : %08X %08X\n", ES32(*(u32*)data), ES32(*(u32*)(data+4)));

	csum = ES32(add(data + 8, 0x3428));
	memcpy(data, &csum, sizeof(u32));
	printf("[*] New Checksum    : %08X ", ES32(csum));

	csum = ES32(calc_checksum(data + 8, 0x3428));
	memcpy(data + 4, &csum, sizeof(u32));
	printf("%08X\n", ES32(csum));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
