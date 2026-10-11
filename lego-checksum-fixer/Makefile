# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o lego-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f lego-checksum-fixer lego-checksum-fixer.exe
