/*
*
*	GTA 5 PS3/PS4 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
*/

#include "../common/iofile.c"
#include "../common/aes.c"

#define CHECKSUM_SEED 0x3FAC7125

const u8 GTAV_PS3_KEY[32] = {
		0x16, 0x85, 0xFF, 0xA3, 0x8D, 0x01, 0x0F, 0x0D, 0xFE, 0x66, 0x1C, 0xF9, 0xB5, 0x57, 0x2C, 0x50,
		0x0D, 0x80, 0x26, 0x48, 0xDB, 0x37, 0xB9, 0xED, 0x0F, 0x48, 0xC5, 0x73, 0x42, 0xC0, 0x22, 0xF5
	};

const u8 CHKS_HEADER[8] = {
	'C', 'H', 'K', 'S',
	0x00, 0x00, 0x00, 0x14
};

int search_data(const u8* data, size_t size, int start, const u8* search, int len)
{
	for (size_t i = start; i <= (size-len); i++)
		if (memcmp(data + i, search, len) == 0)
			return i;

    return -1;
}

// https://github.com/Zhaxxy/rdr2_enc_dec/blob/main/rdr2_enc_dec.py#L10
// https://www.burtleburtle.net/bob/hash/doobs.html
uint32_t jenkins_one_at_a_time_hash(const uint8_t* data, size_t length, uint32_t hash)
{
    while (length--)
    {
        hash += (signed char) *data++;
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

void decrypt_data(u8* data, u32 size)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, GTAV_PS3_KEY);

    printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	for (int i = 0; i < size; i+= AES_BLOCKLEN)
	{
		AES_ECB_decrypt(&ctx, data + i);
	}

    printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, GTAV_PS3_KEY);

    printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	for (int i = 0; i < size; i+= AES_BLOCKLEN)
	{
		AES_ECB_encrypt(&ctx, data + i);
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

	printf("\nGTA5 PS3/PS4 Save Decrypter 0.3.0 - (c) 2021 by Bucanero\n\n");

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

	int isPS4 = (data[0] == 0 && data[1] == 0 && data[2] == 0);
	printf("[i] Platform Type: %s\n\n", isPS4 ? "PS4" : "PS3");

	if (*opt == 'd')
		decrypt_data(data + (isPS4 ? 0x114 : 0), (isPS4 ? (len - 0x114) : len));
	else
	{
		if (isPS4)
		{
			// fix title checksum
			uint32_t seed; // must be 00 00 00 01
			uint8_t title[0x100];

			// read first 4 bytes of file and reverse
			memcpy(&seed, data, sizeof(uint32_t));
			seed = ES32(seed);

			// read title from offset 0x04 to 0x104
			memcpy(title, data + 0x04, sizeof(title));

			// generate seed
			seed = jenkins_one_at_a_time_hash((uint8_t*) &seed, sizeof(uint32_t), 0);

			// use generated seed to calculate title checksum
			uint32_t title_chks = jenkins_one_at_a_time_hash(title, sizeof(title), seed);

			printf(" - Title Seed  : %08X\n", seed);
			printf(" - Old Checksum: %08X\n", ES32(*(uint32_t*)(data + 0x104)));
			printf(" + New Checksum: %08X\n\n", title_chks);
			title_chks = ES32(title_chks);

			// finally, fix the 4 byte checksum at 0x104
			memcpy(data + 0x104, &title_chks, sizeof(uint32_t));
		}

		// fix general checksum

		uint32_t chks, chks_len;
		int chks_off = search_data(data, len, 0, CHKS_HEADER, sizeof(CHKS_HEADER));
	
		if (chks_off < 0)
		{
			printf("[!] CHKS Header Not Found!\n>>> Aborting... (Not a decrypted GTA5 save?)\n\n");
			return -1;
		}
	
		chks     = ES32(*(uint32_t*)(data + chks_off + 4)); // 0x14
		chks_len = ES32(*(uint32_t*)(data + chks_off + 8));
	
		printf(" - CHKS Offset : 0x%X\n", chks_off);
		printf(" - CHKS Size   : 0x%X (%d bytes)\n", chks_len, chks_len);
		printf(" - Old Checksum: %08X\n", ES32(*(uint32_t*)(data + chks_off + 0xC)));
	
		memset(data + chks_off + 8, 0, 8);
		chks = jenkins_one_at_a_time_hash(data + (chks_off - chks_len + 0x14), chks_len, CHECKSUM_SEED);
		printf(" + New Checksum: %08X\n\n", chks);

		chks = ES32(chks);
		chks_len = ES32(chks_len);
		memcpy(data + chks_off + 0xC, &chks, sizeof(uint32_t));
		memcpy(data + chks_off + 0x8, &chks_len, sizeof(uint32_t));
	
		encrypt_data(data + (isPS4 ? 0x114 : 0), (isPS4 ? (len - 0x114) : len));
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
