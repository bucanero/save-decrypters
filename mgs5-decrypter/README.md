# mgs5-tpp-decrypter

A tool to decrypt Metal Gear Solid V: The Phantom Pain PS3/PS4 save-games

```
USAGE: ./mgs5-tpp-decrypter [option] filename

OPTIONS        Explanation:
 -3            Auto-Decrypt/Encrypt PS3 File
 -4            Auto-Decrypt/Encrypt PS4 File
```

### MD5 Hash

**Note:** the tool also updates the MD5 integrity hash.

```
[Update MD5 checksum (Required)]
set range:0x0010,EOF+1
set [hash]:MD5
write at 0x0000:[hash]
```

### Credits

This tool is based (reversed) on the original Xbox [MGS 5: The Phantom Pain SecFixer](https://www.360haven.com/forums/showthread.php/45050-MGS-5-The-Phantom-Pain-SecFixer) by Philymaster
