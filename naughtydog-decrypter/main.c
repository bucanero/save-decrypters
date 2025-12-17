/*
*
*	Naughty Dog PS3 Save Decrypter - (c) 2020 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original tlou_save_data_decrypter by Red-EyeX32 and aerosoul94
*
* Information about the Blowfish encryption method:
*	- https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*
*/

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "../common/iofile.c"
#include "../common/blowfish.c"
#include "../common/sha1.c"
#include "../common/hmac-sha1.c"

#define SECRET_KEY      "(SH[@2>r62%5+QKpy|g6"
#define SHA1_HMAC_KEY   "xM;6X%/p^L/:}-5QoA+K8:F*M!~sb(WK<E%6sW_un0a[7Gm6,()kHoXY+yI/s;Ba"

#define CRC32_POLY    0xEDB88320
#define CRC32C_POLY   0x82F63B78
#define CRC32_INIT    0xFFFFFFFF

typedef enum {
	// PS3: The Last of Us, Uncharted 2, Uncharted 3
	// PS4: The Last of Us, Uncharted 2 Remastered, Uncharted 3 Remastered, Uncharted 4, The Lost Legacy
	A = 'A',
	// PS4: The Last of Us Part 2
	B = 'B',
	// PS4: The Nathan Drake Collection
	C = 'C'
} SaveType;

typedef struct {
	u8 *data;
	u8 start_off;
	u32 dsize;
	bool isPS3;
	SaveType type;
} SaveState;

void init_savestate(SaveState *ss, u8 *data, size_t len, char type)
{
	assert(type == A || type == B || type == C);
	ss->type = type;
	switch (ss->type)
	{
		case A:
			ss->start_off = 0x8;
			break;
		case B:
			ss->start_off = 0x10;
			break;
		case C:
			ss->start_off = 0xC;
			break;
	}

	if (len < 0xC)
	{
		printf("[!] Save is too small!\n");
		*ctx = NULL;
		return;
	}
	// Check for PS3 saves (big-endian)
	ss->isPS3 = (data[0] == 0 && data[1] == 0);

	u32 dsize;
	if (ss->type == B)
		dsize = *(u32 *)&data[0x8];
	else
	{
		dsize = *(u32 *)&data[len-4];
		if (ss->isPS3)
			dsize = ES32(dsize);
	}
	if (ss->start_off + dsize > len || !dsize)
	{
		printf("[!] Invalid dsize!\n");
		*ctx = NULL;
		return;
	}
	ss->dsize = dsize;
	ss->data = data;
	return;
}

void swap_u32_data(u8 *data, u32 size)
{
	int count = size/sizeof(u32);
	u32 *d = (u32 *)data;
	for (int i = 0; i < count; i++)
		d[i] = ES32(d[i]);
	return;
}

void decrypt_data(const SaveState *ss)
{
	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", ss->dsize, ss->dsize);

	if (ss->type == C)
		swap_u32_data(ss->data + ss->start_off, ss->dsize);
	blowfish_decrypt_buffer(ss->data + ss->start_off, ss->dsize);
	if (ss->type == C)
		swap_u32_data(ss->data + ss->start_off, ss->dsize);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(const SaveState *ss)
{
	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", ss->dsize, ss->dsize);

	if (ss->type == C)
		swap_u32_data(ss->data + ss->start_off, ss->dsize);
	blowfish_encrypt_buffer(ss->data + ss->start_off, ss->dsize);
	if (ss->type == C)
		swap_u32_data(ss->data + ss->start_off, ss->dsize);

	printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void init_crc32_table(u32 *crc32_table, u32 poly)
{
	for (int b = 0; b < 256; ++b)
	{
		u32 r = b;

		for (int i = 0; i < 8; ++i)
			r = (r & 1) ? (r >> 1) ^ poly : (r >> 1);

		crc32_table[b] = r;
	}

	return;
}

u32 calc_crc32(const u8 *data, u32 len, u32 poly)
{
	u32 crc32_table[256];
	u32 crc = CRC32_INIT;

	init_crc32_table(crc32_table, poly);

	while (len--)
		crc = crc32_table[(crc ^ *data++) & 0xFF] ^ (crc >> 8);

	return ~crc;
}

bool fix_checksum(const SaveState *ss)
{
	size_t size = ss->start_off + ss->dsize;
	u32 crc;
	u32 crc_poly;
	u32 crc_bl;
	u16 crc_bl_off;
	u16 crc_off;
	u8 hash_sub;

	switch (ss->type)
	{
		case A:
			crc_poly = CRC32_POLY;
			crc_bl_off = 0x58C;
			crc_off = 0x588;
			hash_sub = 0xC;
			break;
		case B:
			crc_poly = CRC32C_POLY;
			crc_bl_off = 0x594;
			crc_off = 0x590;
			hash_sub = 0x4;
			break;
		case C:
			crc_poly = CRC32_POLY;
			crc_bl_off = 0x590;
			crc_off = 0x58C;
			hash_sub = 0x8;
			break;
	}

	// read crc32 block length
	if (crc_bl_off + sizeof(crc_bl) > size)
		goto error;
	crc_bl = *(u32 *)&ss->data[crc_bl_off];
	if (ss->isPS3)
		crc_bl = ES32(crc_bl);

	// compute crc32
	if (crc_bl_off + crc_bl > size)
		goto error;
	crc = calc_crc32(ss->data + crc_bl_off, crc_bl - 4, crc_poly);
	if (ss->isPS3)
		crc = ES32(crc);

	// write crc32
	if (crc_off + sizeof(u32) > size)
		goto error;
	*(u32 *)(ss->data + crc_off) = crc;
	printf("[*] Updated CRC32    : %08X\n", ES32(crc));

	// calculate and write hmac sha1
	if (ss->dsize < 0x14)
		goto error;
	hmac_sha1(ss->data + ss->dsize - hash_sub, SHA1_HMAC_KEY, strlen(SHA1_HMAC_KEY), ss->data + ss->start_off, ss->dsize - 0x14);
	printf("[*] Updated SHA1 HMAC: " SHA1_FMT(ss->data + ss->dsize - hash_sub, "\n"));
	
	return true;
error:
	printf("[!] Invalid read/write\n");
	return false;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] [game] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt File\n");
	printf(" -e            Encrypt File\n\n");
	printf("GAME TYPE      Explanation:\n");
	printf(" -A            PS3: The Last of Us, Uncharted 2, Uncharted 3\n");
	printf(" -A            PS4: The Last of Us, Uncharted 2 Remastered, Uncharted 3 Remastered, Uncharted 4, The Lost Legacy\n");
	printf(" -B            PS4: The Last of Us Part 2\n");
	printf(" -C            PS4: The Nathan Drake Collection\n\n");
	return;
}

int main(int argc, char **argv)
{
	SaveState ctx = {0};
	SaveState *ss = &ctx;
	size_t len;
	u8 *data;
	char *opt, *type, *filepath, *bak;

	printf("\nnaughtydog-save-decrypter 0.2.0 - (c) 2020 by Bucanero\n\n");

	if (--argc < 3)
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

	type = argv[2];
	if (*type++ != '-' || (*type != A && *type != B && *type != C))
	{
		print_usage(argv[0]);
		return -1;
	}

	blowfish_init_key(SECRET_KEY);

	filepath = argv[3];
	if (read_buffer(filepath, &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", filepath);
		return -1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", filepath);
	write_buffer(bak, data, len);

	init_savestate(&ss, data, len, *type);
	if (ss == NULL)
		return -1;

	if (*opt == 'd')
		decrypt_data(ss);
	else
	{
		if (!fix_checksum(ss))
			return -1;
		encrypt_data(ss);
	}

	write_buffer(filepath, data, len);

	free(bak);
	free(data);

	return 0;
}
