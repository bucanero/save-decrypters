#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// custom AES-CBC-256, IV = {0}
// modified KeyExpansion (endian-swap)
#define CBC 1
#include "custom_aes.c"

unsigned char la_noire_save_aes_key[32] = "Wr9uFi4yi*?OESwiavv$ayIAp+u23PIe";
unsigned char la_noire_profile_aes_key[32] = "_!pH4ThU-7N?u&eph4$eaC!aTHaQ5U7u";

unsigned char la_noire_save_aes_key_xbox[32] = "FcHoaCrouPrI-b8EP!e$iURlE#r5e&@?";
unsigned char la_noire_profile_aes_key_xbox[32] = "re$Uph$t7vECufEjey4_he3e2ajuwref";


void la_noire_decrypt_data(unsigned char* data, unsigned int len)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, la_noire_save_aes_key);
	memset(ctx.Iv, 0, AES_BLOCKLEN);

	AES_CBC_decrypt_buffer(&ctx, data, len);
}

void la_noire_encrypt_data(unsigned char* data, unsigned int len)
{
	struct AES_ctx ctx;
	AES_init_ctx(&ctx, la_noire_save_aes_key);
	memset(ctx.Iv, 0, AES_BLOCKLEN);

	AES_CBC_encrypt_buffer(&ctx, data, len);
}

void show_usage(void)
{
	printf("USAGE: la_noire_save_decrypter [option] filename\n");
	printf("\n");
	printf("OPTIONS        Explanation:\n");
	printf(" -d            Decrypt File\n");
	printf(" -e            Encrypt File\n");
}

int is_valid_option(char *option)
{
	if (strcmp(option, "-d") == 0 || strcmp(option, "-e") == 0) {
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	printf("\n\nla_noire_save_decrypter 0.1.0 (c) 2014 by Red-EyeX32\n\n");

	if (argc != 3 || !is_valid_option(argv[1])) {
		show_usage();
		return -1;
	}

	FILE* fp = fopen(argv[2], "rb+");

	if (fp == NULL) {
		printf("[*] Could Not Access The File");
		return -2;
	}

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);

	unsigned char* fileData = (unsigned char*)malloc(fileSize);
	fseek(fp, 0, SEEK_SET);
	fread(fileData, fileSize, 1, fp);

	if (strcmp(argv[1], "-d") == 0)
	{
		printf("[*] Total Decrypted Size Is 0x%X\n", fileSize);
		
		la_noire_decrypt_data(fileData, fileSize);

		fseek(fp, 0, SEEK_SET);
		fwrite(fileData, 1, fileSize, fp);
		fclose(fp);
		printf("Decrypted\n");
	}
	else if (strcmp(argv[1], "-e") == 0)
	{
		printf("[*] Total Encrypted Size Is 0x%X\n", fileSize);

		la_noire_encrypt_data(fileData, fileSize);

		fseek(fp, 0, SEEK_SET);
		fwrite(fileData, 1, fileSize, fp);
		fclose(fp);
		printf("Encrypted\n");
	}

	return 0;
}
