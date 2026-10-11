all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o re6-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f re6-checksum-fixer re6-checksum-fixer.exe

