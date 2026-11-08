# Makefile for the advantage secure transport & acquisition module.
#
# Builds:
#   transport                   - the main CLI
#   bulkfetch                   - rate-limited parallel bulk fetcher CLI
#   libadvantagetransport.a     - static library for linking into other apps
#
# Library sources:
#   keystore.c      - AES-256-GCM encrypted secret store
#   kex.c           - Diffie-Hellman / ECDH key exchange + HKDF
#   digest.cpp      - SHA-256 content addressing
#   uri.cpp         - URI parsing
#   http_client.cpp - TLS-verified HTTP(S) client
#   sftp_client.cpp - SSH/SFTP via OpenSSH (scp)
#   acquire.cpp     - idempotent content-addressed acquisition facade
#   bulk.cpp        - rate-limited parallel bulk fetcher
#
# Requires OpenSSL (libssl/libcrypto) development headers and, at runtime for
# ssh/sftp fetches, the OpenSSH `scp` binary. Bulk fetching uses threads.

CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -O2 -Wall -Wextra -std=c11
CXXFLAGS?= -O2 -Wall -Wextra -std=c++17
AR      ?= ar

OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS   := $(shell pkg-config --libs openssl 2>/dev/null)
ifeq ($(strip $(OPENSSL_LIBS)),)
OPENSSL_LIBS := -lssl -lcrypto
endif
LDLIBS_EXTRA := $(OPENSSL_LIBS) -lpthread

TARGET    := transport
BULK      := bulkfetch
LIB       := libadvantagetransport.a

# Objects that make up the reusable library (everything except the CLI mains).
LIB_C_OBJS   := keystore.o kex.o
LIB_CPP_OBJS := digest.o uri.o http_client.o sftp_client.o acquire.o bulk.o
LIB_OBJS     := $(LIB_C_OBJS) $(LIB_CPP_OBJS)

.PHONY: all clean test lib

all: $(TARGET) $(BULK) $(LIB)

lib: $(LIB)

# Static library
$(LIB): $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

# CLIs link against the static library.
$(TARGET): transport_cli.o $(LIB)
	$(CXX) $(CXXFLAGS) -o $@ transport_cli.o $(LIB) $(LDLIBS_EXTRA)

$(BULK): bulk_cli.o $(LIB)
	$(CXX) $(CXXFLAGS) -o $@ bulk_cli.o $(LIB) $(LDLIBS_EXTRA)

# C objects
keystore.o: keystore.c keystore.h
	$(CC) $(CFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

kex.o: kex.c kex.h
	$(CC) $(CFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

# C++ objects
digest.o: digest.cpp digest.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

uri.o: uri.cpp uri.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

http_client.o: http_client.cpp http_client.h uri.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

sftp_client.o: sftp_client.cpp sftp_client.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

acquire.o: acquire.cpp acquire.h digest.h uri.h http_client.h sftp_client.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

bulk.o: bulk.cpp bulk.h acquire.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

transport_cli.o: transport_cli.cpp acquire.h kex.h keystore.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

bulk_cli.o: bulk_cli.cpp bulk.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

# Safety + functional battery.
test: all
	@bash test.sh

clean:
	rm -f $(TARGET) $(BULK) $(LIB) *.o
