# SLeeLa lib/ballum -- the SLeeLa-facing Ballum(TM) library.
#
# lib/ballum is SLeeLa source (.sleela): the developer-facing Screen/Display/
# Session/Input/Effect/Color classes plus the per-OS profile classes. Its
# RUNTIME rides on the native backend in /ballum (libsleela_ballum.a), which
# links the OS-centric display libraries. So this tributary Makefile:
#   * validates the .sleela sources with the authoritative compiler (`check`), and
#   * dispatches the native build / dependency provisioning / install / clean
#     down to ../../ballum, for whichever OS we are on.
#
# Targets mirror the repo conventions: all, check, deps, check-deps, install,
# uninstall, clean, help. PLATFORM / PREFIX / DESTDIR flow through to ../../ballum.

PREFIX  ?= /usr/local
DESTDIR ?=

# Absolute manifest path so it stays correct when passed into `make -C ../../impl`.
SLEELA_SHA256_MANIFEST ?= $(abspath $(CURDIR)/../../security/sha256-manifest.json)
export SLEELA_SHA256_MANIFEST

SLEELA_BIN := ../../impl/build/sleela

# The runnable + class sources to validate. The demos and the Master Classes are
# one-class-per-file units; `sleela check` resolves referenced library units.
SLEELA_SRCS := $(wildcard *.sleela) \
               $(wildcard platforms/Apple/macOS/*.sleela) \
               $(wildcard platforms/GNU/Linux/*.sleela) \
               $(wildcard platforms/Microsoft/Windows/*.sleela)

.PHONY: all check compiler deps check-deps native install uninstall clean help

# `all` validates the SLeeLa sources and builds the native backend for this OS.
all: check native

# Build the authoritative compiler on demand (same pattern as lib/compiler).
compiler:
	$(MAKE) -C ../../impl build/sleela SLEELA_SHA256_MANIFEST="$(SLEELA_SHA256_MANIFEST)"

# Validate every lib/ballum .sleela source with `sleela check`. A demo (one with
# a main) checks fully; a bare library class reports "no 'main' method found",
# which is expected and not a failure -- those are validated via the demos that
# use them, so we only fail on a genuine parse/semantic error.
check: compiler
	@echo "lib/ballum: validating SLeeLa sources with $(SLEELA_BIN)"
	@cd ../.. && fail=0; for f in $(addprefix lib/ballum/,$(SLEELA_SRCS)); do \
	  out=$$("$(CURDIR)/$(SLEELA_BIN)" check "$$f" 2>&1); \
	  if printf '%s\n' "$$out" | grep -qiE 'error' && ! printf '%s\n' "$$out" | grep -qi "no 'main' method found"; then \
	    echo "FAIL: $$f"; printf '%s\n' "$$out" | grep -iE 'error' | head -3; fail=1; \
	  else echo "  ok: $$f"; fi; \
	done; \
	[ $$fail -eq 0 ] && echo "lib/ballum: all SLeeLa sources valid" || exit 1

# Native backend dependency provisioning / report, delegated to /ballum.
deps:
	$(MAKE) -C ../../ballum deps BALLUM_DEPS_INSTALL="$(BALLUM_DEPS_INSTALL)" PLATFORM="$(PLATFORM)"
check-deps:
	$(MAKE) -C ../../ballum check-deps PLATFORM="$(PLATFORM)"

# Build the native backend library (links this OS's display libraries).
native:
	$(MAKE) -C ../../ballum all PLATFORM="$(PLATFORM)"

# Install the native backend library + headers (the SLeeLa classes are source,
# consumed in-tree by the compiler/loader, so there is nothing to copy for them).
install: all
	$(MAKE) -C ../../ballum install PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" PLATFORM="$(PLATFORM)"

uninstall:
	$(MAKE) -C ../../ballum uninstall PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" PLATFORM="$(PLATFORM)"

clean:
	$(MAKE) -C ../../ballum clean

help:
	@echo "SLeeLa lib/ballum -- the SLeeLa-facing Ballum library (dispatches native work to /ballum)"
	@echo "  make all         Validate the .sleela sources and build the native backend for this OS"
	@echo "  make check       Validate the lib/ballum .sleela sources with the compiler"
	@echo "  make deps        Report/install the native backend's OS display libraries (via /ballum)"
	@echo "  make check-deps  Report whether this OS's display libraries are available"
	@echo "  make native      Build the native backend library (/ballum) for this OS"
	@echo "  make install [PREFIX=/usr/local] Install the native backend library + headers"
	@echo "  make uninstall   Remove the installed native backend library + headers"
	@echo "  make clean       Clean the native backend build"
