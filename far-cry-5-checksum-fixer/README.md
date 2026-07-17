# far-cry-5-checksum-fixer

A tool calculate the checksum of `Far Cry 5` save files.

```
USAGE: ./far-cry-5-checksum-fixer filename
```

### Apollo script example

```
;by bucanero
[Update CRC32 checksum (Required)]
set [size]:read(0x08, 4)
set [size]:endian_swap
set [size]:[size]+0x0F
set range:0x0010,[size]
set [csum]:CRC32
set [csum]:endian_swap
write at 0x0C:[csum]
```

### Credits

This tool is based on work by [Brotherguns](https://github.com/brotherguns)

This tool was created by [hzh](https://github.com/hzhreal/)

