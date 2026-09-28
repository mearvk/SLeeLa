CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic
CPPFLAGS ?= -Iinclude

BUILD_DIR ?= build

C_TEST := $(BUILD_DIR)/regex-c
CPP_TEST := $(BUILD_DIR)/regex-cpp

.PHONY: all c cpp test clean

all: test

c: $(C_TEST)
	$(C_TEST)

cpp: $(CPP_TEST)
	$(CPP_TEST)

test: c cpp

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(C_TEST): src/sleela_regex.c include/sleela_regex.h tests/test_regex.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) src/sleela_regex.c tests/test_regex.c -o $@

$(CPP_TEST): src/sleela_regex.cpp include/sleela_regex.hpp tests/test_regex.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) src/sleela_regex.cpp tests/test_regex.cpp -o $@

clean:
	rm -rf $(BUILD_DIR)
