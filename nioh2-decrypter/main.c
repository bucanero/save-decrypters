/*
*
*	Nioh 2 (PS4) Decrypter - (c) 2025 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the original https://github.com/alfizari/Nioh-2-Save-Decrypt-PS4
*
*/

#include "../common/iofile.c"
#include "aes.c"


static const uint8_t AES_KEY[16] = {
    0x1D, 0x8E, 0xBB, 0x9D, 0x6A, 0x2E, 0x05, 0xBB,
    0x89, 0x6B, 0xF6, 0x9C, 0x03, 0x5A, 0x65, 0x90
};

static const uint8_t NONCE_COUNTER[16] = {
    0xBB, 0x8B, 0xDD, 0xFB, 0x81, 0x2F, 0x8D, 0x35,
    0x6A, 0xC9, 0x3A, 0xC4, 0x11, 0x2A, 0x7F, 0x82
};

void custom_aes_ctr(uint8_t* data, u32 sz)
{
	int i;
	uint8_t counter[16];
	uint8_t ks[16];

	memcpy(counter, NONCE_COUNTER, 16);

	// AES-CTR decrypt/encrypt
	for (sz /= BLOCKLEN; sz > 0; sz--, data += BLOCKLEN)
	{
		AES_ECB_encrypt(counter, AES_KEY, ks, BLOCKLEN);

		for (i = 0; i < BLOCKLEN; ++i)
			data[i] ^= ks[i];

		for (i = 15; i >= 0; --i)
			if (++counter[i] != 0)
				break;
	}
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
	u8 *data;
	char *opt, *bak;

	printf("\nNioh 2 (PS4) Decrypter 0.1.0 - (c) 2025 by Bucanero\n\n");

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
		printf("[*] Could Not Access The File (%s)\n", argv[1]);
		return -1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	printf("[*] Data Size       : %lu bytes\n", len - 0x10);

	if (*opt == 'd')
	{
		custom_aes_ctr(data, len);
	}
	else
	{
		custom_aes_ctr(data, len);
	}

	if (write_buffer(argv[2], data, len) == 0)
		printf("[*] Successfully %s %s!\n\n", (*opt == 'd') ? "Decrypted" : "Encrypted", argv[2]);

	free(bak);
	free(data);

	return 0;
}
