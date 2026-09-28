CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
all: sleela-debugger
sleela-debugger: main.cpp debugger.cpp debugger.hpp
	$(CXX) $(CXXFLAGS) main.cpp debugger.cpp -o $@
clean:
	rm -f sleela-debugger
