CC ?= cc
CXX ?= c++
AR ?= ar
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
CURL_CFLAGS ?= $(shell pkg-config --cflags libcurl 2>/dev/null)
CURL_LIBS ?= $(shell pkg-config --libs libcurl 2>/dev/null || echo -lcurl)
.PHONY: all build test clean
all: libsleela_rest.a rest_connector_cpp.o
rest_connector.o: rest_connector.c rest_connector.h
	$(CC) $(CFLAGS) $(CURL_CFLAGS) -c $< -o $@
rest_connector_cpp.o: rest_connector.cpp rest_connector.hpp rest_connector.h
	$(CXX) $(CXXFLAGS) $(CURL_CFLAGS) -c $< -o $@
libsleela_rest.a: rest_connector.o
	$(AR) rcs $@ $^
test_rest_connector: test_rest_connector.c rest_connector.c rest_connector.h
	$(CC) $(CFLAGS) $(CURL_CFLAGS) test_rest_connector.c rest_connector.c $(CURL_LIBS) -o $@
test: test_rest_connector
	./test_rest_connector
clean:
	rm -f *.o *.a test_rest_connector
build: all
