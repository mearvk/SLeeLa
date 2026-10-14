# =============================================================================
# SLVM/11 "Sovereign" — build.
#
# Two products:
#   1. libslvm11fs.a            the generation feature: the filesystem-module
#                               host (schema-driven module validation, TAC3).
#   2. slvm-11                  a REAL runnable virtual machine: it links the
#                               shared native Core engine (impl/build/libslcore.a)
#                               and runs a compiled .sleela artifact, admitting
#                               the filesystem-module contract before boot.
#
# The Core engine archive is produced by the impl build (`make -C impl libslcore`
# or the master build). SLCORE_LIB / SLCORE_INC point at it; override on the
# command line for an out-of-tree core.
# =============================================================================
CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
# Output dir for the runnable-VM build. NOT `build/`: this level commits a
# hand-written build/<platform>/ set of platform entry points that `make clean`
# must never remove. Our generated artifacts live under vm-build/.
BUILD = vm-build

# Shared native Core engine (base of the real VM).
IMPL ?= ../../impl
SLCORE_LIB ?= $(IMPL)/build/libslcore.a
SLCORE_INC ?= $(IMPL)/core
# The Core links the OS-abstraction backends + GC (bundled in libslcore.a) and
# needs the platform runtime libs + OpenMP (idea-canvas physics step).
ifeq ($(OS),Windows_NT)
  CORE_LIBS ?= -lws2_32 -lm
  THREADFLAG :=
else
  THREADFLAG := -pthread
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
    CORE_LIBS ?= -lm
  else
    CORE_LIBS ?= -ldl -lm
  endif
endif
CORE_LDLIBS ?= $(THREADFLAG) -fopenmp $(CORE_LIBS)

LIB = $(BUILD)/libslvm11fs.a
TEST = $(BUILD)/slvm11-filesystem-module-selftest
VM = $(BUILD)/slvm-11

all: $(LIB) $(VM)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/slvm11_filesystem_module.o: src/slvm11_filesystem_module.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(BUILD)/slvm11_filesystem_module.o
	ar rcs $@ $^

# The real VM: generation feature (libslvm11fs.a) + the shared Core engine.
# The runner is C, but the Core archive contains C++ objects (the idea-canvas
# physics/render layer), so the final link is driven by the C++ compiler to
# pull in the C++ runtime. The C source is compiled to an object with CC first.
$(BUILD)/slvm11_vm.o: src/slvm11_vm.c | $(BUILD)
	$(CC) $(CFLAGS) -I$(SLCORE_INC) -c $< -o $@
$(VM): $(BUILD)/slvm11_vm.o $(LIB) $(SLCORE_LIB) | $(BUILD)
	$(CXX) $(BUILD)/slvm11_vm.o $(LIB) $(SLCORE_LIB) -o $@ $(CORE_LDLIBS)

$(TEST): tests/slvm11_filesystem_module_selftest.c $(LIB) | $(BUILD)
	$(CC) $(CFLAGS) $< $(LIB) -o $@

test: $(TEST) $(VM)
	$(TEST)
	$(VM) --selfcheck

check: test
	test -f file-system/modules/tac3/tac3.definition.json
	echo "SLVM/11 filesystem module check: PASS"

clean:
	rm -rf $(BUILD)

.PHONY: all test check clean
