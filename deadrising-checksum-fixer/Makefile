# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o deadrising-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f deadrising-checksum-fixer deadrising-checksum-fixer.exe
