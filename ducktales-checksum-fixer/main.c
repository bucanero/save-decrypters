/*
*
*	DuckTales: Remastered (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Security Archive" notes by Philymaster (https://community.wemod.com/t/philymasters-security-archive/3923)
*
*/

#include "../common/iofile.c"


u64 CalculateChecksum(const u8* data, int offset, int size) {
    u32  r1, r2, r3, r4, r5, r6, r7, r8;

    r4 = (u32) size + 0xDEADC3C1;
    r6 = r4 + 0x162E;
    r5 = r4;
                
    for (int i = size / 12; i > 0; --i) {
        r7 =  *(u32*) &data[offset];
        r4 += *(u32*) &data[offset + 4];
        r6 += *(u32*) &data[offset + 8];
        offset += 12;

        r5 = (r5 + r7 - r6) ^ ((r6 << 4) | (r6 >> 28));
        r2 = r4 - r5;
        r4 += r6;
        r6 = r2 ^ ((r5 << 6) | (r5 >> 26));
        r5 += r4;         
        r3 = r4 - r6;                  
        r4 = r6 + r5;
        r6 = r3 ^ ((r6 << 8) | (r6 >> 24));                                   
        r3 = r4 + r6;
        r6 = (r5 - r6) ^ ((r6 << 16) | (r6 >> 16));                                  
        r5 = r3 + r6;
        r6 = (r4 - r6) ^ ((r6 >> 13) | (r6 << 19));      
        r4 = r5 + r6;
        r6 = (r3 - r6) ^ ((r6 <<  4) | (r6 >> 28));      
    }

    for (int i = 0; i < 4; ++i)
        r5 += (u32) data[offset++] << (i * 8);

    u32 x0, x1;
    r3 = (r6 ^ r4) - ((r4 << 14) | (r4 >> 18));    
    r1 = (r5 ^ r3) - ((r3 << 11) | (r3 >> 21));        
    r4 = (r4 ^ r1) - ((r1 >>  7) | (r1 << 25));
    r6 = (r3 ^ r4) - ((r4 >> 16) | (r4 << 16));
    r8 = (r1 ^ r6) - ((r6 <<  4) | (r6 >> 28));
    r5 = (r4 ^ r8) - ((r8 << 14) | (r8 >> 18));
    x1 = (r6 ^ r5) - ((r5 >> 8) | (r5 << 24));
    x0 = r5;
            
    return ((u64) x1 << 32) | x0;
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
	u64 csum;
	char *opt, *bak;

	printf("\nDuckTales: Remastered (PS3) checksum fixer 0.1.0 - (c) 2021 by Bucanero\n\n");

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

	printf("[*] File Size       : %lu bytes\n", len);
	printf("[*] Stored Checksum : %016llX\n", ES64(*(u64*)data));

	csum = ES64(CalculateChecksum(data, 0x10, 0x400));
	memcpy(data, &csum, sizeof(u64));

	printf("[*] New Checksum    : %016llX\n", ES64(csum));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
