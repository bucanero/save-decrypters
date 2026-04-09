/*
*
* Death Stranding (PS4) Checksum Fixer - (c) 2026 by Bucanero - www.bucanero.com.ar
*
*/

#include "../common/iofile.c"
#include "../common/md5.c"


void md5(const u8 *data, size_t len, u8 *digest)
{
	MD5_CTX ctx;

	MD5Init(&ctx);
	MD5Update(&ctx, data, len);
	MD5Final(digest, &ctx);
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
	u32 dsize;
	char *opt, *bak;

	printf("\nDeath Stranding (PS4) checksum fixer 0.1.0 - (c) 2026 by Bucanero\n\n");

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
	
	if (data[0] != 'd' || data[1] != 's')
	{
		printf("[X] This is not a Death Stranding save file! (%s)\n", argv[1]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[1]);
	write_buffer(bak, data, len);

	dsize = *(u32*)(data + 0x1FC);
	printf("[*] Hash Size       : %d bytes\n", dsize);
	printf("[*] Stored MD5 Hash : %016llx%016llx\n", ES64(*(u64*)(data + 0x1EC)), ES64(*(u64*)(data + 0x1F4)));

	md5(data + 0x200, dsize, data + 0x1EC);
	printf("[*] New MD5 Hash    : %016llx%016llx\n\n", ES64(*(u64*)(data + 0x1EC)), ES64(*(u64*)(data + 0x1F4)));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New MD5 hash!\n\n");

	free(bak);
	free(data);

	return 0;
}
