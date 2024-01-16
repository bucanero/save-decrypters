/*
*
*	Like a Dragon: Ishin! (PS4) Decrypter - (c) 2024 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the original https://github.com/GarnetSunset/rgg_save_tool_py/
*
*/

#include "../common/iofile.c"

#define RGG_LAD_ISHIN	"fuEw5rWN8MBS"

#define RGG_je			"OphYnzbPoV5lj"
#define RGG_lj			"jKMQEv7S4l9hd"
#define RGG_gd			"jKMQEv7S4l9hd"
#define RGG_y6			"VI3rbPckNsea7JOUMrgT"
#define RGG_y7			"STarYZgr3DL11"
#define RGG_y7_gog		"r3DL11STarYZg"
#define RGG_yk2			"STarYZgr3DL11"


void xor_data(uint8_t* data, u32 sz, const char* Key)
{
	u32 KeyLen = strlen(Key);

	for (int i = 0; i < sz; i++)
		data[i] ^= Key[i % KeyLen];
}

unsigned int crc32b(unsigned char *message, int len)
{
   int j;
   unsigned int crc = 0xFFFFFFFF;

   while (len--)
   {
      crc ^= *message++;            // Get next byte.
      for (j = 7; j >= 0; j--)      // Do eight times.
         crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
   }
   return ~crc;
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
	u8 *data;
	u32 csum;
	char *opt, *bak;

	printf("\nLike a Dragon: Ishin! (PS4) Decrypter 0.1.0 - (c) 2024 by Bucanero\n\n");

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
		printf("[*] Could Not Access The File (%s)\n", argv[1]);
		return -1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	printf("[*] Data Size       : %lu bytes\n", len - 0x10);

	if (*opt == 'd')
		xor_data(data, len - 0x10, RGG_LAD_ISHIN);
	else
	{
		csum = crc32b(data, len - 0x10);
		printf("[*] Stored Checksum : %08X\n", *(u32*)(data + len - 8));
		printf("[*] New Checksum    : %08X\n", csum);

		xor_data(data, len - 0x10, RGG_LAD_ISHIN);
		memcpy(data + len - 8, &csum, 4);
	}

	if (write_buffer(argv[2], data, len) == 0)
		printf("[*] Successfully %s %s!\n\n", (*opt == 'd') ? "Decrypted" : "Encrypted", argv[2]);

	free(bak);
	free(data);

	return 0;
}
