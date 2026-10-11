# Final Fantasy XIII PS3 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o ff13-decrypter main.c $(LDFLAGS)

clean:
	-rm -f ff13-decrypter ff13-decrypter.exe
