# crypto/Makefile — SLeeLa reference cryptography library.
#
# Builds libslcrypto.a and the C++17 façade's Known-Answer Test executable.
# These are reference implementations, not production-hardened cryptography;
# see README.md for the security boundary and PQ-INTEGRATION.md for PQ status.
#
# Tool overrides: make CC=clang CXX=clang++ AR=llvm-ar
# Windows: use GNU Make with a POSIX-compatible shell/toolchain (for example,
# MSYS2/MinGW-w64 or clang). This is not an nmake makefile.

CC       ?= cc
CXX      ?= c++
AR       ?= ar
CFLAGS   ?= -std=c11 -O2 -Wall -Wextra -Iinclude
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Iinclude

SRC := src/sha2.c src/sha3.c src/aes.c src/mac_kdf.c src/pq.c
OBJ := $(SRC:.c=.o)
LIB := libslcrypto.a
TEST_BIN := kat_test
HEADERS := include/slcrypto.h

.PHONY: all test clean help

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $(OBJ)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): test/kat_test.cpp cpp/slcrypto.hpp $(HEADERS) $(LIB)
	$(CXX) $(CXXFLAGS) test/kat_test.cpp $(LIB) -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	$(RM) $(OBJ) $(LIB) $(TEST_BIN)

help:
	@printf '%s\n' \
	  'SLeeLa crypto build targets:' \
	  '  all   Build libslcrypto.a (default)' \
	  '  test  Build and run Known-Answer Tests' \
	  '  clean Remove generated objects, library, and test executable' \
	  '  help  Show this help' \
	  'Overrides: CC, CXX, AR, CFLAGS, CXXFLAGS'
