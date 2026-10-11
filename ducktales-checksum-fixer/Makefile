# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o ducktales-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f ducktales-checksum-fixer ducktales-checksum-fixer.exe
