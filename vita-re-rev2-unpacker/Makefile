# Resident Evil Revelations 2 Vita Save Unpacker
# (c)2020 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o vita-re-rev2-unpacker main.c $(LDFLAGS)

clean:
	-rm -f vita-re-rev2-unpacker vita-re-rev2-unpacker.exe
