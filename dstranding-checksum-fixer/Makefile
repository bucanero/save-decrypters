# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o dstranding-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f dstranding-checksum-fixer dstranding-checksum-fixer.exe
