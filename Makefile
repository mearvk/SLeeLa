# SLeeLa repository build dispatcher
# Max Rupplin - MEARVK LLC - 2026
# Product-specific Makefiles remain authoritative. This root dispatcher only
# enters those existing build systems; it does not duplicate their source lists.

.PHONY: all autocheck core java28 regex compiler decompiler vm scripting jetbrains connector connector-test connector-check-deps codecs codecs-test churn churn-test churn-data-check antivirus antivirus-test antivirus-clamav antivirus-integrity antivirus-integrity-check install install-check install-product tutorial-check tests server config route common-rails registry premium-vms clean help

PREFIX ?= /usr/local\nDESTDIR ?=\n\nall: core java28 regex compiler decompiler vm scripting jetbrains install-check tutorial-check tests config route common-rails codecs churn antivirus registry premium-vms

# The impl build is fail-closed on a trusted SHA-256 manifest. Default it to the
# repository's manifest (absolute path) so `make` works from the repo root; an
# explicit SLEELA_SHA256_MANIFEST on the command line still overrides it.
SLEELA_SHA256_MANIFEST ?= $(abspath $(CURDIR)/security/sha256-manifest.json)
export SLEELA_SHA256_MANIFEST

autocheck:
	@sh ./scripts/autocheck-toolchain.sh

core: autocheck
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

scripting:
	$(MAKE) -C sleela-scripting all

jetbrains:
	$(MAKE) -C jetbrains all

# Optional native connectors have third-party client-library dependencies.
# Keep them out of the default build; invoke explicitly when those dependencies are installed.
connector:
	$(MAKE) -C lib/connector all

connector-test:
	$(MAKE) -C lib/connector test

connector-check-deps:
	$(MAKE) -C lib/connector check-deps

# Audio codec registry/plugins use the codecs package's own build and test rules.
codecs:
	$(MAKE) -C codecs all

codecs-test:
	$(MAKE) -C codecs test

# Churn data/model sets and their Java constraint-report tools.
churn:
	$(MAKE) -C churn all

churn-test:
	$(MAKE) -C churn test

churn-data-check:
	$(MAKE) -C churn data-check

# Sleelavirin's portable C core and self-tests (no third-party engine required).
antivirus:
	$(MAKE) -C antivirus all

antivirus-test:
	$(MAKE) -C antivirus test

antivirus-clamav:
	$(MAKE) -C antivirus clamav

antivirus-integrity:
	$(MAKE) -C antivirus integrity-manifest

antivirus-integrity-check:
	@test -n "$(MANIFEST)" || { echo "Usage: make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json" >&2; exit 2; }
	$(MAKE) -C antivirus integrity-check MANIFEST="$(MANIFEST)"

install:
	$(MAKE) -C install all

# Interactive deployment of the compiled binaries; choose user-local or system-wide.
install-product: autocheck
	@./install/quick-safe-install.sh

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
	@make -C config test

common-rails:
	$(MAKE) -C common-rails check

# Build the registry core and materialize the baseline registry table (text
# store + MySQL export) a fresh install reviews. Part of `all`.
registry:
	./build-registry.sh

# Build the eleven premium SLVM generations as real runnable VMs. Each links
# the shared native Core engine archive (impl/build/libslcore.a, produced by
# `core`) and runs compiled .sleela artifacts; `sleela vm run premium <n> <prog>`
# boots the chosen generation. Depends on `core` for the Core archive.
premium-vms: core
	@for n in 001 002 003 004 005 006 007 008 009 010 011; do \
	  echo "=== premium SLVM $$n ==="; \
	  $(MAKE) -C sleela-virtual-machine/$$n all || exit 1; \
	done
	@echo "SLeeLa premium VMs built: sleela-virtual-machine/0NN/vm-build/slvm-NN"

clean:
	@for n in 001 002 003 004 005 006 007 008 009 010 011; do $(MAKE) -C sleela-virtual-machine/$$n clean; done
	$(MAKE) -C registry clean
	$(MAKE) -C impl clean
	$(MAKE) -C java28 clean
	$(MAKE) -C regex clean
	$(MAKE) -C lib/compiler clean
	$(MAKE) -C lib/decompiler clean
	$(MAKE) -C lib/vm clean
	$(MAKE) -C tests clean
	$(MAKE) -C codecs clean
	$(MAKE) -C churn clean
	$(MAKE) -C antivirus clean
	@make -C config clean

help:
	@echo "SLeeLa repository build dispatcher"
	@echo "  make all       Build core, Java 28, regex, compiler, decompiler, VM, codecs, and tests"
	@echo "  make core      Build impl/"
	@echo "  make java28    Build java28/"
	@echo "  make regex     Build regex/ and its test suites"
	@echo "  make compiler  Build lib/compiler/"
	@echo "  make decompiler Build lib/decompiler/"
	@echo "  make vm        Build lib/vm/"
	@echo "  make codecs    Build the C/C++ codec registry and plugins"
	@echo "  make codecs-test Build and run codec registry/manager tests"
	@echo "  make churn     Compile the five churn constraint-report tools and validate data"
	@echo "  make churn-test Run all five churn constraint reports"
	@echo "  make churn-data-check Validate churn JSON/XML/CSV/TXT files"
	@echo "  make antivirus Build Sleelavirin C core and self-test executable"
	@echo "  make antivirus-test Build and run Sleelavirin C/native scanner self-tests"
	@echo "  make antivirus-clamav Check optional ClamAV scanner integration"
	@echo "  make antivirus-integrity Generate SHA-256 inventory for antivirus source and indexes"
	@echo "  make antivirus-integrity-check MANIFEST=PATH Verify the SHA-256 inventory"
	@echo "  make jetbrains Show JetBrains source acquisition helpers"
	@echo "  make connector Build all optional native database/data-source connectors"
	@echo "  make connector-test Run available offline connector tests"
	@echo "  make connector-check-deps Report native connector dependencies"
	@echo "  make install-check Verify Quick and Safe installer entry points"\n\t@echo "  make install [PREFIX=/usr/local] [DESTDIR=...] Install binaries and runtime files"\n\t@echo "  make install PREFIX=$HOME/.local Install for the current user"
	@echo "  make install-product  Build and interactively install compiled products (user-local or system-wide)"
	@echo "  make tutorial-check Verify tutorial lesson sequence, example XML, and expected-evidence pairing"
	@echo "  make tests     Build and run tests/"
	@echo "  make server    Build api/server/"
	@echo "  make route     Validate unified route data"
	@echo "  make config    Validate configuration policy and run location/VM smoke tests"
	@echo "  make common-rails Build and run CommonRails C/C++/Java/Sleela/SST checks"
	@echo "  make premium-vms Build the 11 premium SLVM generations as real runnable VMs"
	@echo "  make clean     Remove outputs from dispatched build systems"
	@echo ""
	@echo "Product-specific build folders and Makefiles remain authoritative."
