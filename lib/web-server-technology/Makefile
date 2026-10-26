# lib/web-server-technology native library:
# C11 core (slweb) + C++17 RAII facade + C/C++ smoke tests.
# Mirrors the lib/sleela-sql build layout.
CC ?= cc
CXX ?= c++
AR ?= ar
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -I native/include -I cpp/include
BUILD := build
CORE := $(BUILD)/slweb.o
CPP := $(BUILD)/slweb_cpp.o

all: $(BUILD)/libslweb.a $(BUILD)/libslweb++.a

$(BUILD):
	mkdir -p $(BUILD)

$(CORE): native/src/slweb.c native/include/slweb.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(CPP): cpp/src/slweb.cpp cpp/include/slweb.hpp native/include/slweb.h | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/libslweb.a: $(CORE)
	$(AR) rcs $@ $^

$(BUILD)/libslweb++.a: $(CPP) $(CORE)
	$(AR) rcs $@ $^

$(BUILD)/test-c: tests/test_c.c $(CORE) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/test-cpp: tests/test_cpp.cpp $(CPP) $(CORE) | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test: $(BUILD)/test-c $(BUILD)/test-cpp
	$(BUILD)/test-c
	$(BUILD)/test-cpp

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
