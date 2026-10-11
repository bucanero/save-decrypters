all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o mearth-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f mearth-checksum-fixer mearth-checksum-fixer.exe

