SHELL := /bin/sh
CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
.PHONY: all syntax clean
all: syntax
syntax:
	$(CC) $(CFLAGS) -I. -fsyntax-only preferred_router.c
	$(CXX) $(CXXFLAGS) -I. -fsyntax-only preferred_router.cpp
clean:
	true
