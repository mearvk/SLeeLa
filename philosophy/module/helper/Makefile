CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
BUILD_DIR := build
ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

C_EXAMPLE := $(BUILD_DIR)/help-example-c$(EXE)
CPP_EXAMPLE := $(BUILD_DIR)/help-example-cpp$(EXE)
TEST := $(BUILD_DIR)/test-help-desk$(EXE)

.PHONY: all test clean
all: $(C_EXAMPLE) $(CPP_EXAMPLE) $(TEST)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/help_desk.o: help_desk.c help_desk.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/help_desk_cpp.o: help_desk.cpp help_desk.hpp help_desk.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/help-example-c.o: example.c help_desk.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/help-example-cpp.o: example.cpp help_desk.hpp help_desk.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/test-help-desk.o: test_help_desk.cpp help_desk.hpp help_desk.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(C_EXAMPLE): $(BUILD_DIR)/help_desk.o $(BUILD_DIR)/help-example-c.o
	$(CC) $^ -o $@
$(CPP_EXAMPLE): $(BUILD_DIR)/help_desk.o $(BUILD_DIR)/help_desk_cpp.o $(BUILD_DIR)/help-example-cpp.o
	$(CXX) $^ -o $@
$(TEST): $(BUILD_DIR)/help_desk.o $(BUILD_DIR)/help_desk_cpp.o $(BUILD_DIR)/test-help-desk.o
	$(CXX) $^ -o $@

test: $(TEST)
	./$(TEST)
clean:
	rm -rf $(BUILD_DIR)
