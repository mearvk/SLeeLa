CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
BUILD_DIR ?= build
LIB := $(BUILD_DIR)/libslvm8.a
TEST := $(BUILD_DIR)/slvm8-selftest
C_SOURCES := $(wildcard src/*.c)
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

.PHONY: all clean check test config
all: $(LIB)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(LIB): $(C_OBJECTS)
	$(AR) rcs $@ $^
$(TEST): tests/slvm8_selftest.c $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LIB) -o $@
test: $(TEST)
	$(TEST)
check: test
	@test -f config/slvm-runtime.conf
	@test -f include/slvm8.h
	@test -f include/slvm8_policy.h
	@test -f include/slvm8_capability.h
	@test -f include/slvm8_transaction.h
	@test -f include/slvm8_supervisor.h
	@test -f include/slvm8_audit.h
	@test -f include/slvm8_admission.h
	@echo "SLVM/8 required files present"
config:
	@cat config/slvm-runtime.conf
clean:
	rm -rf $(BUILD_DIR)
