CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra

ROUTER_DIR   = ../preferred-routers
RESOLVER_DIR = ../resolver
SLNET_DIR    = ../impl/core

INCLUDES = -I. -I..

PROTOCOL_SOURCES = http4_protocol.c http4_packet_context.c http4_packet_payload.c $(ROUTER_DIR)/preferred_router.c
SEGMENTATION_SOURCES = http4_protocol.c http4_segmentation.c http4_reassembly.c $(ROUTER_DIR)/preferred_router.c
PROTOCOL_OBJECTS = $(PROTOCOL_SOURCES:.c=.o)
SEGMENTATION_OBJECTS = $(SEGMENTATION_SOURCES:.c=.o)

# --- server ---
SERVER_SOURCES = \
	http4_server.c http4_main.c \
	http4_protocol.c http4_packet_context.c http4_packet_payload.c \
	http4_segmentation.c http4_reassembly.c \
	http_negotiation.c \
	$(ROUTER_DIR)/preferred_router.c \
	$(RESOLVER_DIR)/resolver.c \
	$(SLNET_DIR)/sleela_net.c

SERVER_TARGET = http4_server_bin

ifeq ($(OS),Windows_NT)
SERVER_LDLIBS += -lws2_32
endif

.PHONY: all clean test protocol-test segmentation-test session-test resume-test fuzz server server-test

all: http4_selftest http4_segmentation_selftest http4_session_selftest http4_resume_selftest server

http4_selftest: $(PROTOCOL_OBJECTS) http4_selftest.o
	$(CC) $(CFLAGS) -o $@ $^

http4_segmentation_selftest: $(SEGMENTATION_OBJECTS) http4_segmentation_selftest.o
	$(CC) $(CFLAGS) -o $@ $^

http4_session_selftest: http4_protocol.c http4_session.c http4_session_selftest.c $(ROUTER_DIR)/preferred_router.c
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

http4_resume_selftest: http4_resume.c http4_resume_selftest.c http4_resume.h
	$(CC) $(CFLAGS) $(INCLUDES) http4_resume.c http4_resume_selftest.c -o $@

server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES) http4_server.h http4_protocol.h
	$(CC) $(CFLAGS) $(INCLUDES) $(SERVER_SOURCES) -o $@ $(SERVER_LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

test: all
	./http4_selftest
	./http4_segmentation_selftest
	./http4_session_selftest
	./http4_resume_selftest
	./$(SERVER_TARGET) --self-test

protocol-test: http4_selftest
	./http4_selftest

segmentation-test: http4_segmentation_selftest
	./http4_segmentation_selftest

session-test: http4_session_selftest
	./http4_session_selftest

resume-test: http4_resume_selftest
	./http4_resume_selftest

# Optional Clang libFuzzer target; requires Clang with libFuzzer support.
fuzz:
	clang -std=c11 -O1 -g -fsanitize=fuzzer,address,undefined -fno-omit-frame-pointer $(INCLUDES) http4_fuzz.c http4_protocol.c http4_segmentation.c http4_packet_context.c $(ROUTER_DIR)/preferred_router.c -o http4_fuzz

server-test: $(SERVER_TARGET)
	./$(SERVER_TARGET) --self-test

clean:
	rm -f $(PROTOCOL_OBJECTS) $(SEGMENTATION_OBJECTS) \
	      http4_selftest.o http4_segmentation_selftest.o \
	      http4_selftest http4_segmentation_selftest http4_session_selftest http4_resume_selftest http4_fuzz \
	      $(SERVER_TARGET)
