# Silent Hill 3 HD PS3 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o silent-hill3-decrypter main.c $(LDFLAGS)

clean:
	-rm -f silent-hill3-decrypter silent-hill3-decrypter.exe
