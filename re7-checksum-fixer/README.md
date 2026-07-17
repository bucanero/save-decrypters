# re7-checksum-fixer

A tool calculate the checksum of `Monster Hunter Rise`, `Resident Evil 7: Biohazard`, `Resident Evil 3 Remake`, `Resident Evil Resistance`, and `Resident Evil Village` save files.

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

The `Monster Hunter Rise` entry was contributed by [alfizari](https://github.com/alfizari)

This tool was created by [hzh](https://github.com/hzhreal/)

