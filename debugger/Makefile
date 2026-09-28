CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic

COMMON_CPP = debugger.cpp debugger_backend.cpp debugger_actions.cpp debugger_platform.cpp debugger_backend_linux.cpp debugger_backend_macos.cpp debugger_backend_windows.cpp

all: sleela-debugger

sleela-debugger: main.cpp $(COMMON_CPP) debugger.hpp debugger_backend.hpp debugger_actions.hpp debugger_platform.hpp
	$(CXX) $(CXXFLAGS) main.cpp $(COMMON_CPP) -o $@

test:
	./tests/run-tests.sh

clean:
	rm -f sleela-debugger
