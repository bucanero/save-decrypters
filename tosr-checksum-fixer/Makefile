# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o tosr-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f tosr-checksum-fixer tosr-checksum-fixer.exe
