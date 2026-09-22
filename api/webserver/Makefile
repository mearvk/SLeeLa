CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -pedantic

all: libsleela-webserver.a

sleela_webserver.o: sleela_webserver.c sleela_webserver.h
	$(CC) $(CFLAGS) -c sleela_webserver.c -o $@

libsleela-webserver.a: sleela_webserver.o
	ar rcs $@ $<

clean:
	rm -f sleela_webserver.o libsleela-webserver.a
