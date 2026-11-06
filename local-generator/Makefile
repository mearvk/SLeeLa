# SLeeLa Local Generator -- build/preview helpers.
#
# The generator itself is authoritative SLeeLa source under generator/. This
# Makefile only drives it: it runs the SLeeLa runtime on generate.sleela and
# prints the generated project plan. A host CLI would split that plan on the
# ">>> <path>" banners and write each artifact to disk.
#
# The SLeeLa runtime is located via SLEELA_BIN (defaults to the repository's
# staged binary). Override on the command line if yours lives elsewhere.

REPO_ROOT := $(abspath $(CURDIR)/..)
SLEELA_BIN ?= $(REPO_ROOT)/bin/SLeeLa
ENTRY := generator/generate.sleela

export SLEELA_SHEET ?= $(REPO_ROOT)/SHEET.sheet
export SLEELA_SHA256_MANIFEST ?= $(REPO_ROOT)/security/sha256-manifest.json

.PHONY: all generate check help

all: generate

# Run the SLeeLa generator and print the project plan.
generate:
	@echo "Generating project plan via SLeeLa: $(ENTRY)"
	@"$(SLEELA_BIN)" run "$(ENTRY)" \
		|| { echo "SLeeLa runtime not available; see generator/ for the generator source."; exit 1; }

# Type/semantic-check the generator sources.
check:
	@"$(SLEELA_BIN)" check "$(ENTRY)"

help:
	@echo "SLeeLa Local Generator"
	@echo "  make generate  Run the SLeeLa generator -> prints the project plan"
	@echo "  make check     Semantic-check the generator sources"
