all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o kh3-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f kh3-checksum-fixer kh3-checksum-fixer.exe

