# ps4-re4r-decrypter

A tool to decrypt Resident Evil 2/4 Remake savegames.

```
USAGE: ./ps4-re4r-decrypter [option] [game] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File

GAME TYPE      Explanation:
 -2            Resident Evil 2 Remake
 -4            Resident Evil 4 Remake
 ```

### Checksum

**Note:** the tool also updates the Murmur3-32 integrity hash.

```
[Update Murmur3-32 checksum]
set pointer:EOF-3
set range:0,pointer
set [hash]:murmur3_32:0xFFFFFFFF
set [hash]:endian_swap
write next (0):[hash]
```

### Credits

This tool was made possible through the efforts of [Alfizari](https://github.com/alfizari), [Brotherguns5624](https://github.com/Brotherguns5624), and [hzh](https://github.com/hzhreal/)
