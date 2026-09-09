/*
*
*	Patapon 3 PSP Save Decrypter - (c) 2023 by Bucanero - www.bucanero.com.ar
*
* This tool is based on
*	- https://github.com/efonte/patapon-re/tree/main/Tools/patapon3-enc
*
*/

#include "../common/iofile.c"
#include "../common/camellia.c"
#include "../common/sha1.c"
#include "../common/hmac-sha1.c"

//PSP save-game key: 4439565A473232594C57534A474E4C9A
#define PATAPON3_KEY		"SVsyE56pniSRS9dIPTiE8ApDaUnN0AEa"
#define PATAPON3_HMAC_KEY	"CyZ2o3SPBqMWVVvUVt4WwJOBpq9hCjNq"

/*
 * Decrypted save layout (Camellia-256 ECB over the whole file):
 *
 *   0x00000000            .. size-0x28    save data
 *   size-0x28 (u32 LE)                    length of the HMAC'd region
 *   size-0x24 (20 bytes)                  HMAC-SHA1 of data[0 .. length]
 *   size-0x10 (16 bytes)                  padding
 */
#define P3_TRAILER_SIZE		0x28
#define P3_HASH_SIZE		HMAC_SHA1_BYTES


void encrypt_data(uint8_t* data, int size)
{
    CamelliaContext ctx;

	printf("[*] Total Encrypted Size: 0x%X (%d bytes)\n", size, size);
    size /= CAMELLIA_BLOCK_SIZE;

    camelliaInit(&ctx, (uint8_t*) PATAPON3_KEY, strlen(PATAPON3_KEY));

    while (size--)
    {
        camelliaEncryptBlock(&ctx, data, data);
        data += CAMELLIA_BLOCK_SIZE;
    }

	printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void decrypt_data(uint8_t* data, int size)
{
    CamelliaContext ctx;

	printf("[*] Total Decrypted Size: 0x%X (%d bytes)\n", size, size);
    size /= CAMELLIA_BLOCK_SIZE;

    camelliaInit(&ctx, (uint8_t*) PATAPON3_KEY, strlen(PATAPON3_KEY));

    while (size--)
    {
        camelliaDecryptBlock(&ctx, data, data);
        data += CAMELLIA_BLOCK_SIZE;
    }

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

static uint8_t* get_hash_ptr(uint8_t* data, size_t size, uint32_t* hashed_len)
{
	if (size < P3_TRAILER_SIZE + CAMELLIA_BLOCK_SIZE)
	{
		printf("[!] File is too small (0x%X bytes)\n", (int) size);
		return NULL;
	}

	*hashed_len = *(uint32_t*)(data + size - P3_TRAILER_SIZE);

	if (*hashed_len == 0 || *hashed_len > size - P3_TRAILER_SIZE)
	{
		printf("[!] Invalid hashed length (0x%X)\n", *hashed_len);
		return NULL;
	}

	return (data + size - P3_TRAILER_SIZE + sizeof(uint32_t));
}

void print_hash(const char* label, const uint8_t* hash)
{
	printf("%s", label);
	for (int i = 0; i < P3_HASH_SIZE; i++)
		printf("%02X", hash[i]);
	printf("\n");
}

int check_hash(uint8_t* data, size_t size)
{
	uint32_t len;
	uint8_t hmac[P3_HASH_SIZE];
	uint8_t* hash = get_hash_ptr(data, size, &len);

	if (!hash)
		return -1;

	hmac_sha1(hmac, PATAPON3_HMAC_KEY, strlen(PATAPON3_HMAC_KEY), data, len);

	printf("[*] Hashed Data Size : 0x%X (%d bytes)\n", len, len);
	print_hash("[*] Stored HMAC-SHA1 : ", hash);
	print_hash("[*] Calc'd HMAC-SHA1 : ", hmac);
	printf("[*] Save Hash is %s\n\n", memcmp(hash, hmac, P3_HASH_SIZE) == 0 ? "VALID" : "INVALID");

	return 0;
}

int update_hash(uint8_t* data, size_t size)
{
	uint32_t len;
	uint8_t* hash = get_hash_ptr(data, size, &len);

	if (!hash)
		return -1;

	hmac_sha1(hash, PATAPON3_HMAC_KEY, strlen(PATAPON3_HMAC_KEY), data, len);

	printf("[*] Hashed Data Size : 0x%X (%d bytes)\n", len, len);
	print_hash("[*] Updated HMAC-SHA1: ", hash);
	printf("\n");

	return 0;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt PSP File\n");
	printf(" -e            Encrypt PSP File (updates the save hash)\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	char *opt, *bak;

	printf("\nPatapon 3 PSP Save Decrypter 0.2.0 - (c) 2023 by Bucanero\n\n");

	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}

	opt = argv[1];
	if (*opt++ != '-' || (*opt != 'd' && *opt != 'e'))
	{
		print_usage(argv[0]);
		return -1;
	}

	if (read_buffer(argv[2], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[2]);
		return -1;
	}

	if (len % CAMELLIA_BLOCK_SIZE)
	{
		printf("[*] Invalid File Size (0x%X): not a multiple of 0x%X\n", (int) len, CAMELLIA_BLOCK_SIZE);
		free(data);
		return -1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	if (*opt == 'e')
	{
		update_hash(data, len);
		encrypt_data(data, len);
	}
	else
	{
		decrypt_data(data, len);
		check_hash(data, len);
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
