# naughtydog-decrypter
A tool to decrypt Naughty Dog PS3 save-games (The Last of Us, Uncharted 2, Uncharted 3) and  
PS4 save-games (The Last of Us,  Uncharted 4, The Lost Legacy, The Last of Us Part 2, The Nathan Drake collection).

```
USAGE: ./naughtydog-decrypter [option] [game] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
GAME TYPE      Explanation:
 -A            PS3: The Last of Us, Uncharted 2, Uncharted 3
 -A            PS4: The Last of Us, Uncharted 2 Remastered, Uncharted 3 Remastered, Uncharted 4, The Lost Legacy
 -B            PS4: The Last of Us Part 2
 -C            PS4: The Nathan Drake Collection
```

### Hashes

**Note:** the tool also updates the CRC32 and SHA1 HMAC integrity hashes.

#### CRC32

Game type A:

PS3:
```
[Update CRC32 checksum for USR-DATA (Required)]
set [block_length]:read(0x58C,4)
set [block_length]:[block_length]+0x587
set range:0x58C,[block_length]
set [hash]:crc32
write at 0x588:[hash]
```
PS4:
```
[Update CRC32 checksum for USR-DATA (Required)]
set [block_length]:read(0x58C,4)
set [block_length]:endian_swap
set [block_length]:[block_length]+0x587
set range:0x58C,[block_length]
set [hash]:crc32
set [hash]:endian_swap
write at 0x588:[hash]
```

Game type B:

```
[Update CRC32C checksum for USR-DATA (Required)]
set crc_bandwith:32
set crc_polynomial:0x1EDC6F41
set crc_initial_value:0xFFFFFFFF
set crc_output_xor:0xFFFFFFFF
set crc_reflection_input:1
set crc_reflection_output:1
set [block_length]:read(0x594,4)
set [block_length]:endian_swap
set [block_length]:[block_length]+0x58F
set range:0x594,[block_length]
set [hash]:crc
set [hash]:endian_swap
write at 0x590:[hash]
```

Game type C:

```
[Update CRC32 checksum for USR-DATA (Required)]
set [block_length]:read(0x590,4)
set [block_length]:endian_swap
set [block_length]:[block_length]+0x58B
set range:0x590,[block_length]
set [hash]:crc32
set [hash]:endian_swap
write at 0x58C:[hash]
```

#### SHA1 HMAC

Game type A:

```
[Update HMAC SHA1 checksum for USR-DATA (Required)]
set pointer:EOF-0x1F
set range:0x000008,pointer
set [hash]:hmac_sha1("xM;6X%/p^L/:}-5QoA+K8:F*M!~sb(WK<E%6sW_un0a[7Gm6,()kHoXY+yI/s;Ba")
set pointer:EOF-0x1F
write next (0):[hash]
```
Game type B:

```
[Update HMAC SHA1 checksum for USR-DATA (Required)]
set pointer:EOF-0x1F
set range:0x000010,pointer
set [hash]:hmac_sha1("xM;6X%/p^L/:}-5QoA+K8:F*M!~sb(WK<E%6sW_un0a[7Gm6,()kHoXY+yI/s;Ba")
set pointer:EOF-0x1F
write next (0):[hash]
```

Game type C:

```
[Update HMAC SHA1 checksum for USR-DATA (Required)]
set pointer:EOF-0x1F
set range:0x00000C,pointer
set [hash]:hmac_sha1("xM;6X%/p^L/:}-5QoA+K8:F*M!~sb(WK<E%6sW_un0a[7Gm6,()kHoXY+yI/s;Ba")
set pointer:EOF-0x1F
write next (0):[hash]
```

### Credits

This tool is based (reversed) on the original `tlou_save_data_decrypter` by Red-EyeX32 and aerosoul94
