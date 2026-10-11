# Checksum Fixer
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o dbz-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f dbz-checksum-fixer dbz-checksum-fixer.exe
