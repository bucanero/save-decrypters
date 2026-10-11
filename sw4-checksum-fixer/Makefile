# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o sw4-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f sw4-checksum-fixer sw4-checksum-fixer.exe
