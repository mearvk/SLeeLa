# =============================================================================
# SleelaUI(TM) -- SLeeLa's own, original cross-platform User Interface toolkit.
# Self-contained build. Original SLeeLa work: an original widget toolkit with a
# software rasterizer and native per-OS backends -- not GTK, Qt, or a wrapper.
#
# Targets:
#   make            Build the static library (libsleelaui.a) with the native
#                   backend auto-detected for the host OS.
#   make example    Build the C gallery demo (build/slui-gallery).
#   make smoke      Build and run the headless smoke test (no display needed).
#   make install    Install the library, public header, and .desktop entry.
#   make clean      Remove build outputs.
#
# Platform detection mirrors impl/Makefile: the toolkit core is OS-neutral and
# the only per-OS differences are which backend translation unit is compiled
# and which system libraries it links.
#   * Linux / Unix (X11) ... x11.cpp     + -lX11 + FreeType + Fontconfig
#   * macOS (Cocoa) ........ cocoa.mm    + -framework Cocoa
#   * Windows 10+ (Win32) .. win32.cpp   + -lgdi32 -luser32
#
# Max Rupplin -- MEARVK LLC -- 2026
# =============================================================================
CXX        ?= g++
CC         ?= gcc
AR         ?= ar
CXXFLAGS   ?= -std=c++17 -O2 -Wall -Wextra
CFLAGS     ?= -std=c11 -O2 -Wall -Wextra
PKG_CONFIG ?= pkg-config
BUILD      := build

PREFIX     ?= /usr/local
BINDIR     ?= $(PREFIX)/bin
LIBDIR     ?= $(PREFIX)/lib
INCDIR     ?= $(PREFIX)/include
DESKTOPDIR ?= $(PREFIX)/share/applications
DESTDIR    ?=
INSTALL    ?= install

INCLUDES := -Iinclude -Isrc

# Portable core (backend-independent) translation units.
CORE_SRC := src/slui_theme.cpp src/slui_widget.cpp src/slui_window.cpp \
            src/slui_api.cpp src/slui_draw.cpp src/slui_throbber.cpp \
            src/slui_light.cpp src/slui_font.cpp src/slui_mood.cpp \
            src/slui_layout.cpp

# ---------------------------------------------------------------------------
# Host detection and backend selection.
# ---------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
  PLATFORM := windows
  EXE := .exe
  BACKEND_SRC := src/backend/win32.cpp
  BACKEND_DEF := -DSLUI_BACKEND_WIN32_BUILD
  BACKEND_CFLAGS :=
  BACKEND_LIBS := -lgdi32 -luser32
else
  EXE :=
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
    BACKEND_SRC := src/backend/cocoa.mm
    BACKEND_DEF := -DSLUI_BACKEND_COCOA_BUILD
    BACKEND_CFLAGS :=
    BACKEND_LIBS := -framework Cocoa -framework CoreText -framework CoreGraphics
  else
    PLATFORM := linux
    BACKEND_SRC := src/backend/x11.cpp
    BACKEND_DEF := -DSLUI_BACKEND_X11_BUILD
    BACKEND_PKGS := x11 freetype2 fontconfig
    BACKEND_CFLAGS := $(shell $(PKG_CONFIG) --cflags $(BACKEND_PKGS) 2>/dev/null)
    BACKEND_LIBS := $(shell $(PKG_CONFIG) --libs $(BACKEND_PKGS) 2>/dev/null)
  endif
endif

LIB      := $(BUILD)/libsleelaui.a
GALLERY  := $(BUILD)/slui-gallery$(EXE)
SMOKE    := $(BUILD)/slui-smoke$(EXE)

CORE_OBJ    := $(CORE_SRC:%.cpp=$(BUILD)/%.o)
BACKEND_OBJ := $(BUILD)/$(basename $(BACKEND_SRC)).o
HEADLESS_OBJ := $(BUILD)/src/backend/headless.o

.PHONY: all lib example smoke snapshot throbber-snapshot relief-snapshot font-snapshot mood-snapshot install install-strip uninstall clean help

all: lib

lib: $(LIB)

# ---- static library (core + native backend) -------------------------------
$(LIB): $(CORE_OBJ) $(BACKEND_OBJ)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $^
	@echo "SleelaUI library: $@  (backend: $(PLATFORM))"

# C++ core objects.
$(BUILD)/src/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(BACKEND_DEF) -c $< -o $@

# Native backend object (adds backend cflags; .mm compiled as Obj-C++ on macOS).
ifeq ($(PLATFORM),macos)
$(BACKEND_OBJ): $(BACKEND_SRC)
	@mkdir -p $(dir $@)
	$(CXX) -x objective-c++ $(CXXFLAGS) $(INCLUDES) $(BACKEND_DEF) $(BACKEND_CFLAGS) -c $< -o $@
else
$(BACKEND_OBJ): $(BACKEND_SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(BACKEND_DEF) $(BACKEND_CFLAGS) -c $< -o $@
endif

# ---- example (links the native backend) -----------------------------------
example: $(GALLERY)

$(GALLERY): examples/gallery.c $(LIB)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c examples/gallery.c -o $(BUILD)/gallery.o
	$(CXX) $(CXXFLAGS) $(BUILD)/gallery.o $(LIB) $(BACKEND_LIBS) -o $@
	@echo "SleelaUI gallery: $@"

# ---- headless smoke test (no window system) --------------------------------
# Built against the headless backend so it runs on CI with no display on any OS.
smoke: $(SMOKE)
	$(SMOKE)

$(HEADLESS_OBJ): src/backend/headless.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -DSLUI_BACKEND_HEADLESS_BUILD -c $< -o $@

$(SMOKE): tests/smoke.c $(CORE_OBJ) $(HEADLESS_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c tests/smoke.c -o $(BUILD)/smoke.o
	$(CXX) $(CXXFLAGS) $(BUILD)/smoke.o $(CORE_OBJ) $(HEADLESS_OBJ) -o $@

# ---- offscreen snapshot (display-free preview via the headless backend) ----
SNAPSHOT := $(BUILD)/slui-snapshot$(EXE)
snapshot: $(SNAPSHOT)
	$(SNAPSHOT) docs/preview.png

$(SNAPSHOT): tools/snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) tools/snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ) -o $@

# ---- throbber snapshot (display-free water-field preview) ------------------
THROBBER_SNAPSHOT := $(BUILD)/slui-throbber-snapshot$(EXE)
throbber-snapshot: $(THROBBER_SNAPSHOT)
	$(THROBBER_SNAPSHOT) docs/throbber.png

$(THROBBER_SNAPSHOT): tools/throbber_snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) tools/throbber_snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ) -o $@

# ---- relief snapshot (display-free lighting/relief preview) ----------------
RELIEF_SNAPSHOT := $(BUILD)/slui-relief-snapshot$(EXE)
relief-snapshot: $(RELIEF_SNAPSHOT)
	$(RELIEF_SNAPSHOT) docs/relief.png

$(RELIEF_SNAPSHOT): tools/relief_snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) tools/relief_snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ) -o $@

# ---- font snapshot (display-free font-effects preview) ---------------------
FONT_SNAPSHOT := $(BUILD)/slui-font-snapshot$(EXE)
font-snapshot: $(FONT_SNAPSHOT)
	$(FONT_SNAPSHOT) docs/font.png

$(FONT_SNAPSHOT): tools/font_snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) tools/font_snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ) -o $@

# ---- mood snapshot (display-free mood/wash/tanor preview) ------------------
MOOD_SNAPSHOT := $(BUILD)/slui-mood-snapshot$(EXE)
mood-snapshot: $(MOOD_SNAPSHOT)
	$(MOOD_SNAPSHOT) docs/mood.png

$(MOOD_SNAPSHOT): tools/mood_snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) tools/mood_snapshot.cpp $(CORE_OBJ) $(HEADLESS_OBJ) -o $@

# ---- install ---------------------------------------------------------------
install: $(LIB)
	$(INSTALL) -d $(DESTDIR)$(LIBDIR) $(DESTDIR)$(INCDIR) $(DESTDIR)$(DESKTOPDIR)
	$(INSTALL) -m 0644 $(LIB) $(DESTDIR)$(LIBDIR)/libsleelaui.a
	$(INSTALL) -m 0644 include/sleela_ui.h $(DESTDIR)$(INCDIR)/sleela_ui.h
	$(INSTALL) -m 0644 include/sleela_ui_draw.h $(DESTDIR)$(INCDIR)/sleela_ui_draw.h
	$(INSTALL) -m 0644 include/sleela_ui_light.h $(DESTDIR)$(INCDIR)/sleela_ui_light.h
	$(INSTALL) -m 0644 include/sleela_ui_font.h $(DESTDIR)$(INCDIR)/sleela_ui_font.h
	$(INSTALL) -m 0644 include/sleela_ui_mood.h $(DESTDIR)$(INCDIR)/sleela_ui_mood.h
	$(INSTALL) -m 0644 include/sleela_ui_layout.h $(DESTDIR)$(INCDIR)/sleela_ui_layout.h
	$(INSTALL) -m 0644 SleelaUI.desktop $(DESTDIR)$(DESKTOPDIR)/SleelaUI.desktop

install-strip: CXXFLAGS += -s
install-strip: clean install

uninstall:
	rm -f $(DESTDIR)$(LIBDIR)/libsleelaui.a
	rm -f $(DESTDIR)$(INCDIR)/sleela_ui.h
	rm -f $(DESTDIR)$(INCDIR)/sleela_ui_draw.h
	rm -f $(DESTDIR)$(INCDIR)/sleela_ui_light.h
	rm -f $(DESTDIR)$(INCDIR)/sleela_ui_font.h
	rm -f $(DESTDIR)$(INCDIR)/sleela_ui_mood.h
	rm -f $(DESTDIR)$(INCDIR)/sleela_ui_layout.h
	rm -f $(DESTDIR)$(DESKTOPDIR)/SleelaUI.desktop

clean:
	rm -rf $(BUILD)

help:
	@echo "SleelaUI(TM) build -- backend auto-detected: $(PLATFORM)"
	@echo "  make           Build libsleelaui.a with the native backend"
	@echo "  make example   Build the C gallery demo"
	@echo "  make smoke     Build and run the headless smoke test (no display)"
	@echo "  make snapshot  Render a display-free preview PNG (docs/preview.png)"
	@echo "  make throbber-snapshot  Render the water throbber PNG (docs/throbber.png)"
	@echo "  make relief-snapshot    Render the lighting/relief PNG (docs/relief.png)"
	@echo "  make font-snapshot      Render the font-effects PNG (docs/font.png)"
	@echo "  make mood-snapshot      Render the mood/wash/tanor PNG (docs/mood.png)"
	@echo "  make install   Install library, header, and desktop entry"
	@echo "  make clean     Remove build outputs"
