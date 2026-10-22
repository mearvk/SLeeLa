CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
ifneq ($(OS),Windows_NT)
CXXFLAGS += -pthread
endif

ROUTER_DIR   = ../preferred-routers
RESOLVER_DIR = ../resolver
SLNET_DIR    = ../impl/core
INCLUDES = -I. -I..

C_OBJECTS = http7_server.o http7_main.o http7_protocol.o http_negotiation.o preferred_router.o resolver.o sleela_net.o
CXX_OBJECTS = http7_logger_bridge.o sleela_logger.o
SERVER_TARGET = http7_server
ifeq ($(OS),Windows_NT)
SERVER_LDLIBS += -lws2_32
endif

.PHONY: all server proto syntax test server-test demo logger-test clean
all: proto server
proto: http7_protocol.o
server: $(SERVER_TARGET)

http7_server.o: http7_server.c http7_server.h http7_protocol.h http7_logger_bridge.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
http7_main.o: http7_main.c http7_server.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
http7_protocol.o: http7_protocol.c http7_protocol.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
http_negotiation.o: http_negotiation.c http_negotiation.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
preferred_router.o: $(ROUTER_DIR)/preferred_router.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
resolver.o: $(RESOLVER_DIR)/resolver.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
sleela_net.o: $(SLNET_DIR)/sleela_net.c $(SLNET_DIR)/sleela_net.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
http7_logger_bridge.o: http7_logger_bridge.cpp http7_logger_bridge.h ../logger/sleela_logger.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
sleela_logger.o: ../logger/sleela_logger.cpp ../logger/sleela_logger.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(SERVER_TARGET): $(C_OBJECTS) $(CXX_OBJECTS)
	$(CXX) $(CXXFLAGS) $(C_OBJECTS) $(CXX_OBJECTS) -o $@ $(SERVER_LDLIBS)

http7_logger_test: http7_logger_test.cpp http7_logger_bridge.cpp ../logger/sleela_logger.cpp http7_logger_bridge.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) http7_logger_test.cpp http7_logger_bridge.cpp ../logger/sleela_logger.cpp -o $@

logger-test: http7_logger_test
	./http7_logger_test

http70_demo: http70.c
	$(CC) $(CFLAGS) -I. http70.c -o $@
http70_demo_cpp: http70.cpp
	$(CXX) $(CXXFLAGS) -I. http70.cpp -o $@
demo: http70_demo http70_demo_cpp

syntax:
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http7_protocol.c http7_server.c http7_main.c http_negotiation.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only $(ROUTER_DIR)/preferred_router.c $(RESOLVER_DIR)/resolver.c $(SLNET_DIR)/sleela_net.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.cpp http7_logger_bridge.cpp ../logger/sleela_logger.cpp
	$(CC) $(CFLAGS) -I. -fsyntax-only http70.c
	$(CXX) $(CXXFLAGS) -I. -fsyntax-only http70.cpp

test: all http7_logger_test
	./$(SERVER_TARGET) --self-test
	./http7_logger_test
server-test: $(SERVER_TARGET)
	./$(SERVER_TARGET) --self-test

clean:
	rm -f $(C_OBJECTS) $(CXX_OBJECTS) $(SERVER_TARGET) http7_logger_test http70_demo http70_demo_cpp
