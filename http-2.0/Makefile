CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

ROUTER_DIR   = ../preferred-routers
RESOLVER_DIR = ../resolver
SLNET_DIR    = ../impl/core

INCLUDES = -I. -I..

# --- protocol-core objects (envelope + naming + pipeline) ---
PROTO_SOURCES = h21_envelope.c h21_naming.c h21_pipeline.c
PROTO_OBJECTS = $(PROTO_SOURCES:.c=.o)

# --- negotiation objects ---
NEG_C_OBJ   = http_negotiation.o
NEG_CXX_OBJ = http_negotiation_cxx.o

# --- server binary ---
SERVER_SOURCES = h21_server.c h21_main.c \
                 h21_envelope.c h21_naming.c h21_pipeline.c \
                 http_negotiation.c \
                 $(ROUTER_DIR)/preferred_router.c \
                 $(RESOLVER_DIR)/resolver.c \
                 $(SLNET_DIR)/sleela_net.c

SERVER_TARGET = h21_server

# Windows link libraries
ifeq ($(OS),Windows_NT)
LDLIBS += -lws2_32
endif

.PHONY: all server proto syntax test test-python clean

all: server proto $(NEG_C_OBJ) $(NEG_CXX_OBJ)

server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES) h21_server.h h21_protocol.h \
                  h21_envelope.h h21_naming.h h21_pipeline.h http_negotiation.h
	$(CC) $(CFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $@ $(LDLIBS)

# --- protocol-core objects (for linking into other targets) ---
proto: $(PROTO_OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# --- negotiation objects ---
$(NEG_C_OBJ): http_negotiation.c http_negotiation.h \
              $(ROUTER_DIR)/preferred_router.h $(RESOLVER_DIR)/resolver.h
	$(CC) $(CFLAGS) $(INCLUDES) -c http_negotiation.c -o $@

$(NEG_CXX_OBJ): http_negotiation.cpp http_negotiation.hpp \
                 http_negotiation.h $(RESOLVER_DIR)/resolver.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c http_negotiation.cpp -o $@

# --- syntax-only check ---
syntax:
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only $(PROTO_SOURCES)
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only http_negotiation.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.cpp
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only h21_server.c
	$(CC)  $(CFLAGS)  $(INCLUDES) -fsyntax-only h21_main.c

# --- self-test ---
test: $(SERVER_TARGET)
	./$(SERVER_TARGET) --self-test

test-python:
	python3 -m unittest -v test_h21_flow.py

clean:
	rm -f $(PROTO_OBJECTS) $(NEG_C_OBJ) $(NEG_CXX_OBJ) $(SERVER_TARGET)
	rm -rf __pycache__
