# kh3-checksum-fixer

A tool calculate the checksum of `Kingdom Hearts III` savegames.

```
USAGE: ./kh3-checksum-fixer filename
```

### Credits

Source:  
```
; by bucanero
; based on https://github.com/Xeeynamo/KingdomSaveEditor/

:*

[Update CRC32 (Required)]
set range:0x0010,EOF+1
set [hash]:CRC32
set [hash]:endian_swap
write at 0x0C:[hash]
```

This tool was created by [hzh](https://github.com/hzhreal/).

