# trails-cs-unpacker

A tool to unpack The Legend of Heroes: Trails of Cold Steel save-games. The tool also fixes the custom CRC32 integrity hash.

Supported games:
- The Legend of Heroes: Trails of Cold Steel IV (PS4)
- The Legend of Heroes: Trails of Cold Steel II (PS Vita)

```
USAGE: ./trails-cs-unpacker [option] filename

OPTIONS        Explanation:
 -u            Unpack File
 -p            Pack File
```

### Checksum

**Note:** on PS4 the CRC is calculated over the decompressed data and stored at offset `0x0C`.

### Credits

The compression is based on [SenPatcher](https://github.com/AdmiralCurtiss/SenPatcher) by AdmiralCurtiss
