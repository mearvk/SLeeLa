# Makefile for the XLT (.xlt) CPU-model front-end: lexer + parser + resolver +
# the `xltc` CLI. Self-contained (no dependency on the Nordshrift build), mirrors
# its structure. Build:  make -C impl/xlt   ->  impl/xlt/build/xltc
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
BUILD    := build
BIN      := $(BUILD)/xltc

SRC := xlt_lexer.cpp xlt_parser.cpp xlt_resolve.cpp xltc.cpp
OBJ := $(SRC:%.cpp=$(BUILD)/%.o)

.PHONY: all check clean
all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Smoke-check the bundled example: a congruent model builds, exit 0.
check: $(BIN)
	./$(BIN) ../../lib/xlt/examples/epyc-ddr2.xlt

clean:
	@rm -rf $(BUILD)
