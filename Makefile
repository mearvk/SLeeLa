# SLeeLa repository build dispatcher
# Max Rupplin - MEARVK LLC - 2026
# Product-specific Makefiles remain authoritative. This root dispatcher only
# enters those existing build systems; it does not duplicate their source lists.

.PHONY: all core java28 regex compiler decompiler vm scripting jetbrains install tutorial-check tests server config route clean help

all: core java28 regex compiler decompiler vm scripting jetbrains install tutorial-check tests config route

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

scripting:
	$(MAKE) -C sleela-scripting all

jetbrains:
	$(MAKE) -C jetbrains all

install:
	$(MAKE) -C install all

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
	$(MAKE) -C tests clean

help:
	@echo "SLeeLa repository build dispatcher"
	@echo "  make all       Build core, Java 28, regex, compiler, decompiler, VM, JetBrains helper, and tests"
	@echo "  make core      Build impl/"
	@echo "  make java28    Build java28/"
	@echo "  make regex     Build regex/ and its test suites"
	@echo "  make compiler  Build lib/compiler/"
	@echo "  make decompiler Build lib/decompiler/"
	@echo "  make vm        Build lib/vm/"
	@echo "  make jetbrains Show JetBrains source acquisition helpers"
	@echo "  make install     Verify Quick and Safe installer entry points"
	@echo "  make tutorial-check Verify tutorial/example inventories"
	@echo "  make tests     Build and run tests/"
	@echo "  make server    Build api/server/"
	@echo "  make route     Validate unified route data"
	@echo "  make clean     Remove outputs from dispatched build systems"
	@echo ""
	@echo "Product-specific build folders and Makefiles remain authoritative."
