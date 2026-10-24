# Shared platform link dependencies for premium SLVM runners.
# These match the platform providers selected by impl/Makefile when it builds
# libslcore.a. Keep this fragment in sync with the native backend selection.

ifeq ($(OS),Windows_NT)
  CORE_UI_LIBS ?= -lopengl32 -lgdi32 -luser32 -lshell32 -lwlanapi -lole32
else
  UNAME_S ?= $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
    CORE_UI_LIBS ?= -framework Cocoa -framework CoreText -framework CoreGraphics \
                    -framework CoreWLAN -framework Foundation -framework Security \
                    -framework ApplicationServices
  else
    CORE_UI_PKG_LIBS := $(shell pkg-config --libs freetype2 fontconfig 2>/dev/null)
    ifeq ($(strip $(CORE_UI_PKG_LIBS)),)
      CORE_UI_PKG_LIBS := -lfreetype -lfontconfig
    endif
    CORE_UI_LIBS ?= -lX11 $(CORE_UI_PKG_LIBS)
    # impl selects the XRandR backend only when the development header is
    # available; mirror that probe so the premium runner resolves its symbols.
    CORE_BALLUM_HAVE_XRANDR := $(shell printf '\\#include <X11/extensions/Xrandr.h>\\nint main(void){return 0;}\\n' | $(CC) -xc - -o /dev/null 2>/dev/null && echo yes)
    ifeq ($(CORE_BALLUM_HAVE_XRANDR),yes)
      CORE_UI_LIBS += -lXrandr
    endif
  endif
endif
