# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o alien-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f alien-checksum-fixer alien-checksum-fixer.exe
