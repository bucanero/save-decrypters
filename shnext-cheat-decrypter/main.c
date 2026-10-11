/*
*
*	ShnExt Cheat Decrypter - (c) 2026 by Bucanero - www.bucanero.com.ar
*
* based on onionHEN's cheat_engine_parser_shnext.c
*
* File format:
*  - raw deflate (no zlib header) compressed JSON
*  - "ProcessName": base64( IV[16] + AES-256-CBC(.NET BinaryFormatter string) )
*      key = SHA256("#my*S3cr3t")
*  - "patchEntries": array of base64( AES-256-CBC(raw deflate JSON) )
*      key/iv = byte from SHNEXT_FIELD table, based on the entry index
*
*/

#define CBC 1

#include <time.h>
#include "../common/iofile.c"
#include "../common/aes.c"
#include "../common/base64.c"
#include "../common/cJSON.c"
#include "../common/miniz.h"

// SHA256("#my*S3cr3t")
static const uint8_t SHNEXT_PROCESS_KEY[32] = {
	0x3C, 0x3E, 0x57, 0x5E, 0xCE, 0xAC, 0xDD, 0x4C, 0xE3, 0x02, 0x99, 0x3D, 0x1F, 0x34, 0xD9, 0x91,
	0x79, 0x92, 0x48, 0xD1, 0xAC, 0x0D, 0x4C, 0xB7, 0x16, 0x53, 0xFA, 0xE0, 0x6E, 0x85, 0x1F, 0xB7,
};

static const uint8_t SHNEXT_FIELD[64] = {
	0x04, 0x14, 0x02, 0x80, 0x01, 0x20, 0xF3, 0xEB,
	0x03, 0x01, 0x02, 0x80, 0x00, 0x20, 0xF3, 0xEB,
	0x05, 0x18, 0x02, 0x80, 0x00, 0x20, 0xF3, 0xEB,
	0x00, 0x1C, 0x02, 0x80, 0x71, 0x0F, 0x8C, 0xBF,
	0xF2, 0x0E, 0x06, 0x08, 0x03, 0x06, 0x10, 0x10,
	0x00, 0x07, 0x24, 0xC8, 0x08, 0x05, 0x0E, 0x3E,
	0x00, 0x04, 0x20, 0xC8, 0x01, 0x07, 0x25, 0xC8,
	0x00, 0x05, 0x28, 0xC8, 0x00, 0x06, 0x2C, 0xC8,
};

// .NET BinaryFormatter header: SerializationHeaderRecord (RootId=1, HeaderId=-1, v1.0)
static const uint8_t NET_BINFMT_HEADER[17] = {
	0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


static void get_entry_key_iv(int index, uint8_t key[32], uint8_t iv[16])
{
	memset(key, SHNEXT_FIELD[(index * 5) % 64], 32);
	memset(iv, SHNEXT_FIELD[(index * 5) % 32], 16);
}

static size_t pkcs7_unpad(const uint8_t *buf, size_t len)
{
	uint8_t pad = buf[len - 1];

	if (pad == 0 || pad > AES_BLOCKLEN)
		return len;

	for (size_t i = len - pad; i < len; i++)
		if (buf[i] != pad)
			return len;

	return len - pad;
}

static uint8_t* aes_cbc_encrypt(const uint8_t *data, size_t len, const uint8_t *key, const uint8_t *iv, size_t *out_len)
{
	struct AES_ctx ctx;
	uint8_t pad = AES_BLOCKLEN - (len % AES_BLOCKLEN);
	uint8_t *buf = malloc(len + pad);

	memcpy(buf, data, len);
	memset(buf + len, pad, pad);
	*out_len = len + pad;

	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_encrypt_buffer(&ctx, buf, *out_len);

	return buf;
}

// base64 encode without line feeds
static char* b64_encode(const uint8_t *data, size_t len)
{
	char *out, *src, *dst;

	out = (char*) base64_encode(data, len, NULL);
	if (!out)
		return NULL;

	for (src = dst = out; *src; src++)
		if (*src != '\n')
			*dst++ = *src;
	*dst = 0;

	return out;
}

static char* inflate_string(const uint8_t *data, size_t len)
{
	size_t out_len;
	char *out, *str;

	out = tinfl_decompress_mem_to_heap(data, len, &out_len, 0);
	if (!out)
		return NULL;

	str = realloc(out, out_len + 1);
	str[out_len] = 0;

	return str;
}

static uint8_t* deflate_string(const char *str, size_t *out_len)
{
	return tdefl_compress_mem_to_heap(str, strlen(str), out_len, TDEFL_DEFAULT_MAX_PROBES);
}

static char* decrypt_process_name(const char *b64)
{
	struct AES_ctx ctx;
	uint8_t *raw;
	size_t raw_len, len, pos, str_len = 0;
	char *str = NULL;

	raw = base64_decode((const u8*) b64, strlen(b64), &raw_len);
	if (!raw || raw_len < 32 || (raw_len % AES_BLOCKLEN))
	{
		free(raw);
		return NULL;
	}

	AES_init_ctx_iv(&ctx, SHNEXT_PROCESS_KEY, raw);
	AES_CBC_decrypt_buffer(&ctx, raw + 16, raw_len - 16);
	len = pkcs7_unpad(raw + 16, raw_len - 16);

	// BinaryObjectString record: 0x06, ObjectId (int32), 7-bit encoded length, UTF-8 string
	pos = 16 + sizeof(NET_BINFMT_HEADER);
	if (len > sizeof(NET_BINFMT_HEADER) + 6 && raw[pos] == 0x06)
	{
		pos += 5;
		for (int shift = 0; pos < 16 + len && shift < 35; shift += 7)
		{
			str_len |= (size_t)(raw[pos] & 0x7F) << shift;
			if (!(raw[pos++] & 0x80))
				break;
		}

		if (pos + str_len <= 16 + len)
			str = strndup((char*) raw + pos, str_len);
	}

	free(raw);
	return str;
}

static char* encrypt_process_name(const char *name)
{
	uint8_t *plain, *enc, *raw;
	size_t name_len = strlen(name), len = 0, enc_len;
	char *b64;

	plain = malloc(sizeof(NET_BINFMT_HEADER) + 5 + 5 + name_len + 1);
	memcpy(plain, NET_BINFMT_HEADER, sizeof(NET_BINFMT_HEADER));
	len += sizeof(NET_BINFMT_HEADER);

	// BinaryObjectString, ObjectId = 1
	memcpy(plain + len, "\x06\x01\x00\x00\x00", 5);
	len += 5;

	for (size_t v = name_len; ; v >>= 7)
	{
		plain[len++] = (v & 0x7F) | (v > 0x7F ? 0x80 : 0);
		if (v <= 0x7F)
			break;
	}

	memcpy(plain + len, name, name_len);
	len += name_len;
	// MessageEnd
	plain[len++] = 0x0B;

	raw = malloc(16);
	for (int i = 0; i < 16; i++)
		raw[i] = rand() & 0xFF;

	enc = aes_cbc_encrypt(plain, len, SHNEXT_PROCESS_KEY, raw, &enc_len);
	raw = realloc(raw, 16 + enc_len);
	memcpy(raw + 16, enc, enc_len);

	b64 = b64_encode(raw, 16 + enc_len);

	free(enc);
	free(raw);
	free(plain);
	return b64;
}

static cJSON* decrypt_patch_entry(const char *b64, int index)
{
	struct AES_ctx ctx;
	uint8_t key[32], iv[16];
	uint8_t *raw;
	size_t raw_len;
	char *json;
	cJSON *entry;

	raw = base64_decode((const u8*) b64, strlen(b64), &raw_len);
	if (!raw || !raw_len || (raw_len % AES_BLOCKLEN))
	{
		free(raw);
		return NULL;
	}

	get_entry_key_iv(index, key, iv);
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_decrypt_buffer(&ctx, raw, raw_len);

	json = inflate_string(raw, pkcs7_unpad(raw, raw_len));
	free(raw);
	if (!json)
		return NULL;

	entry = cJSON_Parse(json);
	free(json);

	return entry;
}

static char* encrypt_patch_entry(const cJSON *entry, int index)
{
	uint8_t key[32], iv[16];
	uint8_t *comp, *enc;
	size_t comp_len, enc_len;
	char *json, *b64;

	json = cJSON_PrintUnformatted(entry);
	comp = deflate_string(json, &comp_len);
	free(json);
	if (!comp)
		return NULL;

	get_entry_key_iv(index, key, iv);
	enc = aes_cbc_encrypt(comp, comp_len, key, iv, &enc_len);
	b64 = b64_encode(enc, enc_len);

	free(comp);
	free(enc);
	return b64;
}

u8* decrypt_data(u8* data, size_t* size)
{
	cJSON *root, *field, *entries, *item;
	char *json, *str;
	int count;

	printf("[*] Compressed Size    : %zu bytes\n", *size);

	json = inflate_string(data, *size);
	if (!json)
	{
		printf("[!] Deflate Error!\n");
		return NULL;
	}

	root = cJSON_Parse(json);
	free(json);
	if (!root)
	{
		printf("[!] JSON Parse Error!\n");
		return NULL;
	}

	field = cJSON_GetObjectItem(root, "Game");
	if (cJSON_IsString(field))
		printf("[*] Game               : %s\n", field->valuestring);

	field = cJSON_GetObjectItem(root, "ProcessName");
	if (cJSON_IsString(field))
	{
		str = decrypt_process_name(field->valuestring);
		if (!str)
		{
			printf("[!] ProcessName Decryption Error!\n");
			cJSON_Delete(root);
			return NULL;
		}

		printf("[*] Process Name       : %s\n", str);
		cJSON_SetValuestring(field, str);
		free(str);
	}

	entries = cJSON_GetObjectItem(root, "patchEntries");
	count = cJSON_GetArraySize(entries);
	printf("[*] Patch Entries      : %d\n", count);

	for (int i = 0; i < count; i++)
	{
		item = cJSON_GetArrayItem(entries, i);
		if (!cJSON_IsString(item))
			continue;

		cJSON *entry = decrypt_patch_entry(item->valuestring, i);
		if (!entry)
		{
			printf("[!] Patch Entry #%d Decryption Error!\n", i);
			cJSON_Delete(root);
			return NULL;
		}

		field = cJSON_GetObjectItem(entry, "name");
		printf("    [%02d] %s\n", i, cJSON_IsString(field) ? field->valuestring : "");
		cJSON_ReplaceItemInArray(entries, i, entry);
	}

	json = cJSON_Print(root);
	cJSON_Delete(root);

	*size = strlen(json);
	free(data);

	printf("[*] Decrypted JSON Size: %zu bytes\n", *size);
	printf("[*] Decrypted File Successfully!\n\n");
	return (u8*) json;
}

u8* encrypt_data(u8* data, size_t* size)
{
	cJSON *root, *field, *entries, *item;
	uint8_t *comp;
	char *json, *str;
	int count;

	printf("[*] Total JSON Size    : %zu bytes\n", *size);

	root = cJSON_ParseWithLength((char*) data, *size);
	if (!root)
	{
		printf("[!] JSON Parse Error!\n");
		return NULL;
	}

	field = cJSON_GetObjectItem(root, "ProcessName");
	if (cJSON_IsString(field))
	{
		printf("[*] Process Name       : %s\n", field->valuestring);
		str = encrypt_process_name(field->valuestring);
		cJSON_SetValuestring(field, str);
		free(str);
	}

	entries = cJSON_GetObjectItem(root, "patchEntries");
	count = cJSON_GetArraySize(entries);
	printf("[*] Patch Entries      : %d\n", count);

	for (int i = 0; i < count; i++)
	{
		item = cJSON_GetArrayItem(entries, i);
		if (!cJSON_IsObject(item))
			continue;

		field = cJSON_GetObjectItem(item, "name");
		printf("    [%02d] %s\n", i, cJSON_IsString(field) ? field->valuestring : "");

		str = encrypt_patch_entry(item, i);
		if (!str)
		{
			printf("[!] Patch Entry #%d Encryption Error!\n", i);
			cJSON_Delete(root);
			return NULL;
		}

		cJSON_ReplaceItemInArray(entries, i, cJSON_CreateString(str));
		free(str);
	}

	json = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);

	comp = deflate_string(json, size);
	free(json);
	if (!comp)
	{
		printf("[!] Deflate Error!\n");
		return NULL;
	}
	free(data);

	printf("[*] Compressed Size    : %zu bytes\n", *size);
	printf("[*] Encrypted File Successfully!\n\n");
	return comp;
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
	u8 *data, *out;
	char *opt, *bak;

	printf("\nShnExt Cheat Decrypter 0.1.0 - (c) 2026 by Bucanero\n\n");

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
	free(bak);

	srand(time(NULL));

	if (*opt == 'd')
		out = decrypt_data(data, &len);
	else
		out = encrypt_data(data, &len);

	if (!out)
	{
		free(data);
		return -1;
	}

	write_buffer(argv[2], out, len);
	free(out);

	return 0;
}
