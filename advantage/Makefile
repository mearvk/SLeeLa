# Makefile for the advantage module.
#
# Targets:
#   advantage - C++ demo (advantage.cpp) linked against the C core (advantage.c)
#   analyze   - loads advantages.xml into memory and reports on it
# Convenience: `make run` runs the demo, `make run-analyze` runs the loader.

CC      ?= cc
CXX     ?= c++
CFLAGS  ?= -O2 -Wall -Wextra -std=c11
CXXFLAGS?= -O2 -Wall -Wextra -std=c++17

DEMO     := advantage
ANALYZER := analyze

.PHONY: all clean run run-analyze

all: $(DEMO) $(ANALYZER)

# --- advantage demo ---------------------------------------------------------
$(DEMO): advantage.o advantage_cpp.o
	$(CXX) $(CXXFLAGS) -o $@ advantage.o advantage_cpp.o

advantage.o: advantage.c advantage.h
	$(CC) $(CFLAGS) -c advantage.c -o advantage.o

advantage_cpp.o: advantage.cpp advantage.hpp advantage.h
	$(CXX) $(CXXFLAGS) -c advantage.cpp -o advantage_cpp.o

# --- analyzer (XML loader) ---------------------------------------------------
$(ANALYZER): analyze.o advantage_store.o
	$(CXX) $(CXXFLAGS) -o $@ analyze.o advantage_store.o

analyze.o: analyze.cpp advantage_store.hpp
	$(CXX) $(CXXFLAGS) -c analyze.cpp -o analyze.o

advantage_store.o: advantage_store.cpp advantage_store.hpp
	$(CXX) $(CXXFLAGS) -c advantage_store.cpp -o advantage_store.o

# --- convenience -------------------------------------------------------------
run: $(DEMO)
	./$(DEMO)

run-analyze: $(ANALYZER)
	./$(ANALYZER)

clean:
	rm -f $(DEMO) $(ANALYZER) *.o
