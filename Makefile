# SLeeLa repository build dispatcher
# Max Rupplin - MEARVK LLC - 2026
# Product-specific Makefiles remain authoritative. This root dispatcher only
# enters those existing build systems; it does not duplicate their source lists.

.PHONY: all core java28 regex compiler decompiler vm cpu scripting jetbrains install install-check tutorial-check tests server config route clean help

# `make all` builds everything and VERIFIES the installer (install-check); it
# does not deploy. Run `make install` explicitly to build+deploy and set
# SLEELA/PATH — a separate, side-effecting step (it is interactive).
all: core java28 regex compiler decompiler vm cpu scripting jetbrains install-check tutorial-check tests config route

# The impl build is fail-closed on a trusted SHA-256 manifest. Default it to the
# repository's manifest (absolute path) so `make` works from the repo root; an
# explicit SLEELA_SHA256_MANIFEST on the command line still overrides it.
SLEELA_SHA256_MANIFEST ?= $(abspath $(CURDIR)/security/sha256-manifest.json)
export SLEELA_SHA256_MANIFEST

core:
	$(MAKE) -C impl all

java28:
	$(MAKE) -C java28 all

regex:
	$(MAKE) -C regex all

compiler:
	$(MAKE) -C lib/compiler all

decompiler:
	$(MAKE) -C lib/decompiler all

vm:
	$(MAKE) -C lib/vm all

cpu:
	$(MAKE) -C lib/cpu all

scripting:
	$(MAKE) -C sleela-scripting all

jetbrains:
	$(MAKE) -C jetbrains all

install:
	$(MAKE) -C install install

install-check:
	$(MAKE) -C install install-check

tutorial-check:
	@./scripts/tutorial-check.sh

tests:
	$(MAKE) -C tests check

server:
	$(MAKE) -C api/server

route:
	@python3 -m json.tool route/ROUTE.DATA.json >/dev/null
	@echo "SLeeLa unified route data: route/ROUTE.DATA.json"

config:
	@echo "SLeeLa unified configuration: config/sleela.conf"
	@echo "Canonical runtime location: <PROJECT_ROOT>/.sleela/config"
	@echo "Override with config.mode=absolute and config.absolute_path"

clean:
	$(MAKE) -C impl clean
	$(MAKE) -C java28 clean
	$(MAKE) -C regex clean
	$(MAKE) -C lib/compiler clean
	$(MAKE) -C lib/decompiler clean
	$(MAKE) -C lib/vm clean
	$(MAKE) -C lib/cpu clean
	$(MAKE) -C tests clean

help:
	@echo "SLeeLa repository build dispatcher"
	@echo "  make all       Build core, Java 28, regex, compiler, decompiler, VM, CPU, JetBrains helper, and tests"
	@echo "  make core      Build impl/"
	@echo "  make java28    Build java28/"
	@echo "  make regex     Build regex/ and its test suites"
	@echo "  make compiler  Build lib/compiler/"
	@echo "  make decompiler Build lib/decompiler/"
	@echo "  make vm        Build lib/vm/"
	@echo "  make cpu       Check lib/cpu/ (CPU models, components, DMA/GPU options)"
	@echo "  make jetbrains Show JetBrains source acquisition helpers"
	@echo "  make install     Build, deploy, and set SLEELA/PATH (runs the Quick and Safe installer)"
	@echo "  make install-check Verify installer entry points only (no install)"
	@echo "  make tutorial-check Verify tutorial lesson sequence, example XML, and expected-evidence pairing"
	@echo "  make tests     Build and run tests/"
	@echo "  make server    Build api/server/"
	@echo "  make route     Validate unified route data"
	@echo "  make clean     Remove outputs from dispatched build systems"
	@echo ""
	@echo "Product-specific build folders and Makefiles remain authoritative."
