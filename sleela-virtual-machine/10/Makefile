CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
BUILD_DIR ?= build
LIB := $(BUILD_DIR)/libslvm10.a
TEST := $(BUILD_DIR)/slvm10-selftest
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
$(TEST): tests/slvm10_selftest.c $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LIB) -o $@
test: $(TEST)
	$(TEST)
check: test
	test -f include/slvm10_storage.h
	test -f include/slvm10_identity.h
	echo "SLVM/10 verification check: PASS"
clean:
	rm -rf $(BUILD_DIR)
