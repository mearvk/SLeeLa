CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude

BUILD_DIR ?= build

all: $(BUILD_DIR)/slvm

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/slvm: src/slvm.c tests/test_slvm.c include/slvm.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) src/slvm.c tests/test_slvm.c -o $@

test: $(BUILD_DIR)/slvm
	$(BUILD_DIR)/slvm

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean
