# Sleelavirin antivirus co-package build
# Max Rupplin - MEARVK LLC - 2026
#
# Portable C11 core. No third-party engine is required for this local matcher.
# External antivirus providers (for example ClamAV/AMSI) are separate adapters.

CC ?= cc
AR ?= ar
CPPFLAGS ?=
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
LDFLAGS ?=

BUILD_DIR ?= build
SRC_DIR := src
TEST_DIR := tests

SCAN_OBJ := $(BUILD_DIR)/sleelavirin_scan.o
HEURISTIC_OBJ := $(BUILD_DIR)/sleelavirin_heuristic.o
LIBRARY := $(BUILD_DIR)/libsleelavirin.a
TEST_BIN := $(BUILD_DIR)/test_scan

.PHONY: all library test check clean help

all: library $(TEST_BIN)

library: $(LIBRARY)

$(BUILD_DIR):
	mkdir -p "$@"

$(SCAN_OBJ): $(SRC_DIR)/sleelavirin_scan.c $(SRC_DIR)/sleelavirin_scan.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(HEURISTIC_OBJ): $(SRC_DIR)/sleelavirin_heuristic.c $(SRC_DIR)/sleelavirin_heuristic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(LIBRARY): $(SCAN_OBJ) $(HEURISTIC_OBJ)
	$(AR) rcs "$@" $^

$(TEST_BIN): $(TEST_DIR)/test_scan.c $(LIBRARY) $(SRC_DIR)/sleelavirin_scan.h $(SRC_DIR)/sleelavirin_heuristic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) "$<" $(LIBRARY) $(LDFLAGS) -o "$@"

test check: all
	"$(TEST_BIN)"

clean:
	rm -rf "$(BUILD_DIR)"

help:
	@echo "Sleelavirin antivirus co-package"
	@echo "  make          Build the static library and self-test executable"
	@echo "  make library  Build build/libsleelavirin.a"
	@echo "  make test     Build and run the local C self-tests"
	@echo "  make clean    Remove antivirus/build/"
	@echo "  CC=clang make Use Clang instead of the default C compiler"
