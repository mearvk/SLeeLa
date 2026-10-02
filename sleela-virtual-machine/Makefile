CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude -I../runtime

BUILD_DIR ?= build

all: $(BUILD_DIR)/slvm

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/slvm: src/slvm.c src/slvm_security.c src/slvm_io_heuristic.c src/slvm_memory_security.c src/slvm_observer.c src/slvm_broker.c src/slvm_broker_security.c ../runtime/garbage_collector.c tests/test_slvm.c include/slvm.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) src/slvm.c src/slvm_security.c src/slvm_io_heuristic.c src/slvm_memory_security.c src/slvm_observer.c src/slvm_broker.c src/slvm_broker_security.c ../runtime/garbage_collector.c tests/test_slvm.c -o $@

test: $(BUILD_DIR)/slvm
	$(BUILD_DIR)/slvm

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test clean
