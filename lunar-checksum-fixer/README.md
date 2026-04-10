# lunar-checksum-fixer

A tool calculate the checksum of `LUNAR Remastered Collection` savegames.

```
USAGE: ./lunar-checksum-fixer [type] filename

TYPE           Explanation:
 -1            LUNAR 1
 -2            LUNAR 2
```

### Credits

Source:  
```
; checksum by Bucanero

:card00LUNAR*\GAME.BIN
[Group:----< Lunar 1 >----]

[Lunar 1 DWADD_LE Checksum (Required)]
set [csum]:0
set pointer:EOF-3
set [csum]:dwadd_le(0x0000,pointer)
set [csum]:endian_swap
write next 0x0000:[csum]

:card*SAVEDATA\GAME.BIN
[Group:----< Lunar 2 >----]

[Lunar 2 ADD Checksum (Required)]
set [csum]:0
set [csum]:add(0x0300,EOF)
set [csum]:endian_swap
write at 0x0204:[csum]
```

This tool was created by [hzh](https://github.com/hzhreal/).

