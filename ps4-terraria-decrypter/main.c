#define CBC 1
#define AES128 1
#define AES256 0

#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/miniz.h"

const uint8_t SECRET_KEY[] = {
    0x68, 0x00, 0x33, 0x00, 0x79, 0x00, 0x5F, 0x00, 
    0x67, 0x00, 0x55, 0x00, 0x79, 0x00, 0x5A, 0x00
};
const uint32_t COMP_MAGIC = 0x1AA2227E; 

void decrypt_data(uint8_t* data, size_t size)
{
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, SECRET_KEY, SECRET_KEY);

    printf("[*] Total Decrypted Size Is 0x%X (%zu bytes)\n", size, size);

	AES_CBC_decrypt_buffer(&ctx, data, size);

    printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(uint8_t* data, size_t size)
{
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, SECRET_KEY, SECRET_KEY);

    printf("[*] Total Encrypted Size Is 0x%X (%zu bytes)\n", size, size);

    AES_CBC_encrypt_buffer(&ctx, data, size);

    printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] [type] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt/Decompress File\n");
	printf(" -e            Encrypt/Compress File\n\n");
    printf("TYPES          Explanation:\n");
    printf(" 1             .plr\n");
    printf(" 2             .wld\n\n");
	return;
}

int main(int argc, char **argv)
{
	uint8_t *data;
    size_t len;
	char *opt, *bak;
    char type;

	printf("\nTerraria (PS4) decrypter\n\n");
 
	if (--argc < 3)
	{
		print_usage(argv[0]);
		return -1;
	}
    char *filename = argv[3];

    opt = argv[1];
	if (*opt++ != '-' || (*opt != 'd' && *opt != 'e'))
	{
		print_usage(argv[0]);
        return -1;
	}

    type = *argv[2];
    if (type != '1' && type != '2') {
        print_usage(argv[0]);
        return -1;
    }

	if (read_buffer(filename, &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", filename);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", filename);
	write_buffer(bak, data, len);

    switch (type)
    {
        case '1':
        {
            len &= ~15; // just to be safe
            if (*opt++ == 'd') 
                decrypt_data(data, len);
            
            else 
                encrypt_data(data, len);

            write_buffer(filename, data, len);

            break;
        }
        
        case '2':
        {
            if (*opt++ == 'd') 
            {
                if (memcmp(data, &COMP_MAGIC, 4) != 0) {
                    printf("This .wld file is incompatible!\n");
                    break;
                }
                uint32_t expected_len;
                memcpy(&expected_len, data + 4, sizeof(uint32_t));

                uint8_t *decomp;
                size_t decomp_len;

                decomp = tinfl_decompress_mem_to_heap(data + 0x08, len - 0x08, &decomp_len, TINFL_FLAG_PARSE_ZLIB_HEADER);
                if (!decomp || decomp_len != expected_len) {
                    printf("Decompression failed! Expected size: %u, decompressed size: %zu\n", expected_len, decomp_len);
                    break;
                }

                write_buffer(filename, decomp, decomp_len);
                free(decomp);
            }
            else 
            {
                uint8_t *comp;
                size_t comp_len;

                comp = tdefl_compress_mem_to_heap(data, len, &comp_len, TINFL_FLAG_PARSE_ZLIB_HEADER);
                if (!comp) {
                    printf("Compression failed!\n");
                    break;
                }

                data = (uint8_t *)realloc(data, (4 + 4) + comp_len);
                len = (uint32_t)len;

                memcpy(data, &COMP_MAGIC, 4);
                memcpy(data + 4, &len, 4);
                memcpy(data + 8, comp, comp_len);

                write_buffer(filename, data, (4 + 4) + comp_len);
                free(comp);
            }

            break;
        }

    }

    free(data);
    free(bak);
}
