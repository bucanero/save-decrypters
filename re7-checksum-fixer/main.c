/*
*	Resident Evil 7: Biohazard Checksum Fixer - (c) 2025 by hzh
*	https://github.com/hzhreal/
*/

#include <inttypes.h>
#include <limits.h>
#include <string.h>

#include "../common/iofile.c"

#define HIWORD(n) ((u32)(n) >> 16)
#define BS sizeof(u32)
#define BS_BITS BS * 8

u32 ROL4(u32 n, u32 count) {
    count %= BS_BITS;
    u32 high = n >> (BS_BITS - count);
    n <<= count;
    n |= high;
    return n;
}

u32 checksum(u8 *buf, u32 size) {
    u8 val = 0;
    u8 remainder = size & (BS - 1);
    u32 *in = (u32 *)buf;
    u32 csum = -1;
    u32 blocks = size / BS;
    u32 i;
  
    if (size >= BS) 
    {
        for (i = 0; i < blocks; i++) 
        {
            csum = 5 * ROL4(csum ^ (0x1B873593 * ((0x16A88000 * in[i]) | ((-0x3361D2AF * in[i]) >> 17))), 13) - 0x19AB949C;
        }
    }

    switch(remainder)
    {
        case 1:
            val = val ^ *(u8 *)(in + BS * blocks);
            csum ^= 0x1B873593 * ((0x16A88000 * val) | ((-0x3361D2AF * val) >> 17));
            break;
        case 2:
        case 3:
            if (remainder == 3) 
            {
                val = *(u8 *)(in + BS * blocks + 2) << 16;
            }
            val |= *(u8 *)(in + BS * blocks + 1) << 8;
            val = val ^ *(u8 *)(in + BS * blocks);
            csum ^= 0x1B873593 * ((0x16A88000 * val) | ((-0x3361D2AF * val) >> 17));
            break;
    }
    csum = -0x3D4D51CB * ((-0x7A143595 * (size ^ csum ^ ((size ^ csum) >> 16))) ^ ((-0x7A143595 * (size ^ csum ^ ((size ^ csum) >> 16))) >> 13));
    return csum ^ HIWORD(csum);
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s filename\n\n", argv0);
	return;
}

int main(int argc, char **argv)
{
    u8 *data;
    u32 csum;
    size_t len;
    char *bak;
    char *filename;

    printf("\nResident Evil 7: Biohazard (PS4) checksum fixer\n\n");

    if (--argc < 1)
    {
        print_usage(argv[0]);
        return -1;
    }
    filename = argv[1];

    if (read_buffer(filename, &data, &len) != 0)
    {
        printf("[*] Could Not Access The File (%s)\n", filename);
        return -1;
    }
    if (len > UINT32_MAX) {
        printf("[*] File is too large (%s)\n", filename);
        return -1;
    }

    // Save a file backup
    asprintf(&bak, "%s.bak", filename);
    write_buffer(bak, data, len);
    free(bak);

    csum = checksum(data, (u32)(len - sizeof(u32)));
    printf("[*] Updated CHKS: %" PRIX32 "\n", ES32(csum));

    memcpy(data + len - sizeof(u32), &csum, sizeof(u32));
    write_buffer(filename, data, len);
    printf("[*] Patched File Successfully!\n");

    free(data);
    return 0;
}
