/*
*
*	Final Fantasy XIII (1/2/3) PS3 Save Decrypter - (c) 2021 by Bucanero - www.bucanero.com.ar
*
* This tool is based (reversed) on the original FFXIII Encryptor by Jappi88, and McDirge Save editor source by Shademp
*	https://thelifestream.net/forums/threads/dcffvii-research-thread.15951/page-42#post-884518
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

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

#define Read_u32_le(buf)    (*(u32*)(buf))
#define Write_u32_le(a, b)  memcpy(a, b, sizeof(uint32_t))
#define read_u64(bytes)     (*(u64*)(bytes))

#define FFXIII_KEY          0x1317fb09b9b42080L
#define FFXIII_2_KEY        0x9B1F01011A6438B0L
#define FFXIII_3_KEY        0x36545e6ceb9a705fL

#define FFXIII_MASK_1       0xFFFFFFFFFFFFFC00
#define FFXIII_MASK_2       0xFFFFFFFF00000000

// Xbox FFXIII_KEY          0x035ce4275cee246aL
// Xbox FFXIII_2_KEY        0x6920c2168106440fL
// Xbox FFXIII_3_KEY        0x09f0d55db8ea4bfdL

uint8_t KeyBlocksArray[32][8] = {0};

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
    free(data);

    return (key);
}

// keygen reversed from FFXIII Encryptor by Jappi88
void ff_init_key(u8* key_table, u64 ff_key)
{
	u32 init[2];

	ff_key = ((ff_key & 0xFF00000000000000) >> 16) | ((ff_key & 0x0000FF0000000000) << 16) | (ff_key & 0x00FF00FFFFFFFFFF);
	ff_key = ((ff_key & 0x00000000FF00FF00) >>  8) | ((ff_key & 0x0000000000FF00FF) <<  8) | (ff_key & 0xFFFFFFFF00000000);

	memcpy(key_table, &ff_key, 8);

	key_table[0] += 0x45;

	for (int j = 1; j < 8; j++)
	{
		init[0] = key_table[j-1] + key_table[j];
		init[1] = ((key_table[j-1] << 2) & 0x3FC);
		key_table[j] = ((((init[0] + 0xD4) ^ init[1]) & 0xFF) ^ 0x45);
	}

	ff_key = read_u64(key_table);

	for (int j = 1; j < 32; j++)
	{
		ff_key += (u64)((ff_key << 2) & 0xFFFFFFFFFFFFFFFC);
		memcpy(key_table + j*8, &ff_key, 8);
	}
}

u32 ff_xiii_checksum(u8* bytes, u32 len)
{
	u32 ff_csum = 0;
	len /= 4;

	while (len--)
	{
		ff_csum += bytes[0];
		bytes += 4;
	}

	return (ff_csum);
}

// decryption from McDirge by Shademp
void ff_decrypt_data(uint8_t *MemBlock, size_t size)
{
    ///DECODING THE ENCODED INFORMATION NOW IN MemBlock
    uint32_t ByteCounter = 0, BlockCounter = 0, KeyBlockCtr = 0;
    uint32_t Gear1 = 0, Gear2 = 0;
    uint32_t TBlockA = 0, TBlockB = 0, KBlockA = 0, KBlockB = 0;
    uint8_t CarryFlag1 = 0, CarryFlag2 = 0;
    uint8_t IntermediateCarrier = 0;
    uint8_t OldMemblockValue = 0;

    union u {
        uint64_t Cog64B;
        uint32_t Cog32BArray[2];
    } o;

    ///OUTERMOST LOOP OF THE DECODER
    for (; ByteCounter < size; BlockCounter++, KeyBlockCtr++)
    {
        if(KeyBlockCtr > 31)
            KeyBlockCtr = 0;

        o.Cog64B = (uint64_t)ByteCounter << 0x14;
        Gear1 = o.Cog32BArray[0] | (ByteCounter << 0x0A) | ByteCounter; ///Will this work badly when Gear1 becomes higher than 7FFFFFFF?

        CarryFlag1 = (Gear1 > ~0xA1652347) ? 1 : 0;

        Gear1 = Gear1 + 0xA1652347;
        Gear2 = (BlockCounter*2 | o.Cog32BArray[1]) + CarryFlag1;

        ///THE INNER LOOP OF THE DECODER
        for(int iterate = 0, BlockwiseByteCounter = 0; BlockwiseByteCounter < 8; )
        {
            if(iterate==0 && BlockwiseByteCounter==0)
            {
                OldMemblockValue = MemBlock[ByteCounter];
                MemBlock[ByteCounter] = 0x45 ^ BlockCounter ^ MemBlock[ByteCounter];
                iterate++;
            }
            else if(iterate==0 && BlockwiseByteCounter < 8)
            {
                IntermediateCarrier = MemBlock[ByteCounter] ^ OldMemblockValue;
                OldMemblockValue = MemBlock[ByteCounter];
                MemBlock[ByteCounter] = IntermediateCarrier;
                iterate++;
            }
            else if(iterate < 9 && BlockwiseByteCounter < 8)
            {
                MemBlock[ByteCounter] = 0x78 + MemBlock[ByteCounter] - KeyBlocksArray[KeyBlockCtr][iterate-1];
                iterate++;
            }
            else if(iterate==9)
            {
                iterate = 0;
                ByteCounter++;
                BlockwiseByteCounter++;
            }
        }
        ///EXITING THE INNER LOOP OF THE DECOER

	    ///RESUMING THE OUTER LOOP
	    ByteCounter -=8;
	
	    TBlockA = Read_u32_le(&MemBlock[ByteCounter]);
	    TBlockB = Read_u32_le(&MemBlock[ByteCounter+4]);
	
	    KBlockA = Read_u32_le(&KeyBlocksArray[KeyBlockCtr][0]);
	    KBlockB = Read_u32_le(&KeyBlocksArray[KeyBlockCtr][4]);
	
	    CarryFlag2 = (TBlockA < KBlockA) ? 1 : 0;

	    TBlockA = TBlockA - KBlockA;
	    TBlockB = TBlockB - KBlockB - CarryFlag2;
	
	    TBlockA = TBlockA ^ Gear1;
	    TBlockB = TBlockB ^ Gear2;
	
	    TBlockA = KBlockA ^ TBlockA;
	    TBlockB = KBlockB ^ TBlockB;

	    Write_u32_le(&MemBlock[ByteCounter],   &TBlockB);
	    Write_u32_le(&MemBlock[ByteCounter+4], &TBlockA);

	    ByteCounter +=8;
    }
    ///EXITING THE OUTER LOOP. FILE HAS NOW BEEN FULLY DECODED.
}

// encryption from McDirge by Shademp
void ff_encrypt_data(uint8_t *MemBlock, size_t size)
{
    ///ENCODE the file, now that all the changes have been made and the checksum has been updated.
    uint32_t ByteCounter = 0, BlockCounter = 0, KeyBlockCtr = 0;
    uint32_t Gear1 = 0, Gear2 = 0;
    uint32_t TBlockA = 0, TBlockB = 0, KBlockA = 0, KBlockB = 0;
    uint8_t CarryFlag1 = 0, CarryFlag2 = 0;
    uint8_t OldMemblockValue = 0;

    union u {
        uint64_t Cog64B;
        uint32_t Cog32BArray[2];
    } o;

    ///OUTERMOST LOOP OF THE ENCODER
    for (; ByteCounter < size; BlockCounter++, KeyBlockCtr++)
    {
        if(KeyBlockCtr > 31)
            KeyBlockCtr = 0;

        o.Cog64B = (uint64_t)ByteCounter << 0x14;

        Gear1 = o.Cog32BArray[0] | (ByteCounter << 0x0A) | ByteCounter; ///Will this work badly when Gear1 becomes higher than 7FFFFFFF?

        CarryFlag1 = (Gear1 > ~0xA1652347) ? 1 : 0;

	    Gear1 = Gear1 + 0xA1652347;
	    Gear2 = (BlockCounter*2 | o.Cog32BArray[1]) + CarryFlag1;
	
	    TBlockB = Read_u32_le(&MemBlock[ByteCounter]);
	    TBlockA = Read_u32_le(&MemBlock[ByteCounter+4]);

	    KBlockA = Read_u32_le(&KeyBlocksArray[KeyBlockCtr][0]);
	    KBlockB = Read_u32_le(&KeyBlocksArray[KeyBlockCtr][4]);
	
	    TBlockB = KBlockB ^ TBlockB;
	    TBlockA = KBlockA ^ TBlockA;
	
	    TBlockB = TBlockB ^ Gear2;
	    TBlockA = TBlockA ^ Gear1;
	
	    ///Reverse of TBlockA < KBlockA from the Decoder.
	    CarryFlag2 = (TBlockA > ~KBlockA) ? 1 : 0;

	    TBlockB = TBlockB + KBlockB + CarryFlag2;       ///Reversed from subtraction to addition.
	    TBlockA = TBlockA + KBlockA;                    ///Reversed from subtraction to addition.
	
	    Write_u32_le(&MemBlock[ByteCounter],   &TBlockA);
	    Write_u32_le(&MemBlock[ByteCounter+4], &TBlockB);

        ///INNER LOOP OF ENCODER
        for(int iterate = 8, BlockwiseByteCounter = 0; BlockwiseByteCounter < 8; )
        {
            if(iterate != 0 && BlockwiseByteCounter < 8)
            {
                MemBlock[ByteCounter] = MemBlock[ByteCounter] + KeyBlocksArray[KeyBlockCtr][iterate-1] - 0x78;
                iterate--;
            }
            else if(iterate == 0 && BlockwiseByteCounter==0)
            {
                MemBlock[ByteCounter] = 0x45 ^ BlockCounter ^ MemBlock[ByteCounter];
                OldMemblockValue = MemBlock[ByteCounter];
                BlockwiseByteCounter++;
                ByteCounter++;
                iterate = 8;
            }
            else if(iterate == 0 && BlockwiseByteCounter < 8)
            {
                MemBlock[ByteCounter] = MemBlock[ByteCounter] ^ OldMemblockValue;
                OldMemblockValue = MemBlock[ByteCounter];
                iterate = 8;
                BlockwiseByteCounter++;
                ByteCounter++;
            }
        }
        ///EXITING INNER LOOP OF ENCODER
    }
    ///EXITING OUTER LOOP
    ///ENCODING FINISHED
}

void decrypt_data(u8* data, u32 size, u64 key)
{
	u32 csum, ff_csum;

	printf("[*] Total Decrypted Size Is 0x%X (%d bytes)\n", size, size);

	ff_init_key(&KeyBlocksArray[0][0], key);
	ff_decrypt_data(data, size);

//	if (ffgame == 3 && size > 547904)
//		size = 547904;

	ff_csum = ff_xiii_checksum(data, size - 8);
	csum = *(u32*)(data + size - 4);

	if (csum != ff_csum)
		printf("[!] Decrypted data did not pass file integrity check. (Expected: %08X Got: %08X)\n", csum, ff_csum);

	printf("[*] Decrypted File Successfully!\n\n");
	return;
}

void encrypt_data(u8* data, u32 size, u64 key)
{
	u32 csum;

	printf("[*] Total Encrypted Size Is 0x%X (%d bytes)\n", size, size);

	csum = ff_xiii_checksum(data, size - 8);
	memcpy(data + size - 4, &csum, 4);

	ff_init_key(&KeyBlocksArray[0][0], key);
	ff_encrypt_data(data, size);

//	if (ffgame == 3 && size == 547904)
//		Array.Resize<byte>(ref array3, 560736);

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

	write_buffer(argv[3], data, len);

	free(bak);
	free(data);
	
	return 0;
}
