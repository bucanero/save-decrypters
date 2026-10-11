# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o ps4-digimon-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f ps4-digimon-checksum-fixer ps4-digimon-checksum-fixer.exe
