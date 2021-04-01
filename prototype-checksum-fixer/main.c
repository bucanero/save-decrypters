/*
*
*	Prototype 1 & 2 PS3 Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original "Prototype & Prototype 2 checksum fixer" by pink1
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define u8 uint8_t
#define u32 uint32_t

#define CRC32_POLY    0x04C11DB7
#define CRC32_INIT    0xFFFFFFFF

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

void init_crc32_table(uint32_t* crc_table, uint32_t poly)
{
    for (int i = 0; i < 256; ++i)
    {
        uint32_t r = i << 24;
        
        for (int j = 0; j < 8; ++j)
            r = (r & 0x80000000) ? (r << 1) ^ poly : (r << 1);
        
        crc_table[i] = r;
    }

	return;
}

u32 calc_crc32(const u8* data, u32 len, u32 init)
{
	u32 crc32_table[256];
	u32 crc = init;

	init_crc32_table(crc32_table, CRC32_POLY);

	while (len--)
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *data++) & 0xFF];

    return crc;
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
	u32 crc;
	char *opt, *bak;

	printf("\nPrototype 1 & 2 Checksum Fixer 0.1.0 - (c) 2021 by Bucanero\n\n");
	

/*
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
*/

	if (read_buffer("ICON0.PNG", &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", "ICON0.PNG");
		return -1;
	}

	crc = calc_crc32(data, len, CRC32_INIT);
	free(data);

	if (read_buffer("USERDATA.BIN", &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", "USERDATA.BIN");
		return -1;
	}

	crc = calc_crc32(data, len, crc);
	printf("[*] Calculated Checksum: %08X\n", crc);

	crc = ES32(crc);

	if (write_buffer("CRC.BIN", (u8*) &crc, sizeof(u32)) == 0)
		printf("[*] Successfully Wrote New CRC.BIN Checksum!\n\n");

	free(data);

	// Save a file backup
	//asprintf(&bak, "%s.bak", argv[2]);
	//write_buffer(bak, data, len);
	//free(bak);

	return 0;
}
