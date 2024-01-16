/*
*
*	DuckTales: Remastered (PS3) Checksum Fixer - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based on the "Security Archive" notes by Philymaster (https://community.wemod.com/t/philymasters-security-archive/3923)
*
*/

#include "../common/iofile.c"

// lookup3.c, by Bob Jenkins, May 2006, Public Domain.
// https://www.burtleburtle.net/bob/c/lookup3.c

#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

#define mix(a,b,c) \
{ \
  a -= c;  a ^= rot(c, 4);  c += b; \
  b -= a;  b ^= rot(a, 6);  a += c; \
  c -= b;  c ^= rot(b, 8);  b += a; \
  a -= c;  a ^= rot(c,16);  c += b; \
  b -= a;  b ^= rot(a,19);  a += c; \
  c -= b;  c ^= rot(b, 4);  b += a; \
}

#define final(a,b,c) \
{ \
  c ^= b; c -= rot(b,14); \
  a ^= c; a -= rot(c,11); \
  b ^= a; b -= rot(a,25); \
  c ^= b; c -= rot(b,16); \
  a ^= c; a -= rot(c,4);  \
  b ^= a; b -= rot(a,14); \
  c ^= b; c -= rot(b,24); \
}

/*
 * hashlittle2: return 2 32-bit hash values
 *
 * This is identical to hashlittle(), except it returns two 32-bit hash
 * values instead of just one.  This is good enough for hash table
 * lookup with 2^^64 buckets, or if you want a second hash if you're not
 * happy with the first, or if you want a probably-unique 64-bit ID for
 * the key.  *pc is better mixed than *pb, so use *pc first.  If you want
 * a 64-bit value do something like "*pc + (((uint64_t)*pb)<<32)".
 */
void hashlittle2( 
  const uint8_t *k,      /* the key to hash */
  size_t      length,    /* length of the key */
  uint32_t   *pc,        /* IN: primary initval, OUT: primary hash */
  uint32_t   *pb)        /* IN: secondary initval, OUT: secondary hash */
{
    uint32_t a,b,c;                                          /* internal state */

    /* Set up the internal state */
    a = b = c = 0xDEADBEEF + ((uint32_t)length) + *pc;
    c += *pb;

    /* need to read the key one byte at a time */
    /*--------------- all but the last block: affect some 32 bits of (a,b,c) */
    while (length > 12)
    {
        a += k[0];
        a += ((uint32_t)k[1])<<8;
        a += ((uint32_t)k[2])<<16;
        a += ((uint32_t)k[3])<<24;
        b += k[4];
        b += ((uint32_t)k[5])<<8;
        b += ((uint32_t)k[6])<<16;
        b += ((uint32_t)k[7])<<24;
        c += k[8];
        c += ((uint32_t)k[9])<<8;
        c += ((uint32_t)k[10])<<16;
        c += ((uint32_t)k[11])<<24;
        mix(a,b,c);
        length -= 12;
        k += 12;
    }
    /*-------------------------------- last block: affect all 32 bits of (c) */
    switch(length)                   /* all the case statements fall through */
    {
    case 12: c+=((uint32_t)k[11])<<24;
    case 11: c+=((uint32_t)k[10])<<16;
    case 10: c+=((uint32_t)k[9])<<8;
    case 9 : c+=k[8];
    case 8 : b+=((uint32_t)k[7])<<24;
    case 7 : b+=((uint32_t)k[6])<<16;
    case 6 : b+=((uint32_t)k[5])<<8;
    case 5 : b+=k[4];
    case 4 : a+=((uint32_t)k[3])<<24;
    case 3 : a+=((uint32_t)k[2])<<16;
    case 2 : a+=((uint32_t)k[1])<<8;
    case 1 : a+=k[0];
             break;
    case 0 : *pc=c; *pb=b; return;  /* zero length strings require no mixing */
    }

    final(a,b,c);
    *pc=c; *pb=b;
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
	u32 csum1 = 0x4D2, csum2 = 0x162E;
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

	hashlittle2(data + 0x10, 0x400, &csum1, &csum2);
	*(u32*)data = ES32(csum1);
	*(u32*)(data+4) = ES32(csum2);

	printf("[*] New Checksum    : %08X%08X\n", csum1, csum2);

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
