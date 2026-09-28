CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror -pthread
all: http-server-8
http-server-8: main.cpp server.cpp server.hpp ../common/packet_spec.hpp
	$(CXX) $(CXXFLAGS) main.cpp server.cpp -o $@
test: http-server-8
	./http-server-8 --help >/dev/null
clean:
	rm -f http-server-8
.PHONY: all test clean