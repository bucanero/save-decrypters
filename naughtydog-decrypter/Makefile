# Naughty Dog PS3 Save Decrypter
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o naughtydog-decrypter main.c $(LDFLAGS)
clean:
	-rm -f naughtydog-decrypter naughtydog-decrypter.exe
