CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS := $(shell pkg-config --libs openssl 2>/dev/null)

CFLAGS += $(OPENSSL_CFLAGS)

C_SOURCES = \
	crypto_openssl.c \
	crypto_identity_capsule.c \
	crypto_key_agreement.c \
	crypto_route_map.c \
	cryptographic_capsule_set.c \
	route_navigation.c \
	crypto_route_session.c

C_OBJECTS = $(C_SOURCES:.c=.o)

.PHONY: all clean selftest

all: selftest

selftest: crypto_selftest

crypto_selftest: $(C_OBJECTS) crypto_selftest.o
	$(CC) $(CFLAGS) -o $@ $^ $(OPENSSL_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

cpp-check: $(C_OBJECTS)
	$(CXX) $(CXXFLAGS) -I. -x c++ -fsyntax-only http3_crypto.hpp

clean:
	rm -f $(C_OBJECTS) crypto_selftest.o crypto_selftest
