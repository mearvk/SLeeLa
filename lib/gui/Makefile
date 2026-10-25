# SLeeLa lib/gui -- the SLeeLa-facing GUI library (tributary Makefile).
#
# lib/gui is SLeeLa source (.sleela): the developer-facing GUI contract carried
# in five parallel version lines (version-mual 1.0, version-gual 2.0,
# version-thual 3.0, version-dual 4.0, version-military 5.0), each a progressive
# superset. Its RUNTIME rides on the native bridge/theme in gui/native
# (sleela_gui_bridge.*, sleela_gui_theme.*), the C ABI boundary to the core.
#
# So this tributary Makefile:
#   * validates the .sleela sources with the authoritative compiler (`check`), and
#   * dispatches the native build / test / clean down to ../../gui/native.
#
# Mirrors the lib/ballum tributary pattern. PLATFORM/PREFIX/DESTDIR flow through.

PREFIX  ?= /usr/local
DESTDIR ?=

# Absolute manifest path so it stays correct when passed into `make -C ../../impl`.
SLEELA_SHA256_MANIFEST ?= $(abspath $(CURDIR)/../../security/sha256-manifest.json)
export SLEELA_SHA256_MANIFEST

SLEELA_BIN := ../../impl/build/sleela

# The five version lines, in capability order.
LINES := version-mual version-gual version-thual version-dual version-military
# Default line to validate (a superset check of one line is the fast path; the
# lines are supersets, so one representative line catches the shared contract).
# `make check-all` validates every line.
LINE ?= version-mual

.PHONY: all check check-all native test clean help compiler $(LINES)

# `all` validates the default line's SLeeLa sources and builds the native bridge.
all: check native

# Build the authoritative compiler on demand (same pattern as lib/ballum).
compiler:
	$(MAKE) -C ../../impl build/sleela SLEELA_SHA256_MANIFEST="$(SLEELA_SHA256_MANIFEST)"

# Validate one version line's .sleela sources. A demo (with a main) checks
# fully; a bare library class reports "no 'main' method found", which is
# expected -- those are validated via the demos that use them -- so we only fail
# on a genuine parse/semantic error.
check: compiler
	@echo "lib/gui: validating $(LINE) SLeeLa sources with $(SLEELA_BIN)"
	@cd ../.. && fail=0; n=0; \
	for f in lib/gui/$(LINE)/*.sleela; do \
	  [ -f "$$f" ] || continue; n=$$((n+1)); \
	  out=$$("$(CURDIR)/$(SLEELA_BIN)" check "$$f" 2>&1); \
	  if printf '%s\n' "$$out" | grep -qiE 'error' && ! printf '%s\n' "$$out" | grep -qi "no 'main' method found"; then \
	    echo "FAIL: $$f"; printf '%s\n' "$$out" | grep -iE 'error' | head -2; fail=1; \
	  fi; \
	done; \
	[ $$fail -eq 0 ] && echo "lib/gui: $(LINE) OK ($$n files)" || exit 1

# Validate every version line (slower; one invocation per .sleela file).
check-all:
	@for L in $(LINES); do $(MAKE) --no-print-directory check LINE=$$L || exit 1; done
	@echo "lib/gui: all five version lines valid"

# Build the native GUI bridge/theme (the C ABI boundary the library rides on).
native:
	$(MAKE) -C ../../gui/native all

# Build + run the native bridge/theme self-tests.
test:
	$(MAKE) -C ../../gui/native test

# Clean the native build.
clean:
	$(MAKE) -C ../../gui/native clean

help:
	@echo "SLeeLa lib/gui -- the SLeeLa-facing GUI library (5 version lines; dispatches native work to gui/native)"
	@echo "  make all         Validate the default line ($(LINE)) and build the native bridge"
	@echo "  make check       Validate one version line's .sleela sources (LINE=version-<name>)"
	@echo "  make check-all   Validate all five version lines (slower)"
	@echo "  make native      Build the native GUI bridge/theme (gui/native)"
	@echo "  make test        Build + run the native bridge/theme self-tests"
	@echo "  make clean       Clean the native build"
	@echo ""
	@echo "Lines: $(LINES)"
