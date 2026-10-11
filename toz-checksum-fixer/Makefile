# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o toz-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f toz-checksum-fixer toz-checksum-fixer.exe
