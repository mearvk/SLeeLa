CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra

PROTOCOL_SOURCES = http4_protocol.c http4_packet_context.c http4_packet_payload.c
SEGMENTATION_SOURCES = http4_protocol.c http4_segmentation.c http4_reassembly.c
PROTOCOL_OBJECTS = $(PROTOCOL_SOURCES:.c=.o)
SEGMENTATION_OBJECTS = $(SEGMENTATION_SOURCES:.c=.o)

.PHONY: all clean test protocol-test segmentation-test

all: http4_selftest http4_segmentation_selftest

http4_selftest: $(PROTOCOL_OBJECTS) http4_selftest.o
	$(CC) $(CFLAGS) -o $@ $^

http4_segmentation_selftest: $(SEGMENTATION_OBJECTS) http4_segmentation_selftest.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

test: all
	./http4_selftest
	./http4_segmentation_selftest

protocol-test: http4_selftest
	./http4_selftest

segmentation-test: http4_segmentation_selftest
	./http4_segmentation_selftest

clean:
	rm -f $(PROTOCOL_OBJECTS) $(SEGMENTATION_OBJECTS) http4_selftest.o \
		http4_segmentation_selftest.o http4_selftest http4_segmentation_selftest
