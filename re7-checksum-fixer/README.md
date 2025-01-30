# re7-checksum-fixer

A tool calculate the checksum of a `Resident Evil 7: Biohazard` savefile.

```
USAGE: ./re7-checksum-fixer filename
```

#### Apollo script example

```
[Update Murmur3-32 checksum]
set pointer:EOF-3
set range:0,pointer
set [hash]:murmur3_32:0xFFFFFFFF
set [hash]:endian_swap
write next (0):[hash]
```

### Credits

This tool was created by [hzh](https://github.com/hzhreal/)
