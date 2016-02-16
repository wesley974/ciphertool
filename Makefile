#	Makefile

CC = cc
SOURCE = ciphertool.c
CFLAGS = -g

all: ciphertool

install: ciphertool
	install -c -S -s  -o root -g bin  -m 555 ciphertool /usr/local/bin

clean:
	rm -f a.out [Ee]rrs mklog *.core y.tab.h  ciphertool ciphertool.o
