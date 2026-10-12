CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
BUILD := build
TEST := $(BUILD)/alphabet-runtime-tests

.PHONY: all test clean
all: test

$(BUILD):
	mkdir -p $(BUILD)

$(TEST): character_runtime.cpp character_runtime.hpp tests.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) character_runtime.cpp tests.cpp -o $(TEST)

test: $(TEST)
	./$(TEST)

clean:
	rm -rf $(BUILD)
