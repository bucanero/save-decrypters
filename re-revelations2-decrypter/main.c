/*
*
*	Resident Evil Revelation 2 PS3/PS4 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original RE Revelations 2 Decrypter by forjador123
*
* Information about the Blowfish encryption method:
*	- https://www.geeksforgeeks.org/blowfish-algorithm-with-examples/
*
*/

#include "../common/iofile.c"
#include "../common/blowfish.c"
#include "../common/sha1.c"

#define SECRET_KEY      "zW$2eWaHNdT~6j86T_&j"

static const u8 PS4_FINGERPRINT[] = { 0x00, 0x24, 0x12, 0x14, 0xD0, 0x84, 0x12 };


/* PS3: big-endian dword addition */
/* PS4: little-endian dword addition */
u32 dwadd(const u8* data, u32 len, int is_ps4)
{
	u32 csum = 0;

	len /= 4;
	while (len--)
	{
		csum += is_ps4 ? *(u32*)data : ES32(*(u32*)data);
		data += 4;
	}

	return csum;
}

void swap_u32_data(u32* data, int count)
{
	for (int i = 0; i < count; i++)
		data[i] = ES32(data[i]);
}

/* PS3 decrypt/encrypt (no endian swap needed) */
/* PS4 decrypt/encrypt (swap u32 words before/after blowfish) */
void decrypt_data(u32* data, u32 size, int is_ps4)
{
	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	if (is_ps4)
		swap_u32_data(data, size / 4);
	blowfish_decrypt_buffer(data, size);
	if (is_ps4)
		swap_u32_data(data, size / 4);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u32* data, u32 size, int is_ps4)
{
	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	if (is_ps4)
		swap_u32_data(data, size / 4);
	blowfish_encrypt_buffer(data, size);
	if (is_ps4)
		swap_u32_data(data, size / 4);

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

	printf("\nre-revelations2-save-decrypter 0.2.0 - (c) 2021 by Bucanero\n\n");

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

	blowfish_init_key(SECRET_KEY);

	if (read_buffer(argv[2], &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", argv[2]);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", argv[2]);
	write_buffer(bak, data, len);

	if (memcmp(data + 0x10, PS4_FINGERPRINT, sizeof(PS4_FINGERPRINT)) == 0)
	{
		printf("[*] Detected PS4 save file\n");

		if (*opt == 'd')
			decrypt_data((u32*)(data + 0x20), len - 0x20, 1);
		else
		{
			printf("[*] Source DWADD : %" PRIx32 "\n", ES32(*(u32*)(data + 0x18)));
			u32 chks = dwadd(data + 0x20, 0x19490 - 0x20, 1);
			chks += dwadd(data + 0x194A0, 0x1284C0 - 0x194A0, 1);
			*(u32*)(data + 0x18) = chks;
			printf("[*] Source SHA1  : " SHA1_FMT(data + len - 0x20, "\n"));
			sha1(data + len - 0x20, data + 0x20, (len - 0x40));
			swap_u32_data((u32*)(data + len - 0x20), 5);
			printf("[*] Updated DWADD: %" PRIx32 "\n", ES32(*(u32*)(data + 0x18)));
			printf("[*] Updated SHA1 : " SHA1_FMT(data + len - 0x20, "\n"));

			encrypt_data((u32*)(data + 0x20), len - 0x20, 1);
		}
	}
	else
	{
		printf("[*] Detected PS3 save file\n");

		if (*opt == 'd')
			decrypt_data((u32*)(data + 0x10), len - 0x10, 0);
		else
		{
			u32 crc = ES32(dwadd(data + 0x10, len - 0x30, 0));
			memcpy(data + 0x08, &crc, sizeof(u32));
			printf("[*] Updated DWADD: %08X\n", ES32(crc));

			sha1(data + 0x127590, data + 0x10, (len - 0x30));
			printf("[*] Updated SHA1 : " SHA1_FMT(data + 0x127590, "\n"));

			encrypt_data((u32*)(data + 0x10), len - 0x10, 0);
		}
	}

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);

	return 0;
}
