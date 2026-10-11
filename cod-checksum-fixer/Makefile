# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o cod-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f cod-checksum-fixer cod-checksum-fixer.exe
