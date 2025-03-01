/*
*
*	Resident Evil: HD Remaster PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original re_remaster_save_decrypter by Red-EyeX32
*
* Information about the Blowfish encryption method:
*	- https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*
*/

#include "../common/iofile.c"
#include "../common/blowfish.c"
#include "../common/sha1.c"

#define SECRET_KEY      "SBmdYgEamc=#sA0)Mhs9#>/4iiXbMPxW"


void decrypt_data(void* data, u32 size)
{
	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	blowfish_decrypt_buffer(data, size);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(void* data, u32 size)
{
	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	blowfish_encrypt_buffer(data, size);

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

	printf("\nre-remaster-save-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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
		decrypt_data(data, len);
	else
	{
		sha1(data + 0x0C, data + 0x40, (len - 0x40));
		printf("[*] Updated SHA1: " SHA1_FMT(data + 0x0C, "\n"));

		encrypt_data(data, len);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
