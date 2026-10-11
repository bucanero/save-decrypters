all: unpacker

unpacker:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o dl2-unpacker main.c $(LDFLAGS)

clean:
	-rm -f dl2-unpacker dl2-unpacker.exe

