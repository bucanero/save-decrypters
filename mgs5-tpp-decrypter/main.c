/*
*
*	Metal Gear Solid V: The Phantom Pain PS3/PS4 Save Decrypter - (c) 2023 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original Xbox "MGS: The Phantom Pain SecFixer" by Philymaster
*
*/

#include "../common/iofile.c"
#include "../common/md5.c"

#define MGSV_TPP_PS3KEY_BLES02102		0xdd0dcf0e		// md5('BLES02102_MGS_TPP')
#define MGSV_TPP_PS3KEY_BLUS31491		0x22f9710b		// md5('BLUS31491_MGS_TPP')
#define MGSV_TPP_PS3KEY_BLJM61247		0x4099e562		// md5('BLJM61247_MGS_TPP')
#define MGSV_TPP_PS3KEY_NPEB02140		0x7b0ed589		// md5('NPEB02140_MGS_TPP')
#define MGSV_TPP_PS3KEY_NPUB31594		0x1FBAB234		// md5('NPUB31594_MGS_TPP')
#define MGSV_TPP_PS3KEY_NPJB00673		0xe8910fb1		// md5('NPJB00673_MGS_TPP')

#define MGSV_GZ_PS3KEY_BLES01971		0xe13943c4		// md5('BLES01971_MGS_GZ')
#define MGSV_GZ_PS3KEY_BLUS31369		0x9a25f3f5		// md5('BLUS31369_MGS_GZ')
#define MGSV_GZ_PS3KEY_BLJM61135		0xd297d9d6		// md5('BLJM61135_MGS_GZ')
#define MGSV_GZ_PS3KEY_BLAS50697		0x951e18dc		// md5('BLAS50697_MGS_GZ')
#define MGSV_GZ_PS3KEY_NPEB01889		0xae88976a		// md5('NPEB01889_MGS_GZ')
#define MGSV_GZ_PS3KEY_NPUB31318		0xc6a8b93d		// md5('NPUB31318_MGS_GZ')
#define MGSV_GZ_PS3KEY_NPJB00396		0x42bdc79c		// md5('NPJB00396_MGS_GZ')

// PS4 Keys
// https://github.com/mi5hmash/MGSV_SaveTranslator/blob/main/MGSVProfiles.json

#define MGSV_TPP_PS4KEY_CUSA01140		0x4131F8BE		// md5('MGSVTPPSaveDataNA')
#define MGSV_TPP_PS4KEY_CUSA01154		0x4F36C055		// md5('MGSVTPPSaveDataEU')
#define MGSV_TPP_PS4KEY_CUSA01099		0x40FDA272		// md5('MGSVTPPSaveDataJP')

#define MGSV_GZ_PS4KEY_CUSA00218		0xEA11D524		// md5('MGSVGZSaveDataNA')
#define MGSV_GZ_PS4KEY_CUSA00211		0xD2225CCB		// md5('MGSVGZSaveDataEU')
#define MGSV_GZ_PS4KEY_CUSA00225		0x697B6E1B		// md5('MGSVGZSaveDataJP')

const uint32_t MGS5_KEYS[] = {
	MGSV_TPP_PS3KEY_BLES02102, MGSV_TPP_PS3KEY_BLUS31491, MGSV_TPP_PS3KEY_BLJM61247,
	MGSV_TPP_PS3KEY_NPEB02140, MGSV_TPP_PS3KEY_NPUB31594, MGSV_TPP_PS3KEY_NPJB00673,
	MGSV_GZ_PS3KEY_BLES01971 , MGSV_GZ_PS3KEY_BLUS31369 , MGSV_GZ_PS3KEY_BLJM61135,
	MGSV_GZ_PS3KEY_NPEB01889 , MGSV_GZ_PS3KEY_NPUB31318 , MGSV_GZ_PS3KEY_NPJB00396,
	MGSV_TPP_PS4KEY_CUSA01140, MGSV_TPP_PS4KEY_CUSA01154, MGSV_TPP_PS4KEY_CUSA01099,
	MGSV_GZ_PS4KEY_CUSA00218 , MGSV_GZ_PS4KEY_CUSA00211 , MGSV_GZ_PS4KEY_CUSA00225 };

void md5_hash(const u8* in, u32 size, u8* out)
{
	MD5_CTX context;

	MD5Init (&context);
	MD5Update (&context, in, size);
	MD5Final (out, &context);
}

void print_md5(const char* msg, const u8* data)
{
	printf("%s", msg);
	for (int i=0; i<16; i++)
		printf("%02X", data[i]);
	printf("\n");
}

void EncData(u32* source, int length, u8 type)
{
	uint32_t key = MGS5_KEYS[type];

	for (int i = 0; i < length >> 2; i++)
	{
		key ^= (key << 13);
		key ^= (key >> 7);
		key ^= (key << 5);

		source[i] = (type < 12) ? ES32(ES32(source[i]) ^ key) : (source[i] ^ key);
	}
}

void decrypt_data(u8* data, u32 size, u8 type)
{
	printf("[*] Decrypting %s File with Key: 0x%08X\n", (type < 12) ? "PS3" : "PS4", MGS5_KEYS[type]);
	printf("[*] Total Decrypted Size: 0x%X (%d bytes)\n", size, size);

	EncData((u32*) data, size, type);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size, u8 type)
{
	printf("[*] Encrypting %s File with Key: 0x%08X\n", (type < 12) ? "PS3" : "PS4", MGS5_KEYS[type]);
	printf("[*] Total Encrypted Size: 0x%X (%d bytes)\n", size, size);

	EncData((u32*) data, size, type);

	printf("[*] Encrypted File Successfully!\n\n");
	return;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" MGS V: The Phantom Pain\n");
	printf(" -0            Auto-Decrypt/Encrypt PS3 File (BLES02102)\n");
	printf(" -1            Auto-Decrypt/Encrypt PS3 File (BLUS31491)\n");
	printf(" -2            Auto-Decrypt/Encrypt PS3 File (BLJM61247)\n");
	printf(" -3            Auto-Decrypt/Encrypt PS3 File (NPEB02140)\n");
	printf(" -4            Auto-Decrypt/Encrypt PS3 File (NPUB31594)\n");
	printf(" -5            Auto-Decrypt/Encrypt PS3 File (NPJB00673)\n");
	printf(" MGS V: TPP (PS4)\n");
	printf(" -12           Auto-Decrypt/Encrypt PS4 File (CUSA01140)\n");
	printf(" -13           Auto-Decrypt/Encrypt PS4 File (CUSA01154)\n");
	printf(" -14           Auto-Decrypt/Encrypt PS4 File (CUSA01099)\n");
	printf("\n MGS V: Ground Zeroes\n");
	printf(" -6            Auto-Decrypt/Encrypt PS3 File (BLES01971)\n");
	printf(" -7            Auto-Decrypt/Encrypt PS3 File (BLUS31369)\n");
	printf(" -8            Auto-Decrypt/Encrypt PS3 File (BLJM61135)\n");
	printf(" -9            Auto-Decrypt/Encrypt PS3 File (NPEB01889)\n");
	printf(" -10           Auto-Decrypt/Encrypt PS3 File (NPUB31318)\n");
	printf(" -11           Auto-Decrypt/Encrypt PS3 File (NPJB00396)\n");
	printf(" MGS V: GZ (PS4)\n");
	printf(" -15           Auto-Decrypt/Encrypt PS4 File (CUSA00218)\n");
	printf(" -16           Auto-Decrypt/Encrypt PS4 File (CUSA00211)\n");
	printf(" -17           Auto-Decrypt/Encrypt PS4 File (CUSA00225)\n\n");
	return;
}

int main(int argc, char **argv)
{
	size_t len;
	u8* data;
	char *opt, *bak;
	long val;

	printf("\nMetal Gear Solid V: TPP&GZ PS3/PS4 Save Decrypter 0.2.0 - (c) 2023 by Bucanero\n\n");

	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}
	
	opt = argv[1];
	if (*opt++ != '-' || (val = strtol(opt, NULL, 10)) < 0 || val > 17)
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

	if ((data[0x10] == 'S' && data[0x11] == 'V') || (data[0x10] == 'g' && data[0x11] == 'z'))
	{
		print_md5("[*] Old MD5: ", data);
		md5_hash(data + 0x10, len - 0x10, data);
		print_md5("[*] New MD5: ", data);

		encrypt_data(data, len, val);
	}
	else
		decrypt_data(data, len, val);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
