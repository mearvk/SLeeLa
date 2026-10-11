# SLeeLa SQL native engine: C11 core, C++17 facade, CLI, smoke tests.
CC ?= cc
CXX ?= c++
AR ?= ar
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -I native/include -I cpp/include
BUILD := build
CORE := $(BUILD)/sleela_sql.o
CPP := $(BUILD)/sleela_sql_cpp.o

all: $(BUILD)/sleela-sql $(BUILD)/libsleelasql.a $(BUILD)/libsleelasql++.a

$(BUILD):
	mkdir -p $(BUILD)

$(CORE): native/src/sleela_sql.c native/include/sleela_sql.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/sleela_sql_cli.o: native/src/sleela_sql_cli.c native/include/sleela_sql.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/sleela-sql: $(CORE) $(BUILD)/sleela_sql_cli.o
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD)/libsleelasql.a: $(CORE)
	$(AR) rcs $@ $^

$(CPP): cpp/src/sleela_sql.cpp cpp/include/sleela_sql.hpp native/include/sleela_sql.h | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/libsleelasql++.a: $(CPP) $(CORE)
	$(AR) rcs $@ $^

$(BUILD)/test-c: tests/test_c.c $(CORE) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/test-cpp: tests/test_cpp.cpp $(CPP) $(CORE) | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

test: $(BUILD)/test-c $(BUILD)/test-cpp
	rm -rf $(BUILD)/test-data && mkdir -p $(BUILD)/test-data
	$(BUILD)/test-c $(BUILD)/test-data
	rm -rf $(BUILD)/test-data && mkdir -p $(BUILD)/test-data
	$(BUILD)/test-cpp $(BUILD)/test-data

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
