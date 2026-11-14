# CommonRails build and contract checks
# Max Rupplin - MEARVK LLC - 2026
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -Iheritage/printing/c
SLEELA ?= ../impl/build/sleela
NORDSHRIFT ?= ../impl/build/nordshrift
ifeq ($(OS),Windows_NT)
  EXE := .exe
else
  EXE :=
endif
C_TEST := heritage/printing/c/heritage-print-test$(EXE)

SLEELA_HOME := $(abspath ..)
export SLEELA_HOME

.PHONY: all check c-check cpp-check java-check sleela-check sst-check clean help
all: check
check: c-check cpp-check java-check sleela-check sst-check

c-check: $(C_TEST)
	./$(C_TEST)

$(C_TEST): $(wildcard heritage/printing/c/*.c)
	$(CC) $(CFLAGS) -o $@ $^

cpp-check:
	$(MAKE) -C heritage/printing/cpp check EXE=$(EXE)

java-check:
	$(MAKE) -C heritage/printing/java check

sleela-check:
	@if test -x "$(SLEELA)$(EXE)"; then \
	  "$(SLEELA)$(EXE)" check CommonRails.sleela && \
	  "$(SLEELA)$(EXE)" check PrinterConfig.sleela; \
	else echo "SKIP: Sleela CLI not found at $(SLEELA)$(EXE) (build ../impl first)"; fi

sst-check:
	@if test -x "$(NORDSHRIFT)$(EXE)"; then \
	  "$(NORDSHRIFT)$(EXE)" check common-rails.sst; \
	else echo "SKIP: Nordshrift CLI not found at $(NORDSHRIFT)$(EXE) (build ../impl first)"; fi

clean:
	rm -f heritage/printing/c/heritage-print-test heritage/printing/c/heritage-print-test.exe
	$(MAKE) -C heritage/printing/cpp clean EXE=$(EXE)
	$(MAKE) -C heritage/printing/java clean

help:
	@echo "CommonRails targets: make check, c-check, cpp-check, java-check, sleela-check, sst-check, clean"
