/*
*
*	DBZ Xenoverse 2 Decrypter - (c) 2025 by Bucanero - www.bucanero.com.ar
*	based on
		- https://github.com/bawsdeep/xenoverse2_crypt_checksum by bawsdeep
		- https://animegamemods.freeforums.net/thread/4059/tools-eternity by Eternity
*/

#define CTR 1

#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/md5.c"


#define SAVE_HEADER_KEY       "PR]-<Q9*WxHsV8rcW!JuH7k_ug:T5ApX"
#define SAVE_HEADER_IV        "_Y7]mD1ziyH#Ar=0"
#define SAVE_HEADER_SIZE      0x80
#define SAVE_MAGIC_HEADER     "H\x89\x01L"


void md5_hash(const u8* in, u32 size, u8* out)
{
	MD5_CTX context;

	MD5Init (&context);
	MD5Update (&context, in, size);
	MD5Final (out, &context);
}

void add_checksums(u8* header, const u8* data, size_t size)
{
    int i;

    // For checksum 7
    header[0x15] = 0;
    for (i = 0; i < 14; i++)
        header[0x15] += header[0x6 + i];
    printf("[i] Checksum #7: %02X\n", header[0x15]);

    // For checksum 6
    header[0x16] = 0;
    for (i = 0; i < 8; i++)
        header[0x16] += header[0x1C + (i * 4)];
    printf("[i] Checksum #6: %02X\n", header[0x16]);

    // For checksum 5
    header[0x17] = 0;
    for (i = 0; i < 8; i++)
        header[0x17] += header[0x4C + (i * 4)];
    printf("[i] Checksum #5: %02X\n", header[0x17]);

    /// For checksum 4
    header[0x18] = 0;
    for (i = 0; i < 4; i++)
        header[0x18] += header[0x3C + (i * 4)];
    printf("[i] Checksum #4: %02X\n", header[0x18]);

    // For checksum 3
    header[0x19] = 0;
    for (i = 0; i < 4; i++)
        header[0x19] += header[0x6C + (i * 4)];
    printf("[i] Checksum #3: %02X\n", header[0x19]);

    // For checksum 2
    header[0x1B] = 0;
    for (i = 5; i < size / 0x20; i++)
        header[0x1B] += data[i * 0x20];
    printf("[i] Checksum #2: %02X\n", header[0x1B]);

    // For checksum 1
    header[0x14] = header[0x5];
    for (i = 0; i < 7; i++)
        header[0x14] += header[0x15 + i];
    printf("[i] Checksum #1: %02X\n", header[0x14]);
}

void decrypt_data(u8* data, size_t size)
{
	int key_off;
	u8 iv[16];
	u8* header = data + 0x20;
	struct AES_ctx ctx;

	printf("[*] Total Decrypted Size: %zu bytes\n", size);

	md5_hash(data + 0x20, size - 0x20, iv);
	if (memcmp(data + 0x10, iv, 0x10) == 0)
		printf("[*] MD5 Hash OK!\n");
	else
		printf("[*] MD5 Hash Error!\n");

	AES_init_ctx_iv(&ctx, (u8*) SAVE_HEADER_KEY, (u8*) SAVE_HEADER_IV);
	AES_CTR_xcrypt_buffer(&ctx, header, SAVE_HEADER_SIZE);

	key_off = (header[5] & 0x04) ? 0x4C : 0x1C;
	memcpy(iv, &header[key_off + 0x20], 0x10);
	AES_init_ctx_iv(&ctx, &header[key_off], iv);
	AES_CTR_xcrypt_buffer(&ctx, data + 0xA0, size - 0xA0);

	printf("[*] Decrypted File Successfully!\n\n");

	return;
}

void encrypt_data(u8* data, size_t size)
{
	int key_off;
	u8 iv[16];
	u8* header = data + 0x20;
	struct AES_ctx ctx;

	printf("[*] Total Encrypted Size: %zu bytes\n", size);

	// For checksum 8
	header[0x1A] = 0;
	for (int i = 5; i < size / 0x20; i++)
		header[0x1A] += data[i * 0x20];
	printf("[i] Checksum #8: %02X\n", header[0x1A]);

	key_off = (header[5] & 0x04) ? 0x4C : 0x1C;
	memcpy(iv, &header[key_off + 0x20], 0x10);
	AES_init_ctx_iv(&ctx, &header[key_off], iv);
	AES_CTR_xcrypt_buffer(&ctx, data + 0xA0, size - 0xA0);

	// Checksums 1-7 calculated over encrypted data
	add_checksums(header, data, size);

	AES_init_ctx_iv(&ctx, (u8*) SAVE_HEADER_KEY, (u8*) SAVE_HEADER_IV);
	AES_CTR_xcrypt_buffer(&ctx, header, SAVE_HEADER_SIZE);

	printf("[*] Encrypted File Successfully!\n");
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

	printf("\nDBZ Xenoverse 2 Decrypter 0.1.0 - (c) 2025 by Bucanero\n\n");

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

	if (memcmp(data, SAVE_MAGIC_HEADER, 4) != 0)
	{
		printf("[*] Error! Invalid Header\n");
		return(-1);
	}

	if (*opt == 'd')
		decrypt_data(data, len);
	else
	{
		encrypt_data(data, len);
		md5_hash(data + 0x20, len - 0x20, data + 0x10);
		printf("[*] MD5 Hash Updated!\n\n");
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
