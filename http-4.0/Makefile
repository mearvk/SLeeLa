CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra

SOURCES = http4_protocol.c http4_packet_context.c http4_packet_payload.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean test

all: http4_selftest

http4_selftest: $(OBJECTS) http4_selftest.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

test: http4_selftest
	./http4_selftest

clean:
	rm -f $(OBJECTS) http4_selftest.o http4_selftest
