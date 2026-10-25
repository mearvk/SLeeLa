# SPDX-License-Identifier: MIT
CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Werror
LDLIBS_MATH = -lm
SRC = ../src/cdma_common.c ../src/cdma_link.c
.PHONY: all syntax test clean
all: syntax
syntax:
	$(CC) $(CFLAGS) -I../include -fsyntax-only ../src/cdma_common.c
	$(CC) $(CFLAGS) -I../include -fsyntax-only ../src/cdma_link.c
	$(CXX) $(CXXFLAGS) -I../include -fsyntax-only ../src/cdma_common.cpp
test: syntax
	$(CC) $(CFLAGS) -I../include ../src/cdma_common.c test.c -o cdma-common-test
	./cdma-common-test
	$(CC) $(CFLAGS) -I../include $(SRC) test_link.c -o cdma-link-test $(LDLIBS_MATH)
	./cdma-link-test
clean:
	rm -f cdma-common-test cdma-link-test
