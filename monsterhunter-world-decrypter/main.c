/*
 *
 *    Monster Hunter World PS4 Save Decrypter - (c) 2026 by EnderHDMC
 *
 * This tool is based on https://github.com/alfizari/Monster-Hunter-World-Save-Decrypt-PS4
 *
 */

#include "iceborne_crypt.h"

#include "../common/iofile.c"

void print_usage(const char *argv0)
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
    const char *hash_salt;

    printf("\nMonster Hunter World PS4 save decrypter 0.1.0 - (c) 2026 by EnderHDMC\n\n");

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
        printf("Decrypting...");
        DecryptSavePS4(data, len);
    }
    else
    {
        printf("Encrypting...");
        EncryptSavePS4(data, len);
    }

    write_buffer(argv[2], data, len);

    free(bak);
    free(data);

    return 0;
}
