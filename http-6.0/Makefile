CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread

ROUTER_DIR = ../preferred-routers
RESOLVER_DIR = ../resolver
SLNET_DIR = ../impl/core
INCLUDES = -I. -I..

C_OBJECTS = http6_server.o http6_main.o http6_protocol.o http_negotiation.o \
 $(ROUTER_DIR)/preferred_router.o $(RESOLVER_DIR)/resolver.o $(SLNET_DIR)/sleela_net.o
CXX_OBJECTS = http6_consolidated_pack.o http6_logger_bridge.o ../logger/sleela_logger.o
SERVER_TARGET = http6_server
ifeq ($(OS),Windows_NT)
SERVER_LDLIBS += -lws2_32
endif

.PHONY: all server proto syntax test server-test pack-test logger-test clean
all: proto server
proto: http6_protocol.o http6_consolidated_pack.o
server: $(SERVER_TARGET)

$(SERVER_TARGET): $(C_OBJECTS) $(CXX_OBJECTS)
	$(CXX) $(CXXFLAGS) $(C_OBJECTS) $(CXX_OBJECTS) -o $@ $(SERVER_LDLIBS)

http6_server.o: http6_server.c http6_server.h http6_logger_bridge.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
http6_main.o: http6_main.c http6_server.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
http6_protocol.o: http6_protocol.c http6_protocol.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
http_negotiation.o: http_negotiation.c http_negotiation.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(ROUTER_DIR)/preferred_router.o: $(ROUTER_DIR)/preferred_router.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(RESOLVER_DIR)/resolver.o: $(RESOLVER_DIR)/resolver.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(SLNET_DIR)/sleela_net.o: $(SLNET_DIR)/sleela_net.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
http6_consolidated_pack.o: http6_consolidated_pack.cpp http6_consolidated_pack.hpp
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@
http6_logger_bridge.o: http6_logger_bridge.cpp http6_logger_bridge.h ../logger/sleela_logger.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
../logger/sleela_logger.o: ../logger/sleela_logger.cpp ../logger/sleela_logger.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
http6_pack_test: http6_pack_test.cpp http6_consolidated_pack.cpp http6_consolidated_pack.hpp
	$(CXX) $(CXXFLAGS) -I. $< http6_consolidated_pack.cpp -o $@
pack-test: http6_pack_test
	./http6_pack_test

http6_logger_test: http6_logger_test.cpp http6_logger_bridge.cpp ../logger/sleela_logger.cpp http6_logger_bridge.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) http6_logger_test.cpp http6_logger_bridge.cpp ../logger/sleela_logger.cpp -o $@

logger-test: http6_logger_test
	./http6_logger_test
syntax:
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http6_protocol.c http6_server.c http6_main.c http_negotiation.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only $(ROUTER_DIR)/preferred_router.c $(RESOLVER_DIR)/resolver.c $(SLNET_DIR)/sleela_net.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.cpp http6_consolidated_pack.cpp http6_logger_bridge.cpp ../logger/sleela_logger.cpp
test: all pack-test logger-test
	./$(SERVER_TARGET) --self-test
server-test: $(SERVER_TARGET)
	./$(SERVER_TARGET) --self-test
clean:
	rm -f $(C_OBJECTS) $(CXX_OBJECTS) $(SERVER_TARGET) http6_pack_test http6_logger_test
