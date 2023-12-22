/*
*
*	Dead Rising (PS4) Checksum Fixer - (c) 2023 by Bucanero - www.bucanero.com.ar
*
* This tool is based on Philymaster’s Security Archive
* https://community.wemod.com/t/philymasters-security-archive/3923
*
*/

#include "../common/iofile.c"


void fixHashes(uint8_t* data, int size)
{
    uint16_t sumL, sumH;
    u8* e = data + size;

    for (u8* s = e - size; s < e; s += 4)
    {
        sumL = sumH = 0;
        for (int n = 0; n < 16; ++n, ++s)
        {
            sumL += *s;
            sumH += (uint16_t) ((n % 2) == 0 ? *s : -*s);
        }

        *(uint16_t*) s = sumL;
        *(uint16_t*) (s + 2) = sumH;
    }
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
	u32 csum;
	char *opt, *bak;

	printf("\nDead Rising (PS4) checksum fixer 0.1.0 - (c) 2023 by Bucanero\n\n");

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

	printf("[*] Calculation Size: %lu bytes\n", len);

	fixHashes(data, len);
	printf("[*] Updated %lu block hashes\n", len/20);

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
