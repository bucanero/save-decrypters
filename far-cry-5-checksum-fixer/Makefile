all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o far-cry-5-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f far-cry-5-checksum-fixer far-cry-5-checksum-fixer.exe

