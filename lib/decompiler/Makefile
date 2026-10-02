# SLeeLa decompiler package build
# Max Rupplin - MEARVK LLC - 2026
CC ?= cc
CXX ?= c++
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
BUILD_DIR ?= build
C_SOURCES := src/sleela_decompiler.c
CPP_SOURCES := src/sleela_decompiler.cpp
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))
.PHONY: all native sanity clean help
all: native sanity
native: $(C_OBJECTS) $(CPP_OBJECTS)
sanity:
	@printf '%s\n' 'SLeeLa decompiler package sanity check'
	@printf '%s\n' '  pipeline: artifact format/OS decoder CFG SLIR analysis reconstruction output VM'
	@printf '%s\n' '  profiles: BASIC_COMPLETE ADVANCED_TOTAL'
	@printf '%s\n' '  fractional input: PRESERVE PARTIAL REPORT_ONLY STRICT'
	@printf '%s\n' '  OS discernment: weighted evidence with conflict preservation'
	@printf '%s\n' '  source authority: .sleela'
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(BUILD_DIR)
help:
	@printf '%s\n' 'SLeeLa decompiler package build'
	@printf '%s\n' '  make all       Build native decompiler objects and sanity check'
	@printf '%s\n' '  make native    Compile C and C++ decompiler objects'
	@printf '%s\n' '  make sanity    Print decompiler contract'
	@printf '%s\n' '  make clean     Remove package build objects'
