# re-revelations2-decrypter

A tool to decrypt Resident Evil: Revelations 2 PS4 save-games.

```
USAGE: ./re-revelations2-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Hashes

**Note:** this tool also updates the SHA1 integrity hash.

#### SHA1

```
[Update SHA1 for DATA.DAT (Required)]
set range:0x000020,0x1284BF
set [hash]:SHA1
write at 0x1284C0:[hash]
```

(SHA1 hash is stored as 5 DWord little-endian values)

### Credits

This tool is based on the [PS3 RE Revelations 2 Decrypter](../re-revelations2-decrypter)
