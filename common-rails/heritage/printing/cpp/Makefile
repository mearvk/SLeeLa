CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Werror -I.
ifeq ($(OS),Windows_NT)
  EXE ?= .exe
else
  EXE ?=
endif
.PHONY: all check clean
all: check
check: heritage-print-test$(EXE)
	./heritage-print-test$(EXE)
heritage-print-test$(EXE): CommonRailsPrinting.cpp PrintContractTest.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
clean:
	rm -f heritage-print-test heritage-print-test.exe
