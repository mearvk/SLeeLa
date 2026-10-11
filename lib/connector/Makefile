CC ?= cc
CXX ?= c++
AR ?= ar
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
all: libsleela_connector_registry.a connector_registry_cpp.o
connector_registry.o: connector_registry.c connector_registry.h
	$(CC) $(CFLAGS) -c $< -o $@
connector_registry_cpp.o: connector_registry.cpp connector_registry.hpp connector_registry.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
libsleela_connector_registry.a: connector_registry.o
	$(AR) rcs $@ $^
test_connector_registry: test_connector_registry.c connector_registry.c connector_registry.h
	$(CC) $(CFLAGS) test_connector_registry.c connector_registry.c -o $@
test: test_connector_registry
	./test_connector_registry
clean:
	rm -f *.o *.a test_connector_registry
.PHONY: all test clean
