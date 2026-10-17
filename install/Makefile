# SLeeLa conventional installation targets (Linux/macOS POSIX make).
# Max Rupplin - MEARVK LLC - 2026
PREFIX ?= /usr/local
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin
LIBDIR ?= $(PREFIX)/lib
DATADIR ?= $(PREFIX)/share
# Keep the system default conventional, but make a custom/user prefix safe
# without root by defaulting its configuration to the user's config directory.
ifeq ($(origin SYSCONFDIR),undefined)
  ifeq ($(PREFIX),/usr/local)
    SYSCONFDIR := /etc
  else
    SYSCONFDIR := $(HOME)/.config
  endif
endif
RUNTIME_NAME := sleela
RUNTIME_DIR := $(DESTDIR)$(LIBDIR)/$(RUNTIME_NAME)
DATA_DIR := $(DESTDIR)$(DATADIR)/$(RUNTIME_NAME)
CONFIG_DIR := $(DESTDIR)$(SYSCONFDIR)/$(RUNTIME_NAME)
REPO_ROOT := ..

.PHONY: all install install-check install-help uninstall
all: install-check

install-help:
	@echo "SLeeLa installation targets"
	@echo "  make -C install install [PREFIX=/usr/local] [DESTDIR=...]"
	@echo "  make install PREFIX=\$$HOME/.local"
	@echo "  make -C install uninstall [PREFIX=/usr/local]"
	@echo "  BINDIR, LIBDIR, DATADIR, SYSCONFDIR may be overridden."

install-check:
	@test -f quick-safe-install.sh
	@test -f quick-safe-install-macos.sh
	@test -f quick-safe-install.ps1
	@test -f quick-safe-install.cmd
	@grep -q 'Required compiled product missing' quick-safe-install.sh
	@grep -q 'System-wide' README.md
	@echo "Installer entry points and compiled-product installation checks present for Linux, macOS, and Windows 10+."

install:
	@test -x "$(REPO_ROOT)/impl/build/sleela" || { echo "Missing impl/build/sleela; run make -C impl all first." >&2; exit 1; }
	@test -x "$(REPO_ROOT)/impl/build/nordshrift" || { echo "Missing impl/build/nordshrift; run make -C impl all first." >&2; exit 1; }
	@echo "Installing SLeeLa to prefix $(PREFIX) (DESTDIR=$(DESTDIR))"
	install -d "$(DESTDIR)$(BINDIR)" "$(RUNTIME_DIR)/lib" "$(RUNTIME_DIR)/tools" "$(RUNTIME_DIR)/security" "$(DATA_DIR)/docs" "$(DATA_DIR)/examples" "$(CONFIG_DIR)"
	install -m 755 "$(REPO_ROOT)/impl/build/sleela" "$(DESTDIR)$(BINDIR)/sleela"
	install -m 755 "$(REPO_ROOT)/impl/build/nordshrift" "$(DESTDIR)$(BINDIR)/nordshrift"
	cp -a "$(REPO_ROOT)/lib/." "$(RUNTIME_DIR)/lib/"
	install -m 644 "$(REPO_ROOT)/tools/verify-before-execution.py" "$(RUNTIME_DIR)/tools/"
	install -m 644 "$(REPO_ROOT)/security/sha256-manifest.json" "$(RUNTIME_DIR)/security/"
	# Copy every manifest-listed source at its repository-relative path so the installed verifier has a complete source root.
	python3 - "$(REPO_ROOT)" "$(RUNTIME_DIR)" <<'PY_MANIFEST_COPY'
import json, os, shutil, sys
repo, runtime = map(os.path.abspath, sys.argv[1:3])
manifest = os.path.join(repo, "security", "sha256-manifest.json")
with open(manifest, encoding="utf-8") as stream:
    entries = json.load(stream).get("files", [])
for entry in entries:
    rel = entry["path"]
    src = os.path.abspath(os.path.join(repo, rel))
    if os.path.commonpath((repo, src)) != repo or not os.path.isfile(src):
        raise SystemExit("Invalid or missing manifest source: " + rel)
    dst = os.path.join(runtime, rel)
    os.makedirs(os.path.dirname(dst), exist_ok=True)
    shutil.copy2(src, dst)
PY_MANIFEST_COPY
	cp -a "$(REPO_ROOT)/docs/." "$(DATA_DIR)/docs/"
	if [ -d "$(REPO_ROOT)/examples" ]; then cp -a "$(REPO_ROOT)/examples/." "$(DATA_DIR)/examples/"; fi
	install -m 644 "$(REPO_ROOT)/README.md" "$(DATA_DIR)/README.md"
	if [ ! -e "$(CONFIG_DIR)/startup.conf" ]; then install -m 644 "$(REPO_ROOT)/install/startup.conf.example" "$(CONFIG_DIR)/startup.conf"; fi
	printf '%s\n' "$(PREFIX)" > "$(RUNTIME_DIR)/install-prefix"
	printf '%s\n' "$(BINDIR)/sleela" "$(BINDIR)/nordshrift" > "$(RUNTIME_DIR)/installed-binaries"
	@echo "SLeeLa installed. Commands: $(BINDIR)/sleela and $(BINDIR)/nordshrift"
	@echo "Runtime: $(RUNTIME_DIR)"
	@echo "Configuration: $(CONFIG_DIR)/startup.conf (existing configuration preserved)"

uninstall:
	@echo "Removing SLeeLa executables and files installed under $(LIBDIR)/sleela and $(DATADIR)/sleela."
	rm -f "$(DESTDIR)$(BINDIR)/sleela" "$(DESTDIR)$(BINDIR)/nordshrift"
	rm -rf "$(RUNTIME_DIR)" "$(DATA_DIR)"
	@echo "Configuration under $(CONFIG_DIR) is intentionally preserved; remove it manually if no longer needed."
