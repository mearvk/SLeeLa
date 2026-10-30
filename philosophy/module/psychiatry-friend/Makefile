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

EXAMPLE := $(BUILD_DIR)/speculation-example$(EXE)
TEST := $(BUILD_DIR)/test-speculation-invite$(EXE)

.PHONY: all test clean
all: $(EXAMPLE) $(TEST)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/speculation_invite.o: speculation_invite.c speculation_invite.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/speculation_invite_cpp.o: speculation_invite.cpp speculation_invite.hpp speculation_invite.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/speculation-example.o: speculation_example.cpp speculation_invite.hpp speculation_invite.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
$(BUILD_DIR)/test-speculation-invite.o: test_speculation_invite.c speculation_invite.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXAMPLE): $(BUILD_DIR)/speculation_invite.o $(BUILD_DIR)/speculation_invite_cpp.o $(BUILD_DIR)/speculation-example.o
	$(CXX) $^ -o $@
$(TEST): $(BUILD_DIR)/speculation_invite.o $(BUILD_DIR)/test-speculation-invite.o
	$(CC) $^ -o $@

test: $(TEST)
	./$(TEST)
clean:
	rm -rf $(BUILD_DIR)
