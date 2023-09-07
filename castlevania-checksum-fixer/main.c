/*
*
*	Castlevania Lords of Shadow 1/2 (PS3) Checksum Fixer - (c) 2023 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Castlevania LOS Save Editor" by JizzaBeez
*
*/

#include "../common/iofile.c"


int ComputeHash(const u8* Bytes, u32 Length)
{
	int num = 0;
	int num2 = 0;

	for (int i = 0; i < Length; i += 2)
	{
		num += ((int)Bytes[i] ^ (i & 255));
		num2 += ((int)Bytes[i + 1] ^ (i + 1 & 255));
	}

	return (num + num2);
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

	printf("\nCastlevania: Lords of Shadow 1 & 2 (PS3) checksum fixer 0.1.0 - (c) 2023 by Bucanero\n\n");

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
	
	if (!(data[0x20] == '2' && memcmp(data + 0x10, "Lord", 4) == 0) && memcmp(data + 4, "Lord", 4))
	{
		printf("[X] This is not a Castlevania LOS 1/2 save file! (%s)\n", argv[1]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[1]);
	write_buffer(bak, data, len);

	printf("[*] File Size       : %lu bytes\n", len);
	printf("[*] Stored Checksum : %08X\n", *(u32*)(data + len - 4));

	csum = ComputeHash(data, len - 4);
	memcpy(data + len - 4, &csum, sizeof(u32));
	printf("[*] New Checksum    : %08X\n\n", csum);

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
