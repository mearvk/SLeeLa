# SLeeLa VM package build
# Max Rupplin - MEARVK LLC - 2026
CC ?= cc
CXX ?= c++
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
BUILD_DIR ?= build

C_SOURCES := src/sleela_vm_compiler_manager.c src/sleela_vm_management.c src/sleela_vm_linking.c src/sleela_vm_challenge.c src/sleela_vm_reports.c src/sleela_vm_dynamic_memory_guard.c
CPP_SOURCES := src/sleela_vm_compiler_manager.cpp src/sleela_vm_management.cpp src/sleela_vm_linking.cpp src/sleela_vm_challenge.cpp src/sleela_vm_reports.cpp src/sleela_vm_dynamic_memory_guard.cpp src/sleela_vm_gc.cpp
C_OBJECTS := $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SOURCES))

.PHONY: all native cm-check memory-guard-check gc-check runtime-check creator-check tutorial-check clean help
all: native cm-check memory-guard-check gc-check runtime-check creator-check tutorial-check
native: $(C_OBJECTS) $(CPP_OBJECTS)

tutorial-check:
	@printf '%s\n' 'SLeeLa VM tutorial check'
	@printf '%s\n' '  lessons: basic VM, progressive VM source, Startup Module, Dynamic Memory Guard, memory, security, linking'
	@printf '%s\n' '  examples: tutorials/examples/*.sleela, creator/examples/*.sleela'

# The runnable Secondary-VM runtime object model (interpreter over the native
# SLSleelaVM substrate) plus the native-RAM option.
runtime-check:
	@printf '%s\n' 'Secondary VM runtime check'
	@for c in SLVM SLVMHeap SLVMMemory SLVMObject SLVMValue SLVMClass SLVMField SLVMMethod SLVMFrame SLVMThread SLVMModule SLVMModuleLoader SLVMNativeBinding SLNativeMemory ; do \
	  test -f "$$c.sleela" && printf '  present: %s\n' "$$c" || { echo "  MISSING: $$c" ; exit 1 ; } ; \
	done
	@printf '%s\n' '  options: native-RAM residency (SLNativeMemory, MODE_NATIVE) for running outside simulation'

# The VM Creator Edition: builds+boots a live SLVM, hosts a Sleela CPU as the
# executor, runs C/C++/Java/Sleela programs; options DMA / GPU / NATIVE_RAM.
creator-check:
	@printf '%s\n' 'VM Creator check'
	@for c in SleelaVMCreator SleelaVMGenerationCatalog SleelaVMCore SLVMFoundation SLVMOperator SLVMSpecialist SLVMSupervisor SLVMManager SLVMDirector SLVMAdministrator SLVMExecutive SLVMAuthority SLVMPrincipal SLVMSovereign ; do \
	  test -f "creator/$$c.sleela" && printf '  present: %s\n' "$$c" || { echo "  MISSING: creator/$$c" ; exit 1 ; } ; \
	done
	@printf '%s\n' '  generations: /impl Core .. /11 Sovereign; options: requestDMA requestGPU requestNativeRam'
cm-check:
	@printf '%s\n' 'Compiler Manager package sanity check'
	@printf '%s\n' '  profiles: BASIC_COMPLETE ADVANCED_TOTAL'
	@printf '%s\n' '  categories: architecture execution memory security io runtime management linkage observability build'
	@printf '%s\n' '  findings: FINE MISSING EXCESS REQUIRES INVALID'
	@printf '%s\n' '  declaration boundary: explicit; no implicit object insertion/removal'
gc-check:
	@printf '%s\n' 'Modern GC: generational incremental tracing, SATB pre-write barrier, remembered set, safepoints'
	@printf '%s\n' '  source: runtime/garbage_collector.c; C++ RAII: src/sleela_vm_gc.cpp'
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
	@printf '%s\n' '  make memory-guard-check  Print Dynamic Memory Guard policy modes'
	@printf '%s\n' '  make gc-check  Print the common GC contract'
	@printf '%s\n' '  make runtime-check  Verify the Secondary VM runtime object model + native-RAM option'
	@printf '%s\n' '  make creator-check  Verify the VM Creator sources + DMA/GPU/native-RAM options'
	@printf '%s\n' '  make tutorial-check  Print VM tutorial and example inventory'
	@printf '%s\n' '  make clean     Remove package build objects'
