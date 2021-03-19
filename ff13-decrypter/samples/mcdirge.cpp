///MCDIRGE: VERSION 0.3, COMMAND LINE EDITION
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


///GLOBAL VARIABLES

///DECLARING THE KEY BLOCKS IN GLOBAL SPACE TO BE AVAILABLE EVERYWHERE
uint8_t KeyBlocksArray[32][8] =
{
{0x45, 0x48, 0x79, 0xEC, 0x35, 0x82, 0x1B, 0xC6},
{0x59, 0x69, 0x5E, 0x9E, 0x0D, 0x8B, 0x89, 0xDE},
{0xBD, 0x0E, 0xD8, 0x17, 0x44, 0xB7, 0xAF, 0x58},
{0xB1, 0x49, 0x38, 0x77, 0x54, 0x94, 0x6E, 0xBB},
{0x75, 0x70, 0x19, 0x54, 0xA6, 0xE5, 0x28, 0xA9},
{0x49, 0x32, 0x7F, 0xA4, 0x3F, 0x7C, 0xCC, 0x4D},
{0x6D, 0xFB, 0x7B, 0x36, 0x3E, 0x6D, 0xFE, 0x84},
{0x21, 0xE9, 0x6B, 0x10, 0x37, 0x22, 0xF8, 0x98},
{0xA5, 0x8D, 0x1B, 0x52, 0x13, 0xAB, 0xD8, 0xFC},
{0x39, 0xC4, 0x89, 0x9A, 0x60, 0x57, 0x3B, 0xF0},
{0x1D, 0xD5, 0xB0, 0x04, 0xE3, 0xB4, 0x28, 0xB1},
{0x91, 0x29, 0x74, 0x17, 0x6F, 0x88, 0xCB, 0x75},
{0xD5, 0xCF, 0x44, 0x75, 0x2B, 0xAA, 0xF9, 0x4C},
{0x29, 0x0F, 0x58, 0x4A, 0xD9, 0x52, 0xE0, 0x80},
{0xCD, 0x4B, 0xB8, 0x73, 0x3E, 0x9E, 0x61, 0x84},
{0x01, 0x7B, 0x99, 0x42, 0x38, 0x17, 0xE8, 0x95},
{0x05, 0x67, 0xFF, 0x4C, 0x19, 0x74, 0x88, 0xED},
{0x19, 0x03, 0xFD, 0x80, 0x7E, 0x44, 0xAA, 0xA3},
{0x7D, 0x0F, 0xF1, 0x84, 0x78, 0x56, 0x53, 0x32},
{0x71, 0x4D, 0xB5, 0x98, 0x5A, 0xB0, 0xA0, 0xFB},
{0x35, 0x83, 0x8A, 0xFB, 0xC4, 0x71, 0x23, 0xEA},
{0x09, 0x90, 0xB4, 0xE9, 0xD8, 0x38, 0xB1, 0x92},
{0x2D, 0xD0, 0x86, 0x90, 0x3C, 0x1C, 0x76, 0xDD},
{0xE1, 0x10, 0xA2, 0xD2, 0x2E, 0x8D, 0x4E, 0x53},
{0x65, 0x54, 0x2A, 0x1D, 0xEA, 0xC1, 0x88, 0xA0},
{0xF9, 0xA5, 0xD3, 0x91, 0x92, 0xC9, 0xAB, 0x22},
{0xDD, 0x3D, 0x22, 0xD9, 0xDC, 0xEF, 0x5A, 0xAD},
{0x51, 0x35, 0xAB, 0x3D, 0x50, 0xAF, 0xC6, 0x62},
{0x95, 0x0A, 0x58, 0x34, 0x91, 0x6C, 0xE1, 0xED},
{0xE9, 0x34, 0xB8, 0x05, 0xD6, 0x1E, 0x67, 0xA5},
{0x8D, 0x08, 0x99, 0x1C, 0x2E, 0x9A, 0x03, 0x3B},
{0xC1, 0x2A, 0xFD, 0x8E, 0xE6, 0x02, 0x12, 0x27},
};

string userinput;
int numeric_userinput;
string filename;
string filename_decryptedfile = filename + "_decrypted";
streampos size;
bool scenefileselected(0);

uint32_t address;
uint32_t addressvalue;


///"General Settings" variables and strings
bool tempsave; //0="Not Available", 1="Available". Restore most recent tempsave by setting to 1.
bool secret_reset; //0=On, 1=Off. Secret option that sets General Settings and Controller Settings back to default when the memory card save loads in-game.
bool vibration; //0=On, 1=Off.
int8_t sight_speed; //Slowest=192, Fastest=64. 50% speed = 128.
int8_t camera_speed; //Slowest=192, Fastest=64. 50% speed = 128.
int8_t mouse_speed; //Slowest = 240. Fastest = 32. 50% speed = 136.
bool camera_vertical; //0=Inverted, 1=Normal.
bool camera_horizontal; //0=Normal, 1=Inverted.
bool sight_vertical; //0=Inverted, 1=Normal.
bool sight_horizontal; //0=Normal, 1=Inverted.
bool sound_output; //0=Dolby Pro Logic II, 1=Monaural.
bool subtitles; //0=On, 1=Off
bool speakers_name; //0=On, 1=Off
bool keyboard_usage; //0=Chat Only, 1=General Controls. *May only be selected naturally in the Japanese original.
uint16_t gamma_adjustment; //20=Darkest, 180=Brightest.
uint16_t sight_support; //0=Manual, 1=Semi-Automatic, 2=Automatic. *This option does not exist in the Japanese original.
int8_t screenpos_x; //*Functionality limited to the PAL version. Signed so to allow negative values. What are the natural value ranges? -32 to 32?
int8_t screenpos_y; //*Functionality limited to the PAL version. Signed so to allow negative values. What are the natural value ranges? -32 to 32?

string tempsave_setting;
string secret_reset_setting;
string vibration_setting;
string camera_vertical_setting;
string camera_horizontal_setting;
string sight_vertical_setting;
string sight_horizontal_setting;
string sound_output_setting;
string subtitles_setting;
string speakers_name_setting;
string keyboard_usage_setting;
string sight_support_setting;


///Prototyping smaller functions.
uint32_t LittleEndianToBigEndian (char Byte1, char Byte2, char Byte3, char Byte4);
uint32_t LittleEndianToBigEndian_unsignedchar (uint8_t Byte1, uint8_t Byte2, uint8_t Byte3, uint8_t Byte4);


void deletedecodedfiles()
{
    remove("BISLPM-66271000_decrypted");
    remove("BISLPM-66271001_decrypted");
    remove("BASLUS-21419000_decrypted");
    remove("BASLUS-21419001_decrypted");
    remove("BESLES-54185000_decrypted");
    remove("BESLES-54185001_decrypted");
    remove("BISLPM-66629000_decrypted");
    remove("BISLPM-66629001_decrypted");
    remove("cfg000_decrypted");
    remove("scene000_decrypted");
    remove("scene001_decrypted");
    remove("scene002_decrypted");
    remove("scene003_decrypted");
    remove("scene004_decrypted");
    remove("scene005_decrypted");
    remove("scene006_decrypted");
    remove("scene007_decrypted");
    remove("scene008_decrypted");
    remove("scene009_decrypted");
    remove("scene010_decrypted");
    remove("scr000_decrypted");
}


void opendecodeandcopyfile(string filename) ///For non-scene0XX files
{

///OPENING ENCODED FILE AND COPYING CONTENTS INTO EncodeToDecodeMemBlock
    char *EncodeToDecodeMemBlock;
    ifstream ReadingEncodedFile;
    ReadingEncodedFile.open (filename, ios::in | ios::binary | ios::ate);
    size = ReadingEncodedFile.tellg();
    ReadingEncodedFile.seekg(0, ios::beg);
    EncodeToDecodeMemBlock = new char [size];
    ReadingEncodedFile.read (EncodeToDecodeMemBlock, size);
    ReadingEncodedFile.close();
///CONTENTS ARE NOW COPIED ONTO EncodeToDecodeMemBlock

///DECODING THE ENCODED INFORMATION NOW IN EncodeToDecodeMemBlock
    uint32_t ByteCounter(0), BlockCounter(0), KeyBlockCtr(0);
    uint32_t Gear1(0), Gear2(0);
    uint32_t TBlockA(0), TBlockB(0), KBlockA(0), KBlockB(0);
    bool CarryFlag1(0), CarryFlag2(0);
    uint8_t IntermediateCarrier(0);
    uint8_t OldMemblockValue(0);

    union u {
    uint64_t Cog64B;
    uint32_t Cog32BArray[2];
    }o;

///OUTERMOST LOOP OF THE DECODER
    for ( ; ByteCounter < size; BlockCounter++, KeyBlockCtr++)
        {
            if(KeyBlockCtr > 31)
                KeyBlockCtr = 0;

        o.Cog64B = (uint64_t)ByteCounter << 0x14;
        Gear1 = o.Cog32BArray[0] | (ByteCounter << 0x0A) | ByteCounter; ///Will this work badly when Gear1 becomes higher than 7FFFFFFF?
            if (Gear1 > ~0xA1652347)
                CarryFlag1 = 1;
            else
                CarryFlag1 = 0;
        Gear1 = Gear1 + 0xA1652347;
        Gear2 = (BlockCounter*2 | o.Cog32BArray[1]) + CarryFlag1;

///THE INNER LOOP OF THE DECODER
                for(int iterate(0), BlockwiseByteCounter(0); BlockwiseByteCounter < 8; )
                {
                    if(iterate==0 && BlockwiseByteCounter==0)
                        {
                        OldMemblockValue = EncodeToDecodeMemBlock[ByteCounter];
                        EncodeToDecodeMemBlock[ByteCounter] = 0x45 ^ BlockCounter ^ EncodeToDecodeMemBlock[ByteCounter];
                        iterate++;
                        }
                    else if(iterate==0 && BlockwiseByteCounter < 8)
                        {
                        IntermediateCarrier = EncodeToDecodeMemBlock[ByteCounter] ^ OldMemblockValue;
                        OldMemblockValue = EncodeToDecodeMemBlock[ByteCounter];
                        EncodeToDecodeMemBlock[ByteCounter] = IntermediateCarrier;
                        iterate++;
                        }
                    else if(iterate < 9 && BlockwiseByteCounter < 8)
                        {EncodeToDecodeMemBlock[ByteCounter] = 0x78 + EncodeToDecodeMemBlock[ByteCounter] - KeyBlocksArray[KeyBlockCtr][iterate-1];
                        iterate++;}
                    else if(iterate==9)
                        {iterate = 0;
                        ByteCounter++;
                        BlockwiseByteCounter++;}
                }
///EXITING THE INNER LOOP OF THE DECOER


///RESUMING THE OUTER LOOP
    ByteCounter -=8;

    TBlockA = LittleEndianToBigEndian (EncodeToDecodeMemBlock[ByteCounter], EncodeToDecodeMemBlock[ByteCounter+1], EncodeToDecodeMemBlock[ByteCounter+2], EncodeToDecodeMemBlock[ByteCounter+3]);
    TBlockB = LittleEndianToBigEndian (EncodeToDecodeMemBlock[ByteCounter+4], EncodeToDecodeMemBlock[ByteCounter+5], EncodeToDecodeMemBlock[ByteCounter+6], EncodeToDecodeMemBlock[ByteCounter+7]);

    KBlockA = LittleEndianToBigEndian_unsignedchar (KeyBlocksArray[KeyBlockCtr][0], KeyBlocksArray[KeyBlockCtr][1], KeyBlocksArray[KeyBlockCtr][2], KeyBlocksArray[KeyBlockCtr][3]);
    KBlockB = LittleEndianToBigEndian_unsignedchar (KeyBlocksArray[KeyBlockCtr][4], KeyBlocksArray[KeyBlockCtr][5], KeyBlocksArray[KeyBlockCtr][6], KeyBlocksArray[KeyBlockCtr][7]);

    if (TBlockA < KBlockA)
        CarryFlag2 = 1;
    else
        CarryFlag2 = 0;
    TBlockA = TBlockA - KBlockA;
    TBlockB = TBlockB - KBlockB - CarryFlag2;

    TBlockA = TBlockA ^ Gear1;
    TBlockB = TBlockB ^ Gear2;

    TBlockA = KBlockA ^ TBlockA;
    TBlockB = KBlockB ^ TBlockB;

    EncodeToDecodeMemBlock[ByteCounter] =   (TBlockB & 0x000000FF);
    EncodeToDecodeMemBlock[ByteCounter+1] = (TBlockB & 0x0000FF00) >> 8;
    EncodeToDecodeMemBlock[ByteCounter+2] = (TBlockB & 0x00FF0000) >> 16;
    EncodeToDecodeMemBlock[ByteCounter+3] = (TBlockB & 0xFF000000) >> 24;
    EncodeToDecodeMemBlock[ByteCounter+4] = (TBlockA & 0x000000FF);
    EncodeToDecodeMemBlock[ByteCounter+5] = (TBlockA & 0x0000FF00) >> 8;
    EncodeToDecodeMemBlock[ByteCounter+6] = (TBlockA & 0x00FF0000) >> 16;
    EncodeToDecodeMemBlock[ByteCounter+7] = (TBlockA & 0xFF000000) >> 24;

    ByteCounter +=8;
    }
///EXITING THE OUTER LOOP. FILE HAS NOW BEEN FULLY DECODED.

///CREATING A FILE COPY OF THE DECODED INFORMATION FOR USE IN OTHER FUNCTIONS
    filename_decryptedfile = filename + "_decrypted";
    ofstream CopyingDecodedFile;
    CopyingDecodedFile.open (filename_decryptedfile, ios::out | ios::trunc | ios::binary);
    CopyingDecodedFile.write (EncodeToDecodeMemBlock, size);
    CopyingDecodedFile.close();
    delete[] EncodeToDecodeMemBlock;
}


void changeaddressvalue(int address, int newvalue) ///For encoding changes into non-scene0XX files
{
///OPENING THE DECODED FILE AND COPYING IT INTO MEMBLOCK
    char *DecodeToEncodeMemBlock;
    ifstream ReadingDecodedFile;
    ReadingDecodedFile.open (filename_decryptedfile, ios::in | ios::binary | ios::ate);
    size = ReadingDecodedFile.tellg();
    ReadingDecodedFile.seekg(0, ios::beg);
    DecodeToEncodeMemBlock = new char [size];
    ReadingDecodedFile.read (DecodeToEncodeMemBlock, size);
    ReadingDecodedFile.close();
///THE DECODED DATA IS NOW IN DecodeToEncodeMemBlock
///CHANGING THE VALUE OF AN ADDRESS
    DecodeToEncodeMemBlock[address] = newvalue;
    ///VALUE CHANGED

///CHECKSUM CALCULATOR
    uint32_t Checksum(0), MemBlockConverter(0);

    for (int iterate(0); iterate < (size-8); iterate+=4)
    {
        MemBlockConverter = DecodeToEncodeMemBlock[iterate];
        if(MemBlockConverter > 0x7F)
            MemBlockConverter = MemBlockConverter & 0x000000FF;
        Checksum = Checksum + MemBlockConverter;
    }
///CHECKSUM CALCULATED


///INSERTING CHECKSUM INTO END PORTION OF FILE
    DecodeToEncodeMemBlock[size-4] = (Checksum & 0x000000FF);
    DecodeToEncodeMemBlock[size-3] = (Checksum & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[size-2] = (Checksum & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[size-1] = (Checksum & 0xFF000000) >> 24;
///CHECKSUM INSERTED


///ENCODE the file, now that all the changes have been made and the checksum has been updated.
    uint32_t ByteCounter(0), BlockCounter(0), KeyBlockCtr(0);
    uint32_t Gear1(0), Gear2(0);
    uint32_t TBlockA(0), TBlockB(0), KBlockA(0), KBlockB(0);
    bool CarryFlag1(0), CarryFlag2(0);
    uint8_t OldMemblockValue(0);

    union u {
    uint64_t Cog64B;
    uint32_t Cog32BArray[2];
    }o;

///OUTERMOST LOOP OF THE ENCODER
for ( ; ByteCounter < size; BlockCounter++, KeyBlockCtr++)
    {


        if(KeyBlockCtr > 31)
            KeyBlockCtr = 0;

    o.Cog64B = (uint64_t)ByteCounter << 0x14;



    Gear1 = o.Cog32BArray[0] | (ByteCounter << 0x0A) | ByteCounter; ///Will this work badly when Gear1 becomes higher than 7FFFFFFF?
        if (Gear1 > ~0xA1652347)
            CarryFlag1 = 1;
        else
            CarryFlag1 = 0;
    Gear1 = Gear1 + 0xA1652347;
    Gear2 = (BlockCounter*2 | o.Cog32BArray[1]) + CarryFlag1;

    TBlockB = LittleEndianToBigEndian (DecodeToEncodeMemBlock[ByteCounter], DecodeToEncodeMemBlock[ByteCounter+1], DecodeToEncodeMemBlock[ByteCounter+2], DecodeToEncodeMemBlock[ByteCounter+3]);
    TBlockA = LittleEndianToBigEndian (DecodeToEncodeMemBlock[ByteCounter+4], DecodeToEncodeMemBlock[ByteCounter+5], DecodeToEncodeMemBlock[ByteCounter+6], DecodeToEncodeMemBlock[ByteCounter+7]);

    KBlockA = LittleEndianToBigEndian_unsignedchar (KeyBlocksArray[KeyBlockCtr][0], KeyBlocksArray[KeyBlockCtr][1], KeyBlocksArray[KeyBlockCtr][2], KeyBlocksArray[KeyBlockCtr][3]);
    KBlockB = LittleEndianToBigEndian_unsignedchar (KeyBlocksArray[KeyBlockCtr][4], KeyBlocksArray[KeyBlockCtr][5], KeyBlocksArray[KeyBlockCtr][6], KeyBlocksArray[KeyBlockCtr][7]);

    TBlockB = KBlockB ^ TBlockB;
    TBlockA = KBlockA ^ TBlockA;

    TBlockB = TBlockB ^ Gear2;
    TBlockA = TBlockA ^ Gear1;

    if (TBlockA > ~KBlockA)  ///Reverse of TBlockA < KBlockA from the Decoder.
        CarryFlag2 = 1;
    else
        CarryFlag2 = 0;

    TBlockB = TBlockB + KBlockB + CarryFlag2;       ///Reversed from subtraction to addition.
    TBlockA = TBlockA + KBlockA;                    ///Reversed from subtraction to addition.

    DecodeToEncodeMemBlock[ByteCounter] =   (TBlockA & 0x000000FF);
    DecodeToEncodeMemBlock[ByteCounter+1] = (TBlockA & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[ByteCounter+2] = (TBlockA & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[ByteCounter+3] = (TBlockA & 0xFF000000) >> 24;
    DecodeToEncodeMemBlock[ByteCounter+4] = (TBlockB & 0x000000FF);
    DecodeToEncodeMemBlock[ByteCounter+5] = (TBlockB & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[ByteCounter+6] = (TBlockB & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[ByteCounter+7] = (TBlockB & 0xFF000000) >> 24;


///INNER LOOP OF ENCODER
            for(int iterate(8), BlockwiseByteCounter(0); BlockwiseByteCounter < 8; )
            {
                if(iterate != 0 && BlockwiseByteCounter < 8)
                    {DecodeToEncodeMemBlock[ByteCounter] = DecodeToEncodeMemBlock[ByteCounter] + KeyBlocksArray[KeyBlockCtr][iterate-1] - 0x78;
                    iterate--;}
                else if(iterate == 0 && BlockwiseByteCounter==0)
                    {
                    DecodeToEncodeMemBlock[ByteCounter] = 0x45 ^ BlockCounter ^ DecodeToEncodeMemBlock[ByteCounter];
                    OldMemblockValue = DecodeToEncodeMemBlock[ByteCounter];
                    BlockwiseByteCounter++;
                    ByteCounter++;
                    iterate = 8;
                    }
                else if(iterate == 0 && BlockwiseByteCounter < 8)
                    {
                    DecodeToEncodeMemBlock[ByteCounter] = DecodeToEncodeMemBlock[ByteCounter] ^ OldMemblockValue;
                    OldMemblockValue = DecodeToEncodeMemBlock[ByteCounter];
                    iterate = 8;
                    BlockwiseByteCounter++;
                    ByteCounter++;
                    }

            }
///EXITING INNER LOOP OF ENCODER
    }
///EXITING OUTER LOOP
///ENCODING FINISHED

///ENTERING RE-ENCODED DATA INTO THE FILE
    ofstream SavingEncodedFile;
    SavingEncodedFile.open (filename, ios::out | ios::trunc | ios::binary);
    SavingEncodedFile.write (DecodeToEncodeMemBlock, size);
    SavingEncodedFile.close();
    delete[] DecodeToEncodeMemBlock;
///FINISHED ENTERING RE-ENCODED DATA INTO THE FILE

}


void opendecodeandcopyscenefile(string filename) ///For scene0XX files
{

///OPENING ENCODED FILE AND COPYING CONTENTS INTO EncodeToDecodeMemBlock
    char *EncodeToDecodeMemBlock;

    ifstream ReadingEncodedFile;
    ReadingEncodedFile.open (filename, ios::in | ios::binary | ios::ate);
    size = ReadingEncodedFile.tellg();
    ReadingEncodedFile.seekg(0, ios::beg);
    EncodeToDecodeMemBlock = new char [size];
    ReadingEncodedFile.read (EncodeToDecodeMemBlock, size);
    ReadingEncodedFile.close();
///CONTENTS ARE NOW COPIED ONTO EncodeToDecodeMemBlock

///DECODING THE ENCODED INFORMATION NOW IN EncodeToDecodeMemBlock
    uint32_t ByteCounter(0), BlockCounter(0), KeyBlockCtr(0);
    uint32_t Gear1(0), Gear2(0);
    uint32_t TBlockA(0), TBlockB(0), KBlockA(0), KBlockB(0);
    bool CarryFlag1(0), CarryFlag2(0);
    uint8_t IntermediateCarrier(0);
    uint8_t OldMemblockValue(0);

    union u {
    uint64_t Cog64B;
    uint32_t Cog32BArray[2];
    }o;

    uint32_t RegionByteCounter(0);


///OUTERMOST LOOP OF THE DECODER

for ( ; ByteCounter < size; BlockCounter++, KeyBlockCtr++)
    {

    if(RegionByteCounter == 0x948)
        {RegionByteCounter = 0;
        BlockCounter = 0;
        KeyBlockCtr = 0;}

        if(KeyBlockCtr > 31)
            KeyBlockCtr = 0;

    o.Cog64B = (uint64_t)RegionByteCounter << 0x14;

    Gear1 = o.Cog32BArray[0] | (RegionByteCounter << 0x0A) | RegionByteCounter; ///Will this work badly when Gear1 becomes higher than 7FFFFFFF?

        if (Gear1 > ~0xA1652347)
            CarryFlag1 = 1;
        else
            CarryFlag1 = 0;
    Gear1 = Gear1 + 0xA1652347;
    Gear2 = (BlockCounter*2 | o.Cog32BArray[1]) + CarryFlag1;


///THE INNER LOOP OF THE DECODER
                for(int iterate(0), BlockwiseByteCounter(0); BlockwiseByteCounter < 8; )
                {
                    if(iterate==0 && BlockwiseByteCounter==0)
                        {
                        OldMemblockValue = EncodeToDecodeMemBlock[ByteCounter];
                        EncodeToDecodeMemBlock[ByteCounter] = 0x45 ^ BlockCounter ^ EncodeToDecodeMemBlock[ByteCounter];
                        iterate++;
                        }
                    else if(iterate==0 && BlockwiseByteCounter < 8)
                        {
                        IntermediateCarrier = EncodeToDecodeMemBlock[ByteCounter] ^ OldMemblockValue;
                        OldMemblockValue = EncodeToDecodeMemBlock[ByteCounter];
                        EncodeToDecodeMemBlock[ByteCounter] = IntermediateCarrier;
                        iterate++;
                        }
                    else if(iterate < 9 && BlockwiseByteCounter < 8)
                        {EncodeToDecodeMemBlock[ByteCounter] = 0x78 + EncodeToDecodeMemBlock[ByteCounter] - KeyBlocksArray[KeyBlockCtr][iterate-1];
                        iterate++;}
                    else if(iterate==9)
                        {iterate = 0;
                        ByteCounter++;
                        BlockwiseByteCounter++;}
                }
///EXITING THE INNER LOOP OF THE DECOER


///RESUMING THE OUTER LOOP
    ByteCounter -=8;

    TBlockA = LittleEndianToBigEndian (EncodeToDecodeMemBlock[ByteCounter], EncodeToDecodeMemBlock[ByteCounter+1], EncodeToDecodeMemBlock[ByteCounter+2], EncodeToDecodeMemBlock[ByteCounter+3]);
    TBlockB = LittleEndianToBigEndian (EncodeToDecodeMemBlock[ByteCounter+4], EncodeToDecodeMemBlock[ByteCounter+5], EncodeToDecodeMemBlock[ByteCounter+6], EncodeToDecodeMemBlock[ByteCounter+7]);

    KBlockA = LittleEndianToBigEndian_unsignedchar (KeyBlocksArray[KeyBlockCtr][0], KeyBlocksArray[KeyBlockCtr][1], KeyBlocksArray[KeyBlockCtr][2], KeyBlocksArray[KeyBlockCtr][3]);
    KBlockB = LittleEndianToBigEndian_unsignedchar (KeyBlocksArray[KeyBlockCtr][4], KeyBlocksArray[KeyBlockCtr][5], KeyBlocksArray[KeyBlockCtr][6], KeyBlocksArray[KeyBlockCtr][7]);

    if (TBlockA < KBlockA)
        CarryFlag2 = 1;
    else
        CarryFlag2 = 0;
    TBlockA = TBlockA - KBlockA;
    TBlockB = TBlockB - KBlockB - CarryFlag2;

    TBlockA = TBlockA ^ Gear1;
    TBlockB = TBlockB ^ Gear2;

    TBlockA = KBlockA ^ TBlockA;
    TBlockB = KBlockB ^ TBlockB;

    EncodeToDecodeMemBlock[ByteCounter] =   (TBlockB & 0x000000FF);
    EncodeToDecodeMemBlock[ByteCounter+1] = (TBlockB & 0x0000FF00) >> 8;
    EncodeToDecodeMemBlock[ByteCounter+2] = (TBlockB & 0x00FF0000) >> 16;
    EncodeToDecodeMemBlock[ByteCounter+3] = (TBlockB & 0xFF000000) >> 24;
    EncodeToDecodeMemBlock[ByteCounter+4] = (TBlockA & 0x000000FF);
    EncodeToDecodeMemBlock[ByteCounter+5] = (TBlockA & 0x0000FF00) >> 8;
    EncodeToDecodeMemBlock[ByteCounter+6] = (TBlockA & 0x00FF0000) >> 16;
    EncodeToDecodeMemBlock[ByteCounter+7] = (TBlockA & 0xFF000000) >> 24;

    ByteCounter +=8;

    RegionByteCounter +=8;
    }
///EXITING THE OUTER LOOP. cfg000 HAS NOW BEEN FULLY DECODED.

///CREATING A FILE COPY OF THE DECODED INFORMATION FOR USE IN OTHER FUNCTIONS

    filename_decryptedfile = filename + "_decrypted";
    ofstream CopyingDecodedFile;
    CopyingDecodedFile.open (filename_decryptedfile, ios::out | ios::trunc | ios::binary);
    CopyingDecodedFile.write (EncodeToDecodeMemBlock, size);
    CopyingDecodedFile.close();
    delete[] EncodeToDecodeMemBlock;
}


void changeaddressvalueofscenefile(int address, int newvalue) ///For encoding changes into scene0XX files
{

///USER INPUT: EDIT VALUE OF PARTICULAR ADDRESSES
///ENCODER

///OPENING THE DECODED FILE AND COPYING IT INTO MEMBLOCK

    char *DecodeToEncodeMemBlock;
    ifstream ReadingDecodedFile;
    ReadingDecodedFile.open (filename_decryptedfile, ios::in | ios::binary | ios::ate);
    size = ReadingDecodedFile.tellg();
    ReadingDecodedFile.seekg(0, ios::beg);
    DecodeToEncodeMemBlock = new char [size];
    ReadingDecodedFile.read (DecodeToEncodeMemBlock, size);
    ReadingDecodedFile.close();
///THE DECODED DATA IS NOW IN MEMBLOCK
    DecodeToEncodeMemBlock[address] = newvalue;
    ///VALUE CHANGED

///CHECKSUM CALCULATOR
uint32_t ChecksumRegion1(0), ChecksumRegion2(0), ChecksumRegion3(0), ChecksumRegion4(0), ChecksumRegion5(0), ChecksumRegion6(0);
uint32_t MemBlockConverter(0);


///6 REGIONS OF THE FILE MEANS 6 CHECKSUMS


///CHECKSUM REGION1
    for (int iterate(0); iterate < (0x948-0x8); iterate+=4)
    {
        MemBlockConverter = DecodeToEncodeMemBlock[iterate];
        if(MemBlockConverter > 0x7F)
            MemBlockConverter = MemBlockConverter & 0x000000FF;
        ChecksumRegion1 = ChecksumRegion1 + MemBlockConverter;
    }

///CHECKSUM REGION2
    for (int iterate(0x948); iterate < (0x1290-0x8); iterate+=4)
    {
        MemBlockConverter = DecodeToEncodeMemBlock[iterate];
        if(MemBlockConverter > 0x7F)
            MemBlockConverter = MemBlockConverter & 0x000000FF;
        ChecksumRegion2 = ChecksumRegion2 + MemBlockConverter;
    }

///CHECKSUM REGION3
    for (int iterate(0x1290); iterate < (0x1BD8-0x8); iterate+=4)
    {
        MemBlockConverter = DecodeToEncodeMemBlock[iterate];
        if(MemBlockConverter > 0x7F)
            MemBlockConverter = MemBlockConverter & 0x000000FF;
        ChecksumRegion3 = ChecksumRegion3 + MemBlockConverter;
    }

///CHECKSUM REGION4
    for (int iterate(0x1BD8); iterate < (0x2520-0x8); iterate+=4)
    {
        MemBlockConverter = DecodeToEncodeMemBlock[iterate];
        if(MemBlockConverter > 0x7F)
            MemBlockConverter = MemBlockConverter & 0x000000FF;
        ChecksumRegion4 = ChecksumRegion4 + MemBlockConverter;
    }

///CHECKSUM REGION5
    for (int iterate(0x2520); iterate < (0x2E68-0x8); iterate+=4)
    {
        MemBlockConverter = DecodeToEncodeMemBlock[iterate];
        if(MemBlockConverter > 0x7F)
            MemBlockConverter = MemBlockConverter & 0x000000FF;
        ChecksumRegion5 = ChecksumRegion5 + MemBlockConverter;
    }

///CHECKSUM REGION6
    for (int iterate(0x2E68); iterate < (0x37B0-0x8); iterate+=4)
    {
        MemBlockConverter = DecodeToEncodeMemBlock[iterate];
        if(MemBlockConverter > 0x7F)
            MemBlockConverter = MemBlockConverter & 0x000000FF;
        ChecksumRegion6 = ChecksumRegion6 + MemBlockConverter;
    }
///CHECKSUMS CALCULATED


///CheckSumRegion1 into Region1
    DecodeToEncodeMemBlock[0x948-4] = (ChecksumRegion1 & 0x000000FF);
    DecodeToEncodeMemBlock[0x948-3] = (ChecksumRegion1 & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[0x948-2] = (ChecksumRegion1 & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[0x948-1] = (ChecksumRegion1 & 0xFF000000) >> 24;
///CHECKSUM INSERTED


///CheckSumRegion2 into Region2
    DecodeToEncodeMemBlock[0x1290-4] = (ChecksumRegion2 & 0x000000FF);
    DecodeToEncodeMemBlock[0x1290-3] = (ChecksumRegion2 & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[0x1290-2] = (ChecksumRegion2 & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[0x1290-1] = (ChecksumRegion2 & 0xFF000000) >> 24;
///CHECKSUM INSERTED


///CheckSumRegion3 into Region3
    DecodeToEncodeMemBlock[0x1BD8-4] = (ChecksumRegion3 & 0x000000FF);
    DecodeToEncodeMemBlock[0x1BD8-3] = (ChecksumRegion3 & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[0x1BD8-2] = (ChecksumRegion3 & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[0x1BD8-1] = (ChecksumRegion3 & 0xFF000000) >> 24;
///CHECKSUM INSERTED


///CheckSumRegion4 into Region4
    DecodeToEncodeMemBlock[0x2520-4] = (ChecksumRegion4 & 0x000000FF);
    DecodeToEncodeMemBlock[0x2520-3] = (ChecksumRegion4 & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[0x2520-2] = (ChecksumRegion4 & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[0x2520-1] = (ChecksumRegion4 & 0xFF000000) >> 24;
///CHECKSUM INSERTED


///CheckSumRegion5 into Region5
    DecodeToEncodeMemBlock[0x2E68-4] = (ChecksumRegion5 & 0x000000FF);
    DecodeToEncodeMemBlock[0x2E68-3] = (ChecksumRegion5 & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[0x2E68-2] = (ChecksumRegion5 & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[0x2E68-1] = (ChecksumRegion5 & 0xFF000000) >> 24;
///CHECKSUM INSERTED


///CheckSumRegion6 into Region6
    DecodeToEncodeMemBlock[0x37B0-4] = (ChecksumRegion6 & 0x000000FF);
    DecodeToEncodeMemBlock[0x37B0-3] = (ChecksumRegion6 & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[0x37B0-2] = (ChecksumRegion6 & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[0x37B0-1] = (ChecksumRegion6 & 0xFF000000) >> 24;
///CHECKSUM INSERTED


///ENCODE the file, now that all the changes have been made and the checksum has been updated.
    uint32_t ByteCounter(0), BlockCounter(0), KeyBlockCtr(0);
    uint32_t Gear1(0), Gear2(0);
    uint32_t TBlockA(0), TBlockB(0), KBlockA(0), KBlockB(0);
    bool CarryFlag1(0), CarryFlag2(0);
    uint8_t OldMemblockValue(0);

    uint32_t RegionByteCounter(0);


    union u {
    uint64_t Cog64B;
    uint32_t Cog32BArray[2];
    }o;
///OUTERMOST LOOP OF THE ENCODER

for ( ; ByteCounter < size; BlockCounter++, KeyBlockCtr++)
    {

    if(RegionByteCounter == 0x948)
        {RegionByteCounter = 0;
        BlockCounter = 0;
        KeyBlockCtr = 0;}

        if(KeyBlockCtr > 31)
            KeyBlockCtr = 0;

    o.Cog64B = (uint64_t)RegionByteCounter << 0x14;


    Gear1 = o.Cog32BArray[0] | (RegionByteCounter << 0x0A) | RegionByteCounter; ///Will this work badly when Gear1 becomes higher than 7FFFFFFF?
        if (Gear1 > ~0xA1652347)
            CarryFlag1 = 1;
        else
            CarryFlag1 = 0;
    Gear1 = Gear1 + 0xA1652347;
    Gear2 = (BlockCounter*2 | o.Cog32BArray[1]) + CarryFlag1;

    TBlockB = LittleEndianToBigEndian (DecodeToEncodeMemBlock[ByteCounter], DecodeToEncodeMemBlock[ByteCounter+1], DecodeToEncodeMemBlock[ByteCounter+2], DecodeToEncodeMemBlock[ByteCounter+3]);
    TBlockA = LittleEndianToBigEndian (DecodeToEncodeMemBlock[ByteCounter+4], DecodeToEncodeMemBlock[ByteCounter+5], DecodeToEncodeMemBlock[ByteCounter+6], DecodeToEncodeMemBlock[ByteCounter+7]);

    KBlockA = LittleEndianToBigEndian_unsignedchar (KeyBlocksArray[KeyBlockCtr][0], KeyBlocksArray[KeyBlockCtr][1], KeyBlocksArray[KeyBlockCtr][2], KeyBlocksArray[KeyBlockCtr][3]);
    KBlockB = LittleEndianToBigEndian_unsignedchar (KeyBlocksArray[KeyBlockCtr][4], KeyBlocksArray[KeyBlockCtr][5], KeyBlocksArray[KeyBlockCtr][6], KeyBlocksArray[KeyBlockCtr][7]);

    TBlockB = KBlockB ^ TBlockB;
    TBlockA = KBlockA ^ TBlockA;

    TBlockB = TBlockB ^ Gear2;
    TBlockA = TBlockA ^ Gear1;

    if (TBlockA > ~KBlockA)  ///Reverse of TBlockA < KBlockA from the Decoder.
        CarryFlag2 = 1;
    else
        CarryFlag2 = 0;

    TBlockB = TBlockB + KBlockB + CarryFlag2;       ///Reversed from subtraction to addition.
    TBlockA = TBlockA + KBlockA;                    ///Reversed from subtraction to addition.

    DecodeToEncodeMemBlock[ByteCounter] =   (TBlockA & 0x000000FF);
    DecodeToEncodeMemBlock[ByteCounter+1] = (TBlockA & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[ByteCounter+2] = (TBlockA & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[ByteCounter+3] = (TBlockA & 0xFF000000) >> 24;
    DecodeToEncodeMemBlock[ByteCounter+4] = (TBlockB & 0x000000FF);
    DecodeToEncodeMemBlock[ByteCounter+5] = (TBlockB & 0x0000FF00) >> 8;
    DecodeToEncodeMemBlock[ByteCounter+6] = (TBlockB & 0x00FF0000) >> 16;
    DecodeToEncodeMemBlock[ByteCounter+7] = (TBlockB & 0xFF000000) >> 24;


///INNER LOOP OF ENCODER
    for(int iterate(8), BlockwiseByteCounter(0); BlockwiseByteCounter < 8; )
    {
         if(iterate != 0 && BlockwiseByteCounter < 8)
            {DecodeToEncodeMemBlock[ByteCounter] = DecodeToEncodeMemBlock[ByteCounter] + KeyBlocksArray[KeyBlockCtr][iterate-1] - 0x78;
            iterate--;}
         else if(iterate == 0 && BlockwiseByteCounter==0)
            {
            DecodeToEncodeMemBlock[ByteCounter] = 0x45 ^ BlockCounter ^ DecodeToEncodeMemBlock[ByteCounter];
            OldMemblockValue = DecodeToEncodeMemBlock[ByteCounter];
            BlockwiseByteCounter++;
            ByteCounter++;
            iterate = 8;
            }
        else if(iterate == 0 && BlockwiseByteCounter < 8)
            {
            DecodeToEncodeMemBlock[ByteCounter] = DecodeToEncodeMemBlock[ByteCounter] ^ OldMemblockValue;
            OldMemblockValue = DecodeToEncodeMemBlock[ByteCounter];
            iterate = 8;
            BlockwiseByteCounter++;
            ByteCounter++;
            }

    }

    RegionByteCounter +=8;

///EXITING INNER LOOP OF ENCODER

    }
///EXITING OUTER LOOP
///ENCODING FINISHED

///ENTERING RE-ENCODED DATA INTO THE FILE
    ofstream SavingEncodedFile;
    SavingEncodedFile.open (filename, ios::out | ios::trunc | ios::binary);
    SavingEncodedFile.write (DecodeToEncodeMemBlock, size);
    SavingEncodedFile.close();
    delete[] DecodeToEncodeMemBlock;
///FINISHED ENTERING RE-ENCODED DATA INTO THE FILE
}


int checkfileaddressvalue(int address)
{
///OPENING DECODED FILE AND COPYING CONTENTS INTO DecodedMemBlock
    char *DecodedMemBlock;
    ifstream ReadingDecodedFile;
    ReadingDecodedFile.open (filename_decryptedfile, ios::in | ios::binary | ios::ate);
    size = ReadingDecodedFile.tellg();
    ReadingDecodedFile.seekg(0, ios::beg);
    DecodedMemBlock = new char [size];
    ReadingDecodedFile.read (DecodedMemBlock, size);
    ReadingDecodedFile.close();
///CONTENTS ARE NOW COPIED ONTO DecodedMemBlock
    addressvalue = DecodedMemBlock[address] & 0xFF;
    delete[] DecodedMemBlock;
    return addressvalue;
}




int main () ///MAIN IS FOR THE USER SELECTION TREES AND USER INPUTS
{
    cout << "\nWELCOME TO McDIRGE: VERSION 0.3, COMMAND LINE EDITION" << endl;
    cout << "Save editor for Dirge of Cerberus: Final Fantasy VII.\n" << endl;

    cout << "Now user friendly for editing the Configuration file (cfg000)." << endl;
    cout << "Advanced mode allows you to edit any byte in a greater selection of files." << endl;
    cout << "Consult the ReadMe for further instructions.\n" << endl;

    topmenu:

    cout << "\nType and enter the symbol(s) shown within arrow brackets to make a selection.\n" << endl;

    cout << "<cfg>\tConfiguration file (cfg000)\n<adv>\tAdvanced Mode\n<exit>\tExit program" << endl;

    cin >> userinput;

        if(userinput == "cfg" || userinput == "<cfg>" || userinput == "cfg>" || userinput == "<cfg")
            {
            filename = "cfg000";
            ///FUNCTION THAT OPENS cfg000, DECRYPTS, AND CREATES DECRYPTED FILE COPY
            opendecodeandcopyfile(filename);
            Tree_cfg:

            cout << "\n'Configuration file (cfg000)'\n" << endl;
            cout << "\t<gen>\tGeneral Settings\n\t<ctr>\tController Setup **STILL UNDER CONSTRUCTION**\n\t<top>\tGo back to top menu\n\t<exit>\tExit program" << endl;
            cin >> userinput;

            if(userinput == "gen" || userinput == "<gen>" || userinput == "gen>" || userinput == "<gen")
                {
                cout << "\nSelected 'General Settings'." << endl;
                cout << "In-game these constitute 'Control Settings'(Page 1) & 'Other Settings'(Page 5)." << endl;
                Tree_gen:
                cout << "\nType and enter the symbol(s) shown within arrow brackets to change a setting.\n" << endl;

                ///FUNCTIONS TO CHECK RELEVANT VALUES IN DECRYPTED FILE COPY
                tempsave            = checkfileaddressvalue(0x4);
                secret_reset        = checkfileaddressvalue(0x40);
                vibration           = checkfileaddressvalue(0x41);
                sight_speed         = checkfileaddressvalue(0x43);
                camera_speed        = checkfileaddressvalue(0x44);
                mouse_speed         = checkfileaddressvalue(0x46);
                camera_vertical     = checkfileaddressvalue(0x48);
                camera_horizontal   = checkfileaddressvalue(0x49);
                sight_vertical      = checkfileaddressvalue(0x4A);
                sight_horizontal    = checkfileaddressvalue(0x4B);
                sound_output        = checkfileaddressvalue(0x4C);
                subtitles           = checkfileaddressvalue(0x4D);
                speakers_name       = checkfileaddressvalue(0x4E);
                keyboard_usage      = checkfileaddressvalue(0x50);
                gamma_adjustment    = checkfileaddressvalue(0x5A);
                sight_support       = checkfileaddressvalue(0x5E);
                screenpos_x         = checkfileaddressvalue(0x5F);
                screenpos_y         = checkfileaddressvalue(0x60);

                ///
                    {
                    if (tempsave == 0)
                        tempsave_setting = "Not Available";
                    else if (tempsave == 1)
                        tempsave_setting = "Available";
                    else
                        tempsave_setting = "INVALID";

                    if (secret_reset == 0)
                        secret_reset_setting = "On";
                    else if (secret_reset == 1)
                        secret_reset_setting = "Off";
                    else
                        secret_reset_setting = "INVALID";

                    if (vibration == 0)
                        vibration_setting = "On";
                    else if (vibration == 1)
                        vibration_setting = "Off";
                    else
                        vibration_setting = "INVALID";

                    if (camera_vertical == 0)
                        camera_vertical_setting = "Inverted";
                    else if (camera_vertical == 1)
                        camera_vertical_setting = "Normal";
                    else
                        camera_vertical_setting = "INVALID";

                    if (camera_horizontal == 0)
                        camera_horizontal_setting = "Normal";
                    else if (camera_horizontal == 1)
                        camera_horizontal_setting = "Inverted";
                    else
                        camera_horizontal_setting = "INVALID";

                    if (sight_vertical == 0)
                        sight_vertical_setting = "Inverted";
                    else if (sight_vertical == 1)
                        sight_vertical_setting = "Normal";
                    else
                        sight_vertical_setting = "INVALID";

                    if (sight_horizontal == 0)
                        sight_horizontal_setting = "Normal";
                    else if (sight_horizontal == 1)
                        sight_horizontal_setting = "Inverted";
                    else
                        sight_horizontal_setting = "INVALID";

                    if (sound_output == 0)
                        sound_output_setting = "Dolby Pro Logic II";
                    else if (sound_output == 1)
                        sound_output_setting = "Monaural";
                    else
                        sound_output_setting = "INVALID";

                    if (subtitles == 0)
                        subtitles_setting = "On";
                    else if (subtitles == 1)
                        subtitles_setting = "Off";
                    else
                        subtitles_setting = "INVALID";

                    if (speakers_name == 0)
                        speakers_name_setting = "On";
                    else if (speakers_name == 1)
                        speakers_name_setting = "Off";
                    else
                        speakers_name_setting = "INVALID";

                    if (keyboard_usage == 0)
                        keyboard_usage_setting = "Chat Only";
                    else if (keyboard_usage == 1)
                        keyboard_usage_setting = "General Controls";
                    else
                        keyboard_usage_setting = "INVALID";

                    if (sight_support == 0)
                        sight_support_setting = "Manual";
                    else if (sight_support == 1)
                        sight_support_setting = "Semi-Automatic";
                    else if (sight_support == 2)
                        sight_support_setting = "Automatic";
                    else
                        sight_support_setting = "INVALID";
                    }

                ///

                cout << "\tSETTING\t\t\tCURRENT STATUS" << endl;
                cout << "<tp>\tTempsave:\t\t" << tempsave_setting << endl;
                cout << "<rs>\tReset to default:\t" << secret_reset_setting << endl;
                cout << "<vb>\tVibration:\t\t" << vibration_setting << endl;
                cout << "<ss>\tSight Speed:\t\t" << (int)sight_speed << endl;
                cout << "<cs>\tCamera Speed:\t\t" << (int)camera_speed << endl;
                cout << "<ms>\tMouse Speed:\t\t" << (int)mouse_speed << endl;
                cout << "<cv>\tCamera - Vertical:\t" << camera_vertical_setting << endl;
                cout << "<ch>\tCamera - Horizontal:\t" << camera_horizontal_setting << endl;
                cout << "<sv>\tSight - Vertical:\t" << sight_vertical_setting << endl;
                cout << "<sh>\tSight - Horizontal:\t" << sight_horizontal_setting << endl;
                cout << "<so>\tSound Output:\t\t" << sound_output_setting << endl;
                cout << "<sb>\tSubtitles:\t\t" << subtitles_setting << endl;
                cout << "<sn>\tSpeaker's Name:\t\t" << speakers_name_setting << endl;
                cout << "<ku>\tKeyboard Usage:\t\t" << keyboard_usage_setting << endl;
                cout << "<ga>\tGamma Adjustment:\t" << gamma_adjustment << endl;
                cout << "<ssu>\tSight Support:\t\t" << sight_support_setting << endl;
                cout << "<spx>\tScreen Position: X axis\t\t" << (int)screenpos_x << "\t*PAL-exclusive functionality" << endl;
                cout << "<spy>\tScreen Position: Y axis\t\t" << (int)screenpos_y << "\t*PAL-exclusive functionality\n" << endl;

                cout << "<top>\tGo back to top menu\n<exit>\tExit program\n" << endl;

                cin >> userinput;

                    if (userinput == "tp" || userinput == "tp>" || userinput == "<tp" || userinput == "<tp>")
                        {
                        cout << "\n\tTempsave is '" << tempsave_setting << "' at this point." << endl;
                        cout << "Value Guide: 0 = 'Tempsave NOT available', 1 = 'Tempsave available'" << endl;
                        cout << "The most recent Tempsave data can be restored by setting this value to 1." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        tempsave = numeric_userinput;
                        stringstream(userinput) >> numeric_userinput;
                        tempsave = numeric_userinput;
                        ///FUNCTION THAT CHANGES THE NEW VALUE INTO cfg000
                        changeaddressvalue(0x4, tempsave);
                        ///RE-DECODES cfg000 TO CONFIRM THE NEW VALUE
                        opendecodeandcopyfile(filename);
                        ///Retrieving the new value as shown in the decoded file, confirming it rather than assuming.
                        tempsave = checkfileaddressvalue(0x4);
                        if (tempsave == 0)
                            tempsave_setting = "Not Available";
                        else if
                            (tempsave == 1)
                            tempsave_setting = "Available";
                        else
                            tempsave_setting = "INVALID";
                        cout << "Tempsave is now '" << tempsave_setting << "'." << endl;
                        goto Tree_gen;
                        }

                    else if (userinput == "rs" || userinput == "rs>" || userinput == "<rs" || userinput == "<rs>")
                        {
                        cout << "\n\tReset to default is '" << secret_reset_setting << "' at this point." << endl;
                        cout << "Value Guide: 0 = 'Reset will happen', 1 = 'Reset will NOT happen'" << endl;
                        cout << "If set to 0, the game will reset the following settings when you load a save file:" << endl;
                        cout << "\t-General Settings\n\t-Controller Settings\n\t-Keyboard Settings\n\t-Mouse Settings" << endl;
                        cout << "This is a secret switch not available to the player." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        secret_reset = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x40, secret_reset);
                        opendecodeandcopyfile(filename);
                        secret_reset = checkfileaddressvalue(0x40);
                        if (secret_reset == 0)
                            secret_reset_setting = "On";
                        else if
                            (secret_reset == 1)
                            secret_reset_setting = "Off";
                        else
                            secret_reset_setting = "INVALID";
                        cout << "Reset to default is now '" << secret_reset_setting << "'." << endl;
                        goto Tree_gen;
                        }

                    else if (userinput == "vb" || userinput == "vb>" || userinput == "<vb" || userinput == "<vb>")
                       {
                        cout << "\n\tVibration is '" << vibration_setting << "' at this point." << endl;
                        cout << "Value Guide: 0 = 'On', 1 = 'Off'" << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        vibration = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x41, vibration);
                        opendecodeandcopyfile(filename);
                        vibration = checkfileaddressvalue(0x41);
                        if (vibration == 0)
                            vibration_setting = "On";
                        else if
                            (vibration == 1)
                            vibration_setting = "Off";
                        else
                            vibration_setting = "INVALID";
                        cout << "Vibration is now '" << vibration_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "ss" || userinput == "ss>" || userinput == "<ss" || userinput == "<ss>")
                       {
                        cout << "\n\tSight Speed is '" << (int)sight_speed << "' at this point." << endl;
                        cout << "Value Guide:" << endl;
                        cout << "0%\t= -64 [Slowest]\n50%\t= 0 [Default]\n100%\t= 64 [Fastest]" << endl;
                        cout << "In-game you are allowed to change this value in increments of 8." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        sight_speed = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x43, sight_speed);
                        opendecodeandcopyfile(filename);
                        sight_speed = checkfileaddressvalue(0x43);
                        cout << "Sight Speed has now been set to value '" << (int)sight_speed << "'." << endl;
                        if (sight_speed < -64 || sight_speed > 64)
                            cout << "Keep in mind this value is outside of the normal range." << endl;

                        goto Tree_gen;
                       }

                    else if (userinput == "cs" || userinput == "cs>" || userinput == "<cs" || userinput == "<cs>")
                       {
                        cout << "\n\tCamera Speed is '" << (int)camera_speed << "' at this point." << endl;
                        cout << "Value guide:" << endl;
                        cout << "0%\t= -64 [Slowest]\n50%\t= 0 [Default]\n100%\t= 64 [Fastest]" << endl;
                        cout << "In-game you are allowed to change this value in increments of 8." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        camera_speed = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x44, camera_speed);
                        opendecodeandcopyfile(filename);
                        camera_speed = checkfileaddressvalue(0x44);
                        cout << "Camera Speed has now been set to value '" << (int)camera_speed << "'." << endl;
                        if (camera_speed < -64 || camera_speed > 64)
                            cout << "Keep in mind this value is outside of the normal range." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "ms" || userinput == "ms>" || userinput == "<ms" || userinput == "<ms>")
                       {
                        cout << "\n\tMouse Speed is '" << (int)mouse_speed << "' at this point." << endl;
                        cout << "Value guide:" << endl;
                        cout << "0%\t\t= -16 [Slowest]\n25%\t\t= -4\n33.3...%\t= 0 [Default]\n50%\t\t= 8\n75%\t\t= 20\n100%\t\t= 32 [Fastest]" << endl;
                        cout << "In-game you are allowed to change this value in increments of 1." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        mouse_speed = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x46, mouse_speed);
                        opendecodeandcopyfile(filename);
                        mouse_speed = checkfileaddressvalue(0x46);
                        cout << "Mouse Speed has now been set to value '" << (int)mouse_speed << "'." << endl;
                        if (mouse_speed < -16 || mouse_speed > 32)
                            cout << "Keep in mind this value is outside of the normal range." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "cv" || userinput == "cv>" || userinput == "<cv" || userinput == "<cv>")
                       {
                        cout << "\n\tCamera - Vertical is '" << camera_vertical_setting << "' at this point." << endl;
                        cout << "Value guide: 0 = 'Inverted', 1 = 'Normal'" << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        camera_vertical = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x48, camera_vertical);
                        opendecodeandcopyfile(filename);
                        camera_vertical = checkfileaddressvalue(0x48);
                        if (camera_vertical == 0)
                            camera_vertical_setting = "Inverted";
                        else if (camera_vertical == 1)
                            camera_vertical_setting = "Normal";
                        else
                            camera_vertical_setting = "INVALID";
                        cout << "Camera - Vertical is now set to '" << camera_vertical_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "ch" || userinput == "ch>" || userinput == "<ch" || userinput == "<ch>")
                       {
                        cout << "\n\tCamera - Horizontal is '" << camera_horizontal_setting << "' at this point." << endl;
                        cout << "Value guide: 0 = 'Normal', 1 = 'Inverted'" << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        camera_horizontal = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x49, camera_horizontal);
                        opendecodeandcopyfile(filename);
                        camera_horizontal = checkfileaddressvalue(0x49);
                        if (camera_horizontal == 0)
                            camera_horizontal_setting = "Normal";
                        else if (camera_horizontal == 1)
                            camera_horizontal_setting = "Inverted";
                        else
                            camera_horizontal_setting = "INVALID";
                        cout << "Camera - Horizontal is now set to '" << camera_horizontal_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "sv" || userinput == "sv>" || userinput == "<sv" || userinput == "<sv>")
                       {
                        cout << "\n\tSight - Vertical is '" << sight_vertical_setting << "' at this point." << endl;
                        cout << "Value guide: 0 = 'Inverted', 1 = 'Normal'" << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        sight_vertical = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x4A, sight_vertical);
                        opendecodeandcopyfile(filename);
                        sight_vertical = checkfileaddressvalue(0x4A);
                        if (sight_vertical == 0)
                            sight_vertical_setting = "Inverted";
                        else if (sight_vertical == 1)
                            sight_vertical_setting = "Normal";
                        else
                            sight_vertical_setting = "INVALID";
                        cout << "Sight - Vertical is now set to '" << sight_vertical_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "sh" || userinput == "sh>" || userinput == "<sh" || userinput == "<sh>")
                       {
                        cout << "\n\tSight - Horizontal is '" << sight_horizontal_setting << "' at this point." << endl;
                        cout << "Value guide: 0 = 'Normal', 1 = 'Inverted'" << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        sight_horizontal = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x4B, sight_horizontal);
                        opendecodeandcopyfile(filename);
                        sight_horizontal = checkfileaddressvalue(0x4B);
                        if (sight_horizontal == 0)
                            sight_horizontal_setting = "Normal";
                        else if (sight_horizontal == 1)
                            sight_horizontal_setting = "Inverted";
                        else
                            sight_horizontal_setting = "INVALID";
                        cout << "Sight - Horizontal is now set to '" << sight_horizontal_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "so" || userinput == "so>" || userinput == "<so" || userinput == "<so>")
                       {
                        cout << "\n\tSound Output is '" << sound_output_setting << "' at this point." << endl;
                        cout << "Value guide: 0 = 'Dolby Pro Logic II', 1 = 'Monaural'" << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        sound_output = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x4C, sound_output);
                        opendecodeandcopyfile(filename);
                        sound_output = checkfileaddressvalue(0x4C);
                        if (sound_output == 0)
                            sound_output_setting = "Dolby Pro Logic II";
                        else if (sound_output == 1)
                            sound_output_setting = "Monaural";
                        else
                            sound_output_setting = "INVALID";
                        cout << "Sound Output is now set to '" << sound_output_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "sb" || userinput == "sb>" || userinput == "<sb" || userinput == "<sb>")
                       {
                        cout << "\n\tSubtitles are '" << subtitles_setting << "' at this point." << endl;
                        cout << "Value Guide: 0 = 'On', 1 = 'Off'" << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        subtitles = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x4D, subtitles);
                        opendecodeandcopyfile(filename);
                        subtitles = checkfileaddressvalue(0x4D);
                        if (subtitles == 0)
                            subtitles_setting = "On";
                        else if (subtitles == 1)
                            subtitles_setting = "Off";
                        else
                            subtitles_setting = "INVALID";
                        cout << "Subtitles are now set to '" << subtitles_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "sn" || userinput == "sn>" || userinput == "<sn" || userinput == "<sn>")
                       {
                        cout << "\n\tSpeaker's Name is '" << speakers_name_setting << "' at this point." << endl;
                        cout << "Value Guide: 0 = 'On', 1 = 'Off'" << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        speakers_name = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x4E, speakers_name);
                        opendecodeandcopyfile(filename);
                        speakers_name = checkfileaddressvalue(0x4E);
                        if (speakers_name == 0)
                            speakers_name_setting = "On";
                        else if (speakers_name == 1)
                            speakers_name_setting = "Off";
                        else
                            speakers_name_setting = "INVALID";
                        cout << "Speaker's Name is now set to '" << speakers_name_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "ku" || userinput == "ku>" || userinput == "<ku" || userinput == "<ku>")
                       {
                        cout << "\n\tKeyboard Usage is set to '" << keyboard_usage_setting << "' at this point." << endl;
                        cout << "Value guide: 0 = 'Chat Only', 1 = 'General Controls'" << endl;
                        cout << "The Keyboard Usage setting is functional in all versions, but only the JP original shows it in the menu." << endl;
                        cout << "If you select 'Chat Only' then you can't control Vincent with the keyboard." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        keyboard_usage = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x50, keyboard_usage);
                        opendecodeandcopyfile(filename);
                        keyboard_usage = checkfileaddressvalue(0x50);
                        if (keyboard_usage == 0)
                            keyboard_usage_setting = "Chat Only";
                        else if (keyboard_usage == 1)
                            keyboard_usage_setting = "General Controls";
                        else
                            keyboard_usage_setting = "INVALID";
                        cout << "Keyboard Usage is now set to '" << keyboard_usage_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "ga" || userinput == "ga>" || userinput == "<ga" || userinput == "<ga>")
                       {
                        cout << "\n\tGamma Adjustment is '" << gamma_adjustment << "' at this point." << endl;
                        cout << "Value guide:" << endl;
                        cout << "0%\t= 20 [Darkest]\n25%\t= 60\n50%\t= 100\n67.5%\t= 128 [Default]\n75%\t= 140\n100%\t= 180 [Brightest]" << endl;
                        cout << "In-game you are allowed to change this value in increments of 1." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        gamma_adjustment = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x5A, gamma_adjustment);
                        opendecodeandcopyfile(filename);
                        gamma_adjustment = checkfileaddressvalue(0x5A);
                        cout << "Gamma Adjustment is now set to '" << gamma_adjustment << "'." << endl;
                        if (gamma_adjustment < 20 || gamma_adjustment > 180)
                            cout << "Keep in mind this value is outside of the normal range." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "ssu" || userinput == "ssu>" || userinput == "<ssu" || userinput == "<ssu>")
                       {
                        cout << "\n\tSight Support is set to '" << sight_support_setting << "' at this point." << endl;
                        cout << "Value guide: 0 = 'Manual', 1 = 'Semi-Automatic', 2 = 'Automatic'" << endl;
                        cout << "In the original JP version this menu option does not exist. Auto-lock is added with equippable scopes." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        sight_support = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x5E, sight_support);
                        opendecodeandcopyfile(filename);
                        sight_support = checkfileaddressvalue(0x5E);
                        if (sight_support == 0)
                            sight_support_setting = "Manual";
                        else if (sight_support == 1)
                            sight_support_setting = "Semi-Automatic";
                        else if (sight_support == 2)
                            sight_support_setting = "Automatic";
                        else
                            sight_support_setting = "INVALID";
                        cout << "Sight Support is now set to '" << sight_support_setting << "'." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "spx" || userinput == "spx>" || userinput == "<spx" || userinput == "<spx>")
                       {
                        cout << "\n\tScreen Position: X axis is at value '" << (int)screenpos_x << "'." << endl;
                        cout << "Value guide:" << endl;
                        cout << "-32\t[Leftmost]\n0\t[Default]\n32\t[Rightmost]" << endl;
                        cout << "In-game you are allowed to change this value in increments of 1." << endl;
                        cout << "This setting only has functionality in the PAL (European/Australian/Misc) version of the game." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        screenpos_x = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x5F, screenpos_x);
                        opendecodeandcopyfile(filename);
                        screenpos_x = checkfileaddressvalue(0x5F);
                        cout << "Screen Position: X axis is now set to value '" << (int)screenpos_x << "'." << endl;
                        if (screenpos_x < -32 || screenpos_x > 32)
                            cout << "Keep in mind this value is outside of the normal range." << endl;
                        goto Tree_gen;
                       }

                    else if (userinput == "spy" || userinput == "spy>" || userinput == "<spy" || userinput == "<spy>")
                       {
                        cout << "\n\tScreen Position: Y axis is at value '" << (int)screenpos_y << "'." << endl;
                        cout << "Value guide:" << endl;
                        cout << "-32\t[Upmost]\n0\t[Default]\n32\t[Downmost]" << endl;
                        cout << "In-game you are allowed to change this value in increments of 1." << endl;
                        cout << "This setting only has functionality in the PAL (European/Australian/Misc) version of the game." << endl;
                        cout << " Enter a number:" << endl;
                        cin >> userinput;
                        stringstream(userinput) >> numeric_userinput;
                        screenpos_y = numeric_userinput;
                        ///FUNCTIONS THAT HELP SAVE THE CHANGE INTO THE FINAL CFG000 file!
                        changeaddressvalue(0x60, screenpos_y);
                        opendecodeandcopyfile(filename);
                        screenpos_y = checkfileaddressvalue(0x60);
                        cout << "Screen Position: Y axis is now set to value '" << (int)screenpos_y << "'." << endl;
                        if (screenpos_y < -32 || screenpos_y > 32)
                            cout << "Keep in mind this value is outside of the normal range." << endl;
                        goto Tree_gen;
                       }

                    else if(userinput == "top" || userinput == "<top>" || userinput == "top>" || userinput == "<top")
                        {
                        deletedecodedfiles();
                        goto topmenu;
                        }
                    else if (userinput == "exit" || userinput == "<exit>"|| userinput == "exit>" || userinput == "<exit")
                        {
                        deletedecodedfiles();
                        cout << "\nThank you for using McDirge. Have a good day!" << endl;
                        }
                    else
                        {
                        cout << "Invalid input. Try again." << endl;
                        goto Tree_gen;
                        }

                }


            else if(userinput == "ctr" || userinput =="<ctr>" || userinput == "ctr>" || userinput == "<ctr")
                {
                cout << "This section is still under construction. Please return to where you were." << endl;
                goto Tree_cfg;
                }

            else if(userinput == "top" || userinput == "<top>" || userinput == "top>" || userinput == "<top")
                {
                deletedecodedfiles();
                goto topmenu;
                }
            else if (userinput == "exit" || userinput == "<exit>"|| userinput == "exit>" || userinput == "<exit")
                {
                deletedecodedfiles();
                cout << "\nThank you for using McDirge. Have a good day!" << endl;
                }

            else
                {
                cout << "Invalid input. Please try again." << endl;
                goto Tree_cfg;
                }

            }


       else if(userinput == "adv" || userinput == "<adv>" || userinput == "adv>" || userinput == "<adv")
            {
            Tree_adv:
            scenefileselected = 0;
            cout << "\n'Advanced Mode'\n" << endl;

            cout << "Which file do you wish to open and edit?" << endl;
            cout << "Enter the symbols shown within arrow brackets to select the file.\n" << endl;

            cout << "\t<j00>\t\tBISLPM-66271000" << endl;
            cout << "\t<j01>\t\tBISLPM-66271001\n" << endl;

            cout << "\t<na0>\t\tBASLUS-21419000" << endl;
            cout << "\t<na1>\t\tBASLUS-21419001\n" << endl;

            cout << "\t<pal0>\t\tBESLES-54185000" << "\t(*Lower-case of L in pal0)" << endl;
            cout << "\t<pal1>\t\tBESLES-54185001" << "\t(*Lower-case of L in pal1)\n" << endl;

            cout << "\t<j10>\t\tBISLPM-66629000" << endl;
            cout << "\t<j11>\t\tBISLPM-66629001\n" << endl;

            cout << "\t<cfg>\t\tcfg000" << endl;
            cout << "\t<scn00>\t\tscene000" << endl;
            cout << "\t<scn01>\t\tscene001" << endl;
            cout << "\t<scn02>\t\tscene002" << endl;
            cout << "\t<scn03>\t\tscene003" << endl;
            cout << "\t<scn04>\t\tscene004" << endl;
            cout << "\t<scn05>\t\tscene005" << endl;
            cout << "\t<scn06>\t\tscene006" << endl;
            cout << "\t<scn07>\t\tscene007" << endl;
            cout << "\t<scn08>\t\tscene008" << endl;
            cout << "\t<scn09>\t\tscene009" << endl;
            cout << "\t<scn10>\t\tscene010" << endl;
            cout << "\t<scr>\t\tscr000\n" << endl;

            cout << "\t<top>\tGo back to top menu" << endl;
            cout << "\t<exit>\tExit program" << endl;

            cin >> userinput;

            if(userinput == "j00" || userinput == "j00>" || userinput == "<j00" || userinput == "<j00>")
                {
                filename = "BISLPM-66271000";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "j01" || userinput == "j01>" || userinput == "<j01" || userinput == "<j01>")
                {
                filename = "BISLPM-66271001";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "na0" || userinput == "na0>" || userinput == "<na0" || userinput == "<na0>")
                {
                filename = "BASLUS-21419000";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "na1" || userinput == "na1>" || userinput == "<na1" || userinput == "<na1>")
                {
                filename = "BASLUS-21419001";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "pal0" || userinput == "pal0>" || userinput == "<pal0" || userinput == "<pal0>")
                {
                filename = "BESLES-54185000";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "pal1" || userinput == "pal1>" || userinput == "<pal1" || userinput == "<pal1>")
                {
                filename = "BESLES-54185001";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "j10" || userinput == "j10>" || userinput == "<j10" || userinput == "<j10>")
                {
                filename = "BISLPM-66629000";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "j11" || userinput == "j11>" || userinput == "<j11" || userinput == "<j11>")
                {
                filename = "BISLPM-66629001";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "j11" || userinput == "j11>" || userinput == "<j11" || userinput == "<j11>")
                {
                filename = "BISLPM-66629001";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "cfg" || userinput == "cfg>" || userinput == "<cfg" || userinput == "<cfg>")
                {
                filename = "cfg000";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "scn00" || userinput == "scn00>" || userinput == "<scn00" || userinput == "<scn00>")
                {
                filename = "scene000";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn01" || userinput == "scn01>" || userinput == "<scn01" || userinput == "<scn01>")
                {
                filename = "scene001";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn02" || userinput == "scn02>" || userinput == "<scn02" || userinput == "<scn02>")
                {
                filename = "scene002";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn03" || userinput == "scn03>" || userinput == "<scn03" || userinput == "<scn03>")
                {
                filename = "scene003";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn04" || userinput == "scn04>" || userinput == "<scn04" || userinput == "<scn04>")
                {
                filename = "scene004";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn05" || userinput == "scn05>" || userinput == "<scn05" || userinput == "<scn05>")
                {
                filename = "scene005";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn06" || userinput == "scn06>" || userinput == "<scn06" || userinput == "<scn06>")
                {
                filename = "scene006";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn07" || userinput == "scn07>" || userinput == "<scn07" || userinput == "<scn07>")
                {
                filename = "scene007";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn08" || userinput == "scn08>" || userinput == "<scn08" || userinput == "<scn08>")
                {
                filename = "scene008";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn09" || userinput == "scn09>" || userinput == "<scn09" || userinput == "<scn09>")
                {
                filename = "scene009";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scn10" || userinput == "scn10>" || userinput == "<scn10" || userinput == "<scn10>")
                {
                filename = "scene010";
                scenefileselected = 1;
                opendecodeandcopyscenefile(filename);
                }
            else if(userinput == "scr" || userinput == "scr>" || userinput == "<scr" || userinput == "<scr>")
                {
                filename = "scr000";
                opendecodeandcopyfile(filename);
                }
            else if(userinput == "top" || userinput == "<top>" || userinput == "top>" || userinput == "<top")
                {
                deletedecodedfiles();
                goto topmenu;
                }
            else if (userinput == "exit" || userinput == "<exit>"|| userinput == "exit>" || userinput == "<exit")
                {
                deletedecodedfiles();
                cout << "\nThank you for using McDirge. Have a good day!" << endl;
                return 0;
                }
            else
                {
                cout << "Invalid input. Please try again." << endl;
                goto Tree_adv;
                }

            File_Selected:
                cout << std::hex << std::uppercase << "\nWhich address in " << filename << " do you wish to change the value of?" << endl;
                cout << "\tEnter a hex value in the range of 0x0 to 0x" << size - 9 << endl;
                cin >> std::hex >> address;
                cout << "What value do you want 0x" << address << " to have?" << endl;
                cout << "\tEnter a hex value in the range 0x0 to 0xFF" << endl;
                cin >> addressvalue;
                ///FUNCTION THAT EDITS THE VALUE AND SAVES IT INTO THE RE-ENCODED FILE
                if (scenefileselected == 0)
                    changeaddressvalue(address, addressvalue);
                else if (scenefileselected == 1)
                    changeaddressvalueofscenefile(address, addressvalue);
                else
                    {
                    cout << "Error. Return to file selection.\n" << endl;
                    goto Tree_adv;
                    }
                cout << "\nAddress 0x" << address << " now has the value 0x" << addressvalue << "." << endl;
                adv_crossroads:
                cout << "\t<e>\tEdit another address in " << filename << endl;
                cout << "\t<b>\tBack to file selection" << endl;
                cout << "\t<top>\tGo back to top menu\n\t<exit>\tExit program\n" << endl;

                cin >> userinput;

                if (userinput == "e" || userinput == "<e>"|| userinput == "e>" || userinput == "<e")
                    goto File_Selected;
                else if (userinput == "b" || userinput == "<b>"|| userinput == "b>" || userinput == "<b")
                    {
                    deletedecodedfiles();
                    goto Tree_adv;
                    }
                else if(userinput == "top" || userinput == "<top>" || userinput == "top>" || userinput == "<top")
                    {
                    deletedecodedfiles();
                    goto topmenu;
                    }
                else if (userinput == "exit" || userinput == "<exit>"|| userinput == "exit>" || userinput == "<exit")
                    {
                    deletedecodedfiles();
                    cout << "\nThank you for using McDirge. Have a good day!" << endl;
                    }
                else
                    {
                    cout << "Invalid input Try again." << endl;
                    goto adv_crossroads;
                    }


            }

        else if (userinput == "exit" || userinput == "<exit>"|| userinput == "exit>" || userinput == "<exit")
            {
            deletedecodedfiles();
            cout << "\nThank you for using McDirge. Have a good day!" << endl;
            }
        else
            {
            cout << "Invalid input Try again." << endl;
            goto topmenu;
            }

    return 0;
}




///FUNCTIONS FOR BYTE CONVERSION FROM 4-BYTE BLOCKS. FUNCTIONS PROTOTYPED AT THE TOP OF THE PROGRAM.
uint32_t LittleEndianToBigEndian (char Byte1, char Byte2, char Byte3, char Byte4)
{
    static uint32_t BigEndianBlock, ConvertByte4, ConvertByte3, ConvertByte2, ConvertByte1;
    ConvertByte4 = (Byte4 << 24) & 0xFF000000;
    ConvertByte3 = (Byte3 << 16) & 0x00FF0000;
    ConvertByte2 = (Byte2 << 8)  & 0x0000FF00;
    ConvertByte1 =  Byte1       & 0x000000FF;

    BigEndianBlock = ConvertByte4 + ConvertByte3 + ConvertByte2 + ConvertByte1;
    return BigEndianBlock;
}
uint32_t LittleEndianToBigEndian_unsignedchar (uint8_t Byte1, uint8_t Byte2, uint8_t Byte3, uint8_t Byte4)
{
    static uint32_t BigEndianBlock, ConvertByte4, ConvertByte3, ConvertByte2, ConvertByte1;
    ConvertByte4 = Byte4 << 24;
    ConvertByte3 = Byte3 << 16;
    ConvertByte2 = Byte2 << 8;
    ConvertByte1 = Byte1;

    BigEndianBlock = ConvertByte4 + ConvertByte3 + ConvertByte2 + ConvertByte1;
    return BigEndianBlock;
}
