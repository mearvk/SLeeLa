CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror
all: build/sleela-audio-c
build:; mkdir -p build
build/sleela_audio.o: src/sleela_audio.c include/sleela_audio.h | build
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@
build/sleela-audio-c: build/sleela_audio.o
	$(CC) $(CFLAGS) $< -lm -o $@
test: all
	./build/sleela-audio-c >/dev/null 2>&1 || test $$? -eq 0
clean:; rm -rf build
.PHONY: all test clean
