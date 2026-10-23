# SLeeLa Ballum(TM) -- pixel-level display control. Standalone build of the
# portable manager plus the per-OS backend, into one static library, plus the
# test. Exactly one OS backend is compiled in; the OS is auto-detected but can
# be overridden with PLATFORM=linux|macos|windows. The default `test` target
# links the always-present "unavailable" backend so it runs anywhere (no
# display libraries required); `make all` builds the real per-OS backend.

CC  ?= cc
CXX ?= c++
AR  ?= ar
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -Wpedantic -Iinclude
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude
OBJCXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Iinclude

BUILD := build

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

PORTABLE_C := src/ballum_manager.c src/ballum_backend_select.c src/ballum_backend_unavailable.c

# ---- default: the real OS backend library --------------------------------
all: $(BUILD)/libsleela_ballum.a

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/ballum_manager.o: src/ballum_manager.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/ballum_backend_select.o: src/ballum_backend_select.c | $(BUILD)
	$(CC) $(CFLAGS) $(BACKEND_DEFINE) -c $< -o $@

$(BUILD)/ballum_backend_unavailable.o: src/ballum_backend_unavailable.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

ifeq ($(BACKEND_IS_CPP),1)
$(BUILD)/ballum_backend_os.o: $(BACKEND_SRC) | $(BUILD)
	$(CXX) $(OBJCXXFLAGS) $(BACKEND_DEFINE) -c $< -o $@
else
$(BUILD)/ballum_backend_os.o: $(BACKEND_SRC) | $(BUILD)
	$(CC) $(CFLAGS) $(BACKEND_DEFINE) -c $< -o $@
endif

LIB_OBJS := $(BUILD)/ballum_manager.o $(BUILD)/ballum_backend_select.o \
            $(BUILD)/ballum_backend_unavailable.o $(BUILD)/ballum_backend_os.o

$(BUILD)/libsleela_ballum.a: $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

# ---- test: portable, links the unavailable backend (no display libs) -----
# Built without any BACKEND_DEFINE so ballum_backend_select resolves to the
# unavailable fallback; the test supplies its own mock backend.
test: | $(BUILD)
	$(CC) $(CFLAGS) \
	  src/ballum_manager.c src/ballum_backend_select.c src/ballum_backend_unavailable.c \
	  tests/test_ballum_manager.c -o $(BUILD)/ballum-manager-test
	./$(BUILD)/ballum-manager-test
	@echo "ballum: all tests passed"

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
