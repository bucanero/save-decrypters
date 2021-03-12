# re-remaster-decrypter

A tool to decrypt Resident Evil: HD Remaster and Resident Evil 0 (Zero) PS3 save-games.

```
USAGE: ./re-remaster-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Hash

**Note:** the tool doesn't fix the SHA1 integrity hash.

```
[Update SHA1 for DATA0.DAT (Required)]
set pointer:eof+1
set range:0x000040,pointer
set [hash]:SHA1
write at 0x00000C:[hash]
```

### Credits

This tool is based (reversed) on the original `re_remaster_save_decrypter` by Red-EyeX32
