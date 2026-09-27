CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
CPPFLAGS ?= -I../include

.PHONY: all syntax clean

all: syntax

syntax:
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsyntax-only ../src/coorenagraph.c

clean:
	@true
