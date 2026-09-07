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

# ---- Sleela ----
CORE_OBJ := $(BUILD)/sleela_core.o
FE_SRC   := frontend/lexer.cpp frontend/parser.cpp frontend/compiler.cpp
FE_OBJ   := $(patsubst frontend/%.cpp,$(BUILD)/%.o,$(FE_SRC))
DRIVER_OBJ := $(BUILD)/driver.o

# ---- Nordshrift ----
NS_SRC := nordshrift/sst_lexer.cpp nordshrift/sst_parser.cpp nordshrift/sst_sema.cpp \
          nordshrift/emit_java.cpp nordshrift/emit_c.cpp nordshrift/emit_sleela.cpp \
          nordshrift/nordshrift.cpp
NS_OBJ := $(patsubst nordshrift/%.cpp,$(BUILD)/ns_%.o,$(NS_SRC))

.PHONY: all clean test test-sleela test-nordshrift

all: $(SLEELA_BIN) $(NORDSHRIFT_BIN)

# ---- link ----
$(SLEELA_BIN): $(CORE_OBJ) $(FE_OBJ) $(DRIVER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# nordshrift reuses the Sleela front end + core for its `run` path.
$(NORDSHRIFT_BIN): $(NS_OBJ) $(CORE_OBJ) $(FE_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# The C-level threading smoke test drives the core through the builder API.
$(THREAD_SMOKE): core/thread_smoke.c $(CORE_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ---- compile ----
$(BUILD)/sleela_core.o: core/sleela_core.c core/sleela_core.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: frontend/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/ns_%.o: nordshrift/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

# ---- tests ----
test: test-sleela test-nordshrift test-threads

# Threading smoke test: spawn 128 workers, guarded increments + tuple channel.
test-threads: $(THREAD_SMOKE)
	@echo "=== core threading smoke (128 threads) ==="; \
	$(THREAD_SMOKE) 128

test-sleela: $(SLEELA_BIN)
	@for f in examples/*.sleela; do \
		echo "=== $$f ==="; \
		$(SLEELA_BIN) run $$f || exit 1; \
	done

# Run every .sst through the Sleela arm of the manifold.
test-nordshrift: $(NORDSHRIFT_BIN)
	@for f in nordshrift/examples/*.sst; do \
		echo "=== $$f (run via Sleela core) ==="; \
		$(NORDSHRIFT_BIN) run $$f || exit 1; \
	done

clean:
	rm -rf $(BUILD)
