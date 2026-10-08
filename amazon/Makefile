CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2

all: libsleela_amazon.a

amazon.o: amazon.c amazon.h
	$(CC) $(CFLAGS) -c amazon.c -o amazon.o

amazon_cpp.o: amazon.cpp amazon.hpp amazon.h
	$(CXX) $(CXXFLAGS) -c amazon.cpp -o amazon_cpp.o

libsleela_amazon.a: amazon.o amazon_cpp.o
	ar rcs $@ $^

clean:
	rm -f amazon.o amazon_cpp.o libsleela_amazon.a

.PHONY: all clean
