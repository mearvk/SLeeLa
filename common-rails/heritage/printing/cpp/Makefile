CXX?=c++
CXXFLAGS?=-std=c++17 -Wall -Wextra -Werror -I.
all: heritage-print-test
heritage-print-test: CommonRailsPrinting.cpp PrintContractTest.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
check: heritage-print-test
	./heritage-print-test
clean:
	rm -f heritage-print-test
