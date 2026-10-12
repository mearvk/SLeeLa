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
NATIVE_OBJ := $(BUILD_DIR)/sleelavirin_native.o
NATIVE_MAIN_OBJ := $(BUILD_DIR)/sleelavirin_native_main.o
NATIVE_BIN := $(BUILD_DIR)/sleelavirin-scan

.PHONY: all library native native-test clamav clamav-check clamav-update clamav-snapshot-check definitions-import definitions-check test check clean help

all: library $(TEST_BIN) native

library: $(LIBRARY)

$(BUILD_DIR):
	mkdir -p "$@"

$(SCAN_OBJ): $(SRC_DIR)/sleelavirin_scan.c $(SRC_DIR)/sleelavirin_scan.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(HEURISTIC_OBJ): $(SRC_DIR)/sleelavirin_heuristic.c $(SRC_DIR)/sleelavirin_heuristic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(NATIVE_OBJ): $(SRC_DIR)/sleelavirin_native.c $(SRC_DIR)/sleelavirin_native.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(NATIVE_MAIN_OBJ): $(SRC_DIR)/sleelavirin_native_main.c $(SRC_DIR)/sleelavirin_native.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(NATIVE_BIN): $(NATIVE_OBJ) $(NATIVE_MAIN_OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o "$@"

native: $(NATIVE_BIN)

$(LIBRARY): $(SCAN_OBJ) $(HEURISTIC_OBJ)
	$(AR) rcs "$@" $^

$(TEST_BIN): $(TEST_DIR)/test_scan.c $(LIBRARY) $(SRC_DIR)/sleelavirin_scan.h $(SRC_DIR)/sleelavirin_heuristic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) "$<" $(LIBRARY) $(LDFLAGS) -o "$@"

test check: all
	"$(TEST_BIN)"
	$(MAKE) native-test
	python3 -m py_compile tools/sleelavirin_scan.py tools/sleelavirin_clamav.py tools/sleelavirin_import_clamav.py tools/sleelavirin_update_clamav.py tools/sleelavirin_verify_signature.py tests/test_tools.py tests/test_update_clamav.py tests/test_verify_signature.py tests/test_scan_report.py
	python3 -m unittest discover -s tests -p 'test_*.py' -v
	$(MAKE) definitions-check

native-test: $(NATIVE_BIN)
	@set +e; output=$$("./$(NATIVE_BIN)" --database tests/native-sha256.txt tests/native_sample.txt); status=$$?; set -e; test "$$status" -eq 1; echo "$$output"; echo "$$output" | grep -q '^DETECTED tests/native_sample.txt : test-sample$$'
	@set -e; output=$$("./$(NATIVE_BIN)" --database signatures/native-sha256.txt tests/native_sample.txt); status=$$?; test "$$status" -eq 0; echo "$$output"; echo "$$output" | grep -q '^NO_MATCH tests/native_sample.txt$$'
	@echo "Native scanner signature/no-match tests: PASS"

clamav: clamav-check
	python3 tools/sleelavirin_clamav.py --help >/dev/null

clamav-update:
	python3 tools/sleelavirin_update_clamav.py --root signatures/clamav-cache

clamav-snapshot-check:
	@test -n "$(SNAPSHOT)" || { echo "Usage: make clamav-snapshot-check SNAPSHOT=signatures/clamav-cache/snapshots/<id>" >&2; exit 2; }
	python3 tools/sleelavirin_update_clamav.py --check "$(SNAPSHOT)"

definitions-import:
	@echo "Usage: python3 tools/sleelavirin_import_clamav.py --source DIR --output signatures/native-sha256.txt"
	@echo "Only compatible extracted text .hsb SHA-256 records are imported."

definitions-check:
	python3 tools/sleelavirin_import_clamav.py --check signatures/native-sha256.txt

clamav-check:
	@command -v clamscan >/dev/null 2>&1 || { echo "ClamAV not found; install ClamAV and ensure clamscan is on PATH." >&2; exit 2; }
	@clamscan --version

clean:
	rm -rf "$(BUILD_DIR)"

help:
	@echo "Sleelavirin antivirus co-package"
	@echo "  make          Build the static library, self-test, and native scanner"
	@echo "  make library  Build build/libsleelavirin.a"
	@echo "  make test     Build and run the local C and native scanner tests"
	@echo "  make definitions-check Validate the native SHA-256 index"
	@echo "  make definitions-import Show compatible ClamAV import usage"
	@echo "  make clamav-update Retrieve and validate official ClamAV databases"
	@echo "  make clamav-snapshot-check SNAPSHOT=DIR Verify snapshot SHA-256 manifest"
	@echo "  python3 tools/sleelavirin_verify_signature.py --help Verify signed release artifacts"
	@echo "  make clean    Remove antivirus/build/"
	@echo "  CC=clang make Use Clang instead of the default C compiler"
