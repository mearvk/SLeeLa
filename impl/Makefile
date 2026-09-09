# =============================================================================
# Makefile for Sleela (C/C++ core + front end) and Nordshrift (.sst -> triple).
# =============================================================================
CC       ?= gcc
CXX      ?= g++
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -pthread
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pthread
LDFLAGS  ?= -pthread
BUILD    := build

SLEELA_BIN     := $(BUILD)/sleela
NORDSHRIFT_BIN := $(BUILD)/nordshrift
THREAD_SMOKE   := $(BUILD)/thread_smoke

# ---- Shared catalog (SHEET.sheet parser) ----
CATALOG_OBJ := $(BUILD)/sheet_catalog.o

# ---- .xclass loader (SecureJDK 28 XML class input) ----
XCLASS_OBJ := $(BUILD)/xclass_loader.o

# ---- Sleela ----
CORE_OBJ := $(BUILD)/sleela_core.o
FE_SRC   := frontend/lexer.cpp frontend/parser.cpp frontend/compiler.cpp \
            frontend/version.cpp
FE_OBJ   := $(patsubst frontend/%.cpp,$(BUILD)/%.o,$(FE_SRC))
DRIVER_OBJ := $(BUILD)/driver.o

# ---- Nordshrift (NS-SST-0001 driver) ----
NS_SRC := nordshrift/sst_lexer.cpp nordshrift/sst_parser.cpp \
          nordshrift/source_resolve.cpp nordshrift/sleela_emit.cpp \
          nordshrift/object_compat.cpp nordshrift/nordshrift.cpp
NS_OBJ := $(patsubst nordshrift/%.cpp,$(BUILD)/ns_%.o,$(NS_SRC))

.PHONY: all clean test test-sleela test-version test-nordshrift test-threads test-catalog

all: $(SLEELA_BIN) $(NORDSHRIFT_BIN)

# ---- link ----
$(SLEELA_BIN): $(CORE_OBJ) $(FE_OBJ) $(DRIVER_OBJ) $(CATALOG_OBJ) $(XCLASS_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# nordshrift reuses the Sleela front end + core to transpile/run the sources.
$(NORDSHRIFT_BIN): $(NS_OBJ) $(CORE_OBJ) $(FE_OBJ) $(CATALOG_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# The C-level threading smoke test drives the core through the builder API.
$(THREAD_SMOKE): core/thread_smoke.c $(CORE_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ---- compile ----
$(BUILD)/sleela_core.o: core/sleela_core.c core/sleela_core.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: frontend/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/sheet_catalog.o: catalog/sheet_catalog.cpp catalog/sheet_catalog.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/xclass_loader.o: xclass/xclass_loader.cpp xclass/xclass_loader.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/ns_%.o: nordshrift/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

# ---- tests ----
test: test-sleela test-version test-nordshrift test-threads test-catalog

# Threading smoke test: spawn 128 workers, guarded increments + tuple channel.
test-threads: $(THREAD_SMOKE)
	@echo "=== core threading smoke (128 threads) ==="; \
	$(THREAD_SMOKE) 128

test-sleela: $(SLEELA_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; \
	for f in examples/*.sleela; do \
		echo "=== $$f ==="; \
		$(SLEELA_BIN) run $$f || exit 1; \
	done

# Assert SL-META-0001 Section 4.4 version awareness: accepted versions run,
# out-of-range / malformed versions are rejected.
test-version: $(SLEELA_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; \
	bash tests/version/run_version_tests.sh $(SLEELA_BIN)

# Validate every top-level .sst sheet, then drive a real build (sleela target
# runs on the core) from the self-contained demo sheet.
test-nordshrift: $(NORDSHRIFT_BIN)
	@for f in nordshrift/examples/*.sst; do \
		echo "=== check $$f ==="; \
		$(NORDSHRIFT_BIN) check $$f || exit 1; \
	done
	@echo "=== build nordshrift/examples/demo/build.sst (target sleela -> core) ==="; \
	$(NORDSHRIFT_BIN) build nordshrift/examples/demo/build.sst || exit 1

# Exercise the SHEET.sheet object compatibility list + relevance conversion.
test-catalog: $(NORDSHRIFT_BIN)
	@export SLEELA_SHEET=$(CURDIR)/../SHEET.sheet; \
	echo "=== nordshrift objects (summary) ==="; \
	$(NORDSHRIFT_BIN) objects | head -4 || exit 1; \
	echo "=== relevance --target=c Thread ==="; \
	$(NORDSHRIFT_BIN) relevance --target=c Thread || exit 1; \
	echo "=== relevance --target=java (tally) ==="; \
	$(NORDSHRIFT_BIN) relevance --target=java | tail -1 || exit 1

clean:
	rm -rf $(BUILD)
