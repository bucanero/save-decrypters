# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o digimon-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f digimon-checksum-fixer digimon-checksum-fixer.exe
