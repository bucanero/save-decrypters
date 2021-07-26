/*
*
*	Tales of Zestiria (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based/reversed on the "Tales of Zestiria save editor" by Amany,
* and the TZOFixer crypto notes by Vulnavia
*
*/

#include "../common/iofile.c"
#include "../common/sha1.c"

#define HASH_POS		0x000C  // PC Save: 0x0514
#define HASH_START		0x01D0  // PC Save: 0x06D8
#define HASH_LEN		637440


void sha_Compute(u8* hash_out, const u8* msg, u32 length, const char* key)
{
	sha1_ctx_t s;

	sha1_init(&s);
	while(length & (~0x0001ff)) // length>=512
	{
		sha1_nextBlock(&s, msg);
		msg += SHA1_BLOCK_BITS/8; // increment pointer to next block
		length -= SHA1_BLOCK_BITS;
	}

	// hack to append the 'key' to the data being hashed
	sha1_lastBlock(&s, (u8*)key, strlen(key) * 8);
	sha1_ctx2hash(hash_out, &s);
}

/*
    SHA1_CTX ctx;

    SHA1Init(&ctx);
    SHA1Update(&ctx, msg, length);
    SHA1Update(&ctx, (const unsigned char*)key, klen);
    SHA1Final(hash_out, &ctx);
*/

void toz_Compute(u8* hash, const u8* data, u32 len)
{
	u8 tmp[23];
	const char array[8][4] = {
		"SRA", "ROS", "MIC", "LAI", "EDN", "DEZ", "ZAB", "ALI"
	};

	sha_Compute(tmp, data, len * 8, "TO12");

	for (int i = 0; i < 100; i++)
	{
		memcpy(tmp + 20, array[i % 8], 3);
		sha1(tmp, tmp, sizeof(tmp) * 8);
	}
	memcpy(hash, tmp, 20);

	return;
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
	char *opt, *bak;

	printf("\nTales of Zestiria (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

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
	printf("[*] Stored SHA1-ToZ : " SHA1_FMT(data + HASH_POS, "\n"));

	toz_Compute(data + HASH_POS, data + HASH_START, HASH_LEN);
	printf("[*] Updated SHA1-ToZ: " SHA1_FMT(data + HASH_POS, "\n"));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
