/*
*
*	Final Fantasy Pixel Remaster PS4 Save Decrypter - (c) 2026 by Bucanero - github.com/bucanero
*
* based on the work of Anub1sR0cks - https://github.com/Anub1sR0cks/FFPRSaveEditor
* https://github.com/RyudoSynbios/game-tools-collection/blob/master/src/lib/templates/final-fantasy-iv-pixel-remaster-windows/saveEditor/utils/crypto.ts
*
*/

#include "../common/iofile.c"
#include "../common/miniz.h"
#include "../common/sha1.c"
#include "../common/hmac-sha1.c"
#include "../common/pbkdf2-sha1.c"
#include "../common/rijndael.c"

#define FFPR_PASSWORD "TKX73OHHK1qMonoICbpVT0hIDGe7SkW0"
#define FFPR_SALT     "71Ba2p0ULBGaE6oJ7TjCqwsls1jBKmRL"


void derive_key_and_iv(const char* password, const char* salt, u8* key, u8* iv)
{
	u8 tmp[64];

	pbkdf2_sha1((const u8*) password, strlen(password), (const u8*) salt, strlen(salt), 10, tmp, sizeof(tmp));
	memcpy(key, tmp, 32);
	memcpy(iv, tmp + 32, 32);
}

void decrypt_data(u8* data, u32 size)
{
	u8 key[32], iv[32];
	rijn_context ctx;

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	derive_key_and_iv(FFPR_PASSWORD, FFPR_SALT, key, iv);
	rijn_set_key(&ctx, key, 32 * 8, 32 * 8);
	rijn_cbc_decrypt(&ctx, iv, data, data, size);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size)
{
	u8 key[32], iv[32];
	rijn_context ctx;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	derive_key_and_iv(FFPR_PASSWORD, FFPR_SALT, key, iv);
	rijn_set_key(&ctx, key, 32 * 8, 32 * 8);
	rijn_cbc_encrypt(&ctx, iv, data, data, size);

	printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt File\n");
	printf(" -e            Encrypt File\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	char *opt, *bak;

	printf("\nFinal Fantasy Pixel Remaster PS4 Save Decrypter 0.1.0 - (c) 2026 by Bucanero\n\n");

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
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);


	if (*opt == 'd')
	{
		decrypt_data(data, len);
		u8* decomp = tinfl_decompress_mem_to_heap(data, len, &len, 0);
		printf("[*] Decompressed Size Is %ld bytes\n", len);
		free(data);
		data = decomp;
	}
	else
	{
		size_t comp_len;
		u8* comp = tdefl_compress_mem_to_heap(data, len, &comp_len, 0);
		printf("[*] Compressed Size Is %ld bytes\n", comp_len);
		len = (comp_len + 31) & ~31;
		memcpy(data, comp, comp_len);
		memset(data + comp_len, 0, len - comp_len);
		free(comp);
		encrypt_data(data, len);
	}

	write_buffer("out.bin", data, len);
//	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
