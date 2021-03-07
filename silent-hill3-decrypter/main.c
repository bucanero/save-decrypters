/*
*
*	Silent Hill 3 HD PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original Silent Hill 3 HD Save Editor by chaoszage
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define u8 uint8_t
#define u32 uint32_t
#define u64 uint64_t

#define ES32(_val) \
	((u32)(((((u32)_val) & 0xff000000) >> 24) | \
	       ((((u32)_val) & 0x00ff0000) >> 8 ) | \
	       ((((u32)_val) & 0x0000ff00) << 8 ) | \
	       ((((u32)_val) & 0x000000ff) << 24)))

#define SH3_KEY1		0x5b6c3a2aL
#define SH3_KEY2		0x100000000L


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

void decrypt_data(u8* data, u32 size)
{
	u32 out;
	u64 input, key2 = SH3_KEY2;

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	size /= 4;

	while (size--)
	{
	    input = ES32(*(u32*)data);
	    out = ES32((u32)((input ^ key2 - SH3_KEY1) & 0xFFFFFFFF));
	    memcpy(data, &out, sizeof(u32));

	    key2 = (input << 5 | input >> 27) + (u64)SH3_KEY2;
	    data += 4;
	}

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

u32 checksum_data(u8* data, u32 size)
{
	u32 crc = 0;

	// clear the CRC
	*(u32*)(data + 0x10) = 0;
	size /= 4;

	while (size--)
	{
	    crc += ES32(*(u32*)data);
	    data += 4;
	}

	crc = (crc * 2) + 1;

	printf("[*] New Checksum %08X\n", crc);
	return(ES32(crc));
}

void encrypt_data(u8* data, u32 size)
{
	u32 out, tmp;
	u64 input, key2 = SH3_KEY2;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	size /= 4;

	while (size--)
	{
	    input = ES32(*(u32*)data);
	    out = (u32)((input ^ key2 - SH3_KEY1) & 0xFFFFFFFF);
	    tmp = ES32(out);
	    memcpy(data, &tmp, sizeof(u32));

	    key2 = (u64)(out << 5 | out >> 27) + (u64)SH3_KEY2;
	    data += 4;
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

	printf("\nsilent-hill3-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");

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
	
	enc_size = ES32(*(u32*)(data + 12));

	if (*opt == 'd')
		decrypt_data(data + 0x40, enc_size);
	else
	{
		u32 crc = checksum_data(data, enc_size + 0x40);
		memcpy(data + 0x10, &crc, sizeof(u32));
		encrypt_data(data + 0x40, enc_size);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
