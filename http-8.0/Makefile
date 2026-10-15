CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

ROUTER_DIR   = ../preferred-routers
RESOLVER_DIR = ../resolver
SLNET_DIR    = ../impl/core
INCLUDES     = -I. -I..

PROTO_OBJECTS = http8_protocol.o

SERVER_SOURCES = http8_server.c http8_main.c http8_protocol.c \
	http_negotiation.c $(ROUTER_DIR)/preferred_router.c \
	$(RESOLVER_DIR)/resolver.c $(SLNET_DIR)/sleela_net.c

SERVER_TARGET = http8_server

ifeq ($(OS),Windows_NT)
SERVER_LDLIBS += -lws2_32
endif

.PHONY: all server proto demo syntax test server-test clean

all: proto server

proto: $(PROTO_OBJECTS)
http8_protocol.o: http8_protocol.c http8_protocol.h
	$(CC) $(CFLAGS) -I. -c http8_protocol.c -o $@

server: $(SERVER_TARGET)
$(SERVER_TARGET): $(SERVER_SOURCES) http8_server.h http8_protocol.h
	$(CC) $(CFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $@ $(SERVER_LDLIBS)

demo: http80_demo http80_demo_cpp
http80_demo: http80.c
	$(CC) $(CFLAGS) -I. http80.c -o $@
http80_demo_cpp: http80.cpp DarkPower.cpp DarkPower.hpp
	$(CXX) $(CXXFLAGS) -I. http80.cpp DarkPower.cpp -o $@

syntax:
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only http8_protocol.c
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only http8_server.c
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only http8_main.c
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only http_negotiation.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.cpp
	$(CC)  $(CFLAGS)  -I. -fsyntax-only http80.c
	$(CXX) $(CXXFLAGS) -I. -fsyntax-only DarkPower.cpp

test: all
	./$(SERVER_TARGET) --self-test

server-test: $(SERVER_TARGET)
	./$(SERVER_TARGET) --self-test

clean:
	rm -f $(PROTO_OBJECTS) $(SERVER_TARGET) http80_demo http80_demo_cpp
