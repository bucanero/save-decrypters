# mgs-pw-decrypter

A tool to decrypt Metal Gear Solid: Peace Walker save-games (PS3 HD Edition and PSP)

The save type is auto-detected, so the same command works for both platforms:

- **PS3 (HD Edition)**: two encrypted blocks, the main save data plus a second
  block holding the online/comrade data.
- **PSP**: only the main save data block, the second block doesn't exist.

Note: this tool also updates the custom integrity checksums.

```
USAGE: ./mgs-pw-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Credits

This tool is based (reversed) on the original XBOX `MGS Peace Walker - SecFixer` by Philymaster
