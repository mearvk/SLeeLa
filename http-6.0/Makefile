CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

ROUTER_DIR   = ../preferred-routers
RESOLVER_DIR = ../resolver
SLNET_DIR    = ../impl/core

INCLUDES = -I. -I..

# --- protocol objects ---
PROTO_OBJECTS = http6_protocol.o

# --- server ---
SERVER_SOURCES = \
	http6_server.c http6_main.c \
	http6_protocol.c \
	http_negotiation.c \
	$(ROUTER_DIR)/preferred_router.c \
	$(RESOLVER_DIR)/resolver.c \
	$(SLNET_DIR)/sleela_net.c

SERVER_TARGET = http6_server

ifeq ($(OS),Windows_NT)
SERVER_LDLIBS += -lws2_32
endif

.PHONY: all server proto syntax test server-test clean

all: proto server

proto: $(PROTO_OBJECTS)

http6_protocol.o: http6_protocol.c http6_protocol.h
	$(CC) $(CFLAGS) -I. -c http6_protocol.c -o $@

server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES) http6_server.h http6_protocol.h
	$(CC) $(CFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $@ $(SERVER_LDLIBS)

syntax:
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http6_protocol.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http6_server.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http6_main.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.cpp

test: all
	./$(SERVER_TARGET) --self-test

server-test: $(SERVER_TARGET)
	./$(SERVER_TARGET) --self-test

clean:
	rm -f $(PROTO_OBJECTS) $(SERVER_TARGET)
