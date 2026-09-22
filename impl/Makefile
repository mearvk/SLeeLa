# =============================================================================
# Makefile for Sleela (C/C++ core + front end) and Nordshrift (.sst -> triple).
# =============================================================================
CC ?= gcc
CXX ?= g++
SUBJECT_INCLUDES := -Isubjects/native -Isubjects/math -Isubjects/physics \
                    -Isubjects/economics -Isubjects/inference \
                    -Isubjects/chemistry -Isubjects/finance

# ---------------------------------------------------------------------------
# Platform detection. The C/C++ core is OS-neutral (it goes through the
# sleela_thread/net/io abstraction layer), so the only per-OS differences are
# the thread flag, the link libraries, and the executable suffix.
#   * Linux/macOS (POSIX): -pthread, link libdl (dlopen) + libm.
#   * Windows 10+ (MinGW):  Win32 threads (no -pthread), link Winsock (ws2_32);
#                           no libdl (LoadLibrary is used via sleela_library),
#                           binaries get a .exe suffix.
# GNU Make on Windows/MinGW sets OS=Windows_NT.
# ---------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
  PLATFORM := windows
  EXE := .exe
  THREADFLAG :=
  PLATFORM_LIBS := -lws2_32 -lm
else
  EXE :=
  THREADFLAG := -pthread
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
    # macOS (Darwin, Apple clang): dlopen lives in libSystem, so there is no
    # separate -ldl to link (and passing it can error on some SDKs). clock_gettime
    # is also in libSystem, so no -lrt. -pthread is accepted by clang.
    PLATFORM := macos
    PLATFORM_LIBS := -lm
  else
    PLATFORM := linux
    PLATFORM_LIBS := -ldl -lm
  endif
endif

CFLAGS ?= -std=c11 -O2 -Wall -Wextra $(THREADFLAG)
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra $(THREADFLAG)
CPPFLAGS += -Ifrontend $(SUBJECT_INCLUDES) -I../bash
LDFLAGS ?= $(THREADFLAG)
LIBS ?= $(PLATFORM_LIBS)
BUILD := build
STAGE := $(BUILD)/SLeeLa
STAGE_BIN := $(STAGE)/bin
STAGE_CONFIG := $(STAGE)/Config
STAGE_OPTIONS := $(STAGE)/Options
STAGE_LIB := $(STAGE)/lib
STAGE_DOCS := $(STAGE)/docs
SLEELA_BIN := $(BUILD)/sleela$(EXE)
NORDSHRIFT_BIN := $(BUILD)/nordshrift$(EXE)
THREAD_SMOKE := $(BUILD)/thread_smoke
THREAD_PLATFORM_SMOKE := $(BUILD)/thread_platform_smoke
SOCKET_SMOKE := $(BUILD)/socket_smoke
SYNCHRO_SMOKE := $(BUILD)/synchro_smoke
MUNCTION_SMOKE := $(BUILD)/munction_smoke
BESTOF_SMOKE := $(BUILD)/bestof_smoke
IO_PLATFORM_SMOKE := $(BUILD)/io_platform_smoke
NET_PLATFORM_SMOKE := $(BUILD)/net_platform_smoke
PATH_PLATFORM_SMOKE := $(BUILD)/path_platform_smoke
TERMINAL_PLATFORM_SMOKE := $(BUILD)/terminal_platform_smoke
LIBRARY_PLATFORM_SMOKE := $(BUILD)/library_platform_smoke
TIME_PLATFORM_SMOKE := $(BUILD)/time_platform_smoke
PIXEL_TERMINAL_SMOKE := $(BUILD)/pixel_terminal_smoke
PIXEL_TERMINAL_OBJ := $(BUILD)/pixel_terminal.o
CATALOG_OBJ := $(BUILD)/sheet_catalog.o
XCLASS_OBJ := $(BUILD)/xclass_loader.o
LANGIN_OBJ := $(BUILD)/langin.o
CORE_OBJ := $(BUILD)/sleela_core.o
THREAD_OBJ := $(BUILD)/sleela_thread.o
IO_OBJ := $(BUILD)/sleela_io.o
NET_OBJ := $(BUILD)/sleela_net.o
SYNCHRO_OBJ := $(BUILD)/sleela_synchro.o
MUNCTION_OBJ := $(BUILD)/sleela_munction.o
BESTOF_OBJ := $(BUILD)/sleela_bestof.o
PATH_OBJ := $(BUILD)/sleela_path.o
TERMINAL_OBJ := $(BUILD)/sleela_terminal.o
LIBRARY_OBJ := $(BUILD)/sleela_library.o
TIME_OBJ := $(BUILD)/sleela_time.o
MEMORY_OBJ := $(BUILD)/sleela_memory.o
MEMMGR_OBJ := $(BUILD)/sleela_memmgr.o
DESIGN_ACTIVITY_OBJ := $(BUILD)/sleela_design_activity.o
GC_OBJ := $(BUILD)/garbage_collector.o
SECURITY_OBJ := $(BUILD)/security_supervisor.o
PARAMETERS_OBJ := $(BUILD)/Parameters.o
CHEMISTRY_OBJ := $(BUILD)/chemistry.o
FINANCIAL_OBJ := $(BUILD)/financial.o
CORE_SRC := core/sleela_artifact.c
FE_SRC := frontend/lexer.cpp frontend/parser.cpp frontend/compiler.cpp \
          frontend/version.cpp frontend/artifact.cpp \
          subjects/native/native_api.cpp \
          subjects/math/math_native.cpp subjects/physics/physics_native.cpp \
          subjects/economics/economics_native.cpp \
          subjects/inference/inference_native.cpp \
          subjects/chemistry/chemistry_api.cpp subjects/finance/financial_api.cpp
FE_OBJ := $(patsubst %.cpp,$(BUILD)/%.o,$(notdir $(FE_SRC)))
DRIVER_OBJ := $(BUILD)/driver.o
NS_SRC := nordshrift/sst_lexer.cpp nordshrift/sst_parser.cpp \
          nordshrift/source_resolve.cpp nordshrift/sleela_emit.cpp \
          nordshrift/object_compat.cpp nordshrift/subject_model.cpp nordshrift/input_object.cpp \
          nordshrift/component_manifest.cpp nordshrift/ledger_emit.cpp \
          nordshrift/nordshrift.cpp
NS_OBJ := $(patsubst nordshrift/%.cpp,$(BUILD)/ns_%.o,$(NS_SRC))
# Shared ledger core (lives under ../ledger, reused by the standalone tool).
LEDGER_CORE_OBJ := $(BUILD)/ledger_core.o
NS_OBJ += $(LEDGER_CORE_OBJ)
# The subset of Nordshrift the `sleela` binary links for its `nordshrift`
# round-trip subcommand: only the emitter (transpile a Sleela Program to a
# target). It deliberately EXCLUDES nordshrift.cpp (which has its own main()).
NS_EMIT_OBJ := $(BUILD)/ns_sleela_emit.o
VERIFY_TOOL := ../tools/verify-before-execution.py
VERIFY_MANIFEST ?= $(SLEELA_SHA256_MANIFEST)
VERIFY_ARGS = --manifest "$(VERIFY_MANIFEST)" --root "$(CURDIR)/.."
# Python launcher differs between Windows and POSIX shells. Keep the security
# gate in the build itself, but select a launcher that exists on each platform.
ifeq ($(OS),Windows_NT)
  PYTHON ?= py -3
else
  PYTHON ?= python3
endif
ALL_OBJECTS := $(CORE_OBJ) $(THREAD_OBJ) $(IO_OBJ) $(NET_OBJ) $(SYNCHRO_OBJ) $(MUNCTION_OBJ) $(BESTOF_OBJ) $(PATH_OBJ) $(TERMINAL_OBJ) $(LIBRARY_OBJ) $(TIME_OBJ) $(MEMORY_OBJ) $(MEMMGR_OBJ) $(DESIGN_ACTIVITY_OBJ) $(GC_OBJ) $(SECURITY_OBJ) $(PARAMETERS_OBJ) $(FE_OBJ) $(DRIVER_OBJ) $(CATALOG_OBJ) $(XCLASS_OBJ) $(LANGIN_OBJ) $(CHEMISTRY_OBJ) $(FINANCIAL_OBJ) $(PIXEL_TERMINAL_OBJ) $(NS_OBJ) $(THREAD_SMOKE) $(SOCKET_SMOKE) $(SYNCHRO_SMOKE) $(MUNCTION_SMOKE) $(BESTOF_SMOKE) $(IO_PLATFORM_SMOKE) $(NET_PLATFORM_SMOKE) $(PATH_PLATFORM_SMOKE) $(TERMINAL_PLATFORM_SMOKE) $(LIBRARY_PLATFORM_SMOKE) $(PIXEL_TERMINAL_SMOKE) $(THREAD_PLATFORM_SMOKE)

.PHONY: all test-platform stage package build-verified rebuild-verified clean test test-sleela test-version test-nordshrift test-threads test-thread-platform test-network test-network-platform test-synchro test-munction test-bestof test-io-platform test-path-platform test-terminal-platform test-library-platform test-time-platform test-pixel-terminal test-catalog test-chemistry test-financial test-math test-physics test-economics test-inference test-subjects verify-security build-runtime test-runtime test-memmgr

all: verify-security $(SLEELA_BIN) $(NORDSHRIFT_BIN) stage

# Build a stable, self-describing runtime layout. Build objects remain in build/;
# the staged result contains only runtime-facing files and the default Config
# and Options sets. Config is intentionally consolidated so a fresh build has
# one authoritative default configuration set.
stage: verify-security $(SLEELA_BIN) $(NORDSHRIFT_BIN)
	@rm -rf $(STAGE)
	@mkdir -p $(STAGE_BIN) $(STAGE_CONFIG) $(STAGE_OPTIONS) $(STAGE_LIB) $(STAGE_DOCS)
	@cp $(SLEELA_BIN) $(STAGE_BIN)/
	@cp $(NORDSHRIFT_BIN) $(STAGE_BIN)/
	@cp ../sleela.conf.example $(STAGE_CONFIG)/sleela.conf
	@cp ../MEMORY_MANAGER.md $(STAGE_DOCS)/MEMORY_MANAGER.md
	@cp ../PURPOSE.md $(STAGE_DOCS)/PURPOSE.md 2>/dev/null || true
	@printf '%s\n' '# SLeeLa Build Options' '' 'The staged runtime uses the defaults in Config/sleela.conf.' '' 'Available command-line memory options:' '' '  --memory-manager       Use the default 2 GiB ceiling' '  --memory-manager=SIZE  Set 256M through 2G' '  --mm=SIZE              Short form of --memory-manager=SIZE' > $(STAGE_OPTIONS)/OPTIONS.md
	@printf '%s\n' '# SLeeLa Runtime Layout' '' 'bin/       Executables' 'Config/    Consolidated default configuration' 'Options/   Runtime/build option reference' 'lib/       Reserved for runtime libraries' 'docs/      Runtime documentation' > $(STAGE)/README.md
	@echo "=== staged SLeeLa runtime: $(STAGE) ==="

package: stage
	@echo "=== SLeeLa package layout ready: $(STAGE) ==="
build-verified: verify-security $(SLEELA_BIN) $(NORDSHRIFT_BIN) stage
rebuild-verified:
	$(MAKE) clean
	$(MAKE) build-verified

verify-security:
	@if [ -z "$(VERIFY_MANIFEST)" ]; then echo "sleela: SHA-256 verification is required before build; set SLEELA_SHA256_MANIFEST or VERIFY_MANIFEST to a trusted JSON manifest" >&2; exit 1; fi
	@if [ ! -f "$(VERIFY_MANIFEST)" ]; then echo "sleela: trusted SHA-256 manifest not found: $(VERIFY_MANIFEST)" >&2; exit 1; fi
	@if [ ! -r "$(VERIFY_TOOL)" ]; then echo "sleela: verification tool not readable: $(VERIFY_TOOL)" >&2; exit 1; fi
	@command -v $(firstword $(PYTHON)) >/dev/null 2>&1 || { echo "sleela: $(PYTHON) is required for SHA-256 verification" >&2; exit 1; }
	@$(PYTHON) "$(VERIFY_TOOL)" $(VERIFY_ARGS)

# verify-security is an order-only prerequisite: it must run before any
# artifact is produced, but must never appear in $^ (some smoke-test link
# rules link with $^, and a phony target name would leak onto the link line).
$(ALL_OBJECTS): | verify-security

$(SLEELA_BIN): $(DESIGN_ACTIVITY_OBJ) $(CORE_OBJ) $(THREAD_OBJ) $(IO_OBJ) $(NET_OBJ) $(SYNCHRO_OBJ) $(MUNCTION_OBJ) $(BESTOF_OBJ) $(PATH_OBJ) $(TERMINAL_OBJ) $(LIBRARY_OBJ) $(TIME_OBJ) $(MEMORY_OBJ) $(MEMMGR_OBJ) $(GC_OBJ) $(SECURITY_OBJ) $(PARAMETERS_OBJ) $(FE_OBJ) $(DRIVER_OBJ) $(CATALOG_OBJ) $(XCLASS_OBJ) $(LANGIN_OBJ) $(CHEMISTRY_OBJ) $(FINANCIAL_OBJ) $(PIXEL_TERMINAL_OBJ) $(NS_EMIT_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)
$(NORDSHRIFT_BIN): $(DESIGN_ACTIVITY_OBJ) $(NS_OBJ) $(CORE_OBJ) $(THREAD_OBJ) $(IO_OBJ) $(NET_OBJ) $(SYNCHRO_OBJ) $(MUNCTION_OBJ) $(BESTOF_OBJ) $(PATH_OBJ) $(TERMINAL_OBJ) $(LIBRARY_OBJ) $(TIME_OBJ) $(MEMORY_OBJ) $(MEMMGR_OBJ) $(GC_OBJ) $(SECURITY_OBJ) $(PARAMETERS_OBJ) $(FE_OBJ) $(CATALOG_OBJ) $(CHEMISTRY_OBJ) $(FINANCIAL_OBJ) $(PIXEL_TERMINAL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)
$(THREAD_PLATFORM_SMOKE): core/thread_platform_smoke.c $(THREAD_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/thread_platform_smoke.c $(THREAD_OBJ) -o $@ $(LDFLAGS) $(LIBS)
# sleela_core.o (sleela_artifact.c) calls into the OS-abstraction backends
# (net/io/path/library/terminal), so any target that links the core must also
# link those objects, not just the core + time.
$(THREAD_SMOKE): core/thread_smoke.c $(CORE_OBJ) $(THREAD_OBJ) $(IO_OBJ) $(NET_OBJ) $(SYNCHRO_OBJ) $(MUNCTION_OBJ) $(BESTOF_OBJ) $(PATH_OBJ) $(LIBRARY_OBJ) $(TERMINAL_OBJ) $(TIME_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)
$(SOCKET_SMOKE): core/socket_smoke.c $(CORE_OBJ) $(THREAD_OBJ) $(IO_OBJ) $(NET_OBJ) $(SYNCHRO_OBJ) $(MUNCTION_OBJ) $(BESTOF_OBJ) $(PATH_OBJ) $(LIBRARY_OBJ) $(TERMINAL_OBJ) $(TIME_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LIBS)
$(SYNCHRO_SMOKE): core/synchro_smoke.c $(SYNCHRO_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/synchro_smoke.c $(SYNCHRO_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(BESTOF_SMOKE): core/bestof_smoke.c $(BESTOF_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/bestof_smoke.c $(BESTOF_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(MUNCTION_SMOKE): core/munction_smoke.c $(MUNCTION_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/munction_smoke.c $(MUNCTION_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(IO_PLATFORM_SMOKE): core/io_platform_smoke.c $(IO_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/io_platform_smoke.c $(IO_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(NET_PLATFORM_SMOKE): core/net_platform_smoke.c $(NET_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/net_platform_smoke.c $(NET_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(PATH_PLATFORM_SMOKE): core/path_platform_smoke.c $(PATH_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/path_platform_smoke.c $(PATH_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(TERMINAL_PLATFORM_SMOKE): core/terminal_platform_smoke.c $(TERMINAL_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/terminal_platform_smoke.c $(TERMINAL_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(LIBRARY_PLATFORM_SMOKE): core/library_platform_smoke.c $(LIBRARY_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/library_platform_smoke.c $(LIBRARY_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(TIME_PLATFORM_SMOKE): core/time_platform_smoke.c $(TIME_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/time_platform_smoke.c $(TIME_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(THREAD_OBJ): core/sleela_thread.c core/sleela_thread.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_thread.c -o $@
$(IO_OBJ): core/sleela_io.c core/sleela_io.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_io.c -o $@
$(NET_OBJ): core/sleela_net.c core/sleela_net.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_net.c -o $@
$(SYNCHRO_OBJ): core/sleela_synchro.c core/sleela_synchro.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_synchro.c -o $@
$(MUNCTION_OBJ): core/sleela_munction.c core/sleela_munction.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_munction.c -o $@
$(BESTOF_OBJ): core/sleela_bestof.c core/sleela_bestof.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_bestof.c -o $@
$(PATH_OBJ): core/sleela_path.c core/sleela_path.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_path.c -o $@
$(TERMINAL_OBJ): core/sleela_terminal.c core/sleela_terminal.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_terminal.c -o $@
$(LIBRARY_OBJ): core/sleela_library.c core/sleela_library.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_library.c -o $@
$(TIME_OBJ): core/sleela_time.c core/sleela_time.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_time.c -o $@
$(MEMORY_OBJ): core/sleela_memory.c core/sleela_memory.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_memory.c -o $@
$(MEMMGR_OBJ): core/sleela_memmgr.c core/sleela_memmgr.h core/sleela_memory.h core/sleela_thread.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_memmgr.c -o $@
$(PIXEL_TERMINAL_OBJ): ../bash/pixel_terminal.cpp ../bash/pixel_terminal.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c ../bash/pixel_terminal.cpp -o $@
$(PIXEL_TERMINAL_SMOKE): ../bash/pixel_terminal_smoke.cpp ../bash/pixel_terminal.hpp $(PIXEL_TERMINAL_OBJ) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) ../bash/pixel_terminal_smoke.cpp $(PIXEL_TERMINAL_OBJ) -o $@ $(LDFLAGS) $(LIBS)
$(GC_OBJ): ../runtime/garbage_collector.c ../runtime/garbage_collector.h | $(BUILD)
	$(CC) $(CFLAGS) -c ../runtime/garbage_collector.c -o $@
$(SECURITY_OBJ): ../runtime/security_supervisor.c ../runtime/security_supervisor.h | $(BUILD)
	$(CC) $(CFLAGS) -c ../runtime/security_supervisor.c -o $@
$(PARAMETERS_OBJ): ../runtime/Parameters.c ../runtime/Parameters.h | $(BUILD)
	$(CC) $(CFLAGS) -c ../runtime/Parameters.c -o $@
$(CORE_OBJ): $(CORE_SRC) core/sleela_core.c core/sleela_core.h core/sleela_thread.h core/pthread.h core/sleela_synchro.h core/sleela_munction.h core/sleela_bestof.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c $(CORE_SRC) -o $@
$(CHEMISTRY_OBJ): subjects/chemistry/chemistry.cpp subjects/chemistry/chemistry.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c subjects/chemistry/chemistry.cpp -o $@
$(FINANCIAL_OBJ): subjects/finance/financial.cpp subjects/finance/financial.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c subjects/finance/financial.cpp -o $@
$(BUILD)/%.o: frontend/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/native_api.o: subjects/native/native_api.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/math_native.o: subjects/math/math_native.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/physics_native.o: subjects/physics/physics_native.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/economics_native.o: subjects/economics/economics_native.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/inference_native.o: subjects/inference/inference_native.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/chemistry_api.o: subjects/chemistry/chemistry_api.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/financial_api.o: subjects/finance/financial_api.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/sheet_catalog.o: catalog/sheet_catalog.cpp catalog/sheet_catalog.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/xclass_loader.o: xclass/xclass_loader.cpp xclass/xclass_loader.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/langin.o: langin/langin.cpp langin/langin.h xclass/xclass_loader.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c langin/langin.cpp -o $@
$(BUILD)/ns_%.o: nordshrift/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(LEDGER_CORE_OBJ): ../ledger/ledger_core.cpp ../ledger/ledger_core.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/driver.o: frontend/driver.cpp frontend/compiler.h frontend/artifact.h subjects/native/native_api.h subjects/chemistry/chemistry_api.h subjects/finance/financial_api.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD):
	mkdir -p $(BUILD)

test: verify-security test-sleela test-version test-nordshrift test-threads test-thread-platform test-network test-network-platform test-synchro test-munction test-bestof test-io-platform test-path-platform test-terminal-platform test-library-platform test-time-platform test-pixel-terminal test-catalog test-math test-physics test-economics test-inference test-chemistry test-financial test-runtime test-memmgr
test-thread-platform: $(THREAD_PLATFORM_SMOKE)
	@echo "=== OS-aware threading backend ==="; SLEELA_THREAD_PLATFORM=auto $(THREAD_PLATFORM_SMOKE)
test-threads: $(THREAD_SMOKE)
	@echo "=== core threading smoke (128 threads) ==="; $(THREAD_SMOKE) 128
test-sleela: $(SLEELA_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; export SLEELA_SHA256_MANIFEST=$${SLEELA_SHA256_MANIFEST:-$(CURDIR)/../security/sha256-manifest.json}; cd $(CURDIR)/.. && for f in impl/examples/*.sleela; do echo "=== $$f ==="; $(CURDIR)/$(SLEELA_BIN) run $$f || exit 1; done
test-version: $(SLEELA_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; export SLEELA_SHA256_MANIFEST=$${SLEELA_SHA256_MANIFEST:-$(CURDIR)/../security/sha256-manifest.json}; cd $(CURDIR)/.. && bash impl/tests/version/run_version_tests.sh $(CURDIR)/$(SLEELA_BIN)
test-nordshrift: $(NORDSHRIFT_BIN)
	@export SLEELA_SHA256_MANIFEST=$${SLEELA_SHA256_MANIFEST:-$(CURDIR)/../security/sha256-manifest.json}; cd $(CURDIR)/.. && for f in impl/nordshrift/examples/*.sst; do echo "=== check $$f ==="; $(CURDIR)/$(NORDSHRIFT_BIN) check $$f || exit 1; done
	@export SLEELA_SHA256_MANIFEST=$${SLEELA_SHA256_MANIFEST:-$(CURDIR)/../security/sha256-manifest.json}; cd $(CURDIR)/.. && echo "=== build impl/nordshrift/examples/demo/build.sst (target sleela -> runnable artifact) ===" && $(CURDIR)/$(NORDSHRIFT_BIN) build impl/nordshrift/examples/demo/build.sst || exit 1
test-network: $(SOCKET_SMOKE)
	@echo "=== core networking smoke (TCP loopback) ==="; $(SOCKET_SMOKE)
test-synchro: $(SYNCHRO_SMOKE)
	@echo "=== Synchro honest-measurement smoke ==="; $(SYNCHRO_SMOKE)
test-munction: $(MUNCTION_SMOKE)
	@echo "=== Munction reach-composition smoke ==="; $(MUNCTION_SMOKE)
test-bestof: $(BESTOF_SMOKE)
	@echo "=== Best-of route/accuracy selection smoke ==="; $(BESTOF_SMOKE)
test-network-platform: $(NET_PLATFORM_SMOKE)
	@echo "=== OS-aware networking backend ==="; SLEELA_NET_PLATFORM=auto $(NET_PLATFORM_SMOKE)
test-io-platform: $(IO_PLATFORM_SMOKE)
	@echo "=== OS-aware file I/O backend ==="; SLEELA_IO_PLATFORM=auto $(IO_PLATFORM_SMOKE)
test-path-platform: $(PATH_PLATFORM_SMOKE)
	@echo "=== OS-aware paths/filesystem backend ==="; $(PATH_PLATFORM_SMOKE)
test-terminal-platform: $(TERMINAL_PLATFORM_SMOKE)
	@echo "=== OS-aware terminal/PTY backend ==="; SLEELA_TERMINAL_PLATFORM=auto $(TERMINAL_PLATFORM_SMOKE)
test-library-platform: $(LIBRARY_PLATFORM_SMOKE)
	@echo "=== OS-aware native library loader ==="; $(LIBRARY_PLATFORM_SMOKE)
test-time-platform: $(TIME_PLATFORM_SMOKE)
	@echo "=== precision time platform ==="; $(TIME_PLATFORM_SMOKE)
test-pixel-terminal: $(PIXEL_TERMINAL_SMOKE)
	@echo "=== native pixel terminal smoke ==="; $(PIXEL_TERMINAL_SMOKE)
test-catalog: $(NORDSHRIFT_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; echo "=== nordshrift objects (summary) ==="; $(NORDSHRIFT_BIN) objects | head -4 || exit 1; echo "=== relevance --target=c Thread ==="; $(NORDSHRIFT_BIN) relevance --target=c Thread || exit 1; echo "=== relevance --target=java (tally) ==="; $(NORDSHRIFT_BIN) relevance --target=java | tail -1 || exit 1
# Shared runner for a single subject *_values.sleela program: builds the
# binary, then asserts the program runs cleanly, emits no FAIL line, and
# produces exactly its EXPECTED_PASS count. $(1) is the subject basename.
SUBJECT_TEST_ENV = export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; export SLEELA_SHA256_MANIFEST=$${SLEELA_SHA256_MANIFEST:-$(CURDIR)/../security/sha256-manifest.json}
define RUN_SUBJECT_VALUES
	@$(SUBJECT_TEST_ENV); cd $(CURDIR)/.. && \
	  f=impl/tests/subjects/$(1)_values.sleela; \
	  echo "=== $(1) numeric assertions ($$f) ==="; \
	  out="$$($(CURDIR)/$(SLEELA_BIN) run $$f 2>/dev/null)" || { echo "  FAIL ($$f did not run)"; exit 1; }; \
	  printf '%s\n' "$$out" | grep -E '^(PASS|FAIL) '; \
	  fails=$$(printf '%s\n' "$$out" | grep -c '^FAIL '); \
	  passes=$$(printf '%s\n' "$$out" | grep -c '^PASS '); \
	  expected=$$(grep -oE 'EXPECTED_PASS=[0-9]+' $$f | head -1 | cut -d= -f2); \
	  if [ "$$fails" -ne 0 ]; then echo "  $(1): $$fails FAILED"; exit 1; fi; \
	  if [ -n "$$expected" ] && [ "$$passes" -ne "$$expected" ]; then echo "  $(1): expected $$expected checks, got $$passes"; exit 1; fi; \
	  echo "  $(1): $$passes checks passed"
endef

test-math: $(SLEELA_BIN) $(BUILD)/math_native.o
	$(call RUN_SUBJECT_VALUES,math)
test-physics: $(SLEELA_BIN) $(BUILD)/physics_native.o
	$(call RUN_SUBJECT_VALUES,physics)
test-economics: $(SLEELA_BIN) $(BUILD)/economics_native.o
	$(call RUN_SUBJECT_VALUES,economics)
test-inference: $(SLEELA_BIN) $(BUILD)/inference_native.o
	$(call RUN_SUBJECT_VALUES,inference)
test-chemistry: $(SLEELA_BIN) $(CHEMISTRY_OBJ)
	$(call RUN_SUBJECT_VALUES,chemistry)
test-financial: $(SLEELA_BIN) $(FINANCIAL_OBJ)
	$(call RUN_SUBJECT_VALUES,financial)
# Run all subject numeric assertions through the shared harness.
test-subjects: $(SLEELA_BIN)
	@$(SUBJECT_TEST_ENV); cd $(CURDIR)/.. && bash impl/tests/subjects/run_subject_tests.sh $(CURDIR)/$(SLEELA_BIN)
clean:
	rm -rf $(BUILD)

build-runtime: verify-security $(GC_OBJ) $(SECURITY_OBJ) $(PARAMETERS_OBJ)
	@echo "=== SLVM runtime services built: GC + Security Supervisor + Parameters ==="

# Behavioral smoke test for the runtime services: links the test against the
# GC and security-supervisor objects and asserts real behavior (mark-sweep
# reclamation, byte accounting, destructor invocation, and class/role quota
# enforcement) rather than merely compiling them.
RUNTIME_SMOKE := $(BUILD)/runtime_smoke
$(RUNTIME_SMOKE): tests/runtime/runtime_smoke.c $(GC_OBJ) $(SECURITY_OBJ) | verify-security $(BUILD)
	$(CC) $(CFLAGS) -I../runtime tests/runtime/runtime_smoke.c $(GC_OBJ) $(SECURITY_OBJ) -o $@ $(LDFLAGS) $(LIBS)

# Cross-platform source smoke suite. These tests exercise the native
# OS abstraction layer (threads, sockets, I/O, paths, terminal, loader, time).
test-platform: $(THREAD_PLATFORM_SMOKE) $(NET_PLATFORM_SMOKE) $(IO_PLATFORM_SMOKE) $(PATH_PLATFORM_SMOKE) $(TERMINAL_PLATFORM_SMOKE) $(LIBRARY_PLATFORM_SMOKE) $(TIME_PLATFORM_SMOKE)
	@echo "=== SLeeLa native platform smoke suite ($(PLATFORM)) ==="
	@$(THREAD_PLATFORM_SMOKE)
	@$(NET_PLATFORM_SMOKE)
	@$(IO_PLATFORM_SMOKE)
	@$(PATH_PLATFORM_SMOKE)
	@$(TERMINAL_PLATFORM_SMOKE)
	@$(LIBRARY_PLATFORM_SMOKE)
	@$(TIME_PLATFORM_SMOKE)

test-runtime: $(RUNTIME_SMOKE)
	@echo "=== SLVM runtime services -- behavioral smoke ==="; $(RUNTIME_SMOKE)

# Behavioral smoke test for the Memory Manager: links against the manager and
# the OS allocator/thread objects and asserts real behavior (untracked-when-
# disabled, exact byte/alloc accounting across alloc/calloc/realloc/free, and a
# hard byte limit that fails allocation closed).
MEMMGR_SMOKE := $(BUILD)/memmgr_smoke
$(MEMMGR_SMOKE): tests/core/memmgr_smoke.c $(MEMMGR_OBJ) $(MEMORY_OBJ) $(THREAD_OBJ) | verify-security $(BUILD)
	$(CC) $(CFLAGS) -Icore tests/core/memmgr_smoke.c $(MEMMGR_OBJ) $(MEMORY_OBJ) $(THREAD_OBJ) -o $@ $(LDFLAGS) $(LIBS)

test-memmgr: $(MEMMGR_SMOKE)
	@echo "=== SLeeLa Memory Manager -- behavioral smoke ==="; $(MEMMGR_SMOKE)
