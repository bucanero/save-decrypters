all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o sao-fb-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f sao-fb-checksum-fixer sao-fb-checksum-fixer.exe

