# CommonRails build and contract checks
# Max Rupplin - MEARVK LLC - 2026
# Portable entry point for the C, C++, Java Heritage layers and Sleela/SST checks.

CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -Iheritage/printing/c
SLEELA ?= ../impl/build/sleela
NORDSHRIFT ?= ../impl/build/nordshrift

# Anchor the compiler's built-in SHA-256 gate to the repository checkout (the
# parent of this directory) rather than a stale installed tree such as
# ~/.local/sleela. findRepoRoot() in the driver honors SLEELA_HOME first. The
# root dispatcher sets this too; forcing it here keeps a standalone
# `make -C common-rails` correct. Unconditional ':=' on purpose so a stale
# SLEELA_HOME inherited from the environment (the installer exports one pointing
# at ~/.local/sleela) cannot hijack a build of this source tree.
SLEELA_HOME := $(abspath ..)
export SLEELA_HOME

.PHONY: all check c-check cpp-check java-check sleela-check sst-check clean help

all: check

check: c-check cpp-check java-check sleela-check sst-check

c-check:
	$(CC) $(CFLAGS) -o heritage/printing/c/heritage-print-test heritage/printing/c/*.c
	./heritage/printing/c/heritage-print-test

cpp-check:
	$(MAKE) -C heritage/printing/cpp check

java-check:
	$(MAKE) -C heritage/printing/java check

# Skip these two only when the toolchain has not been built yet. CI and local
# developers can build it first with: make -C ../impl all
sleela-check:
	@if test -x "$(SLEELA)"; then \
	  "$(SLEELA)" check CommonRails.sleela && \
	  "$(SLEELA)" check PrinterConfig.sleela; \
	else \
	  echo "SKIP: Sleela CLI not found at $(SLEELA) (build ../impl first)"; \
	fi

sst-check:
	@if test -x "$(NORDSHRIFT)"; then \
	  "$(NORDSHRIFT)" check common-rails.sst; \
	else \
	  echo "SKIP: Nordshrift CLI not found at $(NORDSHRIFT) (build ../impl first)"; \
	fi

clean:
	rm -f heritage/printing/c/heritage-print-test
	$(MAKE) -C heritage/printing/cpp clean
	$(MAKE) -C heritage/printing/java clean

help:
	@echo "CommonRails targets:"
	@echo "  make / make check  Run C, C++, Java, and available Sleela/SST checks"
	@echo "  make c-check       Build and run the C contract test"
	@echo "  make cpp-check     Build and run the C++ contract test"
	@echo "  make java-check    Compile and run the Java contract test"
	@echo "  make sleela-check  Check both Sleela source editions when toolchain exists"
	@echo "  make sst-check     Check the Nordshrift control sheet when toolchain exists"
	@echo "  make clean         Remove generated contract-test artifacts"
