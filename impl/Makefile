# =============================================================================
# Makefile for Sleela (C/C++ core + front end) and Nordshrift (.sst -> triple).
#
# Subject libraries live under subjects/ (one folder per subject):
#   subjects/math       -> addMath
#   subjects/physics    -> addPhysics
#   subjects/economics  -> addEconomics
#   subjects/native     -> shared dispatcher + builders
#   subjects/chemistry  -> chemistry library + frontend chemistry_api
#   subjects/finance    -> financial library + frontend financial_api
#
# Security build order:
#   trusted update/source -> SHA-256 verification -> compilation -> linking
#   -> diagnostics/tests -> execution/install
# =============================================================================
CC ?= gcc
CXX ?= g++
SUBJECT_INCLUDES := -Isubjects/native -Isubjects/math -Isubjects/physics \
                    -Isubjects/economics -Isubjects/chemistry -Isubjects/finance
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -pthread
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pthread
CPPFLAGS += -Ifrontend $(SUBJECT_INCLUDES) -I../bash
LDFLAGS ?= -pthread
BUILD := build
SLEELA_BIN := $(BUILD)/sleela
NORDSHRIFT_BIN := $(BUILD)/nordshrift
THREAD_SMOKE := $(BUILD)/thread_smoke
SOCKET_SMOKE := $(BUILD)/socket_smoke
IO_PLATFORM_SMOKE := $(BUILD)/io_platform_smoke
PIXEL_TERMINAL_SMOKE := $(BUILD)/pixel_terminal_smoke
PIXEL_TERMINAL_OBJ := $(BUILD)/pixel_terminal.o
CATALOG_OBJ := $(BUILD)/sheet_catalog.o
XCLASS_OBJ := $(BUILD)/xclass_loader.o
CORE_OBJ := $(BUILD)/sleela_core.o
IO_OBJ := $(BUILD)/sleela_io.o
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
          subjects/chemistry/chemistry_api.cpp subjects/finance/financial_api.cpp
FE_OBJ := $(patsubst %.cpp,$(BUILD)/%.o,$(notdir $(FE_SRC)))
DRIVER_OBJ := $(BUILD)/driver.o
NS_SRC := nordshrift/sst_lexer.cpp nordshrift/sst_parser.cpp \
          nordshrift/source_resolve.cpp nordshrift/sleela_emit.cpp \
          nordshrift/object_compat.cpp nordshrift/subject_model.cpp \
          nordshrift/component_manifest.cpp nordshrift/nordshrift.cpp
NS_OBJ := $(patsubst nordshrift/%.cpp,$(BUILD)/ns_%.o,$(NS_SRC))
VERIFY_TOOL := ../tools/verify-before-execution.py
VERIFY_MANIFEST ?= $(SLEELA_SHA256_MANIFEST)
VERIFY_ARGS = --manifest "$(VERIFY_MANIFEST)" --root "$(CURDIR)/.."
ALL_OBJECTS := $(CORE_OBJ) $(IO_OBJ) $(GC_OBJ) $(SECURITY_OBJ) $(PARAMETERS_OBJ) $(FE_OBJ) $(DRIVER_OBJ) $(CATALOG_OBJ) $(XCLASS_OBJ) $(CHEMISTRY_OBJ) $(FINANCIAL_OBJ) $(PIXEL_TERMINAL_OBJ) $(NS_OBJ) $(THREAD_SMOKE) $(SOCKET_SMOKE) $(IO_PLATFORM_SMOKE) $(PIXEL_TERMINAL_SMOKE)

.PHONY: all build-verified rebuild-verified clean test test-sleela test-version test-nordshrift test-threads test-network test-io-platform test-pixel-terminal test-catalog test-chemistry test-financial test-math test-physics test-economics verify-security build-runtime test-runtime

all: verify-security $(SLEELA_BIN) $(NORDSHRIFT_BIN)
build-verified: verify-security $(SLEELA_BIN) $(NORDSHRIFT_BIN)
rebuild-verified:
	$(MAKE) clean
	$(MAKE) build-verified

verify-security:
	@if [ -z "$(VERIFY_MANIFEST)" ]; then echo "sleela: SHA-256 verification is required before build; set SLEELA_SHA256_MANIFEST or VERIFY_MANIFEST to a trusted JSON manifest" >&2; exit 1; fi
	@if [ ! -f "$(VERIFY_MANIFEST)" ]; then echo "sleela: trusted SHA-256 manifest not found: $(VERIFY_MANIFEST)" >&2; exit 1; fi
	@if [ ! -r "$(VERIFY_TOOL)" ]; then echo "sleela: verification tool not readable: $(VERIFY_TOOL)" >&2; exit 1; fi
	@command -v python3 >/dev/null 2>&1 || { echo "sleela: python3 is required for SHA-256 verification" >&2; exit 1; }
	@python3 "$(VERIFY_TOOL)" $(VERIFY_ARGS)

$(ALL_OBJECTS): verify-security

$(SLEELA_BIN): $(CORE_OBJ) $(IO_OBJ) $(GC_OBJ) $(SECURITY_OBJ) $(PARAMETERS_OBJ) $(FE_OBJ) $(DRIVER_OBJ) $(CATALOG_OBJ) $(XCLASS_OBJ) $(CHEMISTRY_OBJ) $(FINANCIAL_OBJ) $(PIXEL_TERMINAL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
$(NORDSHRIFT_BIN): $(NS_OBJ) $(CORE_OBJ) $(IO_OBJ) $(GC_OBJ) $(SECURITY_OBJ) $(PARAMETERS_OBJ) $(FE_OBJ) $(CATALOG_OBJ) $(CHEMISTRY_OBJ) $(FINANCIAL_OBJ) $(PIXEL_TERMINAL_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
$(THREAD_SMOKE): core/thread_smoke.c $(CORE_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(SOCKET_SMOKE): core/socket_smoke.c $(CORE_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
$(IO_PLATFORM_SMOKE): core/io_platform_smoke.c $(IO_OBJ) | $(BUILD)
	$(CC) $(CFLAGS) -Icore core/io_platform_smoke.c $(IO_OBJ) -o $@ $(LDFLAGS)
$(IO_OBJ): core/sleela_io.c core/sleela_io.h | $(BUILD)
	$(CC) $(CFLAGS) -Icore -c core/sleela_io.c -o $@
$(PIXEL_TERMINAL_OBJ): ../bash/pixel_terminal.cpp ../bash/pixel_terminal.hpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c ../bash/pixel_terminal.cpp -o $@
$(PIXEL_TERMINAL_SMOKE): ../bash/pixel_terminal_smoke.cpp ../bash/pixel_terminal.hpp $(PIXEL_TERMINAL_OBJ) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) ../bash/pixel_terminal_smoke.cpp $(PIXEL_TERMINAL_OBJ) -o $@ $(LDFLAGS)
$(GC_OBJ): ../runtime/garbage_collector.c ../runtime/garbage_collector.h | $(BUILD)
	$(CC) $(CFLAGS) -c ../runtime/garbage_collector.c -o $@
$(SECURITY_OBJ): ../runtime/security_supervisor.c ../runtime/security_supervisor.h | $(BUILD)
	$(CC) $(CFLAGS) -c ../runtime/security_supervisor.c -o $@
$(PARAMETERS_OBJ): ../Parameters.c | $(BUILD)
	$(CC) $(CFLAGS) -c ../Parameters.c -o $@
$(CORE_OBJ): $(CORE_SRC) core/sleela_core.c core/sleela_core.h | $(BUILD)
	$(CC) $(CFLAGS) -c $(CORE_SRC) -o $@
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
$(BUILD)/chemistry_api.o: subjects/chemistry/chemistry_api.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/financial_api.o: subjects/finance/financial.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/sheet_catalog.o: catalog/sheet_catalog.cpp catalog/sheet_catalog.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/xclass_loader.o: xclass/xclass_loader.cpp xclass/xclass_loader.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/ns_%.o: nordshrift/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD)/driver.o: frontend/driver.cpp frontend/compiler.h frontend/artifact.h subjects/native/native_api.h subjects/chemistry/chemistry_api.h subjects/finance/financial_api.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
$(BUILD):
	mkdir -p $(BUILD)

test: verify-security test-sleela test-version test-nordshrift test-threads test-network test-io-platform test-pixel-terminal test-catalog test-math test-physics test-economics test-chemistry test-financial
test-threads: $(THREAD_SMOKE)
	@echo "=== core threading smoke (128 threads) ==="; $(THREAD_SMOKE) 128
test-sleela: $(SLEELA_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; for f in examples/*.sleela; do echo "=== $$f ==="; $(SLEELA_BIN) run $$f || exit 1; done
test-version: $(SLEELA_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; bash tests/version/run_version_tests.sh $(SLEELA_BIN)
test-nordshrift: $(NORDSHRIFT_BIN)
	@for f in nordshrift/examples/*.sst; do echo "=== check $$f ==="; $(NORDSHRIFT_BIN) check $$f || exit 1; done
	@echo "=== build nordshrift/examples/demo/build.sst (target sleela -> runnable artifact) ==="; $(NORDSHRIFT_BIN) build nordshrift/examples/demo/build.sst || exit 1
test-network: $(SOCKET_SMOKE)
	@echo "=== core networking smoke (TCP loopback) ==="; $(SOCKET_SMOKE)
test-io-platform: $(IO_PLATFORM_SMOKE)
	@echo "=== OS-aware file I/O backend ==="; SLEELA_IO_PLATFORM=auto $(IO_PLATFORM_SMOKE)
test-pixel-terminal: $(PIXEL_TERMINAL_SMOKE)
	@echo "=== native pixel terminal smoke ==="; $(PIXEL_TERMINAL_SMOKE)
test-catalog: $(NORDSHRIFT_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; echo "=== nordshrift objects (summary) ==="; $(NORDSHRIFT_BIN) objects | head -4 || exit 1; echo "=== relevance --target=c Thread ==="; $(NORDSHRIFT_BIN) relevance --target=c Thread || exit 1; echo "=== relevance --target=java (tally) ==="; $(NORDSHRIFT_BIN) relevance --target=java | tail -1 || exit 1
test-math: $(BUILD)/math_native.o
	@echo "=== native math subject library ==="; echo "math object compiled: $(BUILD)/math_native.o"
test-physics: $(BUILD)/physics_native.o
	@echo "=== native physics subject library ==="; echo "physics object compiled: $(BUILD)/physics_native.o"
test-economics: $(BUILD)/economics_native.o
	@echo "=== native economics subject library ==="; echo "economics object compiled: $(BUILD)/economics_native.o"
test-chemistry: $(CHEMISTRY_OBJ)
	@echo "=== normed presumed chemistry library ==="; echo "chemistry object compiled: $(CHEMISTRY_OBJ)"
test-financial: $(FINANCIAL_OBJ)
	@echo "=== financial math/algebra reference library ==="; echo "financial object compiled: $(FINANCIAL_OBJ)"
clean:
	rm -rf $(BUILD)

build-runtime: verify-security $(GC_OBJ) $(SECURITY_OBJ) $(PARAMETERS_OBJ)
	@echo "=== SLVM runtime services built: GC + Security Supervisor + Parameters ==="
test-runtime: build-runtime
	@echo "=== runtime integration objects present ==="
