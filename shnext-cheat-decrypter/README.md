# ShnExt Cheat Decrypter

A tool to decrypt/encrypt ShnExt (`.ShnExt`) cheat files

Also available as a web page, running entirely in your browser: **https://bucanero.github.io/save-decrypters/shnext-cheat-decrypter/web/**

Decrypting a file produces a plain JSON file, with the `ProcessName` and every `patchEntries` item decrypted.
Encrypting takes that JSON file and rebuilds the `.ShnExt` cheat file.

```
USAGE: ./shnext-cheat-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Credits

Based on the [onionHEN](https://github.com/aydencharles/onionHEN) ShnExt cheat parser
