all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o lunar-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f lunar-checksum-fixer lunar-checksum-fixer.exe

