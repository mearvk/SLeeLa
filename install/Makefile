.PHONY: all install install-noninteractive install-help install-check

# Default target VERIFIES the installer entry points (safe, side-effect-free).
# To actually install, run `make install` (interactive) or
# `make install-noninteractive` (uses defaults, no prompts).
all: install-check

# Pick the right installer for this OS (macOS vs Linux); Windows uses the .cmd.
UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(UNAME_S),Darwin)
INSTALLER := ./quick-safe-install-macos.sh
else
INSTALLER := ./quick-safe-install.sh
endif

# Actually run the installer: build SLeeLa, stage it, deploy to the user-local
# root, and set SLEELA_HOME / SLEELA / PATH. This is the step that produces a
# usable `sleela` on PATH — `install-check` alone does NOT install anything.
install:
	@echo "Running the SLeeLa Quick and Safe installer: $(INSTALLER)"
	@echo "(On Windows, run: install\\quick-safe-install.cmd)"
	@$(INSTALLER)

# Non-interactive install: answer every prompt with its default by piping blank
# lines in. Useful for CI / scripted setup.
install-noninteractive:
	@echo "Running $(INSTALLER) with default answers (non-interactive)..."
	@yes '' | $(INSTALLER)

install-help:
	@echo "SLeeLa Quick and Safe Install"
	@echo "  make -C install install                 Build + deploy + set SLEELA/PATH (interactive)"
	@echo "  make -C install install-noninteractive  Same, accepting all defaults (no prompts)"
	@echo "  make -C install install-check           Verify installer entry points only (no install)"
	@echo "  ./quick-safe-install.sh                 Linux installer (direct)"
	@echo "  ./quick-safe-install-macos.sh           macOS installer (direct)"
	@echo "  quick-safe-install.cmd                  Windows 10+ installer (direct)"

install-check:
	@test -f quick-safe-install.sh
	@test -f quick-safe-install-macos.sh
	@test -f quick-safe-install.ps1
	@test -f quick-safe-install.cmd
	@echo "Installer entry points present for Linux, macOS, and Windows 10+."
	@grep -q 'SLEELA_HOME' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer missing SLEELA_HOME/PATH setup"; exit 1; }
	@grep -q 'sleela-env.sh' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer missing PATH snippet"; exit 1; }
	@grep -q 'SLEELA_HOME' quick-safe-install.ps1 || { echo "ERROR: Windows installer missing SLEELA_HOME/PATH setup"; exit 1; }
	@grep -q 'export SLEELA=' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer does not set SLEELA"; exit 1; }
	@grep -q 'impl/build' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer does not install the built sleela binary"; exit 1; }
	@grep -q 'SLEELA"' quick-safe-install.ps1 || { echo "ERROR: Windows installer does not set SLEELA"; exit 1; }
	@grep -q 'impl.build' quick-safe-install.ps1 || { echo "ERROR: Windows installer does not install the built sleela binary"; exit 1; }
	@grep -q 'SLEELA_SHA256_MANIFEST' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer does not set SLEELA_SHA256_MANIFEST"; exit 1; }
	@grep -q 'verify-before-execution.py' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer does not stage the verify tool"; exit 1; }
	@grep -q 'SLEELA_SHA256_MANIFEST' quick-safe-install.ps1 || { echo "ERROR: Windows installer does not set SLEELA_SHA256_MANIFEST"; exit 1; }
	@echo "PATH + SLEELA_HOME + SLEELA + built-binary + SHA-256 verification assets present for all three platforms."
	@echo ""
	@echo "NOTE: this only VERIFIED the installer. To actually install, run:"
	@echo "  make -C install install        (or, from the repo root, make install)"
