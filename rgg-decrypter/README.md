# Like a Dragon: Ishin (PS4) save decrypter

A tool to decrypt/encrypt Like a Dragon: Ishin (PS4) save-games.

### CRC32 checksum

This tool also updates the CRC32 checksum when encrypting a save file.

```
[Update CRC32 (Required)]
set pointer:EOF-0x0F
set range:0x00,pointer
set [crc]:crc32
set [crc]:endian_swap
write next 0x08:[crc]
```

### Credits

This tool is based on [rgg_save_tool_py](https://github.com/GarnetSunset/rgg_save_tool_py)
