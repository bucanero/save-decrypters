# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o castlevania-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f castlevania-checksum-fixer castlevania-checksum-fixer.exe
