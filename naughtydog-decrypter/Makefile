# Naughty Dog PS3 Save Decrypter
# (c)2020 Damian Parrino

all: decrypter

ifeq ($(PS4),1)
    ARG_PS4 = -D__PS4__
endif

decrypter:
	gcc -D _GNU_SOURCE $(ARG_PS4) -o naughtydog-ps3save-decrypter main.c

clean:
	-rm -f naughtydog-ps3save-decrypter
