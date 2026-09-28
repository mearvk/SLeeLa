CXX ?= c++
CC ?= cc
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic

all: sleela-debugger

sleela-debugger: main.cpp debugger.cpp debugger.hpp
	$(CXX) $(CXXFLAGS) main.cpp debugger.cpp -o $@

test:
	./tests/run-tests.sh

clean:
	rm -f sleela-debugger
