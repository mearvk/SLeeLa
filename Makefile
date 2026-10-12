# SLeeLa repository build dispatcher
# Max Rupplin - MEARVK LLC - 2026
# Product-specific Makefiles remain authoritative. This root dispatcher only
# enters those existing build systems; it does not duplicate their source lists.

.PHONY: all autocheck core java28 regex compiler decompiler vm scripting jetbrains connector connector-test connector-check-deps codecs codecs-test churn churn-test churn-data-check install install-product tutorial-check tests server config route common-rails clean help

all: core java28 regex compiler decompiler vm scripting jetbrains install tutorial-check tests config route common-rails codecs

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

clean:
	$(MAKE) -C impl clean
	$(MAKE) -C java28 clean
	$(MAKE) -C regex clean
	$(MAKE) -C lib/compiler clean
	$(MAKE) -C lib/decompiler clean
	$(MAKE) -C lib/vm clean
	$(MAKE) -C tests clean
	$(MAKE) -C codecs clean
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
	@echo "  make jetbrains Show JetBrains source acquisition helpers"
	@echo "  make connector Build all optional native database/data-source connectors"
	@echo "  make connector-test Run available offline connector tests"
	@echo "  make connector-check-deps Report native connector dependencies"
	@echo "  make install     Verify Quick and Safe installer entry points"
	@echo "  make install-product  Build and interactively install compiled products (user-local or system-wide)"
	@echo "  make tutorial-check Verify tutorial lesson sequence, example XML, and expected-evidence pairing"
	@echo "  make tests     Build and run tests/"
	@echo "  make server    Build api/server/"
	@echo "  make route     Validate unified route data"
	@echo "  make config    Validate configuration policy and run location/VM smoke tests"
	@echo "  make common-rails Build and run CommonRails C/C++/Java/Sleela/SST checks"
	@echo "  make clean     Remove outputs from dispatched build systems"
	@echo ""
	@echo "Product-specific build folders and Makefiles remain authoritative."
