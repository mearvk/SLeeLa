CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
all: check
check:
	$(CC) $(CFLAGS) -c sleela_http_api.c -o sleela_http_api.o
clean:
	rm -f sleela_http_api.o
