# example/science/Makefile
# Two kinds of science example live here:
#   * BODI XML projects (*.xml), run through the BODI XML runner;
#   * SLeeLa programs (*.sleela), run through the sleela interpreter.
#
#   make              # run every XML example and print its witnesses
#   make check        # run every XML example and diff vs *.expected.txt
#   make sleela       # run every science .sleela program
#   make sleela-check # run every .sleela program and diff vs *.sleela.expected.txt
#   make check-all    # both check targets
#   make math         # a single XML discipline example
#   make regen        # regenerate the XML *.expected.txt evidence files
#   make sleela-regen # regenerate the .sleela *.expected.txt evidence files
#
# RUNNER / SLEELA can be overridden if the binaries live elsewhere.

RUNNER ?= ../../api/bodi/bodi-xml-runner
SLEELA ?= ../../impl/build/sleela
REPO_ROOT := ../..

# Anchor the compiler's built-in SHA-256 gate to the repository checkout rather
# than a stale installed tree (e.g. ~/.local/sleela). findRepoRoot() in the
# driver honors SLEELA_HOME first. Derive the real checkout root from THIS
# Makefile's own location (this file lives at lib/ui/example/science/), so the
# anchor is correct no matter the caller's working directory. Overridable from
# the environment.
SLEELA_HOME ?= $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../../../..)
export SLEELA_HOME

EXAMPLES := math physics chemistry data-analytics social all-disciplines
SLEELA_PROGS := math physics chemistry astrophysics sociology

# The sleela runtime security gate needs these.
export SLEELA_SHEET ?= $(abspath $(REPO_ROOT))/SHEET.sheet
export SLEELA_SHA256_MANIFEST ?= $(abspath $(REPO_ROOT))/security/sha256-manifest.json

# sleela prints runtime banners on stderr/stdout; strip them for diffing.
FILTER := grep -vE '^\[memory|^\[security|^\[defender|^verification'

.PHONY: all check regen sleela sleela-check sleela-regen check-all help $(EXAMPLES)

all: $(EXAMPLES)

help:
	@echo "targets: all | check | sleela | sleela-check | check-all | regen | sleela-regen | $(EXAMPLES)"

$(EXAMPLES):
	@echo "== $@ =="
	@$(RUNNER) $@.xml

check:
	@fail=0; for e in $(EXAMPLES); do \
	  if $(RUNNER) $$e.xml | diff -u $$e.expected.txt - >/dev/null; then \
	    echo "PASS  $$e.xml"; \
	  else \
	    echo "FAIL  $$e.xml"; fail=1; \
	  fi; \
	done; exit $$fail

regen:
	@for e in $(EXAMPLES); do $(RUNNER) $$e.xml > $$e.expected.txt; echo "wrote $$e.expected.txt"; done

sleela:
	@for p in $(SLEELA_PROGS); do \
	  echo "== $$p.sleela =="; \
	  $(SLEELA) run $$p.sleela 2>/dev/null | $(FILTER); \
	done

sleela-check:
	@fail=0; for p in $(SLEELA_PROGS); do \
	  if $(SLEELA) run $$p.sleela 2>/dev/null | $(FILTER) | diff -u $$p.sleela.expected.txt - >/dev/null; then \
	    echo "PASS  $$p.sleela"; \
	  else \
	    echo "FAIL  $$p.sleela"; fail=1; \
	  fi; \
	done; exit $$fail

sleela-regen:
	@for p in $(SLEELA_PROGS); do \
	  $(SLEELA) run $$p.sleela 2>/dev/null | $(FILTER) > $$p.sleela.expected.txt; \
	  echo "wrote $$p.sleela.expected.txt"; \
	done

check-all: check sleela-check
