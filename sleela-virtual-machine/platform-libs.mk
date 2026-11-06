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
    # impl/Makefile compiles the SleelaUI Image widget against libpng/libavif
    # whenever pkg-config finds them (-DSLUI_HAVE_LIBPNG / -DSLUI_HAVE_LIBAVIF),
    # so libslcore.a's slui_widget.o then references those codecs. Mirror the
    # same probe here, otherwise the final link fails with e.g.
    #   undefined reference to symbol 'png_image_finish_read@@PNG16_0'.
    # Prefer pkg-config; but the authoritative signal is whether the Core
    # archive actually references the codec (pkg-config .pc files can be absent
    # even when libpng16.so is installed). If the archive needs PNG/AVIF, fall
    # back to the conventional link flags so the final link always resolves.
    CORE_UI_PNG_LIBS := $(shell pkg-config --libs libpng 2>/dev/null)
    CORE_UI_AVIF_LIBS := $(shell pkg-config --libs libavif 2>/dev/null)
    ifeq ($(strip $(CORE_UI_PNG_LIBS)),)
      CORE_UI_ARCHIVE_NEEDS_PNG := $(shell nm -u "$(SLCORE_LIB)" 2>/dev/null | grep -q png_image && echo yes)
      ifeq ($(CORE_UI_ARCHIVE_NEEDS_PNG),yes)
        CORE_UI_PNG_LIBS := -lpng16 -lz
      endif
    endif
    ifeq ($(strip $(CORE_UI_AVIF_LIBS)),)
      CORE_UI_ARCHIVE_NEEDS_AVIF := $(shell nm -u "$(SLCORE_LIB)" 2>/dev/null | grep -q avif && echo yes)
      ifeq ($(CORE_UI_ARCHIVE_NEEDS_AVIF),yes)
        CORE_UI_AVIF_LIBS := -lavif
      endif
    endif
    CORE_UI_LIBS ?= -lX11 $(CORE_UI_PKG_LIBS) $(CORE_UI_PNG_LIBS) $(CORE_UI_AVIF_LIBS)
    # impl selects the XRandR backend only when the development header is
    # available; mirror that probe so the premium runner resolves its symbols.
    CORE_BALLUM_HAVE_XRANDR := $(shell printf '\#include <X11/extensions/Xrandr.h>\nint main(void){return 0;}\n' | $(CC) -xc - -o /dev/null 2>/dev/null && echo yes)
    ifeq ($(CORE_BALLUM_HAVE_XRANDR),yes)
      CORE_UI_LIBS += -lXrandr
    endif
  endif
endif
