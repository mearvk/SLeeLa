CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c++17
XML_CFLAGS ?= $(shell pkg-config --cflags libxml-2.0 2>/dev/null)
XML_LIBS ?= $(shell pkg-config --libs libxml-2.0 2>/dev/null || echo -lxml2)
all: xml_connector.o xml_connector_cpp.o
xml_connector.o: xml_connector.c xml_connector.h
	$(CC) $(CFLAGS) $(XML_CFLAGS) -c $< -o $@
xml_connector_cpp.o: xml_connector.cpp xml_connector.hpp xml_connector.h
	$(CXX) $(CXXFLAGS) $(XML_CFLAGS) -c $< -o $@
clean:
	rm -f *.o
.PHONY: all clean
