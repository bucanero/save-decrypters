all: unpacker

unpacker:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o di2-unpacker main.c $(LDFLAGS)

clean:
	-rm -f di2-unpacker di2-unpacker.exe

