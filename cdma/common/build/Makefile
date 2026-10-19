CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Werror
.PHONY: all syntax test clean
all: syntax
syntax:
	$(CC) $(CFLAGS) -I../include -fsyntax-only ../src/cdma_common.c
	$(CXX) $(CXXFLAGS) -I../include -fsyntax-only ../src/cdma_common.cpp
test: syntax
	$(CC) $(CFLAGS) -I../include ../src/cdma_common.c test.c -o cdma-common-test
	./cdma-common-test
clean:
	rm -f cdma-common-test
