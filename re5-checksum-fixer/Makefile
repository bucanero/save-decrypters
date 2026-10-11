all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o re5-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f re5-checksum-fixer re5-checksum-fixer.exe

