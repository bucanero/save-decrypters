# dmc-ps3save-decrypter

A tool to decrypt DmC: Devil May Cry PS3 save-games.

```
USAGE: ./dmc-ps3save-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Hash

**Note:** the tool doesn't fix the SHA1 integrity hash.

```
[Update SHA1 for PROFILE (Required)]
set pointer:read(0x18, 4)
set range:0x00001C,pointer+0x1B
set [hash]:SHA1
write at 0x4:[hash]
```

### Credits

This tool is based (reversed) on the original Devil May Cry 5 Crypto Toolkit by fairchild
