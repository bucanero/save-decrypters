/*
*
*	Dead Space 2 & 3 (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original DS2_Checksum_Fixer, DS3_Checksum_Fixer by Red-EyeX32
*
* Information about the SDBM hash method:
*	- http://www.cse.yorku.ca/~oz/hash.html#sdbm
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define u8 uint8_t
#define u32 uint32_t

#define DS2_CSUM_OFFSET    0x04
#define DS2_SIZE_OFFSET    0x6C
#define DS3_CSUM_OFFSET    0x08
#define DS3_SIZE_OFFSET    0x4C
#define DS3_HED_SIZE       0x80

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

u32 sdbm_hash(const u8* data, u32 len, u32 init)
{
    u32 crc = init;
    
    while (len--)
        crc = (crc * 0x1003f) + *data++;

    return (crc);
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
	u8 *data_hed, *data_usr;
	u32 csum_off, size_off;
	u32 csum, usr_size;
	char *opt, *bak;

	printf("\nDead Space 2 & 3 (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");
	

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

	if (read_buffer("HED-DATA", &data_hed, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", "HED-DATA");
		return -1;
	}

	if (read_buffer("USR-DATA", &data_usr, NULL) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", "USR-DATA");
		return -1;
	}

	// Save a file backup
	//asprintf(&bak, "%s.bak", "HED-DATA");
	//write_buffer(bak, data_hed, len);

	csum_off = (len == DS3_HED_SIZE) ? DS3_CSUM_OFFSET : DS2_CSUM_OFFSET;
	size_off = (len == DS3_HED_SIZE) ? DS3_SIZE_OFFSET : DS2_SIZE_OFFSET;
	usr_size = ES32(*(u32*)(data_hed + size_off));

	printf("[*] Calculation Size: %d bytes\n", usr_size);
	printf("[*] Stored Checksum : %08X\n", ES32(*(u32*)(data_hed + csum_off)));

	memset(data_hed + csum_off, 0, sizeof(u32));
	csum = sdbm_hash(data_hed, len, 0);
	csum = sdbm_hash(data_usr, usr_size, csum);
	printf("[*] New Checksum    : %08X\n", csum);
	
	csum = ES32(csum);
	memcpy(data_hed + csum_off, &csum, sizeof(u32));

	if (write_buffer("HED-DATA", data_hed, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");
	
	//free(bak);
	free(data_hed);
	free(data_usr);

	return 0;
}
