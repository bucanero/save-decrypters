# Metal Gear Solid 5: The Phantom Pain PS3 Save Decrypter
# (c)2023 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o mgs5-tpp-decrypter main.c $(LDFLAGS)

clean:
	-rm -f mgs5-tpp-decrypter mgs5-tpp-decrypter.exe
