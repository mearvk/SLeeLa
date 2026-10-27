CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror
BUILD_DIR ?= build/tests

.PHONY: all test clean
all: test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/test_gc_security: tests/test_gc_security.c garbage_collector.c security_supervisor.c garbage_collector.h security_supervisor.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) tests/test_gc_security.c garbage_collector.c security_supervisor.c -o $@

test: $(BUILD_DIR)/test_gc_security
	$(BUILD_DIR)/test_gc_security

clean:
	rm -rf $(BUILD_DIR)
