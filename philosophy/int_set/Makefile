CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
BUILD_DIR := build
ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

GENERATOR := $(BUILD_DIR)/int-set-generate$(EXE)
TEST := $(BUILD_DIR)/test-int-set$(EXE)

.PHONY: all test data clean
all: $(GENERATOR) $(TEST)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/int_set.o: int_set.c int_set.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/generate.o: generate.c int_set.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/test-int-set.o: test_int_set.c int_set.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(GENERATOR): $(BUILD_DIR)/int_set.o $(BUILD_DIR)/generate.o
	$(CC) $^ -o $@
$(TEST): $(BUILD_DIR)/int_set.o $(BUILD_DIR)/test-int-set.o
	$(CC) $^ -o $@

test: $(TEST)
	./$(TEST)
# Regenerate int_set.json and int_set.xml from the schema.
data: $(GENERATOR)
	./$(GENERATOR)
clean:
	rm -rf $(BUILD_DIR)
