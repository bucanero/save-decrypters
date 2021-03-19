/*
*
*	Kingdom Hearts HD 2.5 ReMIX PS3 Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Kingdom Save Editor" source by Xeeynamo
*	https://github.com/Xeeynamo/KH3SaveEditor
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void kh25_crc32_table(uint32_t* crc_table, uint32_t polynomial)
{
    for (int x = 0; x < 0x100; x++)
    {
        int r = x << 24;
        for (int j = 0; j < 0xff; j++)
            r = (r << 1) ^ (r < 0 ? polynomial : 0);

        crc_table[x] = r;
    }
}

u32 calc_crc32(const u8* data, u32 len, u32 init)
{
	u32 crc32_table[256];
	u32 crc = init;

	kh25_crc32_table(crc32_table, CRC32_POLY);

	while (len--)
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *data++) & 0xFF];

    return crc;
}

u32 dwadd_le(const u32* data, u32 len)
{
    u32 checksum = 0;
    len /= 4;

    while (len--)
		checksum += *data++;

    return checksum;
}

u32 kh_com_checksum(u8* data, u32 len)
{
    int checksum = CRC32_INIT;

    while (len--)
    {
        checksum ^= (*data++ << 31);
        checksum = (checksum << 1) ^ (checksum < 0 ? CRC32_POLY : 0);
    }

    return (~checksum);
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -1            Kingdom Hearts HD 2.5 ReMIX\n");
	printf(" -2            Kingdom Hearts HD Birth by Sleep (BBS)\n");
	printf(" -3            Kingdom Hearts Re: Chain of Memories (CoM)\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	u32 checksum;
	char *opt, *bak;

	printf("\nKingdom Hearts HD 2.5 ReMIX+ Checksum Fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}

	opt = argv[1];
	if (*opt++ != '-' || (*opt != '1' && *opt != '2' && *opt != '3'))
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

	if (*opt == '1')
	{
	    checksum = calc_crc32(data, 8, CRC32_INIT);
	    checksum = ~calc_crc32(data + 0xc, len - 0xc, checksum);
	
		printf("[*] Stored Checksum    : %08X\n", *(u32*)(data+8));
		memcpy(data + 8, &checksum, sizeof(u32));
	}
	else if (*opt == '2')
	{
		checksum = dwadd_le((u32*) (data + 0x10), len - 0x10);

		printf("[*] Stored Checksum    : %08X\n", *(u32*)(data+12));
		memcpy(data + 12, &checksum, sizeof(u32));
	}
	else
	{
		checksum = ES32(kh_com_checksum(data + 0x10, len - 0x10));

		printf("[*] Stored Checksum    : %08X\n", *(u32*)(data+4));
		memcpy(data + 4, &checksum, sizeof(u32));
	}

	printf("[*] Calculated Checksum: %08X\n", checksum);

	if (write_buffer(argv[2], data, len) == 0)
		printf("[*] Successfully Wrote New %s Checksum!\n\n", argv[2]);

	free(data);
	free(bak);

	return 0;
}
