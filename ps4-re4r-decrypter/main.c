/*
*	Resident Evil 4 Remake PS4 Decrypter - (c) 2025 by hzh
*	https://github.com/hzhreal/
*
*   Information about the Blowfish encryption method:
*	    - https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*/

#include "../common/iofile.c"
#include "../common/blowfish.c"
#include "../common/mmh3.c"

#define SECRET_KEY "wa9Ui_tFKa_6E_D5gVChjM69xMKDX8QxEykYKhzb4cRNLknpCZUra"

void swap_u32_data(u8 *data, u32 size)
{
	int count = size/sizeof(u32);
	u32 *d = (u32 *)data;
	for (int i = 0; i < count; i++)
		d[i] = ES32(d[i]);
}

void decrypt_data(u8 *data, u32 size)
{
	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	// header
	u8 header[0x10] = {0};
	memcpy(header, data, sizeof(header));
    swap_u32_data(header, sizeof(header));
	blowfish_decrypt_buffer(header, sizeof(header));
    swap_u32_data(header, sizeof(header));

    // data
	swap_u32_data(data, size);
	blowfish_decrypt_buffer_cbc(data, size, 0);
	swap_u32_data(data, size);
	memcpy(data, header, sizeof(header));

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8 *data, u32 size)
{
	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	// header
	swap_u32_data(data, 0x10);
	blowfish_encrypt_buffer(data, 0x10);
	blowfish_decrypt_buffer_cbc(data, 0x10, 0);
	swap_u32_data(data, 0x10);

	// data
	swap_u32_data(data, size);
    blowfish_encrypt_buffer_cbc(data, size, 0);
	swap_u32_data(data, size);

	printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void print_usage(const char *argv0)
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
	u8 *data;
	u32 csum;
	char *opt, *bak;
	int n;

	printf("\nPS4 re4r-decrypter\n\n");

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

	if (len - 0x20 > 0 && (len - 0x20) % 8 == 0)
		n = 8;
	else
		n = 0;

	if (*opt == 'd')
		decrypt_data(data + 0x10, len - 0x10 - n);
	else
	{
		encrypt_data(data + 0x10, len - 0x10 - n);
		csum = murmur3_32(data, len - sizeof(u32), 0xFFFFFFFF);
    	printf("[*] Updated Checksum: %" PRIX32 "\n", csum);
    	memcpy(data + len - sizeof(u32), &csum, sizeof(u32));
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
