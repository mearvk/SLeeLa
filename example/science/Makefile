# example/science/Makefile
# Runs the BODI science example projects through the XML runner and checks each
# witness stream against its recorded expected-output evidence file.
#
#   make            # run every example and print its witnesses
#   make check      # run every example and diff against *.expected.txt
#   make math       # run a single discipline example
#   make regen      # regenerate the *.expected.txt evidence files
#
# RUNNER can be overridden if the binary lives elsewhere.

RUNNER ?= ../../api/bodi/bodi-xml-runner
EXAMPLES := math physics chemistry data-analytics social all-disciplines

.PHONY: all check regen help $(EXAMPLES)

all: $(EXAMPLES)

help:
	@echo "targets: all | check | regen | $(EXAMPLES)"

$(EXAMPLES):
	@echo "== $@ =="
	@$(RUNNER) $@.xml

check:
	@fail=0; for e in $(EXAMPLES); do \
	  if $(RUNNER) $$e.xml | diff -u $$e.expected.txt - >/dev/null; then \
	    echo "PASS  $$e"; \
	  else \
	    echo "FAIL  $$e"; fail=1; \
	  fi; \
	done; exit $$fail

regen:
	@for e in $(EXAMPLES); do $(RUNNER) $$e.xml > $$e.expected.txt; echo "wrote $$e.expected.txt"; done
