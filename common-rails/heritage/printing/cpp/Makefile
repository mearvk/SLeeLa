CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Werror -I.
EXE ?=
.PHONY: all check clean
all: check
check: heritage-print-test$(EXE)
	./heritage-print-test$(EXE)
heritage-print-test$(EXE): CommonRailsPrinting.cpp PrintContractTest.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
clean:
	rm -f heritage-print-test heritage-print-test.exe
