# Makefile for the advantage secure transport & acquisition module.
#
# Builds the `transport` CLI, which links:
#   keystore.c      - AES-256-GCM encrypted secret store
#   kex.c           - Diffie-Hellman / ECDH key exchange + HKDF
#   digest.cpp      - SHA-256 content addressing
#   uri.cpp         - URI parsing
#   http_client.cpp - TLS-verified HTTP(S) client
#   sftp_client.cpp - SSH/SFTP via OpenSSH (scp)
#   acquire.cpp     - idempotent content-addressed acquisition facade
#
# Requires OpenSSL (libssl/libcrypto) development headers and, at runtime for
# ssh/sftp fetches, the OpenSSH `scp` binary.

CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -O2 -Wall -Wextra -std=c11
CXXFLAGS?= -O2 -Wall -Wextra -std=c++17

OPENSSL_CFLAGS := $(shell pkg-config --cflags openssl 2>/dev/null)
OPENSSL_LIBS   := $(shell pkg-config --libs openssl 2>/dev/null)
ifeq ($(strip $(OPENSSL_LIBS)),)
OPENSSL_LIBS := -lssl -lcrypto
endif

TARGET := transport

C_OBJS   := keystore.o kex.o
CPP_OBJS := digest.o uri.o http_client.o sftp_client.o acquire.o transport_cli.o
OBJS     := $(C_OBJS) $(CPP_OBJS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(OPENSSL_LIBS)

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

transport_cli.o: transport_cli.cpp acquire.h kex.h keystore.h
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
