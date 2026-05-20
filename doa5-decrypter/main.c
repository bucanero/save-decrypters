/*
*
*	Dead or Alive 5 PS3 and PS4 Save Decrypter - (c) 2026 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the original doa5.py script by alfazari911
*
*/

#include "../common/iofile.c"

typedef struct {
	u32 magic;
	u32 size;
	u32 checksum;
	u32 seed;
} block_header;


// PS3 saves have their block header fields stored in big-endian format, while PS4 saves use little-endian.
// The ES32 macro converts a 32-bit value from big-endian to little-endian or vice versa.
int isPS3 = 0;

uint32_t next_seed(uint32_t seed)
{
	return ((seed << 4) + (seed >> 4)) & 0xFFFFFFFF;
}

void crypt_block(u32* block, u32 size, u32 seed)
{
	printf("    Processing %d bytes ...\n", size);

	for (u32 i = 0; i < size/4; i++)
	{
		block[i] ^= (isPS3) ? ES32(seed) : seed;
		seed = next_seed(seed);
	}
}

uint32_t add_csum(uint8_t* data, uint32_t size)
{
	uint32_t csum = 0;

	while (size--)
		csum += *data++;

	return csum;
}

void decrypt_data(u8* data, u32 size)
{
	void* block;
	u32 block_size;
	block_header header = {0};

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	for (u32 i = 0; i < size; i += header.size)
	{
		printf("[*] Decrypting Block At Offset 0x%X\n", i);

		block = (&data[i] + sizeof(block_header));
		memcpy(&header, &data[i], sizeof(block_header));

		if (isPS3)
		{
			header.size = ES32(header.size);
			header.checksum = ES32(header.checksum);
			header.seed = ES32(header.seed);
		}

		block_size = (header.size) ? header.size - sizeof(block_header) : (size - i - sizeof(block_header));

//		printf(" - Block Magic: 0x%08X Size: 0x%08X Checksum: 0x%08X Seed: 0x%08X\n", header.magic, header.size, header.checksum, header.seed);

		if (add_csum(block, block_size) != header.checksum)
		{
			printf("[!] Block Checksum Mismatch! Expected %08X\n", header.checksum);
		}

		crypt_block(block, block_size, header.seed);

		if (!header.size)
			break;
	}

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
	void* block;
	u32 block_size;
	block_header header = {0};

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	for (u32 i = 0; i < size; i += header.size)
	{
		printf("[*] Encrypting Block At Offset 0x%X\n", i);

		block = (&data[i] + sizeof(block_header));
		memcpy(&header, &data[i], sizeof(block_header));

		if (isPS3)
		{
			header.size = ES32(header.size);
			header.seed = ES32(header.seed);
		}

		block_size = (header.size) ? header.size - sizeof(block_header) : (size - i - sizeof(block_header));

//		printf(" - Block Magic: 0x%08X Size: 0x%08X Checksum: 0x%08X Seed: 0x%08X\n", header.magic, header.size, header.checksum, header.seed);

		crypt_block(block, block_size, header.seed);

		header.checksum = add_csum(block, block_size);
		printf("[i] New Block Checksum: %08X\n", header.checksum);

		if (isPS3)
			header.checksum = ES32(header.checksum);

		memcpy(&data[i] + 8, &header.checksum, sizeof(uint32_t));
			
		if (!header.size)
			break;
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

	printf("\nDOA5 Save Decrypter 0.1.0 - (c) 2026 by Bucanero\n\n");

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

	isPS3 = memcmp(data, "\0\0\0\0\0", 6) == 0;

	if (*opt == 'd')
		decrypt_data(data, len);
	else
		encrypt_data(data, len);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
