# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o kh25-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f kh25-checksum-fixer kh25-checksum-fixer.exe
