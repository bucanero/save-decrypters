/*
*
*	Naughty Dog PS3 Save Decrypter - (c) 2020 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original tlou_save_data_decrypter by Red-EyeX32 and aerosoul94
*
* Information about the Blowfish encryption method:
*	- https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*
*/

#include "../common/iofile.c"
#include "../common/blowfish.c"
#include "../common/sha1.c"
#include "../common/hmac-sha1.c"

#define SECRET_KEY      "(SH[@2>r62%5+QKpy|g6"
#define SHA1_HMAC_KEY   "xM;6X%/p^L/:}-5QoA+K8:F*M!~sb(WK<E%6sW_un0a[7Gm6,()kHoXY+yI/s;Ba"

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

	return ~crc;
}

void decrypt_data(void* data, u32 size)
{
    printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	blowfish_decrypt_buffer(data, size);

    printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(void* data, u32 size)
{
    printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	blowfish_encrypt_buffer(data, size);

    printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt File\n");
	printf(" -e            Encrypt File\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	u32 dsize;
	char isPS3, *opt, *bak;

	printf("\nnaughtydog-save-decrypter 0.2.0 - (c) 2020 by Bucanero\n\n");

	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}
	
	opt = argv[1];
	if (*opt++ != '-' || (*opt != 'd' && *opt != 'e'))
	{
		print_usage(argv[0]);
		return -1;
	}

	blowfish_init_key(SECRET_KEY);

	if (read_buffer(argv[2], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[2]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	dsize = *(u32*) &data[len-4];

	// Check for PS3 saves (big-endian)
	isPS3 = (data[0] == 0 && data[1] == 0);
	if (isPS3) dsize = ES32(dsize);

	if (*opt == 'd')
		decrypt_data(data+8, dsize);
	else
	{
		u32 crc = *(u32*) &data[0x58C];
		crc = (isPS3 ? ES32(calc_crc32(data + 0x58C, ES32(crc) - 4)) : calc_crc32(data + 0x58C, crc - 4));
		memcpy(data + 0x588, &crc, sizeof(u32));
		printf("[*] Updated CRC32    : %08X\n", ES32(crc));

		hmac_sha1(data + dsize - 0xC, SHA1_HMAC_KEY, 8 * strlen(SHA1_HMAC_KEY), data + 8, (dsize - 0x14)*8);
		printf("[*] Updated SHA1 HMAC: " SHA1_FMT(data + dsize - 0xC, "\n"));

		encrypt_data(data+8, dsize);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
