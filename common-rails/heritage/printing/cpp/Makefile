CXX?=c++
CXXFLAGS?=-std=c++17 -Wall -Wextra -Werror -I.
all: check
check:
	$(CXX) $(CXXFLAGS) -o heritage-print-test CommonRailsPrinting.cpp PrintContractTest.cpp
	./heritage-print-test
clean:
	rm -f heritage-print-test
