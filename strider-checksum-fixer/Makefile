all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o strider-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f strider-checksum-fixer strider-checksum-fixer.exe

