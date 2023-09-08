/*
*
*	Metal Gear Solid V: The Phantom Pain PS3/PS4 Save Decrypter - (c) 2023 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original Xbox "MGS: The Phantom Pain SecFixer" by Philymaster
*
*/

#include "../common/iofile.c"
#include "../common/md5.c"

#define MGSV_TPP_PS3KEY		0x1FBAB234
#define MGSV_TPP_PS4KEY		0x4131F8BE


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

void EncData(u32* source, int length, char type)
{
	uint32_t key = (type == '3') ? MGSV_TPP_PS3KEY : MGSV_TPP_PS4KEY;

	for (int i = 0; i < length >> 2; i++)
	{
		key ^= (key << 13);
		key ^= (key >> 7);
		key ^= (key << 5);

		source[i] = (type == '3') ? ES32(ES32(source[i]) ^ key) : (source[i] ^ key);
	}
}

void decrypt_data(u8* data, u32 size, char type)
{
	printf("[*] Total Decrypted Size: 0x%X (%d bytes)\n", size, size);

	EncData((u32*) data, size, type);

	printf("[*] Decrypted PS%c File Successfully!\n\n", type);
	return;
}

void encrypt_data(u8* data, u32 size, char type)
{
	printf("[*] Total Encrypted Size: 0x%X (%d bytes)\n", size, size);

	EncData((u32*) data, size, type);

	printf("[*] Encrypted PS%c File Successfully!\n\n", type);
	return;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -3            Auto-Decrypt/Encrypt PS3 File\n");
	printf(" -4            Auto-Decrypt/Encrypt PS4 File\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	char *opt, *bak;

	printf("\nMetal Gear Solid V: TPP PS3/PS4 Save Decrypter 0.1.0 - (c) 2023 by Bucanero\n\n");

	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}
	
	opt = argv[1];
	if (*opt++ != '-' || (*opt != '3' && *opt != '4'))
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

	if (data[0x10] == 'S' && data[0x11] == 'V')
	{
		print_md5("[*] Old MD5: ", data);
		md5_hash(data + 0x10, len - 0x10, data);
		print_md5("[*] New MD5: ", data);

		encrypt_data(data, len, *opt);
	}
	else
		decrypt_data(data, len, *opt);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
