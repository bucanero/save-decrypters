/*
*
*	Final Fantasy XIII (1/2/3) PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original FFXIII Encryptor by Jappi88
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define u8 uint8_t
#define u32 uint32_t
#define u64 uint64_t

#define ES32(_val) \
	((u32)(((((u32)_val) & 0xff000000) >> 24) | \
	       ((((u32)_val) & 0x00ff0000) >> 8 ) | \
	       ((((u32)_val) & 0x0000ff00) << 8 ) | \
	       ((((u32)_val) & 0x000000ff) << 24)))

#define ES64(_val) \
	((u64)(((((u64)_val) & 0xff00000000000000ull) >> 56) | \
	       ((((u64)_val) & 0x00ff000000000000ull) >> 40) | \
	       ((((u64)_val) & 0x0000ff0000000000ull) >> 24) | \
	       ((((u64)_val) & 0x000000ff00000000ull) >> 8 ) | \
	       ((((u64)_val) & 0x00000000ff000000ull) << 8 ) | \
	       ((((u64)_val) & 0x0000000000ff0000ull) << 24) | \
	       ((((u64)_val) & 0x000000000000ff00ull) << 40) | \
	       ((((u64)_val) & 0x00000000000000ffull) << 56)))

#define FFXIII_KEY          0x1317fb09b9b42080L
#define FFXIII_2_KEY        0x9B1F01011A6438B0L 
#define FFXIII_3_KEY        0x36545e6ceb9a705fL

#define FFXIII_MASK_1       0xFFFFFFFFFFFFFC00
#define FFXIII_MASK_2       0xFFFFFFFF00000000

// Xbox FFXIII_KEY          0x035ce4275cee246aL
// Xbox FFXIII_2_KEY        0x6920c2168106440fL
// Xbox FFXIII_3_KEY        0x09f0d55db8ea4bfdL

const u8 ff_decrypt_table[256] = { 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
	136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
	156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
	176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,
	196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215,
	216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235,
	236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
	27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
	51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74,
	75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
	99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119
};

const u8 ff_encrypt_table[256] = { 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151,
	152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171,
	172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
	192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211,
	212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231,
	232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
	252, 253, 254, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
	22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
	46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93,
	94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
	115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135
};


int read_buffer(const char *file_path, u8 **buf, size_t *size)
{
	FILE *fp;
	u8 *file_buf;
	size_t file_size;
	
	if ((fp = fopen(file_path, "rb")) == NULL)
        return -1;
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	file_buf = (u8 *)malloc(file_size);
	fread(file_buf, 1, file_size, fp);
	fclose(fp);
	
	if (buf)
        *buf = file_buf;
	else
        free(file_buf);
	if (size)
        *size = file_size;
	
	return 0;
}

int write_buffer(const char *file_path, u8 *buf, size_t size)
{
	FILE *fp;
	
	if ((fp = fopen(file_path, "wb")) == NULL)
        return -1;
	fwrite(buf, 1, size, fp);
	fclose(fp);
	
	return 0;
}

u64 read_key_file(char ff_game)
{
    u64* data;
    u64 key = (ff_game == '2') ? FFXIII_2_KEY : FFXIII_3_KEY;
    
    if (ff_game == '1')
    	return (FFXIII_KEY);

	if (read_buffer("KEY.DAT", (u8**) &data, NULL) != 0)
        return 0;

    key ^= ES64(data[0] ^ data[1]) | 1L;
    return (key);
}

u64 ff_shift32(u64 ulong_0, int bits)
{
	return (ulong_0 << bits | ulong_0 >> (32 - bits));
}

void ff_decrypt_value(u8 byte_0, u64* ulong_0, u64 ulong_1)
{
	*ulong_0 = (u64)(ff_decrypt_table[(ulong_1 & 0xFF)] - byte_0);
}

u64 ff_shift64(int bits, u64 ulong_0)
{
	return (ulong_0 << bits | ulong_0 >> (64 - bits));
}

u64 bswap64(u64 ulong_0)
{
	return ES64(ulong_0);
}

void ff_set_value(u64 ulong_0, u64* ulong_1)
{
	*ulong_1 = ((ulong_0 >> 24 & 0xFFFFFFFFFFFF00FF) | (ff_shift32(ulong_0, 24) & 0xFF00));
	*ulong_1 = ((*ulong_1 & 0xFFFFFFFFFF00FFFF) | (ff_shift32(ulong_0, 8) & 0xFF0000));
	*ulong_1 = ((*ulong_1 & 0xFFFFFFFF00FFFFFF) | (ff_shift32(ulong_0, 24) & 0xFF000000));
}

void ff_encrypt_value(u8 byte_0, u64 ulong_0, u64* ulong_1)
{
	*ulong_1 = (u64)((ff_encrypt_table[(ulong_0 & 255UL)] & 0xFF) + byte_0);
}

u64 read_u64(u8* bytes)
{
	return ES64(*(u64*)bytes);
}

void ff_init_key_value(u64 ulong_0, u64 ulong_1, u64* ulong_2, u64 ulong_3, u64* ulong_4, u8* byte_0, u64 ulong_5)
{
	ulong_3 = (((ulong_1 + 212UL ^ ulong_5) & 0xFF) ^ 0x45);
	*byte_0 = (u8)ulong_3;
	*ulong_2 = ulong_0 + ulong_3;
	*ulong_4 = (ff_shift32(ulong_3, 2) & 0x3FC);
}

u32 ff_decrypt_data(const u8* key_table, u8* data, int len)
{
	u64 ptr24 = 0L;
	u64 ptr32 = 0L;
	u64 ptr40 = 0L;
	u64 ptr48 = 0L;
	u64 ptr64 = 0L;

	u8 array[8];
	u32 ff_pos = 0;
	u32 ff_csum = 0;
	
	len /= 8;

	for (int i = 0; i < len; i++)
	{
		ff_pos = i << 3;
		ptr24 = (long)((ff_shift32(ff_pos, 29) & 0xFF) ^ 0x45);

		for (int j = 0; j < 8; j++)
		{
			ptr40 = (ptr24 & 0xFF);
			ptr24 = (long)(data[ff_pos + j]);
			ff_decrypt_value(key_table[(0xF8 & ff_pos)], &ptr48, (ptr24 ^ ptr40));
			ff_decrypt_value(key_table[(0xF8 & ff_pos) + 1], &ptr32, ptr48);
			ff_decrypt_value(key_table[(0xF8 & ff_pos) + 2], &ptr40, ptr32);
			ff_decrypt_value(key_table[(0xF8 & ff_pos) + 3], &ptr32, ptr40);
			ff_decrypt_value(key_table[(0xF8 & ff_pos) + 4], &ptr40, ptr32);
			ff_decrypt_value(key_table[(0xF8 & ff_pos) + 5], &ptr32, ptr40);
			ff_decrypt_value(key_table[(0xF8 & ff_pos) + 6], &ptr40, ptr32);
			data[ff_pos + j] = ff_decrypt_table[ptr40 & 0xFF] - key_table[(0xF8 & ff_pos) + 7];
		}

		ptr24 = (long)((ff_shift64(10, ff_shift64(10, ff_pos) | ff_pos) & FFXIII_MASK_1) | ff_pos);
		ptr64 = (long)(((ff_shift64(10, ptr24) & FFXIII_MASK_1) | ff_pos) + 0xA1652347);

		memcpy(array, data + ff_pos, 8);

		u64 num = bswap64(read_u64(array));

		memcpy(array, &key_table[0xF8 & ff_pos], 8);
		
		ptr24 = (long)bswap64(num - bswap64(ES64(*(u64*)array)));
//		*ptr2 = (long)bswap64(num - smethod_58(ptr92[4], ptr92[6], ptr92[2], ptr92[0], ptr92[7], ptr92[5], ptr92[3], ptr92[1]));
		ff_set_value((ptr64 & 0xFFFFFFFF), &ptr32);
		ff_set_value((u64)(ptr64 >> 32), &ptr64);

		ptr32 = (long)(read_u64(array) ^ (ptr24 ^ ((ptr64 & 0xFFFFFFFF) | (ff_shift64(32, ptr32) & FFXIII_MASK_2))));
		
		num = ES64((ptr32 >> 32 & 0xFFFFFFFF) | (ff_shift64(32, ptr32) & FFXIII_MASK_2));
		memcpy(data + ff_pos, &num, 8);
		
		if (i + 1 < len)
			ff_csum += data[4 + ff_pos] + data[ff_pos];
	}

	return (ff_csum);
}


void ff_init_key(u8* key_table, u64 ff_key)
{
	u64 ptr16 = 0L;
	u64 ptr24 = 0L;
	u64 ptr32 = 0L;
	u64 ptr40 = 0L;
	u64 ptr48 = 0L;
	u64 ptr56 = 0L;
	u64 ptr64 = 0L;

	u8 bytes[8];
	u64 tmp;

	ff_set_value(ff_key >> 32, &ptr56);
	ff_set_value(ff_key & 0xFFFFFFFF, &ptr64);
	ptr40 = ((ptr56 & 0xFFFFFFFF) | (ff_shift64(32, ptr64) & FFXIII_MASK_2));
	ptr56 = (long)((ff_shift64(24, (ptr40 << 8 & 0xFFFFFFFF)) & FFXIII_MASK_2) | (ptr40 & 0xFF000000));
	ptr64 = (long)(ff_shift64(16, ptr40) & 0xFFFF);
	ptr16 = (long)((ff_shift64(8, ptr56) & 0xFFFFFFFFFFFFFF00) | ptr64 | ((ff_shift64(32, ptr40) & 0xFFFFFFFF) << 16 & 0xFFFFFFFF));
	ptr56 = (long)((bswap64(ptr16 >> 32) >> 32 & 0xFFFFFFFF) | (ff_shift64(32, bswap64(ptr16 & 0xFFFFFFFF) >> 32) & FFXIII_MASK_2));

	tmp = ES64(ptr56);
	memcpy(bytes, &tmp, 8);

	bytes[0] += 69;
	ff_init_key_value(bytes[2], bytes[0] + bytes[1], &ptr64, ptr16, &ptr16, &bytes[1], ff_shift64(2, (u64)bytes[0]));
	ff_init_key_value(bytes[3], ptr64, &ptr64, ptr16, &ptr16, &bytes[2], ptr16);
	ff_init_key_value(bytes[4], ptr64, &ptr56, ptr16, &ptr48, &bytes[3], ptr16);
	ff_init_key_value(bytes[5], ptr56, &ptr24, ptr16, &ptr40, &bytes[4], ptr48);
	ff_init_key_value(bytes[6], ptr24, &ptr32, ptr16, &ptr64, &bytes[5], ptr40);
	ff_init_key_value(bytes[7], ptr32, &ptr56, ptr32, &ptr32, &bytes[6], ptr64);
	bytes[7] = (u8)(((ptr56 + 212L ^ ptr32) & 0xFF) ^ 0x45);

	memcpy(key_table, bytes, 8);

	for (int j = 0; j < 31; j++)
	{
		memcpy(bytes, key_table + (j << 3), 8);

		ptr56 = (long)bswap64(read_u64(bytes));
		ptr56 = ptr56 + (long)(ff_shift64(2, ptr56) & 0xFFFFFFFFFFFFFFFC);
		tmp = (long)((bswap64(ptr56 >> 32) >> 32 & 0xFFFFFFFF) | (ff_shift64(32, bswap64(ptr56 & 0xFFFFFFFF) >> 32) & FFXIII_MASK_2));

		tmp = ES64(tmp);
		memcpy(key_table + (j << 3) + 8, &tmp, 8);
	}
}

void ff_set_checksum(u8* bytes, u64 ff_pos, u32 ff_csum)
{
	u64 ptr = 0L;
	u64 ptr8 = 0L;
	u64 ptr16 = 0L;
	u64 ptr24 = 0L;

	ptr = ((ff_pos & 0xFFFFFFFF) | (ff_shift64(32, ff_csum) & FFXIII_MASK_2));
	ptr8 = (long)((ptr & FFXIII_MASK_2) >> 32);
	ptr16 = (long)(ptr8 >> 24);
	ff_set_value(ptr8, &ptr16);
	ff_set_value((ptr & 0xFFFFFFFF), &ptr24);

	u64 tmp = ES64((ptr16 & 0xFFFFFFFF) | (ff_shift64(32, ptr24) & FFXIII_MASK_2));
	memcpy(bytes + ff_pos, &tmp, 8);
}

void ff_encrypt_data(const u8* key_table, u8* data, int len)
{
	u64 ptr24 = 0;
	u64 ptr32 = 0;
	u64 ptr40 = 0;
	u64 ptr48 = 0;
	u64 ptr56 = 0;
	u64 ptr64 = 0;
	u64 ptr72 = 0;
	u64 ptr88 = 0;

	u8 array[8];
	u32 ff_pos = 0;
	u32 ff_csum = 0;
	
	len /= 8;

	for (int i = 0; i < len; i++)
	{
		ff_pos = (i << 3);

		if (i + 1 < len)
			ff_csum += (u32)data[4 + ff_pos] + data[ff_pos];
		else
			ff_set_checksum(data, ff_pos, ff_csum);

		memcpy(array, data + ff_pos, 8);

		ptr64 = (long)read_u64(array);
		ptr88 = (long)ff_pos;
		ptr40 = (long)((ff_shift64(10, ff_shift64(10, ptr88) | ptr88) & FFXIII_MASK_1) | ptr88);
		ptr56 = (long)(0xA1652347 + ((ff_shift64(10, ptr40) & FFXIII_MASK_1) | ptr88));
		ptr32 = 0L;
		ff_set_value((ptr56 >> 32), &ptr32);
		ff_set_value((ptr56 & 0xFFFFFFFF), &ptr40);

		memcpy(array, &key_table[0xF8 & ff_pos], 8);

		ptr24 = (((ptr32 & 0xFFFFFFFF) | (ff_shift64(32, ptr40) & FFXIII_MASK_2)) ^ read_u64(array) ^ ((ff_shift64(32, ptr64) & 0xFFFFFFFF) | (ff_shift64(32, ptr64) & FFXIII_MASK_2)));

		ptr48 = bswap64(ES64(*(u64*)array));
//		ptr48 = (long)smethod_58(ptr104[4], ptr104[6], ptr104[2], ptr104[0], ptr104[7], ptr104[5], ptr104[3], ptr104[1]);

		u64 tmp = ES64(bswap64(bswap64(ptr24) + ptr48));
		memcpy(data + ff_pos, &tmp, 8);

		ptr32 = (long)((ff_shift32(ptr88, 29) & 0xFF) ^ 69UL);

		for (int j = 0; j < 8; j++)
		{
			ptr72 = (ptr32 & 0xFF);
			ptr32 = (long)((u64)key_table[(0xF8 & ff_pos)] + (u64)data[ff_pos + j]);
			for (int k = 1; k < 8; k++)
				ff_encrypt_value(key_table[(0xF8 & ff_pos) + k], ptr32, &ptr32);

			ff_encrypt_value(0, ptr32, &ptr32);
			ptr32 = (ptr32 ^ ptr72);
			data[ff_pos + j] = (u8)ptr32;
		}
	}
}


void decrypt_data(u8* data, u32 size, u64 key)
{
	u8 key_table[272];
	u8 ffgame = 1;
	u32 csum, ff_csum;

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	memset(key_table, 0, sizeof(key_table));
	ff_init_key(key_table, key);

	if (ffgame == 3 && size > 547904)
	{
		size = 547904;
	}

	ff_csum = ff_decrypt_data(key_table, data, size);

	csum = *(u32*)(data + size - 4);
	if (csum != ff_csum)
	{
		printf("[!] Decrypted data did not pass file integrity check. (Expected: %08X Got: %08X)\n", csum, ff_csum);
	}

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size, u64 key)
{
	u8 key_table[272];
	u8 ffgame = 1;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	memset(key_table, 0, sizeof(key_table));
	ff_init_key(key_table, key);

	ff_encrypt_data(key_table, data, size);

	if (ffgame == 3 && size == 547904)
	{
//		Array.Resize<byte>(ref array3, 560736);
	}

	printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] [type] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt File\n");
	printf(" -e            Encrypt File\n\n");
	printf("GAME TYPE      Explanation:\n");
	printf(" -1            Final Fantasy XIII\n");
	printf(" -2            Final Fantasy XIII-2\n");
	printf(" -3            Lightning Returns: Fantasy XIII\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	u64 ff_key;
	char *opt, *type, *bak;

	printf("\nff13-ps3save-decrypter 0.1.0 - (c) 2021 by Bucanero\n\n");
	
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
	if (*type++ != '-' || (*type != '1' && *type != '2' && *type != '3'))
	{
		print_usage(argv[0]);
		return -1;
	}
	ff_key = read_key_file(*type);
	if (!ff_key)
	{
		printf("[*] Could Not Access The File (KEY.DAT)\n");
		return -1;
	}

	if (read_buffer(argv[3], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[3]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[3]);
	write_buffer(bak, data, len);

	if (*opt == 'd')
		decrypt_data(data, len, ff_key);
	else
		encrypt_data(data, len, ff_key);

//	write_buffer("out.bin", data, len);
	write_buffer(argv[3], data, len);

	free(bak);
	free(data);
	
	return 0;
}
