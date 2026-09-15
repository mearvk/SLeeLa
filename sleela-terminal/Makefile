# =============================================================================
# SleelaTerminal(TM) shell -- self-contained build.
# Original SLeeLa work (see NOTICE): an original shell, not derived from Bash.
# =============================================================================
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
BUILD    := build

CORE_SRC := core.cpp arith.cpp lexer.cpp parser.cpp expand.cpp executor.cpp
M5_SRC   := m5.cpp
CLI_SRC  := slsh.cpp $(M5_SRC) $(CORE_SRC)
SMOKE_SRC := smoke.cpp $(CORE_SRC)

HEADERS := core.hpp arith.hpp lexer.hpp parser.hpp expand.hpp executor.hpp m5.hpp

SLSH  := $(BUILD)/slsh
SMOKE := $(BUILD)/smoke

.PHONY: all smoke test clean

all: $(SLSH) $(SMOKE)

$(SLSH): $(CLI_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CLI_SRC) -o $@

$(SMOKE): $(SMOKE_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(SMOKE_SRC) -o $@

$(BUILD):
	mkdir -p $(BUILD)

smoke test: $(SMOKE)
	$(SMOKE)

clean:
	rm -rf $(BUILD)
