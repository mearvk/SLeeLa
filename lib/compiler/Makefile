# SLeeLa compiler package build
# Max Rupplin - MEARVK LLC - 2026
CC ?= cc
CXX ?= c++
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
BUILD_DIR ?= build

C_SOURCES := src/sleela_compiler.c
CPP_SOURCES := src/sleela_compiler.cpp
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))

.PHONY: all native sanity clean help
all: native sanity tutorial-check
native: $(C_OBJECTS) $(CPP_OBJECTS)

tutorial-check:
	@printf '%s\n' 'SLeeLa compiler tutorial check'
	@printf '%s\n' '  lessons: 01-source-to-vm 02-compiler-manager-and-profiles 03-language-and-binary-format-reference'
	@printf '%s\n' '  examples: tutorials/examples/*.sleela'
sanity:
	@printf '%s\n' 'SLeeLa compiler package sanity check'
	@printf '%s\n' '  pipeline: source lexer parser symbols semantics IR lowering codegen VM-target'
	@printf '%s\n' '  profiles: BASIC_COMPLETE ADVANCED_TOTAL'
	@printf '%s\n' '  source authority: .sleela'\n\t@printf '%s\n' '  language/format reference: LANGUAGE.FORMAT.REFERENCE.md'\n\t@printf '%s\n' '  safety rule: references identify; they do not authorize execution'
	@printf '%s\n' '  native boundary: C ABI + C++ orchestration'
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(BUILD_DIR)
help:
	@printf '%s\n' 'SLeeLa compiler package build'
	@printf '%s\n' '  make all       Build native compiler objects and sanity check'
	@printf '%s\n' '  make native    Compile C and C++ compiler objects'
	@printf '%s\n' '  make sanity    Print compiler contract'
	@printf '%s\n' '  make tutorial-check  Print compiler tutorial and example inventory''
	@printf '%s\n' '  make clean     Remove package build objects'
