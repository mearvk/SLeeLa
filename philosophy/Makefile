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

C_EXAMPLE := $(BUILD_DIR)/example-c$(EXE)
CPP_EXAMPLE := $(BUILD_DIR)/example-cpp$(EXE)
TEST_PROGRAM := $(BUILD_DIR)/test-screen-map$(EXE)

.PHONY: all test clean
all: $(C_EXAMPLE) $(CPP_EXAMPLE) $(TEST_PROGRAM)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/screen_map.o: screen_map.c screen_map.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c screen_map.c -o $@

$(BUILD_DIR)/conversation_mask.o: conversation_mask.cpp conversation_mask.hpp screen_map.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c conversation_mask.cpp -o $@

$(BUILD_DIR)/example-c.o: example.c screen_map.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c example.c -o $@

$(BUILD_DIR)/example-cpp.o: example.cpp conversation_mask.hpp screen_map.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c example.cpp -o $@

$(BUILD_DIR)/test-screen-map.o: test_screen_map.cpp conversation_mask.hpp screen_map.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c test_screen_map.cpp -o $@

$(C_EXAMPLE): $(BUILD_DIR)/screen_map.o $(BUILD_DIR)/example-c.o
	$(CC) $^ -lm -o $@

$(CPP_EXAMPLE): $(BUILD_DIR)/screen_map.o $(BUILD_DIR)/conversation_mask.o $(BUILD_DIR)/example-cpp.o
	$(CXX) $^ -lm -o $@

$(TEST_PROGRAM): $(BUILD_DIR)/screen_map.o $(BUILD_DIR)/conversation_mask.o $(BUILD_DIR)/test-screen-map.o
	$(CXX) $^ -lm -o $@

test: $(TEST_PROGRAM)
	./$(TEST_PROGRAM)

clean:
	rm -rf $(BUILD_DIR)
