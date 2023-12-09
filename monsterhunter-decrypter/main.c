/*
*
*    Monster Hunter 2/3 PSP Save Decrypter - (c) 2023 by Bucanero - www.bucanero.com.ar
*
* This tool is based on https://github.com/svanheulen/mhef
* and MHFU Save Tools https://forums.ppsspp.org/showthread.php?tid=5476
*
*/

#include "../common/iofile.c"
#include "../common/sha1.c"

#define MHP2G_JP_HASH_SALT  "S)R?Bf8xW3#5h9lGU8wR"
#define MHP2G_NA_HASH_SALT  "3Nc94Hq1zOLh8d62Sb69"
#define MHP3_JP_HASH_SALT   "VQ(DOdIO9?X3!2GmW#XF"

//uint32_t mh_key_default[2] = {0xdfa3, 0x215f};
//uint32_t mh_key_modifier[2] = {0xffef, 0xff8f};

#define MH_KEY_DEF0        0xdfa3
#define MH_KEY_DEF1        0x215f
#define MH_KEY_MOD0        0xffef
#define MH_KEY_MOD1        0xff8f

#define QUEST_MHP2G_JP_HASH_SALT    "37wyS2Jfc3x5w9oG"
#define QUEST_MHP2G_NA_HASH_SALT    "Vd6gh8F30wA86Ex5"
#define QUEST_MHP3_JP_HASH_SALT     "sR2Tf4eLAj8b3TH7"

/* \MHFU Save Tools\data\MHFUdic_de.bin (8/10/2014 7:10:44 PM)
   StartOffset(h): 00000000, EndOffset(h): 000000FF, Length(h): 00000100 */
const unsigned char mh2_dec_table[256] = {
    0x56, 0x7C, 0x9A, 0x8A, 0x1F, 0x38, 0x10, 0x3F, 0x65, 0xEA, 0x40, 0xF8,
    0x5C, 0xFC, 0xA2, 0x76, 0xC7, 0x01, 0x8B, 0xEB, 0xAB, 0xCD, 0xD7, 0x14,
    0x98, 0xBF, 0xA6, 0x52, 0xE9, 0x85, 0xF6, 0x48, 0x89, 0x9C, 0x7E, 0x06,
    0x7A, 0xE6, 0x8F, 0x1A, 0xB0, 0x1D, 0x69, 0xF4, 0x3E, 0xAC, 0x4D, 0x04,
    0x55, 0x28, 0xC2, 0x23, 0x9B, 0xD6, 0x3D, 0xED, 0xC5, 0x32, 0x00, 0x6C,
    0x77, 0x47, 0xD3, 0xC8, 0xD0, 0x0B, 0x87, 0x7D, 0xFB, 0xCC, 0xC0, 0x1B,
    0x93, 0x24, 0x75, 0xEC, 0xF0, 0x37, 0x8D, 0xE5, 0x84, 0xDC, 0xF3, 0x4C,
    0x44, 0x7B, 0xD1, 0x0C, 0x49, 0x41, 0xBA, 0x1E, 0x09, 0x0D, 0x22, 0x11,
    0x68, 0xA0, 0x6B, 0xA5, 0xBD, 0x82, 0xB3, 0x83, 0x36, 0xD8, 0xB7, 0x08,
    0xC9, 0xB8, 0x03, 0x30, 0x81, 0xFE, 0x02, 0xFD, 0xE2, 0xB1, 0x2D, 0x3A,
    0x61, 0xA1, 0x05, 0x27, 0x70, 0x59, 0xE8, 0x78, 0x97, 0x2E, 0x15, 0xF9,
    0x2B, 0xDA, 0x19, 0x33, 0x18, 0x4E, 0x7F, 0x9D, 0xBB, 0xE1, 0x20, 0xDB,
    0x67, 0xB4, 0x39, 0xA3, 0x46, 0x8E, 0x86, 0xBE, 0x51, 0x3C, 0xC6, 0x74,
    0x50, 0xA7, 0xAF, 0xC4, 0x80, 0x66, 0xDD, 0xA4, 0xAE, 0x26, 0x4A, 0x6D,
    0x0F, 0xB2, 0x4B, 0x35, 0x5E, 0x2F, 0x21, 0x63, 0x64, 0xE4, 0x0E, 0xA8,
    0xCE, 0x2A, 0xE0, 0x29, 0x91, 0x96, 0x73, 0x16, 0x43, 0xB5, 0x62, 0x5A,
    0x34, 0x54, 0xF2, 0x95, 0x5D, 0xDE, 0x79, 0x1C, 0xD5, 0x12, 0x0A, 0xFA,
    0xE3, 0xA9, 0x88, 0xEE, 0x94, 0xCB, 0x17, 0xB6, 0x6A, 0x99, 0x53, 0x92,
    0x13, 0x6E, 0xEF, 0x25, 0xAD, 0xFF, 0xCF, 0x9E, 0x31, 0x45, 0xCA, 0x8C,
    0x90, 0xD2, 0xAA, 0xBC, 0x5B, 0x9F, 0x60, 0x4F, 0x72, 0x6F, 0xDF, 0x58,
    0x07, 0xF5, 0xC3, 0x5F, 0x57, 0x71, 0x2C, 0xF1, 0xF7, 0xD4, 0x42, 0xB9,
    0xD9, 0xE7, 0xC1, 0x3B
};

const unsigned char mh3_dec_table[256] = {
    0xa6, 0x21, 0x22, 0xf9, 0x3d, 0xfb, 0xf5, 0x06, 0x87, 0x54, 0x76, 0x75, 0xed, 0x16, 0x33, 0x2e, 
    0x5a, 0x17, 0x50, 0x1c, 0xde, 0xaf, 0x73, 0x39, 0xb5, 0x28, 0xd9, 0xf1, 0xcd, 0x98, 0x5f, 0x2d, 
    0x78, 0x62, 0x29, 0xc9, 0xfd, 0xea, 0x32, 0xcc, 0x31, 0x70, 0x34, 0x61, 0xae, 0x4d, 0xfe, 0xc2, 
    0x45, 0x24, 0xf8, 0xfc, 0xd7, 0x2f, 0xd8, 0x26, 0x59, 0xa0, 0xbd, 0xa5, 0x01, 0x18, 0xa1, 0x95, 
    0xee, 0x4b, 0x1a, 0x7a, 0x5b, 0xdb, 0xf0, 0x27, 0xe6, 0xf4, 0xb2, 0xad, 0x4a, 0x14, 0x9a, 0x20, 
    0xb9, 0x36, 0x4f, 0x3b, 0xca, 0xe4, 0x41, 0x85, 0x3a, 0x46, 0x5c, 0xcb, 0x3c, 0xec, 0x63, 0x84, 
    0x66, 0xc4, 0xeb, 0x25, 0xaa, 0x7e, 0xc3, 0x47, 0x9b, 0x74, 0x8b, 0x5d, 0x23, 0x8f, 0x72, 0x81, 
    0x8c, 0x56, 0xc6, 0xd4, 0x40, 0x60, 0xa2, 0x6d, 0xe5, 0xb0, 0x15, 0x58, 0x52, 0x0f, 0x7d, 0x67, 
    0xef, 0xd2, 0x0b, 0xcf, 0x0d, 0xd6, 0x2b, 0x0a, 0x9f, 0x80, 0x5e, 0x3f, 0x71, 0x68, 0x05, 0x86, 
    0xbb, 0x38, 0xce, 0x7f, 0xf3, 0x83, 0x03, 0x48, 0xd3, 0xfa, 0x35, 0xdf, 0x44, 0x7c, 0x82, 0x93, 
    0x53, 0xa3, 0x09, 0x6a, 0x77, 0x13, 0x55, 0xdd, 0xbe, 0x00, 0x9c, 0xb4, 0xe3, 0x9e, 0xbc, 0x96, 
    0xa7, 0xe2, 0x42, 0x4e, 0x37, 0x0e, 0xf6, 0x9d, 0x1b, 0x2a, 0x79, 0xbf, 0xdc, 0xa9, 0x88, 0x4c, 
    0xd0, 0xab, 0xc5, 0x69, 0xd1, 0xe1, 0xac, 0xe9, 0xc1, 0xb6, 0x7b, 0x57, 0x90, 0x07, 0x30, 0x92, 
    0xb7, 0x1e, 0xda, 0x49, 0x0c, 0xb8, 0xa4, 0xff, 0xb3, 0xf2, 0x64, 0x8a, 0xc7, 0x02, 0x6c, 0x08, 
    0x6f, 0xf7, 0x89, 0xc0, 0x10, 0xa8, 0x8d, 0xba, 0x91, 0x43, 0x6b, 0x3e, 0xd5, 0x65, 0xe0, 0x6e, 
    0x12, 0x2c, 0x94, 0x99, 0xe8, 0x1f, 0xc8, 0x11, 0x8e, 0xb1, 0xe7, 0x51, 0x1d, 0x97, 0x04, 0x19, 
};

const uint16_t q_mhp2g_key_default[4]  = {0x1709, 0x3df3, 0x747b, 0xb381};
const uint16_t q_mhp2g_key_modifier[4] = {0xff9d, 0xffa9, 0xffc7, 0xfff1};

const uint16_t q_mhp3_key_default[4]  = {0x3df3, 0x1709, 0xb381, 0x747b};
const uint16_t q_mhp3_key_modifier[4] = {0xffa9, 0xff9d, 0xfff1, 0xffc7};

// 142300 148B00

void mh_init_key(uint32_t* mh_key, uint32_t seed)
{
    // Initialize the XOR cipher key using a seed
    mh_key[0] = seed >> 16;
    if (mh_key[0] == 0)
        mh_key[0] = MH_KEY_DEF0;

    mh_key[1] = seed & 0xffff;
    if (mh_key[1] == 0)
        mh_key[1] = MH_KEY_DEF1;
}

uint32_t mh_next_key(uint32_t* mh_key)
{
    // Calculate a new XOR cipher key based on the previous key
    mh_key[0] *= MH_KEY_DEF0;
    mh_key[0] %= MH_KEY_MOD0;
    mh_key[1] *= MH_KEY_DEF1;
    mh_key[1] %= MH_KEY_MOD1;

    return ((mh_key[0] << 16) + mh_key[1]);
}

uint16_t quest_next_key(uint32_t* q_key, int i, const uint16_t* key_default, const uint16_t* key_modifier)
{
    // Calculate a new XOR cipher key based on the previous key
    q_key[i] *= key_default[i];
    q_key[i] %= key_modifier[i];
    q_key[i] &= 0xFFFF;

    return (q_key[i]);
}

void mh_buffer_translate(uint8_t* data, int len, const uint8_t* table)
{
    for(int i=0; i < len; i++)
        data[i] = table[data[i]];
}

/*
    Return a decrypted copy of the given data blocks.

    Arguments:
    buff -- Data blocks read from an encrypted DATA.BIN file
    lba -- Block address of the given data blacks
*/
void decrypt_block(uint8_t* data, int len, int lba, const uint8_t* dec_table)
{
    uint32_t keys[2];

    // Apply a substitution cipher to the data using the decode table
    mh_buffer_translate(data, len, dec_table);
    uint32_t* buff = (uint32_t*) data;
    len /= 4;

    // Use the block address to seed the XOR cipher key
    mh_init_key(keys, lba);

    // Apply an XOR cipher to the data using a new key every 4 bytes
    for (int i=0; i < len; i++)
        buff[i] ^= mh_next_key(keys);

    return;
}

/*
    Return an encrypted copy of the given data blocks.

    Arguments:
    buff -- Data blocks read from a decrypted DATA.BIN file
    lba -- Block address of the given data blocks
*/
void encrypt_block(uint8_t* data, int len, int lba, const uint8_t* enc_table)
{
    uint32_t keys[2];
    uint32_t* buff = (uint32_t*) data;

    // Use the block address to seed the XOR cipher key
    mh_init_key(keys, lba);

    // Apply an XOR cipher to the data using a new key every 4 bytes
    for (int i=0; i < len/4; i++)
        buff[i] ^= mh_next_key(keys);

    // Apply a substitution cipher to the data using the encode table
    mh_buffer_translate(data, len, enc_table);

    return;
}

/*
    Return a decrypted copy of the given save file data.

    Arguments:
    buff -- Data read from an encrypted save file
*/
void decrypt_data(u8* buff, u32 size, const char* salt, const uint8_t* dec_table)
{
    uint32_t seed;
    uint8_t sha[20], tmp[20];

    printf("[*] Total Decrypted Size: 0x%X (%d bytes)\n", size, size);

    // Get the XOR cipher seed from the end of the data and apply a
    // substitution cipher
    mh_buffer_translate(&buff[size-4], 4, dec_table);
    mh_buffer_translate(&buff[size-4], 4, dec_table);
    seed = *((uint32_t*) &buff[size-4]);
    
    printf("[*] Encryption Seed: %08X\n", seed);

    // Decrypted the data
    decrypt_block(buff, size-4, seed, dec_table);

    // Apply a substitution cipher to the data
    mh_buffer_translate(buff, size-4, dec_table);

    // Get the hash from the end of the data
    printf("[*] Stored SHA1: " SHA1_FMT(&buff[size-24], "\n"));
    printf("[*] SHA1 Calculation Length: %d bytes\n", size-16);

    // Hash the decrypted data with the salt and compare it
    memcpy(tmp, &buff[size-36], 20);
    memcpy(&buff[size-36], salt, 20);
    sha1(sha, buff, (size-16) * 8);
    memcpy(&buff[size-36], tmp, 20);

    if (memcmp(sha, &buff[size-24], 20) != 0)
        printf("Invalid SHA1 hash in header.\n");
    else
        printf("[*] Decrypted File Successfully!\n\n");

    return;
}

/*
    Return an encrypted copy of the given save file data.

    Arguments:
    buff -- Data read from a decrypted save file
*/
void encrypt_data(u8* buff, u32 size, const char* salt, const uint8_t* dec_table)
{
    uint32_t seed;
    uint8_t sha[20], tmp[20];
    uint8_t enc_table[256];

    printf("[*] Total Encrypted Size: 0x%X (%d bytes)\n", size, size);

    // Generate encoding table
    for (int i=0; i < 0x100; i++)
        enc_table[dec_table[i]] = i;

    // Hash the unencrypted data with the salt and append it to the data
    memcpy(tmp, &buff[size-36], 20);
    memcpy(&buff[size-36], salt, 20);
    sha1(sha, buff, (size-16) * 8);
    memcpy(&buff[size-36], tmp, 20);
    memcpy(&buff[size-24], sha, 20);

    printf("[*] Updated SHA1: " SHA1_FMT(&buff[size-24], "\n"));
    printf("[*] SHA1 Calculation Length: 0x%X\n", size-16);

    // Get a new seed for the XOR cipher
    seed = *((uint32_t*) &buff[size-4]);
    printf("[*] Encryption Seed: %08X\n", seed);

    // Apply a substitution cipher to the data and encrypt it
    mh_buffer_translate(buff, size-4, enc_table);
    encrypt_block(buff, size-4, seed, enc_table);

    // Apply a substitution cipher to the XOR cipher seed and append it to the data
    mh_buffer_translate(&buff[size-4], 4, enc_table);
    mh_buffer_translate(&buff[size-4], 4, enc_table);

    printf("[*] Encrypted File Successfully!\n\n");
    return;
}

/*
    Return a decrypted copy of the given quest file data.

    Arguments:
    buff -- Data read from an encrypted quest file
*/
void decrypt_quest(uint8_t* data, int len, const uint16_t* k_def, const uint16_t* k_mod, const char* salt)
{
    uint32_t keys[4];
    uint16_t* buff = (uint16_t*) data;

    // Get the XOR cipher seeds from the start of the data
    for (int i=0; i < 4; i++)
    {
        // Initialize the XOR cipher key using a seed
        keys[i] = buff[i] ? buff[i] : k_def[i];
    }

    // Apply an XOR cipher to the data using a new key every 2 bytes
    for (int i=4; i < len/2; i++)
        buff[i] ^= quest_next_key(keys, i%4, k_def, k_mod);

    // Get the size from the start of the data
    // Check the size to make sure it matches
    if (*(uint32_t*)(data+8) != len - 0x20)
        printf("[!] Invalid file size in Quest header.\n");

    // Hash the decrypted data with the salt and compare it
    uint8_t *tmp = malloc(len - 0x10);
    memcpy(tmp, data + 0x20, len - 0x20);
    memcpy(tmp + len - 0x20, salt, 0x10);
    sha1(tmp, tmp, (len-0x10) * 8);

    printf("[*] Stored SHA1: " SHA1_FMT(&data[12], "\n"));
    // Get the hash from the start of the data
    if (memcmp(data+12, tmp, 20) != 0)
        printf("[!] Invalid SHA1 hash in header.\n");
    else
        printf("[*] Decrypted Quest Successfully!\n\n");

    free(tmp);
}

/*
    Return an encrypted copy of the given quest file data.

    Arguments:
    buff -- Data read from a decrypted quest file
*/
void encrypt_quest(uint8_t* data, int len, const uint16_t* k_def, const uint16_t* k_mod, const char* salt)
{
    uint32_t keys[4];
    uint16_t* buff = (uint16_t*) data;

    // Add the size and hash to the start of the data
    *(uint32_t*)(data+8) = len - 0x20;

    // Hash the unencrypted data with the salt
    uint8_t *tmp = malloc(len - 0x10);
    memcpy(tmp, data + 0x20, len - 0x20);
    memcpy(tmp + len - 0x20, salt, 0x10);
    sha1(data+12, tmp, (len-0x10) * 8);
    free(tmp);

    printf("[*] Updated SHA1: " SHA1_FMT(&data[12], "\n"));
    printf("[*] SHA1 Calculation Length: 0x%X\n", len-16);

    // Get the XOR cipher seeds from the start of the data
    for (int i=0; i < 4; i++)
    {
        // Initialize the XOR cipher key using a seed
        keys[i] = buff[i] ? buff[i] : k_def[i];
    }

    // Apply an XOR cipher to the data using a new key every 2 bytes
    for (int i=4; i < len/2; i++)
        buff[i] ^= quest_next_key(keys, i%4, k_def, k_mod);

    printf("[*] Encrypted Quest Successfully!\n\n");
}

void print_usage(const char* argv0)
{
    printf("USAGE: %s [option] [game] filename\n\n", argv0);
    printf("OPTIONS        Explanation:\n");
    printf(" -d            Decrypt File\n");
    printf(" -e            Encrypt File\n\n");
    printf("GAME           Game/DLC Title:\n");
    printf("  1            Monster Hunter Freedom Unite (ULUS10391/ULES01213)\n");
    printf("  2            Monster Hunter Portable 2nd G (ULJM05500)\n");
    printf("  3            Monster Hunter Portable 3rd (ULJM05800)\n");
    printf("  4            Quests - MH Freedom Unite (ULUS10391/ULES01213)\n");
    printf("  5            Quests - MH Portable 2nd G (ULJM05500)\n");
    printf("  6            Quests - MH Portable 3rd (ULJM05800)\n\n");
    return;
}

int main(int argc, char **argv)
{
    size_t len;
    u8* data;
    char *opt, *bak;
    const char *hash_salt;
    const uint8_t *d_table;
    const uint16_t *qk_def, *qk_mod;

    printf("\nMonster Hunter PSP save decrypter 0.1.0 - (c) 2023 by Bucanero\n\n");

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

    switch (argv[2][0])
    {
        case '1':
            hash_salt = MHP2G_NA_HASH_SALT;
            d_table = mh2_dec_table;
            break;

        case '2':
            hash_salt = MHP2G_JP_HASH_SALT;
            d_table = mh2_dec_table;
            break;

        case '3':
            hash_salt = MHP3_JP_HASH_SALT;
            d_table = mh3_dec_table;
            break;

        case '4':
            hash_salt = QUEST_MHP2G_NA_HASH_SALT;
            qk_def = q_mhp2g_key_default;
            qk_mod = q_mhp2g_key_modifier;
            break;

        case '5':
            hash_salt = QUEST_MHP2G_JP_HASH_SALT;
            qk_def = q_mhp2g_key_default;
            qk_mod = q_mhp2g_key_modifier;
            break;

        case '6':
            hash_salt = QUEST_MHP3_JP_HASH_SALT;
            qk_def = q_mhp3_key_default;
            qk_mod = q_mhp3_key_modifier;
            break;

        default:
            printf("Invalid game selected.\n\n");
            return (-1);
    }

    if (read_buffer(argv[3], &data, &len) != 0)
    {
        printf("[*] Could Not Access The File (%s)\n", argv[2]);
        return -1;
    }

    // Save a file backup
    asprintf(&bak, "%s.bak", argv[3]);
    write_buffer(bak, data, len);

    if (*opt == 'd')
        if (argv[2][0] > '3')
            decrypt_quest(data, len, qk_def, qk_mod, hash_salt);
        else
            decrypt_data(data, len, hash_salt, d_table);
    else
        if (argv[2][0] > '3')
            encrypt_quest(data, len, qk_def, qk_mod, hash_salt);
        else
            encrypt_data(data, len, hash_salt, d_table);

    write_buffer(argv[3], data, len);

    free(bak);
    free(data);
    
    return 0;
}
