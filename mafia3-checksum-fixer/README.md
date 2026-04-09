# mafia3-checksum-fixer

A tool calculate the checksum of `Mafia III` savegames.

```
USAGE: ./mafia3-checksum-fixer filename
```

### Credits

Source:  
```
; by bucanero
; based on https://github.com/XB36Hazard/vantage-editor-mafia-iii/

:*

[Update CRC32 (Required)]
set pointer:EOF-3
set range:0x0000,pointer
set [hash]:CRC32
set [hash]:endian_swap
write next (0):[hash]
```

This tool was created by [hzh](https://github.com/hzhreal/).

