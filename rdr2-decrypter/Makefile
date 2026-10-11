# RDR2 PS4 Save Decrypter
# (c)2023 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o rdr2-decrypter main.c $(LDFLAGS)

clean:
	-rm -f rdr2-decrypter rdr2-decrypter.exe
