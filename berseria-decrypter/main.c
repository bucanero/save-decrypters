/*
*
*	Tales of Berseria (PS3/PS4/PC) Save Decrypter - (c) 2026 by Bucanero - github.com/bucanero
*
* This tool is based on the ToB save decrypter python script by alfizari
*  https://github.com/alfizari
*
*/

#include "../common/iofile.c"
#include "../common/sha1.c"

#define HASH_POS		0x001C
#define HASH_SALT		0x000C
#define HASH_START		0x0034

typedef struct tob_entry
{
	uint32_t item_id;
	uint32_t data_offset;
	uint32_t data_len;
	uint32_t flags;
	uint8_t salt[12];
	uint8_t sha1_key[20];
} tob_entry_t;

int isPS3 = 0;


void sha1_Custom(u8* hash_out, const u8* msg, u32 length, const u8* salt, u32 slen)
{
	u8* tmp;

	tmp = (u8*)malloc(length + slen);
	memcpy(tmp, msg, length);
	memcpy(tmp + length, salt, slen);

	sha1(hash_out, tmp, length + slen);
	free(tmp);
}

u32 read32_val(const u8* buf)
{
	u32 ret = (u32)buf[0] | ((u32)buf[1] << 8) | ((u32)buf[2] << 16) | ((u32)buf[3] << 24);

	if (isPS3)
		ret = ES32(ret);

	return ret;
}

void encrypt_entry(tob_entry_t* entry, u8* entry_data)
{
	entry_data += entry->data_offset;

	sha1_Custom(entry->sha1_key, entry_data, entry->data_len, entry->salt, sizeof(entry->salt));

	for (int i = 0; i < entry->data_len; i++)
		entry_data[i] ^= entry->sha1_key[i % 20] ^ (i & 0xFF);
}

void encrypt_data(u8* data, u32 entry_count)
{
	tob_entry_t *entry;

	printf("[*] Encrypting Entries");
	for (int i = 0; i < entry_count; i++)
	{
		entry = (tob_entry_t*)(data + 0x30 + i * 0x30);
		if (isPS3)
		{
			entry->data_offset = ES32(entry->data_offset);
			entry->data_len = ES32(entry->data_len);
		}

		printf(".");
//		printf("[i] Encrypting Entry %02d: offset=0x%08X size=0x%08X\n", i, entry->data_offset, entry->data_len);
		encrypt_entry(entry, data);

		if (isPS3)
		{
			entry->data_offset = ES32(entry->data_offset);
			entry->data_len = ES32(entry->data_len);
		}
	}
	printf("\n");
}

void decrypt_entry(const tob_entry_t* entry, u8* entry_data)
{
	u8 sha_chk[20];

	entry_data += entry->data_offset;

	for (int i = 0; i < entry->data_len; i++)
		entry_data[i] ^= entry->sha1_key[i % 20] ^ (i & 0xFF);

	sha1_Custom(sha_chk, entry_data, entry->data_len, entry->salt, sizeof(entry->salt));
	if (memcmp(sha_chk, entry->sha1_key, sizeof(sha_chk)) != 0)
		printf("[!] Warning: Entry ID %08X failed integrity check!\n", entry->item_id);
}

void decrypt_data(u8* data, u32 entry_count)
{
	tob_entry_t entry;

	printf("[*] Decrypting Entries");
	for (int i = 0; i < entry_count; i++)
	{
		memcpy(&entry, data + 0x30 + i * 0x30, sizeof(tob_entry_t));
		if (isPS3)
		{
			entry.data_offset = ES32(entry.data_offset);
			entry.data_len = ES32(entry.data_len);
		}

		printf(".");
//		printf("[i] Decrypting Entry %02d: offset=0x%08X size=0x%08X\n", i, entry.data_offset, entry.data_len);
		decrypt_entry(&entry, data);
	}
	printf("\n");
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
	u8 *data;
	char *opt, *bak;
    u32 magic, entry_count, data_size, data_start;

	printf("\nTales of Berseria (PS3/PS4/PC) Save Decrypter 0.1.0 - (c) 2026 by Bucanero\n\n");

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

	isPS3 = memcmp(data, "\x00\x00\x00\x64\x00\x00\x00", 7) == 0;
	magic = read32_val(data + 0x00);
	entry_count = read32_val(data + 0x04);
	data_size   = read32_val(data + 0x08);
	data_start  = read32_val(data + HASH_START);

	if (!isPS3 && magic != 0x64)
	{
		printf("[*] Invalid Save File (%s)\n", argv[2]);
		return -1;
	}

	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	printf("[*] Magic      : %08X\n", magic);
	printf("[*] Entries    : %d\n", entry_count);
	printf("[*] Data Size  : %d bytes\n", data_size);
	printf("[*] File Size  : %lu bytes\n", len);
	printf("[*] Stored SHA1: " SHA1_FMT(data + HASH_POS, "\n"));

	if (*opt == 'd')
	{
		u8 sha_chk[20];

		sha1_Custom(sha_chk, data + data_start, data_size - data_start, data + HASH_SALT, 16);
		if (memcmp(sha_chk, data + HASH_POS, sizeof(sha_chk)) != 0)
			printf("[*] Warning: Invalid Checksum! The file may be already decrypted or corrupted.\n");

		decrypt_data(data, entry_count);
		printf("[*] Decrypted Successfully!\n\n");
	}
	else
	{
		encrypt_data(data, entry_count);

		sha1_Custom(data + HASH_POS, data + data_start, data_size - data_start, data + HASH_SALT, 16);
		printf("[*] New SHA1   : " SHA1_FMT(data + HASH_POS, "\n"));
		printf("[*] Encrypted Successfully!\n\n");
	}

	if (write_buffer(argv[2], data, len) != 0)
		printf("[*] Error! Failed to save file!\n\n");

	free(bak);
	free(data);

	return 0;
}
