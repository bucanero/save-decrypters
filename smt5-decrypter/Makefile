# Shin Megami Tensei 5 PS4 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o smt5-decrypter main.c $(LDFLAGS)

clean:
	-rm -f smt5-decrypter smt5-decrypter.exe
