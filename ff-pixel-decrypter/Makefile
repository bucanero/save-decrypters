# FF Pixel Remaster PS4 Save Decrypter
# (c)2026 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o ff-pixel-decrypter main.c $(LDFLAGS)

clean:
	-rm -f ff-pixel-decrypter ff-pixel-decrypter.exe
