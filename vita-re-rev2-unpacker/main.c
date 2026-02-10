/*
*
*	Resident Evil Revelation 2 Vita Save Unpacker - (c) 2026 by Bucanero - www.bucanero.com.ar
*
*/

#include "../common/iofile.c"
#include "../common/miniz.h"
#include "../common/sha1.c"


u32 dwadd(const u8* data, u32 len)
{
	u32 csum = 0;

	len /= 4;
	while (len--)
	{
		csum += (*(u32*)data);
		data += 4;
	}

	return csum;
}

void swap_u32_data(u32* data, int count)
{
	for (int i=0; i < count; i++)
		data[i] = ES32(data[i]);
}

void unpack_data(u8** data, size_t *size)
{
	uint8_t* out;
	size_t outlen;

	printf("[*] Total Compressed Size Is 0x%lX (%ld bytes)\n", *size, *size);

	out = tinfl_decompress_mem_to_heap(*data, *size, &outlen, TINFL_FLAG_PARSE_ZLIB_HEADER);
	if (out)
	{
		printf("[*] Decompressed File Successfully! (%ld bytes)\n\n", outlen);
		free(*data);
		*data = out;
		*size = outlen;
	}
	else 
		printf("[*] Decompression Failed!\n");

	return;
}

void pack_data(u8** data, size_t* size)
{
	uint8_t* out;
	size_t outlen;

	printf("[*] Total Decompressed Size Is 0x%lX (%ld bytes)\n", *size, *size);

	out = tdefl_compress_mem_to_heap(*data, *size, &outlen, TDEFL_DEFAULT_MAX_PROBES | TDEFL_WRITE_ZLIB_HEADER);
	if (out)
	{
		printf("[*] Compressed File Successfully! (%ld bytes)\n\n", outlen);
		free(*data);
		*data = out;
		*size = outlen;
	}
	else
		printf("[*] Compression Failed!\n");

	return;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -u            Unpack File\n");
	printf(" -p            Pack File\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	char *opt, *bak;

	printf("\nVita re-revelations2-unpacker 0.1.0 - (c) 2026 by Bucanero\n\n");

	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}
	
	opt = argv[1];
	if (*opt++ != '-' || (*opt != 'u' && *opt != 'p'))
	{
		print_usage(argv[0]);
		return -1;
	}

	if (read_buffer(argv[2], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[2]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	if (*opt == 'u')
		unpack_data(&data, &len);
	else
	{
		u32 crc = dwadd(data + 0x10, len - 0x30);
		printf("[*] Old Checksum : 0x%08X\n", *(u32*)(data + 8));
		printf("[*] New Checksum : 0x%08X\n", crc);
		memcpy(data + 8, &crc, 4);

		printf("[*] Stored SHA1  : " SHA1_FMT(data + len - 0x20, "\n"));
		sha1(data + len - 0x20, data + 0x10, (len - 0x30));
		swap_u32_data((u32*)(data + len - 0x20), 5);
		printf("[*] Updated SHA1 : " SHA1_FMT(data + len - 0x20, "\n"));

		pack_data(&data, &len);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
