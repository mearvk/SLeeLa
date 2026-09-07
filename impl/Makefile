# =============================================================================
# Makefile for the Sleela interpreter (C/C++ core + Sleela front end).
# =============================================================================
CC       ?= gcc
CXX      ?= g++
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
BUILD    := build
BIN      := $(BUILD)/sleela

CORE_SRC := core/sleela_core.c
FE_SRC   := frontend/lexer.cpp frontend/parser.cpp frontend/compiler.cpp frontend/driver.cpp

CORE_OBJ := $(BUILD)/sleela_core.o
FE_OBJ   := $(patsubst frontend/%.cpp,$(BUILD)/%.o,$(FE_SRC))

.PHONY: all clean test run

all: $(BIN)

$(BIN): $(CORE_OBJ) $(FE_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/sleela_core.o: $(CORE_SRC) core/sleela_core.h | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: frontend/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

# Run every bundled example.
test: $(BIN)
	@for f in examples/*.sleela; do \
		echo "=== $$f ==="; \
		$(BIN) run $$f || exit 1; \
	done

clean:
	rm -rf $(BUILD)
