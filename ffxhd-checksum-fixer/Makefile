# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o ffxhd-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f ffxhd-checksum-fixer ffxhd-checksum-fixer.exe
