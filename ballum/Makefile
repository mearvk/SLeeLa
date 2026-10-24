# SLeeLa Ballum(TM) -- pixel-level display control. Standalone build of the
# portable manager plus the per-OS backend and the effects/image helpers, into
# one static library, plus the tests, plus install/uninstall and per-OS
# dependency provisioning. Exactly one OS backend is compiled in; the OS is
# auto-detected but can be overridden with PLATFORM=linux|macos|windows.
#
# The OS-centric libraries Ballum links against are provided by the build:
#   - Linux   : X11 + Xrandr development packages (installed/reported by `deps`)
#   - macOS   : CoreGraphics/ApplicationServices/Foundation frameworks (ship
#               with the Xcode command-line tools; `deps` verifies them)
#   - Windows : gdi32/user32 import libraries (ship with the Windows SDK /
#               MinGW; `deps` verifies them)
# `make all` runs `check-deps` first, so a missing OS library is reported as a
# clear, actionable message instead of a raw compiler/linker error.

CC  ?= cc
CXX ?= c++
AR  ?= ar
PKG_CONFIG ?= pkg-config
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -Wpedantic -Iinclude
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude
OBJCXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Iinclude

BUILD  := build
PREFIX ?= /usr/local
DESTDIR ?=
LIBDIR     ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include
INSTALL ?= install

# `deps` install behaviour. By default `deps` only REPORTS and, where a package
# manager is available, uses its download-only / non-privileged path. Set
# BALLUM_DEPS_INSTALL=1 to actually install OS packages (which may use sudo on
# Linux). This keeps the default build side-effect-free and never silently
# elevates privileges.
BALLUM_DEPS_INSTALL ?= 0

# ---- OS detection (override with PLATFORM=...) ---------------------------
UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(OS),Windows_NT)
  PLATFORM ?= windows
else ifeq ($(UNAME_S),Darwin)
  PLATFORM ?= macos
else
  PLATFORM ?= linux
endif

# ---- per-OS backend source, define, and system libs ----------------------
ifeq ($(PLATFORM),windows)
  BACKEND_SRC    := src/ballum_backend_windows.c
  BACKEND_DEFINE := -DBALLUM_BACKEND_WINDOWS
  BACKEND_LIBS   := -lgdi32 -luser32
  BACKEND_IS_CPP := 0
else ifeq ($(PLATFORM),macos)
  BACKEND_SRC    := src/ballum_backend_macos.mm
  BACKEND_DEFINE := -DBALLUM_BACKEND_MACOS
  BACKEND_LIBS   := -framework CoreGraphics -framework ApplicationServices -framework Foundation
  BACKEND_IS_CPP := 1
else
  BACKEND_SRC    := src/ballum_backend_x11.c
  BACKEND_DEFINE := -DBALLUM_BACKEND_X11
  BACKEND_LIBS   := -lX11 -lXrandr -lm
  BACKEND_IS_CPP := 0
endif

NANOJPEG_DIR := ../user-interface/src

PORTABLE_C := src/ballum_manager.c src/ballum_backend_select.c src/ballum_backend_unavailable.c

# ---- default: verify OS deps, then build the real OS backend library ------
all: check-deps $(BUILD)/libsleela_ballum.a
	@echo "ballum: built $(BUILD)/libsleela_ballum.a for PLATFORM=$(PLATFORM) (links: $(BACKEND_LIBS))"

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/ballum_manager.o: src/ballum_manager.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/ballum_backend_select.o: src/ballum_backend_select.c | $(BUILD)
	$(CC) $(CFLAGS) $(BACKEND_DEFINE) -c $< -o $@

$(BUILD)/ballum_backend_unavailable.o: src/ballum_backend_unavailable.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/ballum_effects.o: src/ballum_effects.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/ballum_image.o: src/ballum_image.c | $(BUILD)
	$(CC) $(CFLAGS) -I$(NANOJPEG_DIR) -c $< -o $@

ifeq ($(BACKEND_IS_CPP),1)
$(BUILD)/ballum_backend_os.o: $(BACKEND_SRC) | $(BUILD)
	$(CXX) $(OBJCXXFLAGS) $(BACKEND_DEFINE) -c $< -o $@
else
$(BUILD)/ballum_backend_os.o: $(BACKEND_SRC) | $(BUILD)
	$(CC) $(CFLAGS) $(BACKEND_DEFINE) -c $< -o $@
endif

LIB_OBJS := $(BUILD)/ballum_manager.o $(BUILD)/ballum_backend_select.o \
            $(BUILD)/ballum_backend_unavailable.o $(BUILD)/ballum_backend_os.o \
            $(BUILD)/ballum_effects.o $(BUILD)/ballum_image.o

$(BUILD)/libsleela_ballum.a: $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

# ---- per-OS dependency provisioning --------------------------------------
# `check-deps` verifies the OS-centric libraries this PLATFORM needs. On Linux
# it checks X11 + Xrandr via pkg-config; if missing and BALLUM_DEPS_INSTALL=1,
# `deps` installs them with the detected package manager. macOS/Windows system
# frameworks/import libs ship with the platform SDK, so there is nothing to
# download -- `check-deps` only confirms the toolchain can see them.
.PHONY: check-deps deps
check-deps:
	@echo "ballum deps: PLATFORM=$(PLATFORM)"
	@command -v "$(CC)"  >/dev/null 2>&1 && echo "  CC  $$($(CC) --version 2>/dev/null | head -n1)"  || { echo "  MISSING: $(CC)"; exit 1; }
ifeq ($(PLATFORM),linux)
	@missing=0; \
	for p in x11 xrandr; do \
	  if command -v "$(PKG_CONFIG)" >/dev/null 2>&1 && $(PKG_CONFIG) --exists $$p; then \
	    echo "  AVAILABLE: $$p ($$($(PKG_CONFIG) --modversion $$p))"; \
	  else echo "  MISSING: $$p development files"; missing=1; fi; \
	done; \
	if [ $$missing -ne 0 ]; then \
	  echo "  -> run 'make deps' (reports how to install) or 'make deps BALLUM_DEPS_INSTALL=1' to install."; \
	  echo "     Ballum still builds its portable 'test' target without these."; \
	  exit 1; \
	fi
else ifeq ($(PLATFORM),macos)
	@echo "  macOS frameworks (CoreGraphics/ApplicationServices/Foundation) ship with the"
	@echo "  Xcode command-line tools. Verifying the toolchain can link them..."
	@printf 'int main(void){return 0;}\n' > $(BUILD)/.depcheck.m 2>/dev/null || { mkdir -p $(BUILD); printf 'int main(void){return 0;}\n' > $(BUILD)/.depcheck.m; }
	@$(CC) $(BUILD)/.depcheck.m $(BACKEND_LIBS) -o $(BUILD)/.depcheck 2>/dev/null \
	  && echo "  AVAILABLE: CoreGraphics/ApplicationServices/Foundation" \
	  || { echo "  MISSING: Apple frameworks -- install the Xcode command-line tools: xcode-select --install"; exit 1; }
	@rm -f $(BUILD)/.depcheck $(BUILD)/.depcheck.m
else
	@echo "  Windows system import libs (gdi32/user32) ship with the Windows SDK / MinGW."
	@echo "  Build with the MSVC or MinGW toolchain; no download is required."
endif
	@echo "ballum deps: OK for PLATFORM=$(PLATFORM)"

deps:
ifeq ($(PLATFORM),linux)
	@echo "ballum deps (linux): X11 + Xrandr development files are required."
	@if command -v apt-get >/dev/null 2>&1; then \
	   pkgs="libx11-dev libxrandr-dev"; mgr="apt-get install -y $$pkgs"; \
	 elif command -v dnf >/dev/null 2>&1; then \
	   pkgs="libX11-devel libXrandr-devel"; mgr="dnf install -y $$pkgs"; \
	 elif command -v pacman >/dev/null 2>&1; then \
	   pkgs="libx11 libxrandr"; mgr="pacman -S --noconfirm $$pkgs"; \
	 elif command -v zypper >/dev/null 2>&1; then \
	   pkgs="libX11-devel libXrandr-devel"; mgr="zypper install -y $$pkgs"; \
	 else \
	   echo "  No supported package manager found (apt/dnf/pacman/zypper)."; \
	   echo "  Install the X11 and Xrandr development packages for your distro, then re-run 'make all'."; \
	   exit 1; \
	 fi; \
	 if [ "$(BALLUM_DEPS_INSTALL)" = "1" ]; then \
	   echo "  Installing: $$pkgs"; \
	   if [ "$$(id -u)" = "0" ]; then sh -c "$$mgr"; else sudo $$mgr; fi; \
	 else \
	   echo "  To install, run:  sudo $$mgr"; \
	   echo "  or non-interactively:  make deps BALLUM_DEPS_INSTALL=1"; \
	 fi
else ifeq ($(PLATFORM),macos)
	@echo "ballum deps (macos): the required frameworks ship with the Xcode command-line tools."
	@xcode-select -p >/dev/null 2>&1 && echo "  Xcode command-line tools: present" \
	  || { echo "  Installing the Xcode command-line tools..."; [ "$(BALLUM_DEPS_INSTALL)" = "1" ] && xcode-select --install || echo "  Run: xcode-select --install"; }
else
	@echo "ballum deps (windows): gdi32/user32 ship with the Windows SDK / MinGW; nothing to download."
endif

# ---- test: portable, links the unavailable backend (no display libs) -----
# Built without any BACKEND_DEFINE so ballum_backend_select resolves to the
# unavailable fallback; the test supplies its own mock backend. Needs no OS
# display libraries, so it runs anywhere without `deps`.
test: | $(BUILD)
	$(CC) $(CFLAGS) \
	  src/ballum_manager.c src/ballum_backend_select.c src/ballum_backend_unavailable.c \
	  tests/test_ballum_manager.c -o $(BUILD)/ballum-manager-test
	./$(BUILD)/ballum-manager-test
	$(CC) $(CFLAGS) -I$(NANOJPEG_DIR) \
	  src/ballum_effects.c src/ballum_image.c $(NANOJPEG_DIR)/nanojpeg.c \
	  tests/test_ballum_effects.c -o $(BUILD)/ballum-effects-test -lm
	./$(BUILD)/ballum-effects-test
	@echo "ballum: all tests passed"

# ---- install / uninstall --------------------------------------------------
# Install the static library and the public headers under $(DESTDIR)$(PREFIX).
install: all
	$(INSTALL) -d "$(DESTDIR)$(LIBDIR)"
	$(INSTALL) -m 0644 $(BUILD)/libsleela_ballum.a "$(DESTDIR)$(LIBDIR)/libsleela_ballum.a"
	$(INSTALL) -d "$(DESTDIR)$(INCLUDEDIR)/sleela_ballum"
	$(INSTALL) -m 0644 include/sleela_ballum/ballum_manager.h "$(DESTDIR)$(INCLUDEDIR)/sleela_ballum/ballum_manager.h"
	$(INSTALL) -m 0644 include/sleela_ballum/ballum_effects.h "$(DESTDIR)$(INCLUDEDIR)/sleela_ballum/ballum_effects.h"
	$(INSTALL) -m 0644 include/sleela_ballum/ballum_image.h   "$(DESTDIR)$(INCLUDEDIR)/sleela_ballum/ballum_image.h"
	@echo "ballum: installed libsleela_ballum.a and headers to $(DESTDIR)$(PREFIX) (link with $(BACKEND_LIBS))"

uninstall:
	rm -f "$(DESTDIR)$(LIBDIR)/libsleela_ballum.a"
	rm -f "$(DESTDIR)$(INCLUDEDIR)/sleela_ballum/ballum_manager.h" \
	      "$(DESTDIR)$(INCLUDEDIR)/sleela_ballum/ballum_effects.h" \
	      "$(DESTDIR)$(INCLUDEDIR)/sleela_ballum/ballum_image.h"
	-rmdir "$(DESTDIR)$(INCLUDEDIR)/sleela_ballum" 2>/dev/null || true
	@echo "ballum: uninstalled from $(DESTDIR)$(PREFIX)"

clean:
	rm -rf $(BUILD)

help:
	@echo "SLeeLa Ballum build (PLATFORM=$(PLATFORM); override PLATFORM=linux|macos|windows)"
	@echo "  make all         Verify OS deps, then build build/libsleela_ballum.a for this OS"
	@echo "  make check-deps  Report whether this OS's display libraries are available"
	@echo "  make deps        Show how to install the OS libraries (BALLUM_DEPS_INSTALL=1 to install)"
	@echo "  make test        Build and run the portable tests (no display libraries required)"
	@echo "  make install     Install the library + headers to \$$(PREFIX) [default $(PREFIX)]"
	@echo "  make uninstall   Remove the installed library + headers"
	@echo "  make clean       Remove the build/ directory"

.PHONY: all test clean install uninstall help check-deps deps
