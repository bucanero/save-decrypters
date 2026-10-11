# The Legend of Heroes: Trails of Cold Steel IV - Save Unpacker
# (c)2026 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o trails-cs-unpacker main.c $(LDFLAGS)

clean:
	-rm -f trails-cs-unpacker trails-cs-unpacker.exe
