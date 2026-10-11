# Diablo III PS3 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o diablo3-decrypter main.c $(LDFLAGS)

clean:
	-rm -f diablo3-decrypter diablo3-decrypter.exe
