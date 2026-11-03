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

.PHONY: all cli all-gui smoke test m5-smoke gui install install-gui install-all check-gui-deps do-install do-install-gui install-strip uninstall clean sanitize fuzz

all: all-gui

cli: $(SLSH) $(SMOKE)

all-gui: cli $(GUI)

$(SLSH): $(CLI_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) -pthread $(CLI_SRC) -o $@

$(SMOKE): $(SMOKE_SRC) $(HEADERS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(SMOKE_SRC) -o $@

$(GUI): $(GUI_SRC) file-list-renderer.hpp $(SLSH) | $(BUILD)
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

# Validate DESTDIR staging, executable permissions, and the installed CLI.
install-smoke: $(SLSH)
	sh ./install-smoke.sh

# ---------------------------------------------------------------------------
# Installation is SCOPE-AWARE. `install`, `install-gui`, and `install-all` ask
#   1) Local   -> $HOME/.local  (user-local, never uses sudo)
#   2) System  -> /usr/local    (prompts for the sudo password as needed)
# on a terminal, then dispatch to the raw do-* recipes below. The System path
# re-invokes make under sudo ONLY for the privileged copy, so you are asked for
# your password exactly when system paths are written.
#
# Non-interactive / packaging stays unchanged:
#   * an explicit PREFIX= on the command line, or any DESTDIR, is used verbatim
#     with no prompt and no sudo (staged/distro builds behave as before);
#   * SLEELA_INSTALL_SCOPE=local|system (or 1|2) answers without a prompt;
#   * with no terminal and no scope set, it defaults to a user-local install.
SLEELA_INSTALL_SCOPE ?=

# $(call scope_install,<do-target>) — resolve scope, then run <do-target>,
# elevating with sudo for a system install when not already root.
define scope_install
	@if [ "$(origin PREFIX)" = "command line" ] || [ -n "$(DESTDIR)" ]; then \
	  echo "Installing into $(DESTDIR)$(BINDIR) (explicit PREFIX/DESTDIR; no sudo, no prompt)."; \
	  $(MAKE) --no-print-directory $(1) PREFIX="$(PREFIX)" BINDIR="$(BINDIR)" \
	    DESKTOPDIR="$(DESKTOPDIR)" DESTDIR="$(DESTDIR)"; \
	else \
	  scope="$(SLEELA_INSTALL_SCOPE)"; \
	  if [ -z "$$scope" ] && [ -t 0 ]; then \
	    echo "Where should SleelaTerminal be installed?"; \
	    echo "  1) Local   user-local install under \$$HOME/.local (never uses sudo)"; \
	    echo "  2) System  system-wide install under /usr/local (prompts for sudo as needed)"; \
	    printf 'Choose [1/2] (default 1): '; \
	    read scope || scope=1; \
	  fi; \
	  [ -z "$$scope" ] && scope=1; \
	  case "$$scope" in \
	    1|local|LOCAL|Local|user|USER|User) \
	      p="$$HOME/.local"; \
	      echo "Local install selected (no sudo): $$p/bin"; \
	      $(MAKE) --no-print-directory $(1) PREFIX="$$p" BINDIR="$$p/bin" \
	        DESKTOPDIR="$$p/share/applications" DESTDIR="" ;; \
	    2|system|SYSTEM|System) \
	      echo "System install selected: /usr/local (sudo requested for system paths only)."; \
	      if [ "$$(id -u)" -eq 0 ]; then SUDO=""; \
	      elif command -v sudo >/dev/null 2>&1; then SUDO="sudo"; \
	      else echo "System install needs root; install sudo or run as root." >&2; exit 1; fi; \
	      $$SUDO $(MAKE) --no-print-directory $(1) PREFIX=/usr/local BINDIR=/usr/local/bin \
	        DESKTOPDIR=/usr/local/share/applications DESTDIR="" ;; \
	    *) echo "Unrecognized choice '$$scope'; expected 1 (local) or 2 (system)." >&2; exit 2 ;; \
	  esac; \
	fi
endef

# Scope-aware front-ends.
#
# `install`       installs the slsh shell only.
# `install-gui` / `install-all` build and install the GUI SleelaTerminal
#                 program (plus slsh + assets + .desktop). The GUI REQUIRES the
#                 GTK4 and VTE development packages: if they are missing these
#                 targets STOP with exact install instructions rather than
#                 silently installing only slsh -- so you always get the GUI
#                 (or a clear reason and the command to fix it).
install: $(SLSH)
	$(call scope_install,do-install)

install-gui install-all: check-gui-deps $(GUI) $(SLSH)
	$(call scope_install,do-install-gui)

# Fail early, with actionable guidance, when the GUI toolchain is absent. This
# is what turns "silently slsh-only" into "here is exactly what to install".
.PHONY: check-gui-deps
check-gui-deps:
	@if ! command -v $(PKG_CONFIG) >/dev/null 2>&1; then \
	  echo "ERROR: pkg-config is not installed; the GUI build needs it to locate GTK4/VTE." >&2; \
	  echo "  Debian/Ubuntu:  sudo apt install pkg-config" >&2; \
	  exit 1; \
	fi; \
	miss=""; \
	$(PKG_CONFIG) --exists gtk4 2>/dev/null || miss="$$miss gtk4"; \
	$(PKG_CONFIG) --exists vte-2.91-gtk4 2>/dev/null || miss="$$miss vte-2.91-gtk4"; \
	if [ -n "$$miss" ] || [ -z "$(GTK4_VTE_CFLAGS)" ] || [ -z "$(GTK4_VTE_LIBS)" ]; then \
	  echo "ERROR: the SleelaTerminal GUI needs the GTK4 and VTE (gtk4) development packages." >&2; \
	  echo "       Missing pkg-config module(s):$${miss:- (gtk4 vte-2.91-gtk4)}" >&2; \
	  echo >&2; \
	  echo "  Debian/Ubuntu:  sudo apt install libgtk-4-dev libvte-2.91-gtk4-dev pkg-config g++" >&2; \
	  echo "  Fedora/RHEL:    sudo dnf install gtk4-devel vte291-gtk4-devel pkgconf-pkg-config gcc-c++" >&2; \
	  echo "  Arch:           sudo pacman -S gtk4 vte4 pkgconf gcc" >&2; \
	  echo "  openSUSE:       sudo zypper install gtk4-devel vte-gtk4-devel pkg-config gcc-c++" >&2; \
	  echo "  macOS (brew):   brew install gtk4 vte3 pkg-config" >&2; \
	  echo >&2; \
	  echo "Then re-run this install. To install ONLY the slsh shell (no GUI), use: make install" >&2; \
	  exit 1; \
	fi; \
	echo "GTK4/VTE development packages found -- building and installing the GUI SleelaTerminal."

# Raw copy recipes (honor PREFIX/BINDIR/DESKTOPDIR/DESTDIR as given). These are
# what actually write files; the scope front-ends above re-invoke them with the
# chosen prefix (and under sudo for a system install).
do-install: $(SLSH)
	$(INSTALL) -d $(DESTDIR)$(BINDIR)
	$(INSTALL) -m 0755 $(SLSH) $(DESTDIR)$(BINDIR)/slsh

do-install-gui: do-install $(GUI)
	$(INSTALL) -d $(DESTDIR)$(BINDIR)
	$(INSTALL) -d $(DESTDIR)$(BINDIR)/assets
	$(INSTALL) -d $(DESTDIR)$(DESKTOPDIR)
	$(INSTALL) -m 0755 $(GUI) $(DESTDIR)$(BINDIR)/SleelaTerminal
	$(INSTALL) -m 0644 assets/* $(DESTDIR)$(BINDIR)/assets/
	$(INSTALL) -m 0644 SleelaTerminal.desktop $(DESTDIR)$(DESKTOPDIR)/SleelaTerminal.desktop

# Keep clean/rebuild/install ordered even when make is invoked with -j.
install-strip:
	$(MAKE) clean
	$(MAKE) CXXFLAGS="$(CXXFLAGS) -s" install

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/slsh
	rm -f $(DESTDIR)$(BINDIR)/SleelaTerminal
	rm -f $(DESTDIR)$(DESKTOPDIR)/SleelaTerminal.desktop

clean:
	rm -rf $(BUILD)

# Security validation builds. These intentionally use debug-friendly flags.
SANITIZE_FLAGS := -std=c++17 -O1 -g -Wall -Wextra -fno-omit-frame-pointer -fsanitize=address,undefined
sanitize: | $(BUILD)
	$(CXX) $(SANITIZE_FLAGS) -pthread $(CLI_SRC) -o $(BUILD)/slsh-sanitize
	$(CXX) $(SANITIZE_FLAGS) $(SMOKE_SRC) -o $(BUILD)/smoke-sanitize
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 $(BUILD)/smoke-sanitize

# Lightweight lexer/parser fuzz target. Clang/libFuzzer is the reference
# toolchain; the harness remains independent of the production binary.
FUZZ_SRC := fuzz.cpp
FUZZ_FLAGS := -std=c++17 -O1 -g -Wall -Wextra -fsanitize=fuzzer,address,undefined
fuzz: | $(BUILD)
	$(FUZZ_CXX) $(FUZZ_FLAGS) $(FUZZ_SRC) $(CORE_SRC) -o $(BUILD)/slsh-fuzz
