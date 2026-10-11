# DOA5 Save Decrypter for PS3/PS4/Vita
# (c)2026 Bucanero

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o doa5-decrypter main.c $(LDFLAGS)

clean:
	-rm -f doa5-decrypter doa5-decrypter.exe
