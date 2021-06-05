/*
*
*	Final Fantasy X & X-2 HD (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based on:
* https://github.com/MarkH221/FFX-HD-Checksum-Utility
* https://github.com/BitPatty/FFXChecksumRepairTool
* http://forums.pcsx2.net/Thread-Celsius-FFX-2-Save-game-editor?pid=182634#pid182634
*
*/

#include "../common/iofile.c"

#define FFX_FILE_SIZE      0x68F8
#define FFX_END_OFFSET     0x64F8
#define FFX2_END_OFFSET    0x1626C

#define CRC16_POLY         0x1021
#define CRC16_INIT         0xFFFF


void init_crc16_table(uint16_t* crc_table, uint16_t poly)
{
    for (int i = 0; i < 256; ++i)
    {
        uint16_t r = i << 8;
        
        for (int j = 0; j < 8; ++j)
            r = (r & 0x8000) ? (r << 1) ^ poly : (r << 1);
        
        crc_table[i] = r;
    }

	return;
}

u16 calc_crc16(const u8* data, u32 len)
{
	u16 crc16_table[256];
	u16 crc = CRC16_INIT;

	init_crc16_table(crc16_table, CRC16_POLY);
	crc16_table[255] = 0;

	while (len--)
        crc = (crc << 8) ^ crc16_table[((crc >> 8) ^ *data++) & 0xFF];

    return ~crc;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s filename\n\n", argv0);
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8 *data;
	u16 csum;
	u32 offset;
	char *bak;

	printf("\nFinal Fantasy X & X-2 HD (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

	if (--argc < 1)
	{
		print_usage(argv[0]);
		return -1;
	}
	
	if (read_buffer(argv[1], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[1]);
		return -1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[1]);
	write_buffer(bak, data, len);
	
	offset = (len > FFX_FILE_SIZE) ? FFX2_END_OFFSET : FFX_END_OFFSET;

	printf("[*] Calculation Size: %d bytes\n", offset - 0x40);
	printf("[*] Stored Checksum : %02X%02X\n", data[0x1A], data[0x1B]);

	memset(data + offset - 4, 0, sizeof(u16));
	csum = calc_crc16(data + 0x40, offset - 0x40);

	printf("[*] New Checksum    : %04X\n", ES16(csum));

	memcpy(data + 0x1A, &csum, sizeof(u16));
	memcpy(data + offset - 4, &csum, sizeof(u16));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");
	
	free(bak);
	free(data);
	
	return 0;
}
