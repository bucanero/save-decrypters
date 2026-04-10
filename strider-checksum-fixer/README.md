# strider-checksum-fixer

A tool calculate the checksum of `Strider` savegames.

```
USAGE: ./strider-checksum-fixer filename
```

### Credits

Source:  
```
:*

;checksum found by chaoszage
[Update CRC32 for SAVEGAME (Required)]
set range:0x000004,0x077FF
set [hash]:CRC32
write at 0x000000:[hash]
set [hash]:0
;set range:0x007804,0x0FFFFF
set pointer:eof+1
set range:0x007804,pointer
set [hash]:CRC32
write at 0x007800:[hash]
```

This tool was created by [hzh](https://github.com/hzhreal/).

