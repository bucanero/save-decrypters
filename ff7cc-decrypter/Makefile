# FF7 CC PS4 Save Decrypter
# (c)2026 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o ff7cc-decrypter main.c $(LDFLAGS)

clean:
	-rm -f ff7cc-decrypter ff7cc-decrypter.exe
