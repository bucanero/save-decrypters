/*
*	Resident Evil 7: Biohazard Checksum Fixer - (c) 2025 by hzh
*	https://github.com/hzhreal/
*/

#include <inttypes.h>
#include <limits.h>
#include <string.h>

#include "../common/iofile.c"

// https://en.wikipedia.org/wiki/MurmurHash
static inline uint32_t murmur_32_scramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed)
{
    uint32_t h = seed;
    uint32_t k;
    /* Read in groups of 4. */
    for (size_t i = len >> 2; i; i--) {
        // Here is a source of differing results across endiannesses.
        // A swap here has no effects on hash properties though.
        memcpy(&k, key, sizeof(uint32_t));
        key += sizeof(uint32_t);
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
    /* Read the rest. */
    k = 0;
    for (size_t i = len & 3; i; i--) {
        k <<= 8;
        k |= key[i - 1];
    }
    // A swap is *not* necessary here because the preceding loop already
    // places the low bytes in the low places according to whatever endianness
    // we use. Swaps only apply when the memory is copied in a chunk.
    h ^= murmur_32_scramble(k);
    /* Finalize. */
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
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

    printf("[*] Current Checksum: %" PRIX32 "\n", *(u32*)(data + len - 4));
    csum = murmur3_32(data, len - sizeof(u32), 0xFFFFFFFF);
    printf("[*] Updated Checksum: %" PRIX32 "\n", csum);

    memcpy(data + len - sizeof(u32), &csum, sizeof(u32));
    if (write_buffer(filename, data, len) == 0)
        printf("[*] Patched File Successfully!\n");

    free(data);
    return 0;
}
