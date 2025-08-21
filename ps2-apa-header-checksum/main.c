/*
*
*	PS2 APA Header Checksum Fixer - (c) 2024 by Bucanero - www.bucanero.com.ar
*
*/

#include "../common/iofile.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int ComputeHash(const uint32_t* Bytes, uint32_t Length)
{
    uint32_t sum = 0;
    Length /= 4;

    while (Length--)
    {
        sum += *Bytes++;
    }

	return (sum);
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s filename\n\n", argv0);
	return;
}

int main(int argc, char **argv)
{
	FILE *fp;
	uint8_t data[1024];
	uint32_t csum;
	char *opt, *bak;

	printf("\nPS2 APA Header checksum 0.1.0 - (c) 2024 by Bucanero\n\n");

	if (--argc < 1)
	{
		print_usage(argv[0]);
		return -1;
	}

	fp = fopen(argv[1], "rb");
	if (fread(data, 1, sizeof(data), fp) != 0x400)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[1]);
		return -1;
	}

	printf("[*] Header Size     : %d bytes\n", 0x400);
	printf("[*] Stored Checksum : %08X\n", ES32(*(uint32_t*)data));

	csum = ComputeHash((uint32_t*)(data + 4), 1020);
	printf("[*] New Checksum    : %08X\n\n", ES32(csum));

	return 0;
}
