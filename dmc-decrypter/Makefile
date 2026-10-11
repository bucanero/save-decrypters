# DmC: Devil May Cry PS3 Save Decrypter
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o dmc-decrypter main.c $(LDFLAGS)

clean:
	-rm -f dmc-decrypter dmc-decrypter.exe
