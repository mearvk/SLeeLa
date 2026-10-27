# Sleelavirin antivirus co-package build
# Max Rupplin - MEARVK LLC - 2026
#
# Portable C11 core. No third-party engine is required for this local matcher.
# External antivirus providers (for example ClamAV/AMSI) are separate adapters.

CC ?= cc
CXX ?= c++
AR ?= ar
CPPFLAGS ?=
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -O2 -Wall -Wextra -Wpedantic
LDFLAGS ?=

BUILD_DIR ?= build
SRC_DIR := src
TEST_DIR := tests

SCAN_OBJ := $(BUILD_DIR)/sleelavirin_scan.o
HEURISTIC_OBJ := $(BUILD_DIR)/sleelavirin_heuristic.o
NATIVE_OBJ := $(BUILD_DIR)/sleelavirin_native.o
AC_OBJ := $(BUILD_DIR)/sleelavirin_ac.o
SIG_OBJ := $(BUILD_DIR)/sleelavirin_sig.o
BRIDGE_OBJ := $(BUILD_DIR)/sleelavirin_bridge.o
CPP_OBJ := $(BUILD_DIR)/sleelavirin_cpp.o

# The C static library bundles the whole native engine: hash map + Trap-2
# heuristics + SHA-256 + Aho-Corasick content matcher + signature loader +
# the stable scan bridge.
LIB_OBJS := $(SCAN_OBJ) $(HEURISTIC_OBJ) $(NATIVE_OBJ) $(AC_OBJ) $(SIG_OBJ) $(BRIDGE_OBJ)
LIBRARY := $(BUILD_DIR)/libsleelavirin.a
CPP_LIBRARY := $(BUILD_DIR)/libsleelavirin_cpp.a

TEST_BIN := $(BUILD_DIR)/test_scan
TEST_AC_BIN := $(BUILD_DIR)/test_ac
TEST_BRIDGE_BIN := $(BUILD_DIR)/test_bridge
TEST_CPP_BIN := $(BUILD_DIR)/test_cpp_scan
NATIVE_MAIN_OBJ := $(BUILD_DIR)/sleelavirin_native_main.o
NATIVE_BIN := $(BUILD_DIR)/sleelavirin-scan

.PHONY: all library cpp-library native native-test content-test cpp-test clamav clamav-check clamav-update clamav-snapshot-check definitions-import definitions-check integrity-manifest integrity-check test check clean help

all: library cpp-library $(TEST_BIN) $(TEST_AC_BIN) $(TEST_BRIDGE_BIN) $(TEST_CPP_BIN) native

library: $(LIBRARY)

$(BUILD_DIR):
	mkdir -p "$@"

$(SCAN_OBJ): $(SRC_DIR)/sleelavirin_scan.c $(SRC_DIR)/sleelavirin_scan.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(HEURISTIC_OBJ): $(SRC_DIR)/sleelavirin_heuristic.c $(SRC_DIR)/sleelavirin_heuristic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(NATIVE_OBJ): $(SRC_DIR)/sleelavirin_native.c $(SRC_DIR)/sleelavirin_native.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(AC_OBJ): $(SRC_DIR)/sleelavirin_ac.c $(SRC_DIR)/sleelavirin_ac.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(SIG_OBJ): $(SRC_DIR)/sleelavirin_sig.c $(SRC_DIR)/sleelavirin_sig.h $(SRC_DIR)/sleelavirin_ac.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(BRIDGE_OBJ): $(SRC_DIR)/sleelavirin_bridge.c $(SRC_DIR)/sleelavirin_bridge.h $(SRC_DIR)/sleelavirin_native.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

$(CPP_OBJ): $(SRC_DIR)/sleelavirin_cpp.cpp $(SRC_DIR)/sleelavirin_cpp.hpp $(SRC_DIR)/sleelavirin_scan.h $(SRC_DIR)/sleelavirin_native.h | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -std=c++17 -I$(SRC_DIR) -c "$<" -o "$@"

$(NATIVE_MAIN_OBJ): $(SRC_DIR)/sleelavirin_native_main.c $(SRC_DIR)/sleelavirin_native.h $(SRC_DIR)/sleelavirin_ac.h $(SRC_DIR)/sleelavirin_sig.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) -c "$<" -o "$@"

# The native scanner links the full engine library so it can do both hash and
# content (Aho-Corasick) scanning.
$(NATIVE_BIN): $(NATIVE_MAIN_OBJ) $(LIBRARY)
	$(CC) $(CFLAGS) $(NATIVE_MAIN_OBJ) $(LIBRARY) $(LDFLAGS) -o "$@"

native: $(NATIVE_BIN)

$(LIBRARY): $(LIB_OBJS)
	$(AR) rcs "$@" $^

library: $(LIBRARY)

$(CPP_LIBRARY): $(CPP_OBJ)
	$(AR) rcs "$@" $^

cpp-library: $(CPP_LIBRARY)

$(TEST_BIN): $(TEST_DIR)/test_scan.c $(LIBRARY) $(SRC_DIR)/sleelavirin_scan.h $(SRC_DIR)/sleelavirin_heuristic.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) "$<" $(LIBRARY) $(LDFLAGS) -o "$@"

$(TEST_AC_BIN): $(TEST_DIR)/test_ac.c $(LIBRARY) $(SRC_DIR)/sleelavirin_ac.h $(SRC_DIR)/sleelavirin_sig.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) "$<" $(LIBRARY) $(LDFLAGS) -o "$@"

$(TEST_BRIDGE_BIN): $(TEST_DIR)/test_bridge.c $(LIBRARY) $(SRC_DIR)/sleelavirin_bridge.h | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -std=c11 -I$(SRC_DIR) "$<" $(LIBRARY) $(LDFLAGS) -o "$@"

$(TEST_CPP_BIN): $(TEST_DIR)/test_cpp_scan.cpp $(CPP_LIBRARY) $(LIBRARY) $(SRC_DIR)/sleelavirin_cpp.hpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -std=c++17 -I$(SRC_DIR) "$<" $(CPP_LIBRARY) $(LIBRARY) $(LDFLAGS) -o "$@"

test check: all
	"$(TEST_BIN)"
	"$(TEST_AC_BIN)"
	"$(TEST_BRIDGE_BIN)"
	"$(TEST_CPP_BIN)"
	$(MAKE) native-test
	$(MAKE) content-test
	python3 -m py_compile tools/sleelavirin_scan.py tools/sleelavirin_clamav.py tools/sleelavirin_import_clamav.py tools/sleelavirin_update_clamav.py tools/sleelavirin_verify_signature.py tools/sleelavirin_manifest.py tests/test_tools.py tests/test_update_clamav.py tests/test_verify_signature.py tests/test_scan_report.py tests/test_eicar_integration.py tests/test_manifest.py
	python3 -m unittest discover -s tests -p 'test_*.py' -v
	$(MAKE) definitions-check

native-test: $(NATIVE_BIN)
	@set +e; output=$$("./$(NATIVE_BIN)" --database tests/native-sha256.txt tests/native_sample.txt); status=$$?; set -e; test "$$status" -eq 1; echo "$$output"; echo "$$output" | grep -q '^DETECTED tests/native_sample.txt : test-sample$$'
	@set -e; output=$$("./$(NATIVE_BIN)" --database signatures/native-sha256.txt tests/native_sample.txt); status=$$?; test "$$status" -eq 0; echo "$$output"; echo "$$output" | grep -q '^NO_MATCH tests/native_sample.txt$$'
	@echo "Native scanner signature/no-match tests: PASS"

# Verify the Aho-Corasick content scanner end-to-end through the CLI.
content-test: $(NATIVE_BIN)
	@printf 'harmless EICAR-STANDARD-ANTIVIRUS\r\n tail' > "$(BUILD_DIR)/content_hit.tmp"
	@set +e; output=$$("./$(NATIVE_BIN)" --content-db tests/content-signatures.slv "$(BUILD_DIR)/content_hit.tmp" 2>/dev/null); status=$$?; set -e; test "$$status" -eq 1; echo "$$output" | grep -q '^DETECTED .*content:EICAR-Test-Fragment'
	@printf 'nothing notable here' > "$(BUILD_DIR)/content_clean.tmp"
	@set -e; output=$$("./$(NATIVE_BIN)" --content-db tests/content-signatures.slv "$(BUILD_DIR)/content_clean.tmp" 2>/dev/null); status=$$?; test "$$status" -eq 0; echo "$$output" | grep -q '^NO_MATCH '
	@rm -f "$(BUILD_DIR)/content_hit.tmp" "$(BUILD_DIR)/content_clean.tmp"
	@echo "Native content-signature scanner tests: PASS"

cpp-test: $(TEST_CPP_BIN)
	"$(TEST_CPP_BIN)"

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

integrity-manifest:
	python3 tools/sleelavirin_manifest.py --root . --write build/INTEGRITY-MANIFEST.json

integrity-check:
	@test -n "$(MANIFEST)" || { echo "Usage: make integrity-check MANIFEST=PATH" >&2; exit 2; }
	python3 tools/sleelavirin_manifest.py --root . --verify "$(MANIFEST)"

clamav-check:
	@command -v clamscan >/dev/null 2>&1 || { echo "ClamAV not found; install ClamAV and ensure clamscan is on PATH." >&2; exit 2; }
	@clamscan --version

clean:
	rm -rf "$(BUILD_DIR)"

help:
	@echo "Sleelavirin antivirus co-package"
	@echo "  make          Build the C library, C++ library, tests, and native scanner"
	@echo "  make library  Build build/libsleelavirin.a (C engine: hash+heuristic+AC+bridge)"
	@echo "  make cpp-library Build build/libsleelavirin_cpp.a (C++ wrapper)"
	@echo "  make test     Build and run all C, C++, native, and Python tests"
	@echo "  make native   Build build/sleelavirin-scan (hash + content scanner)"
	@echo "                  Usage: sleelavirin-scan [--database H] [--content-db C] FILE..."
	@echo "  make content-test Verify the Aho-Corasick content scanner via the CLI"
	@echo "  make cpp-test Build and run the C++ library test"
	@echo "  make definitions-check Validate the native SHA-256 index"
	@echo "  make definitions-import Show compatible ClamAV import usage"
	@echo "  make integrity-manifest Generate SHA-256 inventory at build/INTEGRITY-MANIFEST.json"
	@echo "  make integrity-check MANIFEST=PATH Verify hashes and detect missing/extra files"
	@echo "  make clamav-update Retrieve and validate official ClamAV databases"
	@echo "  make clamav-snapshot-check SNAPSHOT=DIR Verify snapshot SHA-256 manifest"
	@echo "  python3 tools/sleelavirin_verify_signature.py --help Verify signed release artifacts"
	@echo "  make clean    Remove antivirus/build/"
	@echo "  CC=clang make Use Clang instead of the default C compiler"
