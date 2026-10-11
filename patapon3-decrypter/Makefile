# Patapon 3 Data Decrypter
# (c)2023 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o patapon3-decrypter main.c $(LDFLAGS)

clean:
	-rm -f patapon3-decrypter patapon3-decrypter.exe
