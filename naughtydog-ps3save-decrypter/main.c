/*
*
*	Naughty Dog PS3 Save Decrypter - (c) 2020 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original tlou_save_data_decrypter by Red-EyeX32 and aerosoul94
*
* Information about the Blowfish encryption method:
*	- https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "keys.h"

#define u8 uint8_t
#define u32 uint32_t

#define ES32(_val) \
	((u32)(((((u32)_val) & 0xff000000) >> 24) | \
	       ((((u32)_val) & 0x00ff0000) >> 8 ) | \
	       ((((u32)_val) & 0x0000ff00) << 8 ) | \
	       ((((u32)_val) & 0x000000ff) << 24)))


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

void crypt_64bit_up(const u32* keybuf, u32* ptr)
{
	u32 x = ptr[0];
	u32 y = ptr[1];
	u32 z;
	int i;

	for (i = 0; i < 0x10; i++) {
		z = keybuf[i] ^ x;
		x = keybuf[0x012 + ((z>>24)&0xff)];
		x = keybuf[0x112 + ((z>>16)&0xff)] + x;
		x = keybuf[0x212 + ((z>> 8)&0xff)] ^ x;
		x = keybuf[0x312 + ((z>> 0)&0xff)] + x;
		x = y ^ x;
		y = z;
	}

	ptr[1] = x ^ keybuf[0x10];
	ptr[0] = y ^ keybuf[0x11];
}

void crypt_64bit_down(const u32* keybuf, u32* ptr)
{
	u32 x = ptr[0];
	u32 y = ptr[1];
	u32 z;
	int i;

	for (i = 0x11; i > 0x01; i--) {
		z = keybuf[i] ^ x;
		x = keybuf[0x012 + ((z>>24)&0xff)];
		x = keybuf[0x112 + ((z>>16)&0xff)] + x;
		x = keybuf[0x212 + ((z>> 8)&0xff)] ^ x;
		x = keybuf[0x312 + ((z>> 0)&0xff)] + x;
		x = y ^ x;
		y = z;
	}

	ptr[1] = x ^ keybuf[0x01];
	ptr[0] = y ^ keybuf[0x00];
}

void apply_keycode(u32* keybuf, const u32* keydata, const char* keycode)
{
	int i;
	u32 scratch[2] = {0, 0};
	char tmp[4];
	int len = strlen(keycode);

	memcpy(keybuf + 0x12, keydata, 0x1000);

  	for (i = 0; i < 0x12; i++)
  	{
  		//Little-Endian
	    tmp[3]=keycode[(i*4   ) % len];
	    tmp[2]=keycode[(i*4 +1) % len];
	    tmp[1]=keycode[(i*4 +2) % len];
	    tmp[0]=keycode[(i*4 +3) % len];

	    keybuf[i] = keydata[0x400 + i] ^ *(u32*)tmp;
	}

	for (i = 0; i < 0x412; i += 2)
	{
		crypt_64bit_up(keybuf, scratch);
		keybuf[i] = scratch[0];
		keybuf[i+1] = scratch[1];
	}
}

void decrypt_data(const u32* key_buffer, u32* data, u32 size)
{
	int i;
	u32 buf[2];

    printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);
    size = size/4;

	data[0] = ES32(0x0000157C);
	data += 2;

	for (i = 0; i < size; i+= 2)
	{
		buf[0] = ES32(data[i]);
		buf[1] = ES32(data[i+1]);
		crypt_64bit_down(key_buffer, buf);

		data[i] = ES32(buf[0]);
		data[i+1] = ES32(buf[1]);
	}

    printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(const u32* key_buffer, u32* data, u32 size)
{
	int i;
	u32 buf[2];

    printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);
    size = size/4;

	data[0] = ES32(0x0000157D);
	data += 2;

	for (i = 0; i < size; i+= 2)
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
	u32 dsize;
	char *opt, *bak;

	printf("\nnaughtydog-ps3save-decrypter 0.1.0 - (c) 2020 by Bucanero\n\n");

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

	u8* key_table = malloc(KEYSIZE);
	if (!key_table)
		return -1;

	apply_keycode((u32*) key_table, (u32*) KEY_DATA, SECRET_KEY);

	if (read_buffer(argv[2], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[2]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	dsize = *(u32*) &data[len-4];
	dsize = ES32(dsize);

	if (*opt == 'd')
		decrypt_data((u32*) key_table, (u32*) data, dsize);
	else
		encrypt_data((u32*) key_table, (u32*) data, dsize);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	free(key_table);
	
	return 0;
}
