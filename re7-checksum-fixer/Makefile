all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o re7-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f re7-checksum-fixer re7-checksum-fixer.exe