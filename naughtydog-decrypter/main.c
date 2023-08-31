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

void decrypt_data(const u32* key_buffer, u32* data, u32 size)
{
	int i;
	u32 buf[2];

    printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);
    size = size/4;

	data[0] = ES32(0x0000157C);
	data += 2;

	for (i = 0; i < size; i+= 2)
	{
		buf[0] = ES32(data[i]);
		buf[1] = ES32(data[i+1]);
		crypt_64bit_down(key_buffer, buf);

		data[i] = ES32(buf[0]);
		data[i+1] = ES32(buf[1]);
	}

    printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(const u32* key_buffer, u32* data, u32 size)
{
	int i;
	u32 buf[2];

    printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);
    size = size/4;

	data[0] = ES32(0x0000157D);
	data += 2;

	for (i = 0; i < size; i+= 2)
	{
		buf[0] = ES32(data[i]);
		buf[1] = ES32(data[i+1]);
		crypt_64bit_up(key_buffer, buf);

		data[i] = ES32(buf[0]);
		data[i+1] = ES32(buf[1]);
	}

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
	char *opt, *bak;

	printf("\nnaughtydog-ps3save-decrypter 0.1.0 - (c) 2020 by Bucanero\n\n");

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

	u8* key_table = malloc(KEYSIZE);
	if (!key_table)
		return -1;

	apply_keycode((u32*) key_table, (u32*) KEY_DATA, SECRET_KEY);

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
	if (data[len-4] == 0x00) dsize = ES32(dsize);

	if (*opt == 'd')
		decrypt_data((u32*) key_table, (u32*) data, dsize);
	else
	{
		u32 crc = ES32(calc_crc32(data + 0x58C, ES32(*(u32*)&data[0x58C]) - 4));
		memcpy(data + 0x588, &crc, sizeof(u32));
		printf("[*] Updated CRC32    : %08X\n", ES32(crc));

		hmac_sha1(data + len - 0x20, SHA1_HMAC_KEY, 8 * strlen(SHA1_HMAC_KEY), data + 8, (len - 40)*8);
		printf("[*] Updated SHA1 HMAC: " SHA1_FMT(data + len - 0x20, "\n"));

		encrypt_data((u32*) key_table, (u32*) data, dsize);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	free(key_table);
	
	return 0;
}
