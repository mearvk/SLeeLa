# SLeeLa text package build
# Max Rupplin - MEARVK LLC - 2026
#
# Builds the native string-processing boundary (C ABI + C++ orchestration) for
# the text library family. The .sleela definitions remain the source-level
# contract; this native layer is the implementation boundary below the VM/OS
# bridge, matching the lib/compiler package convention.
CC ?= cc
CXX ?= c++
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
BUILD_DIR ?= build

C_SOURCES := src/sleela_string.c
CPP_SOURCES := src/sleela_string.cpp
# Distinct object stems (.c.o / .cpp.o) so the C and C++ translation units never
# collide on a shared build/<stem>.o target: both are always compiled.
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.c.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.cpp.o,$(CPP_SOURCES))

.PHONY: all native sanity clean help
all: native sanity
native: $(C_OBJECTS) $(CPP_OBJECTS)

sanity:
	@printf '%s\n' 'SLeeLa text package sanity check'
	@printf '%s\n' '  services: substring slice find rfind count contains starts/ends-with'
	@printf '%s\n' '            trim case-fold reverse replace split join'
	@printf '%s\n' '  front-end objects: SLString SLStringBuilder SLStringBuffer'
	@printf '%s\n' '  source authority: .sleela'
	@printf '%s\n' '  native boundary: C ABI (sleela_string.h) + C++ orchestration (sleela_string.hpp)'
	@printf '%s\n' '  contract: length-explicit, NUL-tolerant, allocation-owning'

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.c.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.cpp.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(BUILD_DIR)
help:
	@printf '%s\n' 'SLeeLa text package build'
	@printf '%s\n' '  make all     Build native text objects and sanity check'
	@printf '%s\n' '  make native  Compile C and C++ string-processing objects'
	@printf '%s\n' '  make sanity  Print text package contract'
	@printf '%s\n' '  make clean   Remove package build objects'
