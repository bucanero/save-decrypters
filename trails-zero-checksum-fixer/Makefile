# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o trails-zero-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f trails-zero-checksum-fixer trails-zero-checksum-fixer.exe
