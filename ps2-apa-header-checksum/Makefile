# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o ps2-apa-header-checksum main.c $(LDFLAGS)

clean:
	-rm -f ps2-apa-header-checksum ps2-apa-header-checksum.exe
