/*
*
*	Diablo III PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original diablo_3_save_data_decrypter by Red-EyeX32
*
*/

#include "../common/iofile.c"

#define DIABLO3_KEY1		0x305f92d8
#define DIABLO3_KEY2		0x2ec9a01b


void decrypt_data(u8* data, u32 size)
{
	u32 xor_key1 = DIABLO3_KEY1;
	u32 xor_key2 = DIABLO3_KEY2;
	u32 tmp;

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	for (int i = 0; i < size; i++)
	{
		data[i] ^= (xor_key1 & 0xFF);
		tmp = data[i] ^ xor_key1;
		xor_key1 = xor_key1 >> 8 | xor_key2 << 0x18;
		xor_key2 = xor_key2 >> 8 | tmp << 0x18;
	}

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
	u32 xor_key1 = DIABLO3_KEY1;
	u32 xor_key2 = DIABLO3_KEY2;
	u32 tmp;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	for (int i = 0; i < size; i++)
	{
		tmp = data[i] ^ xor_key1;
		data[i] ^= (xor_key1 & 0xFF);
		xor_key1 = xor_key1 >> 8 | xor_key2 << 0x18;
		xor_key2 = xor_key2 >> 8 | tmp << 0x18;
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
	char *opt, *bak;

	printf("\ndiablo3-save-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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
