# re-revelations2-decrypter

A tool to decrypt Resident Evil: Revelations 2 PS3 and PS4 save-games.

The save format (PS3 or PS4) is auto-detected from the file fingerprint at offset `0x10`.

```
USAGE: ./re-revelations2-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Hashes

**Note:** this tool also updates the DWADD/SHA1 integrity hashes.

#### PS3 Hashes

##### DWADD

```
[Update DWADD for DATA0.DAT (Required)]
set [csum]:0
set [csum]:dwadd(0x000010,0x12758F)
write at 0x000008:[csum]
```

##### SHA1

```
[Update SHA1 for DATA0.DAT (Required)]
set range:0x000010,0x12758F
set [hash]:SHA1
write at 0x127590:[hash]
```

#### PS4 Hashes

##### DWADD LE

```
[Update DWADD LE for DATA0.DAT (Required)]
set [csum]:0
set [csum]:dwadd_le(0x000020,0x19490)
set [csum]:dwadd_le(0x0194A0,0x1284C0)
set [csum]:endian_swap
write at 0x000018:[csum]
```

##### SHA1

```
[Update SHA1 for DATA0.DAT (Required)]
set range:0x000020,0x1284BF
set [hash]:SHA1
write at 0x1284C0:[hash]
```

(SHA1 hash is stored as 5 DWord little-endian values)

### Credits

This tool is based (reversed) on the original RE Revelations 2 Decrypter by forjador123
