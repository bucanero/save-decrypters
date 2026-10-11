# NFS Undercover PS3 Save Decrypter
# (c)2021 Damian Parrino

all: decrypter

decrypter:
	$(CC) $(CFLAGS) -D _GNU_SOURCE -o nfs-undercover-decrypter main.c $(LDFLAGS)

clean:
	-rm -f nfs-undercover-decrypter nfs-undercover-decrypter.exe
