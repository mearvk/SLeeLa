CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -I../../include
BUILD_DIR ?= ../../build/linux-obj
all:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c ../../src/slvm9.c -o $(BUILD_DIR)/slvm9.o
clean:
	rm -rf $(BUILD_DIR)
