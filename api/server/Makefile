CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic

ifeq ($(OS),Windows_NT)
  EXE := .exe
else
  EXE :=
endif

all: sleelas$(EXE)

sleelas$(EXE): sleelas.cpp nat_aware.cpp nat_aware.h
	$(CXX) $(CXXFLAGS) sleelas.cpp nat_aware.cpp -o $@

clean:
	rm -f sleelas sleelas.exe

.PHONY: all clean
