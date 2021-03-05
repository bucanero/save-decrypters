/*
*
*	Need for Speed Undercover PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original NFS_Undercover_Save_Decrypter by Red-EyeX32
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.c"

#define u8 uint8_t
#define u32 uint32_t

#define ES32(_val) \
	((u32)(((((u32)_val) & 0xff000000) >> 24) | \
	       ((((u32)_val) & 0x00ff0000) >> 8 ) | \
	       ((((u32)_val) & 0x0000ff00) << 8 ) | \
	       ((((u32)_val) & 0x000000ff) << 24)))

#define BLOCK_SIZE	16

const uint8_t NFS_XOR_KEY[16] = {
	33, 243, 198, 210, 8, 99, 170, 172, 56, 226, 32, 98, 13, 13, 77, 82
};


int read_buffer(const char *file_path, u8 **buf, size_t *size)
{
	FILE *fp;
	u8 *file_buf;
	size_t file_size;
	
	if ((fp = fopen(file_path, "rb")) == NULL)
        return -1;
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	file_buf = (u8 *)malloc(file_size);
	fread(file_buf, 1, file_size, fp);
	fclose(fp);
	
	if (buf)
        *buf = file_buf;
	else
        free(file_buf);
	if (size)
        *size = file_size;
	
	return 0;
}

int write_buffer(const char *file_path, u8 *buf, size_t size)
{
	FILE *fp;
	
	if ((fp = fopen(file_path, "wb")) == NULL)
        return -1;
	fwrite(buf, 1, size, fp);
	fclose(fp);
	
	return 0;
}

void md5_block(const u8* in, u8* out)
{
	MD5_CTX context;

	MD5Init (&context);
	MD5Update (&context, in, BLOCK_SIZE);
	MD5Final (out, &context);
}

void xor_block(const u8* in, u8* out)
{
	for (int i = 0; i < BLOCK_SIZE; i++)
		out[i] ^= in[i];
}

void decrypt_data(u8* data, u32 size)
{
	u8 xor_key[BLOCK_SIZE];
	u8 tmp[BLOCK_SIZE];

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	// init xor key
	memcpy(xor_key, NFS_XOR_KEY, BLOCK_SIZE);
	xor_block(data, xor_key);
	md5_block(xor_key, xor_key);

	size /= BLOCK_SIZE;
	
	while (size--)
	{
		data += BLOCK_SIZE;

		md5_block(data, tmp);
		xor_block(xor_key, data);
		memcpy(xor_key, tmp, BLOCK_SIZE);
	}

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
	u8 xor_key[BLOCK_SIZE];

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	// init xor key
	memcpy(xor_key, NFS_XOR_KEY, BLOCK_SIZE);
	xor_block(data, xor_key);
	md5_block(xor_key, xor_key);

	size /= BLOCK_SIZE;

	while (size--)
	{
		data += BLOCK_SIZE;

		xor_block(xor_key, data);
		md5_block(data, xor_key);
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
	u32 enc_size;
	char *opt, *bak;

	printf("\nnfs-undercover-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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

	enc_size = ES32(*(u32*)(data + 0x64)) - 20;

	if (*opt == 'd')
		decrypt_data(data + 0x70, enc_size);
	else
		encrypt_data(data + 0x70, enc_size);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
