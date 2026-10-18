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
	@test -f install-runtime.py
	@grep -q 'Required compiled product missing' quick-safe-install.sh
	@grep -q 'System-wide' README.md
	@echo "Installer entry points and compiled-product installation checks present for Linux, macOS, and Windows 10+."

install:
	@if [ -z "$(DESTDIR)" ] && [ "$$(id -u)" -ne 0 ] && { \
		case "$(BINDIR) $(LIBDIR) $(DATADIR) $(SYSCONFDIR)" in \
			*/usr/*|*/usr|*/opt/*|*/opt|*/etc/*|*/etc|*/var/*|*/var) true ;; \
			*) false ;; \
		esac; \
	}; then \
		if command -v sudo >/dev/null 2>&1; then \
			echo "System installation requires administrator privileges; requesting sudo for system paths only."; \
			exec sudo $(MAKE) --no-print-directory -C "$(CURDIR)" install \
				PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" BINDIR="$(BINDIR)" \
				LIBDIR="$(LIBDIR)" DATADIR="$(DATADIR)" SYSCONFDIR="$(SYSCONFDIR)" \
				SLEELA_INSTALL_ELEVATED=1; \
		else echo "System installation needs root privileges; install sudo or run this target as root." >&2; exit 1; fi; \
	fi
	@if [ "$$(id -u)" -eq 0 ] && [ -z "$(DESTDIR)" ] && [ -n "$SUDO_USER" ] && { \
		case "$(PREFIX)" in "/home/$SUDO_USER"/*|"/Users/$SUDO_USER"/*) true ;; *) false ;; esac; \
	}; then \
		echo "Refusing to install into a user's home directory as root. Run make install as that user without sudo." >&2; exit 1; \
	fi
	@test -x "$(REPO_ROOT)/impl/build/sleela" || { echo "Missing impl/build/sleela; run make -C impl all first." >&2; exit 1; }
	@test -x "$(REPO_ROOT)/impl/build/sleelvac" || { echo "Missing impl/build/sleelvac; run make -C impl all first." >&2; exit 1; }
	@test -x "$(REPO_ROOT)/impl/build/nordshrift" || { echo "Missing impl/build/nordshrift; run make -C impl all first." >&2; exit 1; }
	@if { [ -e "$(DESTDIR)$(BINDIR)/sleela" ] || [ -e "$(DESTDIR)$(BINDIR)/sleelvac" ] || [ -e "$(DESTDIR)$(BINDIR)/nordshrift" ]; } && [ ! -f "$(RUNTIME_DIR)/.sleela-install-manifest.json" ]; then echo "Refusing to overwrite existing command(s) not tracked as a SLeeLa install. Inspect them or choose another PREFIX." >&2; exit 1; fi
	@echo "Installing SLeeLa to prefix $(PREFIX) (DESTDIR=$(DESTDIR))"
	install -d "$(DESTDIR)$(BINDIR)" "$(RUNTIME_DIR)/lib" "$(RUNTIME_DIR)/tools" "$(RUNTIME_DIR)/security" "$(DATA_DIR)/docs" "$(DATA_DIR)/examples" "$(CONFIG_DIR)"
	install -m 755 "$(REPO_ROOT)/impl/build/sleela" "$(DESTDIR)$(BINDIR)/sleela"
	install -m 755 "$(REPO_ROOT)/impl/build/sleelvac" "$(DESTDIR)$(BINDIR)/sleelvac"
	install -m 755 "$(REPO_ROOT)/impl/build/nordshrift" "$(DESTDIR)$(BINDIR)/nordshrift"
	cp -a "$(REPO_ROOT)/lib/." "$(RUNTIME_DIR)/lib/"
	install -m 644 "$(REPO_ROOT)/tools/verify-before-execution.py" "$(RUNTIME_DIR)/tools/"
	install -m 644 "$(REPO_ROOT)/security/sha256-manifest.json" "$(RUNTIME_DIR)/security/"
	python3 "$(REPO_ROOT)/install/install-runtime.py" copy-manifest "$(REPO_ROOT)" "$(RUNTIME_DIR)"
	cp -a "$(REPO_ROOT)/docs/." "$(DATA_DIR)/docs/"
	if [ -d "$(REPO_ROOT)/examples" ]; then cp -a "$(REPO_ROOT)/examples/." "$(DATA_DIR)/examples/"; fi
	install -m 644 "$(REPO_ROOT)/README.md" "$(DATA_DIR)/README.md"
	if [ ! -e "$(CONFIG_DIR)/startup.conf" ]; then install -m 644 "$(REPO_ROOT)/install/startup.conf.example" "$(CONFIG_DIR)/startup.conf"; fi
	printf '%s\\n' "$(PREFIX)" > "$(RUNTIME_DIR)/install-prefix"
	printf '%s\\n' "$(BINDIR)/sleela" "$(BINDIR)/sleelvac" "$(BINDIR)/nordshrift" > "$(RUNTIME_DIR)/installed-binaries"
	python3 "$(REPO_ROOT)/install/install-runtime.py" record "$(RUNTIME_DIR)" "$(DATA_DIR)" "$(DESTDIR)$(BINDIR)"
	@echo "SLeeLa installed. Commands: $(BINDIR)/sleela, $(BINDIR)/sleelvac, and $(BINDIR)/nordshrift"
	@echo "Runtime: $(RUNTIME_DIR)"
	@echo "Configuration: $(CONFIG_DIR)/startup.conf (existing configuration preserved)"
uninstall:
	@if [ -z "$(DESTDIR)" ] && [ "$$(id -u)" -ne 0 ] && { \
		case "$(BINDIR) $(LIBDIR) $(DATADIR) $(SYSCONFDIR)" in \
			*/usr/*|*/usr|*/opt/*|*/opt|*/var/*|*/var|*/etc/*|*/etc) true ;; \
			*) false ;; \
		esac; \
	}; then \
		if command -v sudo >/dev/null 2>&1; then \
			echo "System uninstall requires administrator privileges; requesting sudo."; \
			exec sudo $(MAKE) --no-print-directory -C "$(CURDIR)" uninstall \
				PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" BINDIR="$(BINDIR)" \
				LIBDIR="$(LIBDIR)" DATADIR="$(DATADIR)" SYSCONFDIR="$(SYSCONFDIR)"; \
		else echo "System uninstall needs root privileges; install sudo or run this target as root." >&2; exit 1; fi; \
	fi
	@echo "Removing tracked SLeeLa files while preserving configuration and untracked data."
	python3 "$(REPO_ROOT)/install/install-runtime.py" uninstall "$(RUNTIME_DIR)" "$(DATA_DIR)" "$(DESTDIR)$(BINDIR)"
	@echo "Configuration under $(CONFIG_DIR) is intentionally preserved; remove it manually if no longer needed."
