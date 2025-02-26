# naughtydog-decrypter
A tool to decrypt Naughty Dog PS3 save-games (The Last of Us, Uncharted 2, Uncharted 3)

```
USAGE: ./naughtydog-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Hashes

**Note:** the tool also updates the CRC32 and SHA1 HMAC integrity hashes.

#### CRC32

```
[Update CRC32 checksum for USR-DATA (Required)]
set [block_length]:read(0x58C,4)
set [block_length]:[block_length]+0x587
set range:0x58C,[block_length]
set [hash]:crc32
write at 0x588:[hash]
```

#### SHA1 HMAC

```
[Update HMAC SHA1 checksum for USR-DATA (Required)]
set pointer:EOF-0x1F
set range:0x000008,pointer
set [hash]:hmac_sha1("xM;6X%/p^L/:}-5QoA+K8:F*M!~sb(WK<E%6sW_un0a[7Gm6,()kHoXY+yI/s;Ba")
set pointer:EOF-0x1F
write next (0):[hash]
```

### Credits

This tool is based (reversed) on the original `tlou_save_data_decrypter` by Red-EyeX32 and aerosoul94
