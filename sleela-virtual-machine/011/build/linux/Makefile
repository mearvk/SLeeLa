CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -I../../include
all:
	$(CC) $(CFLAGS) -c ../../src/slvm11_filesystem_module.c -o slvm11_filesystem_module.o
	ar rcs libslvm11fs.a slvm11_filesystem_module.o
clean:
	rm -f slvm11_filesystem_module.o libslvm11fs.a
