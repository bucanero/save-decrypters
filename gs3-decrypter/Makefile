all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o gs3-decrypter main.c $(LDFLAGS)

clean:
	-rm -f gs3-decrypter gs3-decrypter.exe

