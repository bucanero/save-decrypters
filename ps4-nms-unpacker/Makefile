all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o ps4-nms-unpacker main.c $(LDFLAGS)

clean:
	-rm -f ps4-nms-unpacker ps4-nms-unpacker.exe