# re-revelations2-unpacker

A tool to unpack Resident Evil: Revelations 2 PS Vita save-games.

```
USAGE: ./vita-re-rev2-unpacker [option] filename

OPTIONS        Explanation:
 -u            Unpack File
 -p            Pack File
```

### Hashes

**Note:** this tool also updates the DWADD checksum and SHA1 integrity hash.

#### DWADD (Little-endian)

```
[Update DWADD checksum (Required)]
set [csum]:0
set [csum]:dwadd_le(0x000010,0x1279AF)
set [csum]:endian_swap
write at 0x000008:[csum]
```

#### SHA1

```
[Update SHA1 hash (Required)]
set pointer:eof-0x1F
set range:0x0010,pointer
set [hash]:SHA1
write next (0):[hash]
set range:pointer,pointer+0x14
endian_swap(4)
```

(SHA1 hash is stored as 5 DWord little-endian values)

### Credits

Unpacker Tool by [Bucanero](https://github.com/bucanero)
