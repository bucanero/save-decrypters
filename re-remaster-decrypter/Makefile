# Resident Evil: HD Remaster PS3 Save Decrypter
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o re-remaster-decrypter main.c $(LDFLAGS)

clean:
	-rm -f re-remaster-decrypter re-remaster-decrypter.exe
