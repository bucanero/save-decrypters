/*
*
*	Resident Evil Revelation 2 PS4 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original RE Revelations 2 Decrypter by forjador123
*
* Information about the Blowfish encryption method:
*	- https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*
*/

#include "../common/iofile.c"
#include "../common/blowfish.c"
#include "../common/sha1.c"

#define SECRET_KEY      "zW$2eWaHNdT~6j86T_&j"


u32 dwadd(const u8* data, u32 len)
{
	u32 csum = 0;

	len /= 4;
	while (len--)
	{
		csum += (*(u32*)data);
		data += 4;
	}

	return csum;
}

void swap_u32_data(u32* data, int count)
{
	for (int i=0; i < count; i++)
		data[i] = ES32(data[i]);
}

void decrypt_data(u32* data, u32 size)
{
	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	swap_u32_data(data, size/4);
	blowfish_decrypt_buffer(data, size);
	swap_u32_data(data, size/4);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u32* data, u32 size)
{
	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	swap_u32_data(data, size/4);
	blowfish_encrypt_buffer(data, size);
	swap_u32_data(data, size/4);

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
	char *opt, *bak;

	printf("\nPS4 re-revelations2-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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
		decrypt_data((u32*)(data + 0x20), len - 0x20);
	else
	{
		printf("[*] Source SHA1  : " SHA1_FMT(data + len - 0x20, "\n"));
		sha1(data + len - 0x20, data + 0x20, (len - 0x40));
		swap_u32_data((u32*)(data + len - 0x20), 5);
		printf("[*] Updated SHA1 : " SHA1_FMT(data + len - 0x20, "\n"));

		encrypt_data((u32*)(data + 0x20), len - 0x20);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
