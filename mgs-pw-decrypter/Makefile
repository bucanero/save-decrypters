# MGS PS3 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o mgs-pw-decrypter main.c $(LDFLAGS)

clean:
	-rm -f mgs-pw-decrypter mgs-pw-decrypter.exe
