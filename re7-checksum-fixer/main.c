/*
*	Resident Evil 7: Biohazard Checksum Fixer - (c) 2025 by hzh
*	https://github.com/hzhreal/
*/

#include <inttypes.h>
#include <limits.h>
#include <string.h>

#include "../common/iofile.c"
#include "../common/mmh3.c"

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

    printf("[*] Current Checksum: %" PRIX32 "\n", *(u32*)(data + len - 4));
    csum = murmur3_32(data, len - sizeof(u32), 0xFFFFFFFF);
    printf("[*] Updated Checksum: %" PRIX32 "\n", csum);

    memcpy(data + len - sizeof(u32), &csum, sizeof(u32));
    if (write_buffer(filename, data, len) == 0)
        printf("[*] Patched File Successfully!\n");

    free(data);
    return 0;
}
