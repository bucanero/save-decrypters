# re-revelations2-decrypter

A tool to decrypt Resident Evil: Revelations 2 PS3 save-games.

```
USAGE: ./re-revelations2-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Hashes

**Note:** this tool also updates the DWADD/SHA1 integrity hashes.

#### DWADD

```
[Update DWADD for DATA0.DAT (Required)]
set [csum]:0
set [csum]:dwadd(0x000010,0x12758F)
write at 0x000008:[csum]
```

#### SHA1

```
[Update SHA1 for DATA0.DAT (Required)]
set range:0x000010,0x12758F
set [hash]:SHA1
write at 0x127590:[hash]
```

### Credits

This tool is based (reversed) on the original RE Revelations 2 Decrypter by forjador123
