# nfsrivals-ps3save-decrypter

A tool to decrypt Need For Speed: Rivals PS3 save-games. The tool also fixes the custom CRC32 integrity hash.

```
USAGE: ./nfsrivals-ps3save-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Checksum

**Note:** the checksum is calculated over the encrypted data.

```
[Update Custom CRC32 for USR-DATA (Required)]
set crc_bandwidth:32
set crc_polynomial:0x4C11DB7
set crc_initial_value:0xE195D3B7
set crc_output_xor:0xFFFFFFFF
set crc_reflection_input:1
set crc_reflection_output:1
set range:0x00004,eof+1
set [hash]:crc
write at 0x0:[hash]
```

### Credits

This tool is based (reversed) on the original `need_for_speed_rivals_save_decrypter` by Red-EyeX32
