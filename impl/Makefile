# =============================================================================
# Makefile for Sleela (C/C++ core + front end) and Nordshrift (.sst -> triple).
# =============================================================================
CC       ?= gcc
CXX      ?= g++
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -pthread
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pthread
LDFLAGS  ?= -pthread
BUILD    := build

# ---- STP-0001 secure transition (SecureJDK 28 bridge) ----
# STP_SSL=1 (default) links OpenSSL libcrypto for the full crypto client.
# STP_SSL=0 builds a no-crypto client that always falls back to safe-trim.
STP_SSL  ?= 1
ifeq ($(STP_SSL),1)
  STP_DEFS := -DSTP_HAVE_OPENSSL
  STP_LIBS := -lcrypto
else
  STP_DEFS :=
  STP_LIBS :=
endif

SLEELA_BIN     := $(BUILD)/sleela
NORDSHRIFT_BIN := $(BUILD)/nordshrift
THREAD_SMOKE   := $(BUILD)/thread_smoke

# ---- Shared catalog (SHEET.sheet parser) ----
CATALOG_OBJ := $(BUILD)/sheet_catalog.o

# ---- Sleela ----
CORE_OBJ := $(BUILD)/sleela_core.o
FE_SRC   := frontend/lexer.cpp frontend/parser.cpp frontend/compiler.cpp
FE_OBJ   := $(patsubst frontend/%.cpp,$(BUILD)/%.o,$(FE_SRC))
DRIVER_OBJ := $(BUILD)/driver.o

# ---- STP transition objects (linked into sleela) ----
STP_OBJ := $(BUILD)/stp_wire.o $(BUILD)/stp_client.o \
           $(BUILD)/stp_sha256_portable.o $(BUILD)/mem_model.o
ifeq ($(STP_SSL),1)
  STP_OBJ += $(BUILD)/stp_crypto.o
endif

# ---- Nordshrift (NS-SST-0001 driver) ----
NS_SRC := nordshrift/sst_lexer.cpp nordshrift/sst_parser.cpp \
          nordshrift/source_resolve.cpp nordshrift/sleela_emit.cpp \
          nordshrift/object_compat.cpp nordshrift/nordshrift.cpp
NS_OBJ := $(patsubst nordshrift/%.cpp,$(BUILD)/ns_%.o,$(NS_SRC))

.PHONY: all clean test test-sleela test-nordshrift

all: $(SLEELA_BIN) $(NORDSHRIFT_BIN)

# ---- link ----
$(SLEELA_BIN): $(CORE_OBJ) $(FE_OBJ) $(DRIVER_OBJ) $(CATALOG_OBJ) $(STP_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(STP_LIBS)

# nordshrift reuses the Sleela front end + core to transpile/run the sources.
$(NORDSHRIFT_BIN): $(NS_OBJ) $(CORE_OBJ) $(FE_OBJ) $(CATALOG_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# The C-level threading smoke test drives the core through the builder API.
$(THREAD_SMOKE): core/thread_smoke.c $(CORE_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ---- compile ----
$(BUILD)/sleela_core.o: core/sleela_core.c core/sleela_core.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/driver.o: frontend/driver.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(STP_DEFS) -c $< -o $@

$(BUILD)/%.o: frontend/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---- STP transition sources ----
$(BUILD)/stp_wire.o: transition/stp_wire.c transition/stp_wire.h | $(BUILD)
	$(CC) $(CFLAGS) $(STP_DEFS) -c $< -o $@
$(BUILD)/stp_crypto.o: transition/stp_crypto.c transition/stp_crypto.h | $(BUILD)
	$(CC) $(CFLAGS) $(STP_DEFS) -c $< -o $@
$(BUILD)/stp_client.o: transition/stp_client.c transition/stp_client.h | $(BUILD)
	$(CC) $(CFLAGS) $(STP_DEFS) -c $< -o $@
$(BUILD)/stp_sha256_portable.o: transition/stp_sha256_portable.c transition/stp_crypto_optional.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILD)/mem_model.o: transition/mem_model.cpp transition/mem_model.h | $(BUILD)
	$(CXX) $(CXXFLAGS) $(STP_DEFS) -c $< -o $@

$(BUILD)/sheet_catalog.o: catalog/sheet_catalog.cpp catalog/sheet_catalog.h | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/ns_%.o: nordshrift/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

# ---- tests ----
test: test-sleela test-nordshrift test-threads test-catalog

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
