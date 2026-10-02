# SLeeLa repository build dispatcher
# Max Rupplin - MEARVK LLC - 2026
# Product-specific Makefiles remain authoritative. This root dispatcher only
# enters those existing build systems; it does not duplicate their source lists.

.PHONY: all core java28 regex compiler decompiler vm tutorial-check tests server clean help

all: core java28 regex compiler decompiler vm tutorial-check tests

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

tests:
	$(MAKE) -C tests check

server:
	$(MAKE) -C api/server

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
	@echo "  make all       Build core, Java 28, regex, compiler, decompiler, VM, and tests"
	@echo "  make core      Build impl/"
	@echo "  make java28    Build java28/"
	@echo "  make regex     Build regex/ and its test suites"
	@echo "  make compiler  Build lib/compiler/"
	@echo "  make decompiler Build lib/decompiler/"
	@echo "  make vm        Build lib/vm/\n\t@echo "  make tutorial-check Verify tutorial/example inventories""
	@echo "  make tests     Build and run tests/"
	@echo "  make server    Build api/server/"
	@echo "  make clean     Remove outputs from dispatched build systems"
	@echo ""
	@echo "Product-specific build folders and Makefiles remain authoritative."
