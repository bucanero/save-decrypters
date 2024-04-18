/*
*
*	Invizimals (PSP) Checksum Fixer - (c) 2023 by Bucanero - www.bucanero.com.ar
*
*/

#include "../common/iofile.c"


uint32_t adler32_fetch( uint8_t *buff, size_t size, uint32_t prior_key )
{
    size_t i, j;
    uint32_t lower_word    = prior_key & 0xffff;
    uint32_t upper_word    = (prior_key >> 16) & 0xffff;
 
    lower_word += buff[0];
    upper_word += lower_word;
    --size;
 
    for( i = 1, j = 1; i < size; ++i, ++j )
    {
        lower_word += buff[i];
        upper_word += lower_word;
 
        if ( j == 0x15b0 )
        {
            lower_word = lower_word % 0xfff1;
            upper_word = upper_word % 0xfff1;
            j = 0;
        }
    }
 
    lower_word += buff[i];
    upper_word += lower_word;
 
    lower_word = lower_word % 0xfff1;
    upper_word = upper_word % 0xfff1;
 
    return( ( upper_word << 16 ) | lower_word );
}

static inline uint32_t murmur_32_scramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t h)
{
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
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -0            Invizimals (UCES01241)\n");
	printf(" -1            Invizimals (UCES01411/UCES01525/UCUS98760)\n");
	printf(" -2            Invizimals (UCES01581)\n\n");
	return;
}


int main(int argc, char **argv)
{
	char *opt, *bak;
	uint8_t* data;
	size_t size;
	uint32_t csum;

	printf("\nInvizimals (PSP) checksum fixer+unlocker 0.1.0 - (c) 2023 by Bucanero\n\n");

	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}

	opt = argv[1];
	if (*opt++ != '-' || (*opt != '0' && *opt != '1' && *opt != '2'))
	{
		print_usage(argv[0]);
		return -1;
	}

	if (read_buffer(argv[2], &data, &size) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[1]);
		return -1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, size);

	csum = *(uint32_t*)(data + 0xC);
	printf("[*] Calculation Size: %lu bytes\n", size);
	printf("[*] Stored Checksum : %08X\n", csum);

	// set magic value
	data[0x0C] = 0x99;
	data[0x0D] = 0x54;
	data[0x0E] = 0x76;
	data[0x0F] = 0x88;

	if (*opt == '0')
	{
		data[0x0D] = 0x67;
		data[0x0E] = 0x45;
	}
	else
	// unlock save to load in any system
	data[0x30] = 0;

	// UCES01241 - adler32, initial value 0
	// UCES01411 - adler32, initial value 0
	// UCES01581 - MurmurHash3, initial value 0
	// https://en.wikipedia.org/wiki/MurmurHash
	
	if (*opt == '2')
		csum = murmur3_32(data, size, 0);
	else
		csum = adler32_fetch(data, size, 0);

	printf("[*] New Checksum    : %08X\n", csum);
	memcpy(data + 0x0C, &csum, 4);

	if (write_buffer(argv[2], data, size) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
