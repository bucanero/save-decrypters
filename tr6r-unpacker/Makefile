# Tomb Raider VI Remaster - Save Unpacker
# (c)2026 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o tr6r-unpacker main.c $(LDFLAGS)

clean:
	-rm -f tr6r-unpacker tr6r-unpacker.exe
