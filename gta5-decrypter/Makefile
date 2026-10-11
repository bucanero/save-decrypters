# GTA 5 PS3 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o gta5-decrypter main.c $(LDFLAGS)

clean:
	-rm -f gta5-decrypter gta5-decrypter.exe
