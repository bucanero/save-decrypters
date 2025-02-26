/*
*
*	Dynasty Warriors 8 Xtreme Legends PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original DW8XL Decryptor 1.1 by chaoszage
*
*/

#include "../common/iofile.c"

#define DW8XL_KEY1		0x13100200L
#define DW8XL_KEY2		0x41c64e6dL


void decrypt_data(u8* data, u32 size)
{
	u32 xor_key = DW8XL_KEY1;

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

    for(int i = 0; i <= 0xb7f43; i++)
    {
        xor_key = (xor_key * DW8XL_KEY2) + 0x3039;
        data[i] ^= ((xor_key >> 16) & 0xff);
    }

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
	u32 xor_key = DW8XL_KEY1;
    u32 csum = 0;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

    for (int i = 0; i <= 0xb7f43; i++)
    {
        csum += data[i];
        xor_key = (xor_key * DW8XL_KEY2) + 0x3039;
        data[i] ^= ((xor_key >> 16) & 0xff);
    }

    data[0xb7f44] = (u8)(csum & 0xff);

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

	printf("\ndw8xl-save-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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

	if (read_buffer(argv[2], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[2]);
		return -1;
	}

	if (*(u32*)data != ES32(0x9DA2DD60) && *(u32*)data != ES32(0xF0021013))
	{
		printf("[!] Error: Please choose a decrypted Dynasty Warriors 8 Xtreme Legends file");
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	if (*opt == 'd')
		decrypt_data(data, len);
	else
		encrypt_data(data, len);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
