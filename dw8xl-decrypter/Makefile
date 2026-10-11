# DW8XL PS3 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o dw8xl-decrypter main.c $(LDFLAGS)

clean:
	-rm -f dw8xl-decrypter dw8xl-decrypter.exe
