CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
BUILD_DIR ?= build
LIB := $(BUILD_DIR)/libslvm9.a
TEST := $(BUILD_DIR)/slvm9-selftest
NATIVE_TEST := $(BUILD_DIR)/slvm9-native-selftest
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
$(NATIVE_TEST): tests/slvm9_native_selftest.c $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LIB) -o $@
test: $(TEST) $(NATIVE_TEST)
	$(TEST)
	$(NATIVE_TEST)
check: test
	test -f include/slvm9_native.h
	test -f include/slvm9_filesystem_module.h
	test -f file-system/modules/tac3/tac3.definition.json
	echo "SLVM/9 filesystem/native/module check: PASS"
clean:
	rm -rf $(BUILD_DIR)
