/*
*
*	Samurai Warriors 4 (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original Samurai Warriors 4 checksum fixer by chaoszage
*
*/

#include "../common/iofile.c"

#define SW4_OFF_1      0x00004
#define SW4_OFF_2      0x000A8
#define SW4_OFF_3      0x00C16
#define SW4_OFF_JP     0x58292
#define SW4_OFF_US     0x58532

u32 add(const u8* data, u32 len)
{
    u32 checksum = 0;

    while (len--)
        checksum += *data++;

    return checksum;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s filename\n\n", argv0);
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8 *data, is_jp;
	u32 csum;
	char *opt, *bak;

	printf("\nSamurai Warriors 4 (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

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

	if (data[2] != 0x21 || data[3] != 0x18)
	{
		printf("[*] Not A Samurai Warriors 4 File (%s)\n", argv[1]);
		return -1;
	}
	
	is_jp = (*(u32*)(data + SW4_OFF_JP) != 0);

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[1]);
	write_buffer(bak, data, len);

	printf("[*] File Size       : %lu bytes\n", len);
	printf("[*] Stored Checksum : %08X %08X %08X %08X\n", 
		ES32(*(u32*)&data[SW4_OFF_1]),
		ES32(*(u32*)&data[SW4_OFF_2]),
		ES32(*(u32*)&data[SW4_OFF_3]),
		ES32(*(u32*)&data[is_jp ? SW4_OFF_JP : SW4_OFF_US]));

	memset(data + SW4_OFF_1, 0, sizeof(u32));
	memset(data + SW4_OFF_2, 0, sizeof(u32));
	memset(data + SW4_OFF_3, 0, sizeof(u32));
	memset(data + (is_jp ? SW4_OFF_JP : SW4_OFF_US), 0, sizeof(u32));

	u32 num1, num2, num3, num4, num5, num6;

	num2 = add(data + SW4_OFF_1 + 4, SW4_OFF_2 - (SW4_OFF_1 + 4));
	num3 = add(data + SW4_OFF_2 + 4, 2284 - (SW4_OFF_2 + 4));

	if (is_jp)
	{
		num1 = add(data + SW4_OFF_3 + 4, 30294 - (SW4_OFF_3 + 4));
		num4 = add(data + 33630, 361106 - 33630);
		num5 = add(data + 30294, 33631 - 30294);
		num6 = add(data + SW4_OFF_1 + 4, 30294 - (SW4_OFF_1 + 4));
	}
	else
	{
		num1 = add(data + SW4_OFF_3 + 4, 30934 - (SW4_OFF_3 + 4));
		num4 = add(data + 34270, 361778 - 34270);
		num5 = add(data + 30934, 34271 - 30934);
		num6 = add(data + SW4_OFF_1 + 4, 30934 - (SW4_OFF_1 + 4));
	}

//	num5 += (num1 + num2) * num3;
//	num5 &= 0x7FFFFFFF;
//	num4 = num2 * (num1 + num3 + num4);
//	num4 &= 0x7FFFFFFF;
//	u32 num7 = num5 + num4 + num6;
//	num7 &= 0x7FFFFFFF;

	u32 crc1 = 0x7FFFFFFF & (num5 + (num1 + num2) * num3);
	u32 crc2 = 0x7FFFFFFF & (num2 * (num1 + num3 + num4));
	u32 crc3 = 0x7FFFFFFF & (crc1 + crc2 + num6);

	printf("[*] New Checksum    : %08X %08X %08X %08X\n", crc1, crc2, num6, crc3);

	crc1 = ES32(crc1);
	crc2 = ES32(crc2);
	crc3 = ES32(crc3);
	num6 = ES32(num6);

	memcpy(data + SW4_OFF_1, &crc1, sizeof(u32));
	memcpy(data + SW4_OFF_2, &crc2, sizeof(u32));
	memcpy(data + SW4_OFF_3, &num6, sizeof(u32));
	memcpy(data + (is_jp ? SW4_OFF_JP : SW4_OFF_US), &crc3, sizeof(u32));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
