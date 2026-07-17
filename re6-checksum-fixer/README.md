# re6-checksum-fixer

A tool calculate the checksum of `Resident Evil 6` save files.

```
USAGE: ./re6-checksum-fixer filename
```

### Apollo script example

```
[Update DWADD LE Checksum (Required)]
set [csum]:dwadd_le(0x34, EOF+1)
set [csum]:endian_swap
write at 0x30:[csum]
```

### Credits

This tool was created by [hzh](https://github.com/hzhreal/)

