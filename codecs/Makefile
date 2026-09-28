CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude

all: build/libsleela_codecs.a

build:
	mkdir -p build

build/sleela_codecs.o: src/sleela_codecs.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/libsleela_codecs.a: build/sleela_codecs.o
	ar rcs $@ $<

test: build/test_registry
	./build/test_registry

build/test_registry: tests/test_registry.c build/sleela_codecs.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf build

.PHONY: all test clean
