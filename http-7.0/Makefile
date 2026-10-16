CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

ROUTER_DIR   = ../preferred-routers
RESOLVER_DIR = ../resolver
SLNET_DIR    = ../impl/core

INCLUDES = -I. -I..

PROTO_OBJECTS = http7_protocol.o

SERVER_SOURCES = \
	http7_server.c http7_main.c \
	http7_protocol.c \
	http_negotiation.c \
	$(ROUTER_DIR)/preferred_router.c \
	$(RESOLVER_DIR)/resolver.c \
	$(SLNET_DIR)/sleela_net.c

SERVER_TARGET = http7_server

ifeq ($(OS),Windows_NT)
SERVER_LDLIBS += -lws2_32
endif

.PHONY: all server proto syntax test server-test demo clean

all: proto server

proto: $(PROTO_OBJECTS)

http7_protocol.o: http7_protocol.c http7_protocol.h
	$(CC) $(CFLAGS) -I. -c http7_protocol.c -o $@

server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES) http7_server.h http7_protocol.h
	$(CC) $(CFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $@ $(SERVER_LDLIBS)

# Build the original demo executables (http70.c / http70.cpp).
demo: http70_demo http70_demo_cpp

http70_demo: http70.c
	$(CC) $(CFLAGS) -I. http70.c -o $@

http70_demo_cpp: http70.cpp
	$(CXX) $(CXXFLAGS) -I. http70.cpp -o $@

syntax:
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http7_protocol.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http7_server.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http7_main.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.cpp
	$(CC) $(CFLAGS) -I. -fsyntax-only http70.c
	$(CXX) $(CXXFLAGS) -I. -fsyntax-only http70.cpp

test: all
	./$(SERVER_TARGET) --self-test

server-test: $(SERVER_TARGET)
	./$(SERVER_TARGET) --self-test

clean:
	rm -f $(PROTO_OBJECTS) $(SERVER_TARGET) http70_demo http70_demo_cpp
