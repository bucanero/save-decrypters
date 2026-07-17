# re5-checksum-fixer

A tool calculate the checksum of `Resident Evil 5` save files.

```
USAGE: ./re5-checksum-fixer filename
```

### Credits

Source:  
```
;by bucanero
[Update DWADD LE Checksum (Required)]
set [csum]:dwadd_le(0x38, 0x52D8)
set [csum]:endian_swap
write at 0x30:[csum]
```

This tool was created by [hzh](https://github.com/hzhreal/)

