CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
all: libsleela_delimited.a delimited_connector_cpp.o
delimited_connector.o: delimited_connector.c delimited_connector.h
	$(CC) $(CFLAGS) -c $< -o $@
delimited_connector_cpp.o: delimited_connector.cpp delimited_connector.hpp delimited_connector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
libsleela_delimited.a: delimited_connector.o
	ar rcs $@ $^
test_delimited_connector: test_delimited_connector.c delimited_connector.c delimited_connector.h
	$(CC) $(CFLAGS) test_delimited_connector.c delimited_connector.c -o $@
test: test_delimited_connector
	./test_delimited_connector
clean:
	rm -f *.o *.a test_delimited_connector
.PHONY: all test clean

# Explicit alias for scripts and IDE build actions.
build: all
.PHONY: build
