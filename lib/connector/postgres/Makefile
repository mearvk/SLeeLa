CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
PG_CFLAGS ?= $(shell pkg-config --cflags libpq 2>/dev/null)
PG_LIBS ?= $(shell pkg-config --libs libpq 2>/dev/null || echo -lpq)
all: libsleela_postgres.a postgres_connector_cpp.o
postgres_connector.o: postgres_connector.c postgres_connector.h
	$(CC) $(CFLAGS) $(PG_CFLAGS) -c $< -o $@
postgres_connector_cpp.o: postgres_connector.cpp postgres_connector.hpp postgres_connector.h
	$(CXX) $(CXXFLAGS) $(PG_CFLAGS) -c $< -o $@
libsleela_postgres.a: postgres_connector.o
	ar rcs $@ $^
clean:
	rm -f *.o *.a
.PHONY: all clean
