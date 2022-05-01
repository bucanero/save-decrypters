/*
*
*	Borderlands 3 PS4 Save Decrypter - (c) 2022 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the original CSave Editor by HackerSmacker
*
*/

#include "../common/iofile.c"

#define SAVEGAME_STRING		"OakSaveGame"
#define PROFILE_STRING		"BP_DefaultOakProfile_C"

const char ProfilePrefixMagic_PS4[] = {
	0xad, 0x1e, 0x60, 0x4e, 0x42, 0x9e, 0xa9, 0x33, 0xb2, 0xf5, 0x01, 0xe1, 0x02, 0x4d, 0x08, 0x75,
	0xb1, 0xad, 0x1a, 0x3d, 0xa1, 0x03, 0x6b, 0x1a, 0x17, 0xe6, 0xec, 0x0f, 0x60, 0x8d, 0xb4, 0xf9
};

const char ProfileXorMagic_PS4[] = {
	0xba, 0x0e, 0x86, 0x1d, 0x58, 0xe1, 0x92, 0x21, 0x30, 0xd6, 0xcb, 0xf0, 0xd0, 0x82, 0xd5, 0x58,
	0x36, 0x12, 0xe1, 0xf6, 0x39, 0x44, 0x88, 0xea, 0x4e, 0xfb, 0x04, 0x74, 0x07, 0x95, 0x3a, 0xa2
};

const char PrefixMagic_PS4[] = {
	0xd1, 0x7b, 0xbf, 0x75, 0x4c, 0xc1, 0x80, 0x30, 0x37, 0x92, 0xbd, 0xd0, 0x18, 0x3e, 0x4a, 0x5f,
	0x43, 0xa2, 0x46, 0xa0, 0xed, 0xdb, 0x2d, 0x9f, 0x56, 0x5f, 0x8b, 0x3d, 0x6e, 0x73, 0xe6, 0xb8
};

const char XorMagic_PS4[] = {
	0xfb, 0xfd, 0xfd, 0x51, 0x3a, 0x5c, 0xdb, 0x20, 0xbb, 0x5e, 0xc7, 0xaf, 0x66, 0x6f, 0xb6, 0x9a,
	0x9a, 0x52, 0x67, 0x0f, 0x19, 0x5d, 0xd3, 0x84, 0x15, 0x19, 0xc9, 0x4a, 0x79, 0x67, 0xda, 0x6d
};


void decrypt_data(uint8_t* buffer, int length, const char* PrefixMagic, const char* XorMagic)
{
	char b;

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", length, length);

	for(int i = length - 1; i >= 0; i--)
	{
		b = (i < 32) ? PrefixMagic[i] : buffer[i - 32];
		b ^= XorMagic[i % 32];
		buffer[i] ^= b;
	}

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(uint8_t* buffer, int length, const char* PrefixMagic, const char* XorMagic)
{
	char b;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", length, length);

	for(int i = 0; i < length; i++)
	{
		b = (i < 32) ? PrefixMagic[i] : buffer[i - 32];
		b ^= XorMagic[i % 32];
		buffer[i] ^= b;
	}

	printf("[*] Encrypted File Successfully!\n\n");
	return;
}

static size_t search_data(const uint8_t* data, size_t size, const char* search, int len)
{
	for (long i = 0; i < (size-len); i++)
		if (memcmp(data + i, search, len) == 0)
			return i;

    return -1;
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
	size_t len, offset;
	u8* data;
	u32 size;
	char *opt, *bak;
	const char *pre, *xor;

	printf("\nBorderlands 3 PS4 decrypter 0.1.0 - (c) 2022 by Bucanero\n\n");

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
	
	offset = search_data(data, len, PROFILE_STRING, strlen(PROFILE_STRING));
	if (offset > 0)
	{
		pre = ProfilePrefixMagic_PS4;
		xor = ProfileXorMagic_PS4;
	}
	else
	{
		offset = search_data(data, len, SAVEGAME_STRING, strlen(SAVEGAME_STRING));
		if (offset > 0)
		{
			pre = PrefixMagic_PS4;
			xor = XorMagic_PS4;
		}
		else
		{
			printf("[!] Error: Please choose a Borderlands 3 PS4 file\n");
			return -1;
		}
	}
	printf("[*] Type: %s\n", data + offset);

	offset += strlen((char*)data + offset)+1;
	size = *(u32*)(data + offset);
	offset += 4;

	printf("[*] Start position: %ld\n", offset);

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	if (*opt == 'd')
		decrypt_data(data + offset, size, pre, xor);
	else
		encrypt_data(data + offset, size, pre, xor);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
