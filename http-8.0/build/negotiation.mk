CXX ?= c++
CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
.PHONY: negotiation syntax
negotiation: syntax
resolver-syntax:
	$(CC) $(CFLAGS) -fsyntax-only ../../resolver/resolver.c

syntax: resolver-syntax
	$(CC) $(CFLAGS) -fsyntax-only ../http_negotiation.c
	$(CXX) $(CXXFLAGS) -fsyntax-only ../http_negotiation.cpp
