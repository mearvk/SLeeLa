CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
SQLITE_CFLAGS ?= $(shell pkg-config --cflags sqlite3 2>/dev/null)
SQLITE_LIBS ?= $(shell pkg-config --libs sqlite3 2>/dev/null || echo -lsqlite3)
all: libsleela_sqlite.a sqlite_connector_cpp.o
sqlite_connector.o: sqlite_connector.c sqlite_connector.h
	$(CC) $(CFLAGS) $(SQLITE_CFLAGS) -c $< -o $@
sqlite_connector_cpp.o: sqlite_connector.cpp sqlite_connector.hpp sqlite_connector.h
	$(CXX) $(CXXFLAGS) $(SQLITE_CFLAGS) -c $< -o $@
libsleela_sqlite.a: sqlite_connector.o
	ar rcs $@ $^
clean:
	rm -f *.o *.a
.PHONY: all clean

# Explicit alias for scripts and IDE build actions.
build: all
.PHONY: build
