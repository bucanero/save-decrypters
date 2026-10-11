# Resident Evil Revelations 2 PS3 Save Decrypter
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o re-revelations2-decrypter main.c $(LDFLAGS)

clean:
	-rm -f re-revelations2-decrypter re-revelations2-decrypter.exe
