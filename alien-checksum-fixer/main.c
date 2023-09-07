/*
*
*	Alien: Isolation (PS3/PS4) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Security Archive" notes by Philymaster (https://community.wemod.com/t/philymasters-security-archive/3923)
*
*/

#include "../common/iofile.c"
#include "../common/sha1.c"


u64 sha1_xor64(const u8* data, int size)
{
	u64 sha[3] = {0, 0, 0};

	sha1(sha, data, size * 8);
	
	return (sha[0] ^ sha[1] ^ sha[2]);
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
	u64 csum;
	char *opt, *bak;

	printf("\nAlien: Isolation (PS3/PS4) checksum fixer 0.2.0 - (c) 2021 by Bucanero\n\n");

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

	// detect PS4 save
	if (memcmp(data, "IAAC", 4) == 0)
		csum = (*(u32*)(data + 0x18)) + (*(u32*)(data + 0x1C)) - 0x20;
	// detect PS3 save
	else if (memcmp(data, "CAAI", 4) == 0)
		csum = ES32(*(u32*)(data + 0x18)) + ES32(*(u32*)(data + 0x1C)) - 0x20;
	else
	{
		printf("[X] This is not an Alien Isolation save file! (%s)\n", argv[1]);
		return -1;
	}

	printf("[*] Checksum Size   : %llu bytes\n", csum);
	printf("[*] Stored Checksum : %016llX\n", ES64(*(u64*)(data+8)));

	csum = sha1_xor64(data + 0x20, csum);
	memcpy(data+8, &csum, sizeof(u64));

	printf("[*] New Checksum    : %016llX\n", ES64(csum));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
