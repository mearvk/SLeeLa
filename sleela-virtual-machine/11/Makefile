CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
BUILD=build
LIB=$(BUILD)/libslvm11fs.a
TEST=$(BUILD)/slvm11-filesystem-module-selftest
all: $(LIB)
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/slvm11_filesystem_module.o: src/slvm11_filesystem_module.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(LIB): $(BUILD)/slvm11_filesystem_module.o
	ar rcs $@ $^
$(TEST): tests/slvm11_filesystem_module_selftest.c $(LIB) | $(BUILD)
	$(CC) $(CFLAGS) $< $(LIB) -o $@
test: $(TEST)
	$(TEST)
check: test
	test -f file-system/modules/tac3/tac3.definition.json
	echo "SLVM/11 filesystem module check: PASS"
clean:
	rm -rf $(BUILD)
.PHONY: all test check clean
