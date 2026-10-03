CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
BUILD_DIR ?= build
LIB := $(BUILD_DIR)/libslvm9.a
TEST := $(BUILD_DIR)/slvm9-selftest
SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
.PHONY: all clean test check
all: $(LIB)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(LIB): $(OBJECTS)
	$(AR) rcs $@ $^
$(TEST): tests/slvm9_selftest.c $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LIB) -o $@
test: $(TEST)
	$(TEST)
check: test
	@test -f include/slvm9_filesystem.h
	@test -f include/slvm9_adapter.h
	@test -f include/slvm9_context.h
	@echo "SLVM/9 filesystem contract check: PASS"
clean:
	rm -rf $(BUILD_DIR)
