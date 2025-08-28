/*
*	Resident Evil 4 Remake PS4 Decrypter - (c) 2025 by hzh
*	https://github.com/hzhreal/
*
*   Information about the Blowfish encryption method:
*	    - https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*/

#include "../common/iofile.c"
#include "../common/blowfish.c"

#define SECRET_KEY "wa9Ui_tFKa_6E_D5gVChjM69xMKDX8QxEykYKhzb4cRNLknpCZUra"

void swap_u32_data(u32 *data, int count)
{
	for (int i=0; i < count; i++)
		data[i] = ES32(data[i]);
}

void xor_down(void *bf_data, uint32_t size)
{
	uint32_t buf[2];
	uint32_t *data = bf_data;
    size /= 4;
	u32 l = 0, r = 0;
	u32 l_ = 0, r_ = 0;
	for (int i = 0; i < size; i+= 2)
	{
		buf[0] = data[i];
		buf[1] = data[i+1];
        l = buf[0];
		r = buf[1];

		crypt_64bit_down((uint32_t*) key_buffer, buf);
		data[i] = buf[0];
		data[i] ^= l_;
		data[i+1] = buf[1];
		data[i+1] ^= r_;

        l_ = l;
		r_ = r;
	}
}

void xor_up(void *bf_data, uint32_t size)
{
	uint32_t buf[2];
	uint32_t *data = bf_data;
	size /= 4;
	u32 l = 0, r = 0;
	u32 l_ = 0, r_ = 0;
	for (int i = 0; i < size; i+= 2)
	{
		buf[0] = data[i] ^ l_;
		buf[1] = data[i+1] ^ r_;
        l = buf[0];
		r = buf[1];

		crypt_64bit_up((uint32_t*) key_buffer, buf);
		data[i] = buf[0];
		data[i+1] = buf[1];

        l_ = buf[0];
		r_ = buf[1];
	}
}

void decrypt_data(u8 *data, u32 size)
{
	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

    // header
    swap_u32_data((u32 *)data, 0x10/4);
    blowfish_decrypt_buffer((void *)data, 0x10);
    swap_u32_data((u32 *)data, 0x10/4);

    // data
	xor_down(data + 0x10, size - 0x10);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8 *data, u32 size)
{
	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);
    
    // header
	swap_u32_data((u32 *)data, 0x10/4);
	blowfish_encrypt_buffer((void *)data, 0x10);
	swap_u32_data((u32 *)data, 0x10/4);

    // data
    xor_up(data + 0x10, size - 0x10);

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
	char *opt, *bak;

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

	if (*opt == 'd')
		decrypt_data(data + 0x10, len - 0x10);
	else
	{
		encrypt_data(data + 0x10, len - 0x10);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
