/*
*
*	Metal Gear Solid V: The Phantom Pain PS3 Save Decrypter - (c) 2023 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original diablo_3_save_data_decrypter by Red-EyeX32
*
*/

#include "../common/iofile.c"
#include "../common/md5.c"

#define MGSV_TPP_KEY		0x1FBAB234


void md5_hash(const u8* in, u32 size, u8* out)
{
	MD5_CTX context;

	MD5Init (&context);
	MD5Update (&context, in, size);
	MD5Final (out, &context);
}

void print_md5(const char* msg, const u8* data)
{
	printf("%s", msg);
	for (int i=0; i<16; i++)
		printf("%02X", data[i]);
	printf("\n");
}

void EncData(u32* source, int length)
{
	uint32_t key = MGSV_TPP_KEY;

	for (int i = 0; i < length >> 2; i++)
	{
		key ^= (key << 13);
		key ^= (key >> 7);
		key ^= (key << 5);

		source[i] = ES32(ES32(source[i]) ^ key);
	}
}

void decrypt_data(u8* data, u32 size)
{
	printf("[*] Total Decrypted Size: 0x%X (%d bytes)\n", size, size);

	EncData((u32*) data, size);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
	printf("[*] Total Encrypted Size: 0x%X (%d bytes)\n", size, size);

	EncData((u32*) data, size);

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

	printf("\nMetal Gear Solid V: TPP Save Decrypter 0.1.0 - (c) 2023 by Bucanero\n\n");

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
	{
		print_md5("[*] Old MD5: ", data);
		md5_hash(data + 0x10, len - 0x10, data);
		print_md5("[*] New MD5: ", data);

		encrypt_data(data, len);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
