CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
ODBC_CFLAGS ?= $(shell pkg-config --cflags odbc 2>/dev/null)
ODBC_LIBS ?= $(shell pkg-config --libs odbc 2>/dev/null || echo -lodbc)
all: libsleela_odbc.a odbc_connector_cpp.o
odbc_connector.o: odbc_connector.c odbc_connector.h
	$(CC) $(CFLAGS) $(ODBC_CFLAGS) -c $< -o $@
odbc_connector_cpp.o: odbc_connector.cpp odbc_connector.hpp odbc_connector.h
	$(CXX) $(CXXFLAGS) $(ODBC_CFLAGS) -c $< -o $@
libsleela_odbc.a: odbc_connector.o
	ar rcs $@ $^
clean:
	rm -f *.o *.a
.PHONY: all clean
