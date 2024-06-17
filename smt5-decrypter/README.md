# Shin Megami Tensei V PS4 Save Decrypter

A tool to decrypt Shin Megami Tensei V (SMT 5) PS4 save-games

```
USAGE: ./smt5-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Checksum

**Note:** the tool also updates the SHA1 integrity hash.

```
[Update SHA1 for Megaten5.ps4.sav (Required)]
set pointer:EOF+1
set range:0x000040,pointer
set [hash]:SHA1
write at 0x0000:[hash]
```

### Credits

The decryption is based on [smt5decryptorencryptor](https://www.rytek.me/projects/smt5decryptorencryptor/)
