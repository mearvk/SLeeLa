CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror -pthread
all: http-server-7
http-server-7: main.cpp server.cpp server.hpp
	$(CXX) $(CXXFLAGS) main.cpp server.cpp -o $@
test: http-server-7
	./http-server-7 --help >/dev/null
clean:
	rm -f http-server-7
.PHONY: all test clean