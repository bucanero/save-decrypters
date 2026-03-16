# tosr-checksum-fixer

A tool to fix the checksum on Tales of Symphonia Remastered (PS4) save-games.

### DWADD checksum

```
[Update DWADD LE Checksum (Required)]
set [csum]:dwadd_le(0x0004,0x25F8)
set [csum]:endian_swap
write at 0x00:[csum]
```

### Credits

This tool is based on [SymphoniaSaveEdit](https://github.com/Meth962/SymphoniaSaveEdit) by [Meth962](https://github.com/Meth962)
