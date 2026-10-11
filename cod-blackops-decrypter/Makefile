# Call of Duty: Black Ops PS3 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o cod-blackops-decrypter main.c $(LDFLAGS)

clean:
	-rm -f cod-blackops-decrypter cod-blackops-decrypter.exe
