CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -pedantic
all: libsleela-database.a
sleela_database.o: sleela_database.c sleela_database.h
	$(CC) $(CFLAGS) -c sleela_database.c -o $@
libsleela-database.a: sleela_database.o
	ar rcs $@ $<
clean:
	rm -f sleela_database.o libsleela-database.a
