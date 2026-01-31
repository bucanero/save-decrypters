/*
*
* The Legend of Heroes: Trails from Zero/to Azure (PS4) Checksum Fixer - (c) 2026 by Bucanero - www.bucanero.com.ar
*
* This checksum tool is based on the BZH_AO_NO_KISEKI_Savedata_Editor
* https://github.com/424778940z/BZH_AO_NO_KISEKI_Savedata_Editor/blob/master/BZH_AO_NO_KISEKI_Savedata_Editor/bzh_ank_se_savedata_checksum.h
*
*/

#include "../common/iofile.c"

typedef struct {
    uint32_t file_savedata_checksum;
    uint32_t file_size_checksum;
} ChecksumResult;


ChecksumResult trailsSave_checksum(const uint8_t* data, uint32_t len)
{
    uint32_t file_size_max_pos = len - 1;
    ChecksumResult res = {
        .file_savedata_checksum = 0,
        .file_size_checksum = -1 * (((file_size_max_pos - 0x08) / 0x04) + 0x01)
    };
    uint32_t cache;

    // Read and process data
    for (uint32_t pos = 0x00; pos <= (file_size_max_pos - 0x08); pos += 0x04)
    {
        cache = *(uint32_t*)(data + pos);
        // Assuming little-endian data (as in original Qt code)
        // If the file data needs endian conversion, add it here
        res.file_savedata_checksum += cache;
    }

    res.file_size_checksum -= res.file_savedata_checksum;

    // Apply mask (equivalent to subtracting 0xFFFFFFFF00000000)
    // This effectively keeps only the lower 32 bits
    res.file_size_checksum &= 0xFFFFFFFF;

    // return result
    return res;
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
	ChecksumResult result;
	char *opt, *bak;

	printf("\nThe Legend of Heroes: Trails from Zero/to Azure (PS4) checksum fixer 0.1.0 - (c) 2026 by Bucanero\n\n");

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

	result = trailsSave_checksum(data, len);

	printf("[*] File Size       : %lu bytes\n", len);
	printf("[*] Stored Checksum : %08X %08X\n", *(u32*)(data + len - 8), *(u32*)(data + len - 4));
	printf("[*] New Checksum    : %08X %08X\n\n", result.file_savedata_checksum, result.file_size_checksum);

	// Write new checksum to file data
	memcpy(data + len - 8, &result.file_savedata_checksum, sizeof(u32));
	memcpy(data + len - 4, &result.file_size_checksum, sizeof(u32));

	if (write_buffer(argv[1], data, len) == 0)
		printf("[*] Successfully Wrote New Checksum!\n\n");

	free(bak);
	free(data);

	return 0;
}
