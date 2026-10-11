CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
all: json_connector.o json_connector_cpp.o
json_connector.o: json_connector.c json_connector.h
	$(CC) $(CFLAGS) -c $< -o $@
json_connector_cpp.o: json_connector.cpp json_connector.hpp json_connector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
test_json_connector: test_json_connector.c json_connector.c json_connector.h
	$(CC) $(CFLAGS) test_json_connector.c json_connector.c -o $@
test: test_json_connector
	./test_json_connector
clean:
	rm -f *.o test_json_connector
.PHONY: all test clean

# Explicit alias for scripts and IDE build actions.
build: all
.PHONY: build
