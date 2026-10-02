# SLeeLa VM package build
# Max Rupplin - MEARVK LLC - 2026
CC ?= cc
CXX ?= c++
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
BUILD_DIR ?= build

C_SOURCES := src/sleela_vm_compiler_manager.c src/sleela_vm_management.c src/sleela_vm_linking.c src/sleela_vm_challenge.c src/sleela_vm_reports.c src/sleela_vm_dynamic_memory_guard.c
CPP_SOURCES := src/sleela_vm_compiler_manager.cpp src/sleela_vm_management.cpp src/sleela_vm_linking.cpp src/sleela_vm_challenge.cpp src/sleela_vm_reports.cpp src/sleela_vm_dynamic_memory_guard.cpp
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))

.PHONY: all native cm-check memory-guard-check tutorial-check clean help
all: native cm-check memory-guard-check tutorial-check
native: $(C_OBJECTS) $(CPP_OBJECTS)\n\ntutorial-check:\n\t@printf '%s\\n' 'SLeeLa VM tutorial check'\n\t@printf '%s\\n' '  lessons: basic VM, progressive VM source, Startup Module, Dynamic Memory Guard, memory, security, linking'\n\t@printf '%s\\n' '  examples: tutorials/examples/*.sleela'
cm-check:
	@printf '%s\n' 'Compiler Manager package sanity check'
	@printf '%s\n' '  profiles: BASIC_COMPLETE ADVANCED_TOTAL'
	@printf '%s\n' '  categories: architecture execution memory security io runtime management linkage observability build'
	@printf '%s\n' '  findings: FINE MISSING EXCESS REQUIRES INVALID'
	@printf '%s\n' '  declaration boundary: explicit; no implicit object insertion/removal'
memory-guard-check:
	@printf '%s\n' 'Dynamic Memory Guard modes: HARD SLOW_CAREFUL AGGRESSIVE'
	@printf '%s\n' '  policy: explicit developer configuration; physical limit remains authoritative'
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf $(BUILD_DIR)
help:
	@printf '%s\n' 'SLeeLa VM package build'
	@printf '%s\n' '  make all       Build native VM manager objects and CM sanity check'
	@printf '%s\n' '  make native    Compile C and C++ VM manager objects'
	@printf '%s\n' '  make cm-check  Print the Compiler Manager declaration contract'
	@printf '%s\n' '  make memory-guard-check  Print Dynamic Memory Guard policy modes\n\t@printf '%s\\n' '  make tutorial-check  Print VM tutorial and example inventory''
	@printf '%s\n' '  make clean     Remove package build objects'
