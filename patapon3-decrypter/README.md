# Patapon 3 save decrypter

A tool to decrypt/encrypt Patapon 3 PSP save-games, including the security hash
that the game checks when loading a save.

```
USAGE: ./patapon3-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File (updates the save hash)
```

### Save format

The whole file is encrypted with **Camellia-256 ECB**, key
`SVsyE56pniSRS9dIPTiE8ApDaUnN0AEa`.

Once decrypted, the last 0x28 bytes hold the integrity trailer:

| Offset       | Size | Description                             |
|--------------|------|-----------------------------------------|
| `0`          | ...  | save data                               |
| `size-0x28`  | 4    | length of the hashed region (u32 LE)    |
| `size-0x24`  | 20   | HMAC-SHA1 of `data[0 .. length]`        |
| `size-0x10`  | 16   | padding                                 |

### HMAC-SHA1 Hash

The hash is an **HMAC-SHA1** over the *decrypted* data, using the 32-byte key
`CyZ2o3SPBqMWVVvUVt4WwJOBpq9hCjNq`. Both keys are built byte by byte at runtime
through a jump table, so neither shows up as a plain string in the EBOOT.

The game validates it in `sceCheck`-style wrappers around `0x08A35E78`
(the routine the well-known "Disable Save Hash Check" CWCheat patches), and
regenerates it on save from `0x08A34F40`.

### Credits

This tool is based on [libP3Hash](https://github.com/owodzeg/libP3Hash) by Owocek.
