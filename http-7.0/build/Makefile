SHELL := /bin/sh
CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
C_SOURCES := $(wildcard ../*.c)
CPP_SOURCES := $(wildcard ../*.cpp)
C_OBJECTS := $(patsubst ../%.c,%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst ../%.cpp,%.o,$(CPP_SOURCES))
.PHONY: all preferred syntax clean
all: preferred syntax
preferred:
	@$(MAKE) -C ../../preferred-routers syntax

syntax:
	@set -e; for f in $(C_SOURCES); do $(CC) $(CFLAGS) -I.. -fsyntax-only "$$f"; done
	@set -e; for f in $(CPP_SOURCES); do $(CXX) $(CXXFLAGS) -I.. -fsyntax-only "$$f"; done
clean:
	rm -f *.o
