CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

ROUTER_DIR   = ../preferred-routers
RESOLVER_DIR = ../resolver
SLNET_DIR    = ../impl/core

# --- negotiation library (original) ---
NEG_OBJECTS = http_negotiation.o http_negotiation_cxx.o

# --- server ---
SERVER_SOURCES = http1_server.c http1_main.c \
                 http_negotiation.c \
                 $(ROUTER_DIR)/preferred_router.c \
                 $(RESOLVER_DIR)/resolver.c \
                 $(SLNET_DIR)/sleela_net.c

SERVER_TARGET = http1_server

INCLUDES = -I. -I..

# Windows link libraries (detected by OS or set externally).
ifeq ($(OS),Windows_NT)
LDLIBS += -lws2_32
endif

.PHONY: all server syntax test sanitize fuzz-download clean

all: server $(NEG_OBJECTS)

server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES) http1_server.h http_negotiation.h
	$(CC) $(CFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $@ $(LDLIBS)

# --- negotiation objects (for linking into other targets) ---
http_negotiation.o: http_negotiation.c http_negotiation.h \
                    $(ROUTER_DIR)/preferred_router.h $(RESOLVER_DIR)/resolver.h
	$(CC) $(CFLAGS) $(INCLUDES) -c http_negotiation.c -o $@

http_negotiation_cxx.o: http_negotiation.cpp http_negotiation.hpp \
                        http_negotiation.h $(RESOLVER_DIR)/resolver.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c http_negotiation.cpp -o $@

# --- syntax-only check ---
syntax:
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only http_negotiation.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.cpp
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only http1_server.c
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only http1_main.c

# --- self-test (parse round-trip, negotiation, download state) ---
test: $(SERVER_TARGET)
	./$(SERVER_TARGET) --self-test

clean:
	rm -f $(NEG_OBJECTS) $(SERVER_TARGET)
