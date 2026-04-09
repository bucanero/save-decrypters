# dstranding-checksum-fixer

A tool to fix the MD5 hash on Death Stranding (PS4) save-games.

### MD5

```
[Update MD5 Hash]
set [size]:read(0x1FC, 4)
set [size]:endian_swap
set [size]:[size]+0x1FF
set range:0x0200,[size]
set [hash]:md5
write at 0x01EC:[hash]
```

### Credits

This tool was created by [Bucanero](https://github.com/bucanero)
