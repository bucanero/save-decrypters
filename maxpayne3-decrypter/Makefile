# Max Payne 3 PS3 Save Decrypter

all: decrypter decompressor

decrypter:
	gcc -D _GNU_SOURCE -o maxpayne3-decrypter main.c

decompressor:
	gcc -o decompressor decompressor.c

clean:
	-rm -f maxpayne3-decrypter decompressor
