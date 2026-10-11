# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o tears2-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f tears2-checksum-fixer tears2-checksum-fixer.exe
