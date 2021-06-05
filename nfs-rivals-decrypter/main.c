/*
*
*	Need For Speed: Rivals PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original need_for_speed_rivals_save_decrypter by Red-EyeX32
*
* Information about the Blowfish encryption method:
*	- https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*
*/

#include "../common/iofile.c"
#include "../common/blowfish.c"

#define CRC32_POLY    0xEDB88320
#define CRC32_INIT    0xedcba987
#define SECRET_KEY    "\x23\x91\xF2\x01\xB3\x6C\x85\xE8\x1B\x12\x72\xD6\x90\xFF\xA5\x45"


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
	u32 buf[2];

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);
    size >>= 3;

	for (int i = 0; i < size; i+= 2)
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
	u32 buf[2], crc;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);
    size >>= 3;

	for (int i = 0; i < size; i+= 2)
	{
		buf[0] = ES32(data[i]);
		buf[1] = ES32(data[i+1]);
		crypt_64bit_up(key_buffer, buf);

		data[i] = ES32(buf[0]);
		data[i+1] = ES32(buf[1]);
	}

	printf("[*] Encrypted File Successfully!\n");
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
	char *opt, *bak;

	printf("\nnfsrivals-ps3save-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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

	u32* key_table = malloc(KEYSIZE);
	if (!key_table)
		return -1;

	apply_keycode(key_table, (u32*) KEY_DATA, SECRET_KEY);

	if (read_buffer(argv[2], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[2]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	printf("[*] Stored CRC32: 0x%X%X%X%X\n", data[0], data[1], data[2], data[3]);
	printf("[*] Calculation Length: 0x%lX\n", len-4);

	if (*opt == 'd')
		decrypt_data(key_table, (u32*) (data+4), len);
	else
	{
		encrypt_data(key_table, (u32*) (data+4), len);

		u32 crc = ES32(calc_crc32(data+4, len-4));
		memcpy(data, &crc, 4);

		printf("[*] Calculated Checksum: 0x%X%X%X%X\n", data[0], data[1], data[2], data[3]);
		printf("[*] Successfully Wrote New CRC32 Checksum!\n\n");
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	free(key_table);
	
	return 0;
}
