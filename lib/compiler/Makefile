# SLeeLa compiler package build
# Max Rupplin - MEARVK LLC - 2026
CC ?= cc
CXX ?= c++
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
BUILD_DIR ?= build

C_SOURCES := src/sleela_compiler.c src/sleela_langc.c src/sleela_lang_bridge.c
CPP_SOURCES := src/sleela_compiler.cpp src/sleela_langc.cpp
# C and C++ objects go to distinct names so same-basename units never collide.
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.c.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.cpp.o,$(CPP_SOURCES))

.PHONY: all native runtime inventory sanity tutorial-check selftest clean help
all: native selftest runtime inventory sanity tutorial-check
native: $(C_OBJECTS) $(CPP_OBJECTS)

# Behavioral self-test for the multi-language front-end framework ABI. Links the
# core ABI + bridge objects with the test driver and runs the assertions.
selftest: $(BUILD_DIR)/sleela_langc.c.o $(BUILD_DIR)/sleela_lang_bridge.c.o | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/langc_selftest.c $^ -o $(BUILD_DIR)/langc_selftest
	$(BUILD_DIR)/langc_selftest

# Resolve to an absolute path so it stays correct when passed into `make -C
# ../../impl` (a relative path would otherwise be re-interpreted against impl/).
SLEELA_SHA256_MANIFEST ?= $(abspath $(CURDIR)/../../security/sha256-manifest.json)

runtime:
	$(MAKE) -C ../../impl build/sleela SLEELA_SHA256_MANIFEST="$(SLEELA_SHA256_MANIFEST)"

inventory:
	python3 ../../tools/sleela-build.py inventory-lib

tutorial-check:
	@printf '%s\n' 'SLeeLa compiler tutorial check'
	@printf '%s\n' '  lessons: 01-source-to-vm 02-compiler-manager-and-profiles 03-language-and-binary-format-reference 04-building-a-language-front-end'
	@printf '%s\n' '  examples: tutorials/examples/*.sleela'
sanity:
	@printf '%s\n' 'SLeeLa compiler package sanity check'
	@printf '%s\n' '  pipeline: source lexer parser symbols semantics IR lowering codegen VM-target'
	@printf '%s\n' '  profiles: BASIC_COMPLETE ADVANCED_TOTAL'
	@printf '%s\n' '  source authority: .sleela'
	@printf '%s\n' '  multi-language framework: SLLanguageCompiler + SLCompilerRegistry + frontends/<lang>/'
	@printf '%s\n' '  framework targets: SLVM SLJVM SLIR NATIVE(gated)'
	@printf '%s\n' '  language/format reference: LANGUAGE.FORMAT.REFERENCE.md'
	@printf '%s\n' '  safety rule: references identify; they do not authorize execution'
	@printf '%s\n' '  native boundary: C ABI + C++ orchestration'
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.c.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.cpp.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(BUILD_DIR)
help:
	@printf '%s\n' 'SLeeLa compiler package build'
	@printf '%s\n' '  make all       Build native compiler objects and sanity check'
	@printf '%s\n' '  make native    Compile C and C++ compiler objects'
	@printf '%s\n' '  make selftest  Build and run the multi-language framework self-test'
	@printf '%s\n' '  make sanity    Print compiler contract'
	@printf '%s\n' '  make tutorial-check  Print compiler tutorial and example inventory'
	@printf '%s\n' '  make clean     Remove package build objects'
