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

#define read_u64(bytes)     ES64(*(u64*)(bytes))
#define bswap64(a)          ES64((a))
#define bswap32(a)          ES32((a) & 0xFFFFFFFF);
#define shift_bits(base, bits, ulong)			((u64)(ulong) << bits | (u64)(ulong) >> (base - bits))


#define FFXIII_KEY          0x1317fb09b9b42080L
#define FFXIII_2_KEY        0x9B1F01011A6438B0L
#define FFXIII_3_KEY        0x36545e6ceb9a705fL

#define FFXIII_MASK_1       0xFFFFFFFFFFFFFC00
#define FFXIII_MASK_2       0xFFFFFFFF00000000

// Xbox FFXIII_KEY          0x035ce4275cee246aL
// Xbox FFXIII_2_KEY        0x6920c2168106440fL
// Xbox FFXIII_3_KEY        0x09f0d55db8ea4bfdL


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

void ff_init_key_value(u64 ulong_0, u64 ulong_1, u64* ulong_2, u64 ulong_3, u64* ulong_4, u8* byte_0, u64 ulong_5)
{
	ulong_3 = ((((ulong_1 + 0xD4) ^ ulong_5) & 0xFF) ^ 0x45);
	*byte_0 = (u8)ulong_3;
	*ulong_2 = ulong_0 + ulong_3;
	*ulong_4 = (shift_bits(32, 2, ulong_3) & 0x3FC);
}

u32 ff_decrypt_data(const u8* key_table, u8* data, int len)
{
	u8 sub_table[256];
	u64 block[3] = {0};
	u64 tmp;

	u32 ff_pos = 0;
	u32 ff_csum = 0;
	
	len /= 8;
	
	for (int i = 0; i < 256; i++)
		sub_table[i] = (0x78 + i) & 0xFF;

	for (int i = 0; i < len; i++)
	{
		ff_pos = i << 3;
		block[0] = (u64)((shift_bits(32, 29, ff_pos) & 0xFF) ^ 0x45);

		for (int j = 0; j < 8; j++)
		{
			tmp = (block[0] & 0xFF);
			block[0] = data[ff_pos + j];
			block[2] = sub_table[(block[0] ^ tmp) & 0xFF] - key_table[(0xF8 & ff_pos)];
			block[1] = sub_table[block[2] & 0xFF] - key_table[(0xF8 & ff_pos) + 1];
			tmp = sub_table[block[1] & 0xFF] - key_table[(0xF8 & ff_pos) + 2];
			block[1] = sub_table[tmp & 0xFF] - key_table[(0xF8 & ff_pos) + 3];
			tmp = sub_table[block[1] & 0xFF] - key_table[(0xF8 & ff_pos) + 4];
			block[1] = sub_table[tmp & 0xFF] - key_table[(0xF8 & ff_pos) + 5];
			tmp = sub_table[block[1] & 0xFF] - key_table[(0xF8 & ff_pos) + 6];
			data[ff_pos + j] = sub_table[tmp & 0xFF] - key_table[(0xF8 & ff_pos) + 7];
		}

		block[0] = (u64)((shift_bits(64, 10, shift_bits(64, 10, ff_pos) | ff_pos) & FFXIII_MASK_1) | ff_pos);
		tmp = (u64)(((shift_bits(64, 10, block[0]) & FFXIII_MASK_1) | ff_pos) + 0xA1652347);

		block[2] = read_u64(&key_table[0xF8 & ff_pos]);
		block[0] = bswap64(bswap64(read_u64(data + ff_pos)) - bswap64(block[2]));

		block[1] = bswap32((tmp & 0xFFFFFFFF));
		tmp = bswap32((u64)(tmp >> 32));

		block[1] = (u64)(block[2] ^ (block[0] ^ ((tmp & 0xFFFFFFFF) | (shift_bits(64, 32, block[1]) & FFXIII_MASK_2))));
		
		tmp = ES64((block[1] >> 32 & 0xFFFFFFFF) | (shift_bits(64, 32, block[1]) & FFXIII_MASK_2));
		memcpy(data + ff_pos, &tmp, 8);
		
		if (i + 1 < len)
			ff_csum += data[4 + ff_pos] + data[ff_pos];
	}

	return (ff_csum);
}


void ff_init_key(u8* key_table, u64 ff_key)
{
	u64 init[4] = {0};
	u64 tmp;

	init[2] = bswap32(ff_key >> 32);
	init[3] = bswap32(ff_key & 0xFFFFFFFF);
	init[1] = ((init[2] & 0xFFFFFFFF) | (shift_bits(64, 32, init[3]) & FFXIII_MASK_2));
	init[2] = (u64)((shift_bits(64, 24, (init[1] << 8 & 0xFFFFFFFF)) & FFXIII_MASK_2) | (init[1] & 0xFF000000));
	init[3] = (u64)(shift_bits(64, 16, init[1]) & 0xFFFF);
	init[0] = (u64)((shift_bits(64, 8, init[2]) & 0xFFFFFFFFFFFFFF00) | init[3] | ((shift_bits(64, 32, init[1]) & 0xFFFFFFFF) << 16 & 0xFFFFFFFF));
	init[2] = (u64)((bswap64(init[0] >> 32) >> 32 & 0xFFFFFFFF) | (shift_bits(64, 32, bswap64(init[0] & 0xFFFFFFFF) >> 32) & FFXIII_MASK_2));

	tmp = ES64(init[2]);
	memcpy(key_table, &tmp, 8);

	key_table[0] += 69;
	ff_init_key_value(key_table[2], key_table[0] + key_table[1], &init[3], init[0], &init[0], &key_table[1], shift_bits(64, 2, (u64)key_table[0]));
	ff_init_key_value(key_table[3], init[3], &init[3], init[0], &init[0], &key_table[2], init[0]);
	ff_init_key_value(key_table[4], init[3], &init[2], init[0], &tmp, &key_table[3], init[0]);
	ff_init_key_value(key_table[5], init[2], &tmp, init[0], &init[1], &key_table[4], tmp);
	ff_init_key_value(key_table[6], tmp, &tmp, init[0], &init[3], &key_table[5], init[1]);
	ff_init_key_value(key_table[7], tmp, &init[2], tmp, &tmp, &key_table[6], init[3]);
	key_table[7] = (u8)((((init[2] + 0xD4) ^ tmp) & 0xFF) ^ 0x45);

	for (int j = 0; j < 31; j++)
	{
		init[2] = (u64)bswap64(read_u64(key_table + j*8));
		init[2] = init[2] + (u64)(shift_bits(64, 2, init[2]) & 0xFFFFFFFFFFFFFFFC);
		tmp = (u64)((bswap64(init[2] >> 32) >> 32 & 0xFFFFFFFF) | (shift_bits(64, 32, bswap64(init[2] & 0xFFFFFFFF) >> 32) & FFXIII_MASK_2));

		tmp = ES64(tmp);
		memcpy(key_table + (j+1)*8, &tmp, 8);
	}
}

u32 ff_xiii_checksum(u8* bytes, u32 len)
{
	u32 ff_csum = 0;

	len /= 8;
	
	while (len--)
	{
		ff_csum += (u32)bytes[4] + bytes[0];
		bytes += 8;
	}

	return (ff_csum);
}

void ff_encrypt_data(const u8* key_table, u8* data, int len)
{
	u8 add_table[256];
	u64 block[3] = {0};
	u64 tmp = 0;
	u32 ff_pos = 0;
	
	len /= 8;

	for (int i = 0; i < 256; i++)
		add_table[i] = (0x88 + i) & 0xFF;

	for (int i = 0; i < len; i++)
	{
		ff_pos = (i << 3);

		tmp = read_u64(data + ff_pos);
		block[1] = (u64)((shift_bits(64, 10, shift_bits(64, 10, ff_pos) | ff_pos) & FFXIII_MASK_1) | ff_pos);
		block[2] = (u64)(0xA1652347 + ((shift_bits(64, 10, block[1]) & FFXIII_MASK_1) | ff_pos));
		block[0] = bswap32((block[2] >> 32));
		block[1] = bswap32((block[2] & 0xFFFFFFFF));

		block[2] = read_u64(&key_table[0xF8 & ff_pos]);

		tmp = (((block[0] & 0xFFFFFFFF) | (shift_bits(64, 32, block[1]) & FFXIII_MASK_2)) ^ block[2] ^ ((shift_bits(64, 32, tmp) & 0xFFFFFFFF) | (shift_bits(64, 32, tmp) & FFXIII_MASK_2)));

		tmp = ES64(bswap64(bswap64(tmp) + bswap64(block[2])));
		memcpy(data + ff_pos, &tmp, 8);

		block[0] = (u64)((shift_bits(32, 29, ff_pos) & 0xFF) ^ 0x45);

		for (int j = 0; j < 8; j++)
		{
			tmp = (block[0] & 0xFF);
			block[0] = (u64)((u64)key_table[(0xF8 & ff_pos)] + (u64)data[ff_pos + j]);
			for (int k = 1; k < 8; k++)
				block[0] = add_table[(block[0] & 0xFF)] + key_table[(0xF8 & ff_pos) + k];

			block[0] = (add_table[(block[0] & 0xFF)] ^ tmp);
			data[ff_pos + j] = (u8)block[0];
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
	u32 csum;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	csum = ff_xiii_checksum(data, size - 8);
	memcpy(data + size - 4, &csum, 4);

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
