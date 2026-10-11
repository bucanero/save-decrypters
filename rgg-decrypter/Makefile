# RGG decrypter
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o rgg-decrypter main.c $(LDFLAGS)

clean:
	-rm -f rgg-decrypter rgg-decrypter.exe
