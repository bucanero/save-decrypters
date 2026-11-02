# Dead or Alive 6 PS4 Save Decrypter

A tool to decrypt Dead or Alive 6 (CUSA12153) PS4 save-games (`SYSTEMDATA`).

```
USAGE: ./doa6-decrypter [option] filename [savename]

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

The AES key and IV are derived from the save file name, which defaults to the
input file name up to the first `.` (e.g. `SYSTEMDATA.enc` -> `SYSTEMDATA`).
Pass `savename` to override it.

### Encryption

AES-256-CBC with PKCS#7 padding over the whole file. Key and IV depend only on
the file name:

```
R   = MD5(name zero padded to 32 bytes)
KEY = uppercase hex of R: non A-F chars first, then A-F chars (32 bytes)
IV  = bytes of R: odd values first, then even values (16 bytes)
```

For `SYSTEMDATA`:

```
KEY = "485922144619578157699DAABBBDDDBA"
IV  = 4D85BB6195BD1D5D7B69A9922A1A4478
```

### Checksum

No checksum has been found in the decrypted data.

### Credits

Encryption details from [apollo-patches#33](https://github.com/bucanero/apollo-patches/issues/33)
