CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

.PHONY: all clean test

all: ledger_tool

ledger_tool: ledger_tool.cpp ledger_core.cpp ledger_core.h
	$(CXX) $(CXXFLAGS) -o $@ ledger_tool.cpp ledger_core.cpp

# Quick self-test: build a ledger over this dir's sources, then verify it, and
# cross-check the C++ SHA/record output against the Python reference.
test: ledger_tool
	./ledger_tool build --out /tmp/self.ledger --insignia-dir /tmp/insignia --root . ledger_core.cpp ledger_core.h
	./ledger_tool verify /tmp/self.ledger
	python3 ledger.py verify /tmp/self.ledger

clean:
	rm -f ledger_tool
	rm -rf __pycache__
