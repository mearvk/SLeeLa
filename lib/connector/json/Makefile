CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
all: json_connector.o json_connector_cpp.o
json_connector.o: json_connector.c json_connector.h
	$(CC) $(CFLAGS) -c $< -o $@
json_connector_cpp.o: json_connector.cpp json_connector.hpp json_connector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f *.o
.PHONY: all clean
