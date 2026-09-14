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

# Protocol core: the HTTP 3.0 application data model + flow (spec §4/§5/§7/§9/§19).
# Pure data; no OpenSSL dependency, so it builds and tests independently of the
# cryptographic substrate above.
PROTO_SOURCES = \
	http3_envelope.c \
	http3_naming.c \
	http3_pipeline.c

PROTO_OBJECTS = $(PROTO_SOURCES:.c=.o)

.PHONY: all clean selftest kds cpp-check proto pipeline-demo demo test

all: selftest kds proto

selftest: crypto_selftest

crypto_selftest: $(C_OBJECTS) crypto_selftest.o
	$(CC) $(CFLAGS) -o $@ $^ $(OPENSSL_LIBS) $(THREAD_LIBS)

kds: key_distribution_server

key_distribution_server: $(C_OBJECTS) key_distribution_server_main.o
	$(CC) $(CFLAGS) -o $@ $^ $(OPENSSL_LIBS) $(THREAD_LIBS)

# Build the protocol-core objects and the end-to-end flow demo.
proto: pipeline-demo

pipeline-demo: http3_pipeline_demo
http3_pipeline_demo: $(PROTO_OBJECTS) http3_pipeline_demo.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

cpp-check: $(C_OBJECTS)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -I. -x c++ -fsyntax-only http3_crypto.hpp
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -I. -x c++ -fsyntax-only key_distribution_client.hpp

# Run the C flow demo and the Python flow tests (the spec data-flow layer).
demo: http3_pipeline_demo
	./http3_pipeline_demo

test: demo
	python3 test_http3_flow.py

clean:
	rm -f $(C_OBJECTS) $(PROTO_OBJECTS) crypto_selftest.o key_distribution_server_main.o \
	      http3_pipeline_demo.o crypto_selftest key_distribution_server http3_pipeline_demo
