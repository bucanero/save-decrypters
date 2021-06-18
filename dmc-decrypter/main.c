/*
*
*	DmC: Devil May Cry PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original Devil May Cry 5 Crypto Toolkit by fairchild
*
* Information about the Blowfish encryption method:
*	- https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*
*/

#include "../common/iofile.c"
#include "../common/blowfish.c"
#include "../common/sha1.c"

#define SECRET_KEY      "8jdf*jsd@dfd8*P:QcNt"
// Devil May Cry 5 Retail Xbox360 key = "KA^3k@21\'38gdjfgEFJ#"


void decrypt_data(const u32* key_buffer, u32* data, u32 size)
{
	u32 buf[2];

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);
    size /= 4;

	for (int i = 0; i < size; i+= 2)
	{
		buf[0] = ES32(data[i]);
		buf[1] = ES32(data[i+1]);
		crypt_64bit_down(key_buffer, buf);

		data[i] = ES32(buf[0]);
		data[i+1] = ES32(buf[1]);
	}

	printf("[*] Stored SHA1: " SHA1_FMT((u8*)&data[1], "\n"));
	printf("[*] SHA1 Calculation Length: 0x%X\n", ES32(data[6]));
	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(const u32* key_buffer, u32* data, u32 size)
{
	u32 buf[2];

	sha1(&data[1], &data[7], ES32(data[6]) * 8);

	printf("[*] Updated SHA1: " SHA1_FMT((u8*)&data[1], "\n"));
	printf("[*] SHA1 Calculation Length: 0x%X\n", ES32(data[6]));
	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);
    size /= 4;

	for (int i = 0; i < size; i+= 2)
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
	char *opt, *bak;

	printf("\ndmc-ps3save-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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

	if (*opt == 'd')
		decrypt_data(key_table, (u32*) data, len);
	else
		encrypt_data(key_table, (u32*) data, len);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	free(key_table);
	
	return 0;
}
