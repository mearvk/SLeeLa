CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Werror
CPPFLAGS ?= -I../include

.PHONY: all syntax cpp-syntax clean

all: syntax cpp-syntax

syntax:
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsyntax-only ../src/coorenagraph.c

cpp-syntax:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fsyntax-only ../src/coorenagraph.cpp

clean:
	@true
