# crypto/Makefile — SLeeLa national-grade cryptography reference library.
#
# Builds the C library (libslcrypto.a), the C++ header façade is header-only.
# `make test` builds and runs the Known-Answer Tests against FIPS/NIST/RFC
# vectors.

CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -std=c11   -O2 -Wall -Wextra -Iinclude
CXXFLAGS?= -std=c++17 -O2 -Wall -Wextra -Iinclude

SRC   := src/sha2.c src/sha3.c src/aes.c src/mac_kdf.c src/pq.c
OBJ   := $(SRC:.c=.o)
LIB   := libslcrypto.a

.PHONY: all test clean
all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(LIB)
	$(CXX) $(CXXFLAGS) test/kat_test.cpp $(LIB) -o kat_test
	./kat_test

clean:
	rm -f $(OBJ) $(LIB) kat_test
