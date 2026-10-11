all: checksum-fixer

checksum-fixer:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o mafia3-checksum-fixer main.c $(LDFLAGS)

clean:
	-rm -f mafia3-checksum-fixer mafia3-checksum-fixer.exe

