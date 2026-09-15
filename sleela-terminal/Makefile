# =============================================================================
# SleelaTerminal(TM) shell -- self-contained build.
# Original SLeeLa work (see NOTICE): an original shell, not derived from Bash.
# =============================================================================
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
FUZZ_CXX ?= clang++
BUILD    := build

PREFIX   ?= /usr/local
BINDIR   ?= $(PREFIX)/bin
DESTDIR  ?=
INSTALL  ?= install

CORE_SRC := core.cpp arith.cpp lexer.cpp parser.cpp expand.cpp executor.cpp
M5_SRC   := m5.cpp
CLI_SRC  := slsh.cpp $(M5_SRC) $(CORE_SRC)
SMOKE_SRC := smoke.cpp $(CORE_SRC)

HEADERS := core.hpp arith.hpp lexer.hpp parser.hpp expand.hpp executor.hpp m5.hpp

SLSH  := $(BUILD)/slsh
SMOKE := $(BUILD)/smoke

.PHONY: all smoke test m5-smoke sanitize fuzz install install-strip uninstall clean

all: $(SLSH) $(SMOKE)

$(SLSH): $(CLI_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CLI_SRC) -o $@

$(SMOKE): $(SMOKE_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(SMOKE_SRC) -o $@

$(BUILD):
	mkdir -p $(BUILD)

smoke: $(SMOKE)
	$(SMOKE)

m5-smoke: $(SLSH)
	sh ./m5-smoke.sh $(SLSH)

test: smoke m5-smoke

install: $(SLSH)
	$(INSTALL) -d $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(SLSH) $(DESTDIR)$(BINDIR)/slsh

install-strip: CXXFLAGS += -s
install-strip: clean install

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/slsh

clean:
	rm -rf $(BUILD)

# Security validation builds. These intentionally use debug-friendly flags.
SANITIZE_FLAGS := -std=c++17 -O1 -g -Wall -Wextra -fno-omit-frame-pointer -fsanitize=address,undefined
sanitize: | $(BUILD)
	$(CXX) $(SANITIZE_FLAGS) $(CLI_SRC) -o $(BUILD)/slsh-sanitize
	$(CXX) $(SANITIZE_FLAGS) $(SMOKE_SRC) -o $(BUILD)/smoke-sanitize
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 $(BUILD)/smoke-sanitize

# Lightweight lexer/parser fuzz target. Clang/libFuzzer is the reference
# toolchain; the harness remains independent of the production binary.
FUZZ_SRC := fuzz.cpp
FUZZ_FLAGS := -std=c++17 -O1 -g -Wall -Wextra -fsanitize=fuzzer,address,undefined
fuzz: | $(BUILD)
	$(FUZZ_CXX) $(FUZZ_FLAGS) $(FUZZ_SRC) $(CORE_SRC) -o $(BUILD)/slsh-fuzz
