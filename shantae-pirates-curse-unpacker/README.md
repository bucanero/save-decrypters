# shantae-pirates-curse-unpacker

A tool to unpack `Shantae and the Pirate's Curse` savegames.

```
USAGE: ./shantae-pirates-curse-unpacker [option] filename

OPTIONS        Explanation:
 -u            Unpack file
 -p            Pack file
```

### Credits

Source:  
```
; checksum by Zhaxxy

:*

[Decompress Save (Required)]
Decompress(*, -15)

[Compress Save (Required)]
Compress(*)

[Update JHash (Required)]
set range:0x0004,EOF+1
set [csum]:jhash
set [csum]:[csum]+0x4900DC7C
set [csum]:endian_swap
write at 0x0000:[csum]
```
Python version: [ScurseDecPS4](https://github.com/Zhaxxy/SCurseDecPS4).

This tool was created by [hzh](https://github.com/hzhreal/).

