CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror
AR ?= ar
ARFLAGS ?= rcs

all: build/libsleela_audio_c.a

build:
	mkdir -p build

build/sleela_audio.o: src/sleela_audio.c include/sleela_audio.h | build
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

build/libsleela_audio_c.a: build/sleela_audio.o
	$(AR) $(ARFLAGS) $@ $<

test: all
	$(CC) $(CFLAGS) -Iinclude -fsyntax-only src/sleela_audio.c

clean:
	rm -rf build

.PHONY: all test clean
