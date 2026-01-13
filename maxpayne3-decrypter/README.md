# Max Payne 3 PS3 Save Decrypter

Tools to decrypt and decompress Max Payne 3 PS3 save-games

## Decrypter

Decrypts/encrypts the save file encryption.

```
USAGE: ./maxpayne3-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

## Decompressor

Decompresses the decrypted save data.

```
USAGE: ./decompressor <input_file> <output_file>

Example: ./decompressor decrypted decrypted-decomp
```

The decompressor implements the LZ77-style compression algorithm used by Max Payne 3 save files,
based on the reference C# implementation in [samples/Decompress.cs](samples/Decompress.cs).

### Building

```bash
make          # Build both tools
make decrypter    # Build only the decrypter
make decompressor # Build only the decompressor
```

### Checksums

**Note:** the tool also updates the HMAC-SHA1 and ADD integrity hashes.

### Crypto notes

- [Crypto](samples/Crypto.txt)
- [File format](samples/Fileformat.txt)

```
Cryptoanalysis by Vulnavia
x86-Codeanalysis by Vulnavia
PPC-Codeanalysis by Philymaster and Vulnavia
//------------------------------------------------------------------------------
Decrypt:
01. Double HMACSHA1: Key=InitialKey1, Data1=Secblock, Data2=Secblocksize(binary)
02. PKDF2RFC2898: Password=HMACResult, Salt=Salt1, Iterations=2000, dkLength=32
03. AES-Decrypt(16x): Buffer=0x18~0x43, Key=PKDF2Key1
04. PKDF2RFC2898: Password=0x18~0x43, Salt=Salt2 Iterations=2000, dkLength=32
05. AES-Encrypt(16x): Buffer=PKDF2Key2, Key=StaticAES
//-- Step 6 is only needed if SecblockSize-((SecblockSize SHR 4) SHL 4)) is <> 0
06. AES-Decrypt(16x): Buffer=SecblockSize-16, Key=(Encrypted PBKDF2 Key)
07. AES-Decrypt(16x): Buffer=Secblock, Key=(Encrypted PBKDF2 Key)
//-- Secblock is now fully decrypted! But there is still the last HMACSHA1
08. Double SHA1: Data1=InitialKey1, Data2=SecBlockLength(binary)
09. HMACSHA1: Key=DoubleSHA1, Data=Secblock
10. PKDF2RFC2898: Password=HMACResult+Initialkey2, Salt=Salt3+Integer7 Interations=2000, dkLength=32
11. AES-Decrypt(16x): Buffer=0x18~0x43, Key=StaticAES
12. AES-Decrypt(16x): Buffer=0x18~0x43, Key=PKDF2Key3
//-- The 0x18~0x43 is now fully decrypted too and reveals the HMACSHA1 to verify
```

### Credits

The decryption is based on [the crypto notes by Vulnavia](samples/Crypto.txt), and PS3 keys & [code by Red-EyeX32](samples/MaxPayne3SaveData.cs).
