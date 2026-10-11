# Monster Hunter 2/3 PSP Save Decrypter
# (c)2023 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o monsterhunter-psp-decrypter main.c $(LDFLAGS)

clean:
	-rm -f monsterhunter-psp-decrypter monsterhunter-psp-decrypter.exe
