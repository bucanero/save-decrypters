# Dead or Alive 5 Save Decrypter

A tool to decrypt Dead or Alive 5 PS3, PS4, and PS Vita save-games.

Supports both Dead or Alive 5 and Dead or Alive 5 Last Round saves. The save format (PS3 big-endian, or PS4/Vita little-endian) is detected automatically.

```
USAGE: ./doa5-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Checksum

This tool also updates the custom ADD checksums.

### Credits

This tool is based on the [`doa5.py`](samples/doa5.py) Python script by [alfizari](https://github.com/alfizari)
