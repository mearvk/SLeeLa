CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

ROUTER_DIR = ../preferred-routers
RESOLVER_DIR = ../resolver
INCLUDES = -I. -I..

.PHONY: all syntax demo test clean
all: syntax

syntax:
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http90.c
	$(CC) $(CFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http90.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -fsyntax-only http_negotiation.cpp

demo: http90_demo http90_demo_cpp
http90_demo: http90.c http90.h
	$(CC) $(CFLAGS) -DHTTP90_STANDALONE_DEMO -I. http90.c -o $@
http90_demo_cpp: http90.cpp http90.c http90.hpp http90.h
	$(CXX) $(CXXFLAGS) -I. http90.cpp http90.c -o $@

test: syntax
	@echo "HTTP 9.0 syntax checks passed."
	@echo "Runtime/conformance tests require a configured test harness; see TESTING.md."

clean:
	rm -f http90_demo http90_demo_cpp *.o
