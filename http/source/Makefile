CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS := $(shell pkg-config --libs openssl 2>/dev/null)
THREAD_LIBS := -pthread

CFLAGS += $(OPENSSL_CFLAGS)

C_SOURCES = \
	crypto_openssl.c \
	crypto_identity_capsule.c \
	crypto_key_agreement.c \
	crypto_route_map.c \
	cryptographic_capsule_set.c \
	route_navigation.c \
	crypto_route_session.c \
	key_distribution_server.c

C_OBJECTS = $(C_SOURCES:.c=.o)

.PHONY: all clean selftest kds cpp-check

all: selftest kds

selftest: crypto_selftest

crypto_selftest: $(C_OBJECTS) crypto_selftest.o
	$(CC) $(CFLAGS) -o $@ $^ $(OPENSSL_LIBS) $(THREAD_LIBS)

kds: key_distribution_server

key_distribution_server: $(C_OBJECTS) key_distribution_server_main.o
	$(CC) $(CFLAGS) -o $@ $^ $(OPENSSL_LIBS) $(THREAD_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

cpp-check: $(C_OBJECTS)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -I. -x c++ -fsyntax-only http3_crypto.hpp
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -I. -x c++ -fsyntax-only key_distribution_client.hpp

clean:
	rm -f $(C_OBJECTS) crypto_selftest.o key_distribution_server_main.o crypto_selftest key_distribution_server
