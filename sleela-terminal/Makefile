# SleelaTerminal(TM) shell -- core build with optional GTK4/VTE GUI.
CXX      ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
FUZZ_CXX ?= clang++
BUILD    := build
PREFIX   ?= /usr/local
BINDIR   ?= $(PREFIX)/bin
DESKTOPDIR ?= $(PREFIX)/share/applications
DESTDIR  ?=
INSTALL  ?= install
PKG_CONFIG ?= pkg-config
GTK4_VTE_PKGS ?= gtk4 vte-2.91-gtk4
CORE_SRC := core.cpp arith.cpp lexer.cpp parser.cpp expand.cpp executor.cpp
M5_SRC   := m5.cpp
CLI_SRC  := slsh.cpp $(M5_SRC) $(CORE_SRC)
SMOKE_SRC := smoke.cpp $(CORE_SRC)
GUI_SRC := sleelaterminal-gui.cpp
HEADERS := core.hpp arith.hpp lexer.hpp parser.hpp expand.hpp executor.hpp m5.hpp
SLSH  := $(BUILD)/slsh
SMOKE := $(BUILD)/smoke
GUI   := $(BUILD)/SleelaTerminal
GTK4_VTE_CFLAGS := $(shell $(PKG_CONFIG) --cflags $(GTK4_VTE_PKGS) 2>/dev/null)
GTK4_VTE_LIBS   := $(shell $(PKG_CONFIG) --libs $(GTK4_VTE_PKGS) 2>/dev/null)
.PHONY: all core gui all-gui smoke test m5-smoke sanitize fuzz install install-gui install-strip uninstall clean

# Default build avoids optional desktop dependencies.
all: core
core: $(SLSH) $(SMOKE)
all-gui: core $(GUI)

$(SLSH): $(CLI_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CLI_SRC) -o $@

$(SMOKE): $(SMOKE_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(SMOKE_SRC) -o $@

$(GUI): $(GUI_SRC) $(SLSH) | $(BUILD)
	@test -n "$(GTK4_VTE_CFLAGS)" && test -n "$(GTK4_VTE_LIBS)" || { echo "Missing GTK4/VTE development packages. Install gtk4 and vte-2.91-gtk4 development packages." >&2; exit 1; }
	$(CXX) $(CXXFLAGS) $(GTK4_VTE_CFLAGS) $(GUI_SRC) -o $@ $(GTK4_VTE_LIBS)

$(BUILD):
	mkdir -p $(BUILD)

smoke: $(SMOKE)
	$(SMOKE)
gui: $(GUI)
	@echo "SleelaTerminal GUI: $(GUI)"
m5-smoke: $(SLSH)
	sh ./m5-smoke.sh $(SLSH)
test: smoke m5-smoke

# Core install does not require GTK. GUI install is an explicit opt-in.
install: $(SLSH)
	$(INSTALL) -d $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(SLSH) $(DESTDIR)$(BINDIR)/slsh

install-gui: $(GUI)
	$(INSTALL) -d $(DESTDIR)$(BINDIR)
	$(INSTALL) -d $(DESTDIR)$(DESKTOPDIR)
	$(INSTALL) -m 0755 $(GUI) $(DESTDIR)$(BINDIR)/SleelaTerminal
	$(INSTALL) -m 0644 SleelaTerminal.desktop $(DESTDIR)$(DESKTOPDIR)/SleelaTerminal.desktop

install-strip: CXXFLAGS += -s
install-strip: clean install
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/slsh
	rm -f $(DESTDIR)$(BINDIR)/SleelaTerminal
	rm -f $(DESTDIR)$(DESKTOPDIR)/SleelaTerminal.desktop
clean:
	rm -rf $(BUILD)

SANITIZE_FLAGS := -std=c++17 -O1 -g -Wall -Wextra -fno-omit-frame-pointer -fsanitize=address,undefined
sanitize: | $(BUILD)
	$(CXX) $(SANITIZE_FLAGS) $(CLI_SRC) -o $(BUILD)/slsh-sanitize
	$(CXX) $(SANITIZE_FLAGS) $(SMOKE_SRC) -o $(BUILD)/smoke-sanitize
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 $(BUILD)/smoke-sanitize

FUZZ_SRC := fuzz.cpp
FUZZ_FLAGS := -std=c++17 -O1 -g -Wall -Wextra -fsanitize=fuzzer,address,undefined
fuzz: | $(BUILD)
	$(FUZZ_CXX) $(FUZZ_FLAGS) $(FUZZ_SRC) $(CORE_SRC) -o $(BUILD)/slsh-fuzz
