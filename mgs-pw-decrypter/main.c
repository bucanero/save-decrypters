/*
*
*	MGS PW PS3/PSP Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original XBOX MGS Peace Walker - SecFixer by Philymaster
*
*/

#include "../common/iofile.c"

/*
* Save layout
*
* The PS3 (HD Edition) save holds two encrypted blocks: the main save data,
* followed by a second block with the online/comrade data. The PSP save only
* has the main block, so the second one is skipped when it's not present.
*
* Releases do not all use the same offsets. An array near 0xC0 is four u32
* shorter in some builds, which takes 0x10 bytes off the first checksummed
* region and shifts every later boundary down to match, making the whole save
* that much smaller. The length of that first region therefore pins the entire
* layout, and everything else below is derived from it.
*
*   standard: first region 0x1AF24, main block 0x35998, PSP save 0x3D9D0
*             -> PSP US/EU (ULUS10509, ULES01372) and every PS3 save
*   compact:  first region 0x1AF14, main block 0x35988, PSP save 0x3D9C0
*             -> PSP JP digital (NPJH50045)
*/
#define PW_BLOCK1_OFF		16			/* word offset; 0x40 bytes */
#define PW_BLOCK1_BYTE_OFF	0x40
#define PW_HEADER2_OFF		0xD676		/* 0x359D8 */
#define PW_BLOCK2_OFF		0xD686		/* 0x35A18 */
#define PW_BLOCK2_BYTE_OFF	0x35A18
#define PW_BLOCK2_SIZE		0xF0D0
#define PW_BLOCK2_CSUM_WORD	0xD683

#define PW_REGION1_STANDARD	0x1AF24
#define PW_REGION1_COMPACT	0x1AF14
#define PW_REGION2_SIZE		0x1C00
#define PW_REGION3_SIZE		0x18E68
#define PW_BLOCK1_TAIL		8			/* unchecksummed tail of the main block */
#define PW_PSP_TRAILER		0x7FF8		/* bytes after the main block on PSP */

#define PW_BLOCK1_LEN(r1)	(0x44 + (r1) + PW_REGION2_SIZE + PW_REGION3_SIZE \
							 + PW_BLOCK1_TAIL - PW_BLOCK1_BYTE_OFF)
#define PW_PSP_SAVE_SIZE(r1)	(PW_BLOCK1_BYTE_OFF + PW_BLOCK1_LEN(r1) + PW_PSP_TRAILER)

#define PW_MIN_SAVE_SIZE	(PW_HEADER2_OFF * 4)
#define PW_PS3_SAVE_SIZE	((PW_BLOCK2_OFF * 4) + PW_BLOCK2_SIZE)

typedef struct
{
	int region1;		/* length of the first checksummed region */
	int block1_size;	/* encrypted main block, in bytes */
	int swap_words;		/* words covered by the byte-order swaps */
	int csum_off[3];	/* start of each checksummed range */
	int csum_len[3];	/* and its length */
	int csum_word[3];	/* header word holding each checksum */
} pw_layout_t;

const uint32_t PW_TABLE[256] = { 
	0x50b85761, 0x27bf67f7, 0xbeb6364d, 0xc9b106db, 0x57d59378, 0x20d2a3ee, 0xb9dbf254, 0xcedcc2c2, 
	0x5e63df53, 0x2964efc5, 0xb06dbe7f, 0xc76a8ee9, 0x590e1b4a, 0x2e092bdc, 0xb7007a66, 0xc0074af0, 
	0x4d0f4705, 0x3a087793, 0xa3012629, 0xd40616bf, 0x4a62831c, 0x3d65b38a, 0xa46ce230, 0xd36bd2a6, 
	0x43d4cf37, 0x34d3ffa1, 0xaddaae1b, 0xdadd9e8d, 0x44b90b2e, 0x33be3bb8, 0xaab76a02, 0xddb05a94, 
	0x6bd677a9, 0x1cd1473f, 0x85d81685, 0xf2df2613, 0x6cbbb3b0, 0x1bbc8326, 0x82b5d29c, 0xf5b2e20a, 
	0x650dff9b, 0x120acf0d, 0x8b039eb7, 0xfc04ae21, 0x62603b82, 0x15670b14, 0x8c6e5aae, 0xfb696a38, 
	0x766167cd, 0x166575b, 0x986f06e1, 0xef683677, 0x710ca3d4, 0x60b9342, 0x9f02c2f8, 0xe805f26e, 
	0x78baefff, 0xfbddf69, 0x96b48ed3, 0xe1b3be45, 0x7fd72be6, 0x8d01b70, 0x91d94aca, 0xe6de7a5c, 
	0x266416f1, 0x51632667, 0xc86a77dd, 0xbf6d474b, 0x2109d2e8, 0x560ee27e, 0xcf07b3c4, 0xb8008352, 
	0x28bf9ec3, 0x5fb8ae55, 0xc6b1ffef, 0xb1b6cf79, 0x2fd25ada, 0x58d56a4c, 0xc1dc3bf6, 0xb6db0b60, 
	0x3bd30695, 0x4cd43603, 0xd5dd67b9, 0xa2da572f, 0x3cbec28c, 0x4bb9f21a, 0xd2b0a3a0, 0xa5b79336, 
	0x35088ea7, 0x420fbe31, 0xdb06ef8b, 0xac01df1d, 0x32654abe, 0x45627a28, 0xdc6b2b92, 0xab6c1b04, 
	0x1d0a3639, 0x6a0d06af, 0xf3045715, 0x84036783, 0x1a67f220, 0x6d60c2b6, 0xf469930c, 0x836ea39a, 
	0x13d1be0b, 0x64d68e9d, 0xfddfdf27, 0x8ad8efb1, 0x14bc7a12, 0x63bb4a84, 0xfab21b3e, 0x8db52ba8, 
	0xbd265d, 0x77ba16cb, 0xeeb34771, 0x99b477e7, 0x7d0e244, 0x70d7d2d2, 0xe9de8368, 0x9ed9b3fe, 
	0xe66ae6f, 0x79619ef9, 0xe068cf43, 0x976fffd5, 0x90b6a76, 0x7e0c5ae0, 0xe7050b5a, 0x90023bcc, 
	0xbd00d441, 0xca07e4d7, 0x530eb56d, 0x240985fb, 0xba6d1058, 0xcd6a20ce, 0x54637174, 0x236441e2, 
	0xb3db5c73, 0xc4dc6ce5, 0x5dd53d5f, 0x2ad20dc9, 0xb4b6986a, 0xc3b1a8fc, 0x5ab8f946, 0x2dbfc9d0, 
	0xa0b7c425, 0xd7b0f4b3, 0x4eb9a509, 0x39be959f, 0xa7da003c, 0xd0dd30aa, 0x49d46110, 0x3ed35186, 
	0xae6c4c17, 0xd96b7c81, 0x40622d3b, 0x37651dad, 0xa901880e, 0xde06b898, 0x470fe922, 0x3008d9b4, 
	0x866ef489, 0xf169c41f, 0x686095a5, 0x1f67a533, 0x81033090, 0xf6040006, 0x6f0d51bc, 0x180a612a, 
	0x88b57cbb, 0xffb24c2d, 0x66bb1d97, 0x11bc2d01, 0x8fd8b8a2, 0xf8df8834, 0x61d6d98e, 0x16d1e918, 
	0x9bd9e4ed, 0xecded47b, 0x75d785c1, 0x2d0b557, 0x9cb420f4, 0xebb31062, 0x72ba41d8, 0x5bd714e, 
	0x95026cdf, 0xe2055c49, 0x7b0c0df3, 0xc0b3d65, 0x926fa8c6, 0xe5689850, 0x7c61c9ea, 0xb66f97c, 
	0xcbdc95d1, 0xbcdba547, 0x25d2f4fd, 0x52d5c46b, 0xccb151c8, 0xbbb6615e, 0x22bf30e4, 0x55b80072, 
	0xc5071de3, 0xb2002d75, 0x2b097ccf, 0x5c0e4c59, 0xc26ad9fa, 0xb56de96c, 0x2c64b8d6, 0x5b638840, 
	0xd66b85b5, 0xa16cb523, 0x3865e499, 0x4f62d40f, 0xd10641ac, 0xa601713a, 0x3f082080, 0x480f1016, 
	0xd8b00d87, 0xafb73d11, 0x36be6cab, 0x41b95c3d, 0xdfddc99e, 0xa8daf908, 0x31d3a8b2, 0x46d49824, 
	0xf0b2b519, 0x87b5858f, 0x1ebcd435, 0x69bbe4a3, 0xf7df7100, 0x80d84196, 0x19d1102c, 0x6ed620ba, 
	0xfe693d2b, 0x896e0dbd, 0x10675c07, 0x67606c91, 0xf904f932, 0x8e03c9a4, 0x170a981e, 0x600da888, 
	0xed05a57d, 0x9a0295eb, 0x30bc451, 0x740cf4c7, 0xea686164, 0x9d6f51f2, 0x4660048, 0x736130de, 
	0xe3de2d4f, 0x94d91dd9, 0xdd04c63, 0x7ad77cf5, 0xe4b3e956, 0x93b4d9c0, 0xabd887a, 0x7dbab8ec };

u32 CalculateChecksum(const u8* data, int size)
{
    u32 csum = -1;

    while (size--)
        csum = PW_TABLE[(u8)(*data++ ^ csum)] ^ csum >> 8 ^ PW_TABLE[0];

    return ~csum;
}

void DeEncryptBlock(u32* data, int size, u32* pwSalts)
{
	size /= 4;

	for (int i = 0; i < size; i++)
	{
		data[i] = ES32(ES32(data[i]) ^ pwSalts[0]);
		pwSalts[0] = pwSalts[0] * 0x2e90edd + pwSalts[1];
	}
}

void SetSalts(u32* pwSalts, const u32 *data)
{
    u32 offset = (ES32(data[1]) | 0xAD47DE8F) ^ ES32(data[0]);
    u32 r5 = ES32(data[offset + 7]) ^ 0xBC4DEFA2;
    u32 r4 = ES32(data[offset + 3]) ^ 0x2d71d26c;
    u32 r3 = ES32(data[offset + 2]) ^ 0x1327de73;

    pwSalts[0] = r3 ^ r4;
    pwSalts[1] = pwSalts[0] * r5;
    pwSalts[0] = (pwSalts[0] ^ 0x6576) << 16 | pwSalts[0];
}

void SwapBlock(u32* data, int len)
{
    for (int i = 0; i < len; i++)
    	data[i] = ES32(data[i]);
}

/*
* The PS3 second block is preceded by a plain-text header that follows the same
* layout as the main one. Checking that it decodes to a valid seed offset tells
* PS3 saves from PSP ones, and works on both encrypted and decrypted files.
*/
int HasSecondBlock(const u32* data, size_t len)
{
	if (len < PW_PS3_SAVE_SIZE)
		return 0;

	return (((ES32(data[PW_HEADER2_OFF + 1]) | 0xAD47DE8F) ^ ES32(data[PW_HEADER2_OFF])) < 0x10);
}

/*
* Work out which layout a save uses and derive its offsets. A PS3 save always
* uses the standard one; for a PSP save the total size settles it, since the
* variants differ in size by exactly the amount their first region differs by.
* An unrecognised size falls back to the standard layout, and the checksum
* check below then says so.
*/
void PW_GetLayout(pw_layout_t* lay, size_t len, int ps3)
{
	lay->region1 = (!ps3 && len == PW_PSP_SAVE_SIZE(PW_REGION1_COMPACT))
					? PW_REGION1_COMPACT : PW_REGION1_STANDARD;

	lay->block1_size = PW_BLOCK1_LEN(lay->region1);
	lay->swap_words = PW_BLOCK1_OFF + (lay->block1_size / 4);

	lay->csum_off[0] = 0x44;
	lay->csum_len[0] = lay->region1;
	lay->csum_word[0] = 14;

	lay->csum_off[1] = lay->csum_off[0] + lay->region1;
	lay->csum_len[1] = PW_REGION2_SIZE;
	lay->csum_word[1] = 15;

	lay->csum_off[2] = lay->csum_off[1] + PW_REGION2_SIZE;
	lay->csum_len[2] = PW_REGION3_SIZE;
	lay->csum_word[2] = 12;
}

const char* PW_LayoutName(const pw_layout_t* lay)
{
	return (lay->region1 == PW_REGION1_COMPACT) ? "compact" : "standard";
}

void PW_Decrypt(u32* data, size_t len)
{
    u32 salts[2] = {0, 0};
    int ps3 = HasSecondBlock(data, len);
    pw_layout_t lay;

    PW_GetLayout(&lay, len, ps3);
    printf("[*] Save Type: %s (%s layout)\n", ps3 ? "PS3 (HD Edition)" : "PSP",
    		PW_LayoutName(&lay));

    SwapBlock(data, lay.swap_words);
    SetSalts(salts, data);
    DeEncryptBlock(data + PW_BLOCK1_OFF, lay.block1_size, salts);

    if (ps3)
    {
        SetSalts(salts, data + PW_HEADER2_OFF);
        DeEncryptBlock(data + PW_BLOCK2_OFF, PW_BLOCK2_SIZE, salts);
    }
	SwapBlock(data + 17, lay.swap_words - 17);

    for (int i = 0; i < 3; i++)
        if (CalculateChecksum((u8*)data + lay.csum_off[i], lay.csum_len[i]) != ES32(data[lay.csum_word[i]]))
            printf("[!] Checksum error (%x)\n", lay.csum_off[i]);

    if (ps3 && CalculateChecksum((u8*)data + PW_BLOCK2_BYTE_OFF, PW_BLOCK2_SIZE) != ES32(data[PW_BLOCK2_CSUM_WORD]))
        printf("[!] Checksum error (%x)\n", PW_BLOCK2_BYTE_OFF);

	// The decrypted header is left in the save's native byte order: big-endian
	// for PS3, little-endian for PSP (matching the `transfarmer` PSP tool)
	if (!ps3)
		SwapBlock(data, PW_BLOCK1_OFF + 1);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void PW_Encrypt(u32* data, size_t len)
{
    u32 salts[2] = {0, 0};
    int ps3 = HasSecondBlock(data, len);
    pw_layout_t lay;

    PW_GetLayout(&lay, len, ps3);
    printf("[*] Save Type: %s (%s layout)\n", ps3 ? "PS3 (HD Edition)" : "PSP",
    		PW_LayoutName(&lay));

	if (!ps3)
		SwapBlock(data, PW_BLOCK1_OFF + 1);

    if (ps3)
        data[PW_BLOCK2_CSUM_WORD] = ES32(CalculateChecksum((u8*)data + PW_BLOCK2_BYTE_OFF, PW_BLOCK2_SIZE));

    for (int i = 2; i >= 0; i--)
        data[lay.csum_word[i]] = ES32(CalculateChecksum((u8*)data + lay.csum_off[i], lay.csum_len[i]));

    printf("[*] New Checksums: %08X %08X %08X", data[12], data[14], data[15]);
    if (ps3)
        printf(" %08X", data[PW_BLOCK2_CSUM_WORD]);
    printf("\n");

	SwapBlock(data + 17, lay.swap_words - 17);

    if (ps3)
    {
        SetSalts(salts, data + PW_HEADER2_OFF);
        DeEncryptBlock(data + PW_BLOCK2_OFF, PW_BLOCK2_SIZE, salts);
    }

    SetSalts(salts, data);
    DeEncryptBlock(data + PW_BLOCK1_OFF, lay.block1_size, salts);
	SwapBlock(data, lay.swap_words);

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

	printf("\nMetal Gear Solid Peace Walker save decrypter 0.3.0 - (c) 2021 by Bucanero\n\n");

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

	printf("[*] MGS PW Total File Size Is 0x%X (%d bytes)\n", (int)len, (int)len);

	if (len < PW_MIN_SAVE_SIZE)
	{
		printf("[*] Not A MGS PW Save File (%s)\n", argv[2]);
		free(bak);
		free(data);
		return -1;
	}

	if (*opt == 'd')
		PW_Decrypt((u32*)data, len);
	else
		PW_Encrypt((u32*)data, len);

	write_buffer(argv[2], data, len);

	free(bak);
	free(data);
	
	return 0;
}
