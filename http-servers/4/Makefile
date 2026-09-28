CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror -pthread
all: http-server-4
http-server-4: main.cpp server.cpp server.hpp
	$(CXX) $(CXXFLAGS) main.cpp server.cpp -o $@
test: http-server-4
	./http-server-4 --help >/dev/null
clean:
	rm -f http-server-4
.PHONY: all test clean