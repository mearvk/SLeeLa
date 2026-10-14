CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -I../../include
BUILD_DIR ?= ../../build/linux-obj
LIB := $(BUILD_DIR)/libslvm8.a
SOURCES := $(wildcard ../../src/*.c)
OBJECTS := $(patsubst ../../src/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
.PHONY: all clean
all: $(LIB)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.o: ../../src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(LIB): $(OBJECTS)
	$(AR) rcs $@ $^
clean:
	rm -rf $(BUILD_DIR)
