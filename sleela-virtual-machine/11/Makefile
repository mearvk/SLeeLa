CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
BUILD=build
all: $(BUILD)/libslvm11fs.a
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/slvm11_filesystem_module.o: src/slvm11_filesystem_module.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD)/libslvm11fs.a: $(BUILD)/slvm11_filesystem_module.o
	ar rcs $@ $^
check: all
clean:
	rm -rf $(BUILD)
.PHONY: all check clean
