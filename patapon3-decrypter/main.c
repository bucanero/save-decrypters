/*
*
*	Patapon 3 PSP Save Decrypter - (c) 2023 by Bucanero - www.bucanero.com.ar
*
* This tool is based on
*	- https://github.com/efonte/patapon-re/tree/main/Tools/patapon3-enc
*
*/

#include "../common/iofile.c"
#include "../common/camellia.c"

//PSP save-game key: 4439565A473232594C57534A474E4C9A
#define PATAPON3_KEY "SVsyE56pniSRS9dIPTiE8ApDaUnN0AEa"


void encrypt_data(uint8_t* data, int size)
{
    CamelliaContext ctx;

	printf("[*] Total Encrypted Size: 0x%X (%d bytes)\n", size, size);
    size /= CAMELLIA_BLOCK_SIZE;

    camelliaInit(&ctx, (uint8_t*) PATAPON3_KEY, strlen(PATAPON3_KEY));

    while (size--)
    {
        camelliaEncryptBlock(&ctx, data, data);
        data += CAMELLIA_BLOCK_SIZE;
    }

	printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void decrypt_data(uint8_t* data, int size)
{
    CamelliaContext ctx;

	printf("[*] Total Decrypted Size: 0x%X (%d bytes)\n", size, size);
    size /= CAMELLIA_BLOCK_SIZE;

    camelliaInit(&ctx, (uint8_t*) PATAPON3_KEY, strlen(PATAPON3_KEY));

    while (size--)
    {
        camelliaDecryptBlock(&ctx, data, data);
        data += CAMELLIA_BLOCK_SIZE;
    }

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt PSP File\n");
	printf(" -e            Encrypt PSP File\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	char *opt, *bak;

	printf("\nPatapon 3 PSP Save Decrypter 0.1.0 - (c) 2023 by Bucanero\n\n");

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

	if (*opt == 'e')
	{
		encrypt_data(data, len);
	}
	else
		decrypt_data(data, len);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
