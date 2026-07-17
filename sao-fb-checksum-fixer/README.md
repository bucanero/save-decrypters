# sao-fb-checksum-fixer

A tool calculate the checksum of `Sword Art Online: Fatal Bullet` save files.

```
USAGE: ./sao-fb-checksum-fixer filename
```

### Apollo script example

```
;by bucanero
[python:Update HMAC-SHA1 Hash (Required)]
import sao_fb

if sao_fb.sao_fb_checksum(savedata):
    print("Hash updated OK")
else:
    print("Hash not updated")
```

### Credits

This tool is based on [sao_check.py](samples/sao_check.py) by [alfizari](https://github.com/alfizari/)

This tool was created by [hzh](https://github.com/hzhreal/)

