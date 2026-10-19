CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Werror
COMMON = ../../common/src/cdma_common.c
SRC = ../src/is95.c
CPP = ../src/is95.cpp
.PHONY: all syntax test clean
all: syntax
syntax:
	$(CC) $(CFLAGS) -I../include -I../../common/include -fsyntax-only $(SRC) $(COMMON) test.c
	$(CXX) $(CXXFLAGS) -I../include -I../../common/include -fsyntax-only $(CPP)
test: syntax
	$(CC) $(CFLAGS) -I../include -I../../common/include $(SRC) $(COMMON) test.c -o cdma-test
	./cdma-test
clean:
	rm -f cdma-test
