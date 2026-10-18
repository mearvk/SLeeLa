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
# Where a reinstall snapshots the previous install before overwriting it. Old
# files are kept here (never deleted automatically) so the user can roll back or
# delete them on their own schedule. Kept OUTSIDE the runtime tree so a snapshot
# is never copied into itself.
BACKUP_ROOT ?= $(DESTDIR)$(LIBDIR)/$(RUNTIME_NAME)-backups

# Non-interactive consent for the base-code SHA-256 manifest step. Set
# SLEELA_MANIFEST_CONSENT=yes|no|skip to answer the prompt without a TTY (CI,
# package builds). When unset and a terminal is attached, the user is asked.
SLEELA_MANIFEST_CONSENT ?=

.PHONY: all install install-local install-system do-install install-check install-help uninstall manifest-prompt
all: install-check

install-help:
	@echo "SLeeLa installation targets"
	@echo "  make -C install install-local  [PREFIX=\$$HOME/.local]   User-local install; never uses sudo"
	@echo "  make -C install install-system [PREFIX=/usr/local]      System install; uses sudo for system paths only"
	@echo "  make -C install install        [PREFIX=... DESTDIR=...]  Scope-aware: local prefix stays local, system prefix elevates"
	@echo "  make -C install uninstall      [PREFIX=/usr/local]"
	@echo "  BINDIR, LIBDIR, DATADIR, SYSCONFDIR may be overridden."
	@echo
	@echo "Every install variant first offers to (re)generate and verify the base"
	@echo "SLeeLa code SHA-256 manifest (yes/no). Answer non-interactively with"
	@echo "SLEELA_MANIFEST_CONSENT=yes|no|skip."

# manifest-prompt: before any install variant, offer to (re)generate and verify
# the trusted base-code SHA-256 manifest. This is the integrity baseline the
# runtime's execution gate checks against, so the operator decides -- per
# install -- whether to refresh it from the current source tree. Declining
# keeps the committed manifest as-is; a subsequent verify still runs so a
# tampered tree is caught. The prompt is a plain yes/no; non-interactive callers
# set SLEELA_MANIFEST_CONSENT.
manifest-prompt:
	@consent="$(SLEELA_MANIFEST_CONSENT)"; \
	if [ -z "$$consent" ]; then \
	  if [ -t 0 ]; then \
	    printf 'Regenerate and verify the base SLeeLa code SHA-256 manifest before installing? [yes/no]: '; \
	    read consent || consent=no; \
	  else \
	    consent=skip; \
	    echo "No terminal for the SHA-256 manifest prompt; skipping regeneration (set SLEELA_MANIFEST_CONSENT=yes to force)."; \
	  fi; \
	fi; \
	case "$$consent" in \
	  y|Y|yes|YES|Yes) \
	    echo "Regenerating the base-code SHA-256 manifest from the current source tree..."; \
	    $(MAKE) --no-print-directory -C "$(REPO_ROOT)/impl" regen-manifest && \
	    echo "Verifying the regenerated manifest against the source tree..." && \
	    $(MAKE) --no-print-directory -C "$(REPO_ROOT)/impl" verify-security ;; \
	  n|N|no|NO|No) \
	    echo "Keeping the committed SHA-256 manifest; verifying the source tree against it..."; \
	    $(MAKE) --no-print-directory -C "$(REPO_ROOT)/impl" verify-security ;; \
	  skip|SKIP|Skip) \
	    echo "Skipping the SHA-256 manifest step for this install." ;; \
	  *) echo "Unrecognized answer '$$consent'; expected yes, no, or skip." >&2; exit 2 ;; \
	esac

install-check:
	@test -f quick-safe-install.sh
	@test -f quick-safe-install-macos.sh
	@test -f quick-safe-install.ps1
	@test -f quick-safe-install.cmd
	@test -f install-runtime.py
	@test -f system-install.sh
	@grep -q 'Required compiled product missing' quick-safe-install.sh
	@grep -q 'System-wide' README.md
	@echo "Installer entry points and compiled-product installation checks present for Linux, macOS, and Windows 10+."

# install: scope-aware entry point. Offers the SHA-256 manifest step, then
# installs. A user-local prefix stays as the current user; a system prefix with
# no DESTDIR elevates via sudo (system paths only) and re-enters so the manifest
# prompt has already run as the invoking user.
#
# User-local installs (no explicit PREFIX, not a system path) go to BOTH
# conventional layouts so the commands are present wherever a user expects them,
# with no environment variables required on a first install:
#   * $HOME/.local          (FHS-style: $HOME/.local/bin, $HOME/.local/lib/sleela)
#   * $HOME/.local/sleela    (self-contained: $HOME/.local/sleela/bin, .../lib/sleela)
# Every target directory is created on first install (install -d). A system
# prefix, an explicit PREFIX=, or a DESTDIR staged build installs to that single
# prefix only (unchanged behavior).
install:
	@if [ "$(origin SLEELA_INSTALL_ELEVATED)" = "undefined" ] || [ -z "$(SLEELA_INSTALL_ELEVATED)" ]; then \
		$(MAKE) --no-print-directory -C "$(CURDIR)" manifest-prompt \
			SLEELA_MANIFEST_CONSENT="$(SLEELA_MANIFEST_CONSENT)"; \
	fi
	@if [ -z "$(DESTDIR)" ] && [ "$$(id -u)" -ne 0 ] && { \
		case "$(BINDIR) $(LIBDIR) $(DATADIR) $(SYSCONFDIR)" in \
			*/usr/*|*/usr|*/opt/*|*/opt|*/etc/*|*/etc|*/var/*|*/var) true ;; \
			*) false ;; \
		esac; \
	}; then \
		if command -v sudo >/dev/null 2>&1; then \
			echo "System installation requires administrator privileges; requesting sudo for system paths only."; \
			echo "Note: sudo is used ONLY to copy SLeeLa into system locations as the trusted"; \
			echo "      system account. Your own files keep their current owner, group and"; \
			echo "      permissions -- this install does not chown, rewrite, or take ownership"; \
			echo "      of any file you already own; it only writes new files under the system"; \
			echo "      prefix ($(PREFIX))."; \
			exec sudo $(MAKE) --no-print-directory -C "$(CURDIR)" do-install \
				PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" BINDIR="$(BINDIR)" \
				LIBDIR="$(LIBDIR)" DATADIR="$(DATADIR)" SYSCONFDIR="$(SYSCONFDIR)" \
				SLEELA_INSTALL_ELEVATED=1; \
		else echo "System installation needs root privileges; install sudo or run this target as root." >&2; exit 1; fi; \
	fi
	@if [ -z "$(DESTDIR)" ] && [ "$(PREFIX)" = "/usr/local" ] && [ "$$(id -u)" -ne 0 ]; then \
		base="$$HOME/.local"; \
		echo "User-local install: installing to both $$base and $$base/sleela (no PREFIX given)."; \
		for prefix in "$$base" "$$base/sleela"; do \
			echo "==> installing into $$prefix"; \
			$(MAKE) --no-print-directory -C "$(CURDIR)" do-install \
				PREFIX="$$prefix" DESTDIR="$(DESTDIR)" \
				BINDIR="$$prefix/bin" LIBDIR="$$prefix/lib" DATADIR="$$prefix/share" \
				SYSCONFDIR="$$HOME/.config" SLEELA_INSTALL_ELEVATED=1 || exit 1; \
		done; \
	else \
		$(MAKE) --no-print-directory -C "$(CURDIR)" do-install \
			PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" BINDIR="$(BINDIR)" \
			LIBDIR="$(LIBDIR)" DATADIR="$(DATADIR)" SYSCONFDIR="$(SYSCONFDIR)" \
			SLEELA_INSTALL_ELEVATED="$(SLEELA_INSTALL_ELEVATED)"; \
	fi

# install-local: user-local install that NEVER uses sudo. Defaults the prefix to
# the user's ~/.local when the default system prefix is in effect, and refuses a
# system prefix outright (directing the user to install-system) so a local
# install can never silently try to elevate.
install-local:
	@prefix="$(PREFIX)"; dual=no; \
	if [ "$$prefix" = "/usr/local" ]; then prefix="$$HOME/.local"; dual=yes; \
		echo "User-local install: defaulting PREFIX to $$prefix (override with PREFIX=...)."; fi; \
	case "$$prefix" in \
		/usr/*|/usr|/opt/*|/opt|/etc/*|/etc|/var/*|/var) \
			echo "PREFIX '$$prefix' is a system location; use 'make install-system' for that." >&2; exit 1 ;; \
	esac; \
	if [ "$$(id -u)" -eq 0 ]; then \
		echo "install-local must not run as root; run it as the target user without sudo." >&2; exit 1; fi; \
	$(MAKE) --no-print-directory -C "$(CURDIR)" manifest-prompt \
		SLEELA_MANIFEST_CONSENT="$(SLEELA_MANIFEST_CONSENT)" || exit 1; \
	if [ "$$dual" = yes ]; then \
		set -- "$$prefix" "$$prefix/sleela"; \
		echo "User-local install: installing to both $$prefix and $$prefix/sleela (no PREFIX given)."; \
	else \
		set -- "$$prefix"; \
	fi; \
	for p in "$$@"; do \
		echo "==> installing into $$p"; \
		$(MAKE) --no-print-directory -C "$(CURDIR)" do-install \
			PREFIX="$$p" DESTDIR="$(DESTDIR)" \
			BINDIR="$$p/bin" LIBDIR="$$p/lib" DATADIR="$$p/share" \
			SYSCONFDIR="$$HOME/.config" SLEELA_INSTALL_ELEVATED=1 || exit 1; \
	done

# install-system: system-wide install via the dedicated sudo script. The script
# runs the manifest prompt as the invoking user first, then elevates only for
# the privileged copy into system locations.
install-system:
	@PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" BINDIR="$(BINDIR)" \
		LIBDIR="$(LIBDIR)" DATADIR="$(DATADIR)" SYSCONFDIR="$(SYSCONFDIR)" \
		SLEELA_MANIFEST_CONSENT="$(SLEELA_MANIFEST_CONSENT)" \
		sh "$(CURDIR)/system-install.sh"

# do-install: the actual copy work. No prompting and no elevation decision --
# callers (install / install-local / install-system) have already handled scope,
# consent and privileges. Runs under whatever account invoked it.
do-install:
	@if [ "$$(id -u)" -eq 0 ] && [ -z "$(DESTDIR)" ] && [ -n "$$SUDO_USER" ] && { \
		case "$(PREFIX)" in "/home/$$SUDO_USER"/*|"/Users/$$SUDO_USER"/*) true ;; *) false ;; esac; \
	}; then \
		echo "Refusing to install into a user's home directory as root. Run make install-local as that user without sudo." >&2; exit 1; \
	fi
	@test -x "$(REPO_ROOT)/impl/build/sleela" || { echo "Missing impl/build/sleela; run make -C impl all first." >&2; exit 1; }
	@test -x "$(REPO_ROOT)/impl/build/sleelvac" || { echo "Missing impl/build/sleelvac; run make -C impl all first." >&2; exit 1; }
	@test -x "$(REPO_ROOT)/impl/build/nordshrift" || { echo "Missing impl/build/nordshrift; run make -C impl all first." >&2; exit 1; }
	@if { [ -e "$(DESTDIR)$(BINDIR)/sleela" ] || [ -e "$(DESTDIR)$(BINDIR)/sleelvac" ] || [ -e "$(DESTDIR)$(BINDIR)/nordshrift" ]; } && [ ! -f "$(RUNTIME_DIR)/.sleela-install-manifest.json" ]; then echo "Refusing to overwrite existing command(s) not tracked as a SLeeLa install. Inspect them or choose another PREFIX." >&2; exit 1; fi
	@echo "Installing SLeeLa to prefix $(PREFIX) (DESTDIR=$(DESTDIR))"
	@if [ -z "$(DESTDIR)" ]; then \
		python3 "$(REPO_ROOT)/install/install-runtime.py" backup "$(RUNTIME_DIR)" "$(DATA_DIR)" "$(DESTDIR)$(BINDIR)" "$(BACKUP_ROOT)"; \
	else \
		echo "Staged (DESTDIR) install into a fresh package root; skipping backup of the live system."; \
	fi
	install -d "$(DESTDIR)$(BINDIR)" "$(RUNTIME_DIR)/lib" "$(RUNTIME_DIR)/tools" "$(RUNTIME_DIR)/security" "$(DATA_DIR)/docs" "$(DATA_DIR)/examples" "$(CONFIG_DIR)"
	install -m 755 "$(REPO_ROOT)/impl/build/sleela" "$(DESTDIR)$(BINDIR)/sleela"
	install -m 755 "$(REPO_ROOT)/impl/build/sleelvac" "$(DESTDIR)$(BINDIR)/sleelvac"
	install -m 755 "$(REPO_ROOT)/impl/build/nordshrift" "$(DESTDIR)$(BINDIR)/nordshrift"
	cp -a "$(REPO_ROOT)/lib/." "$(RUNTIME_DIR)/lib/"
	install -m 644 "$(REPO_ROOT)/tools/verify-before-execution.py" "$(RUNTIME_DIR)/tools/"
	install -m 644 "$(REPO_ROOT)/security/sha256-manifest.json" "$(RUNTIME_DIR)/security/"
	if [ -f "$(REPO_ROOT)/SHEET.sheet" ]; then install -m 644 "$(REPO_ROOT)/SHEET.sheet" "$(RUNTIME_DIR)/SHEET.sheet"; fi
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
	@if [ "$$(id -u)" -eq 0 ] && [ -z "$(DESTDIR)" ] && { \
		case "$(PREFIX)" in /usr/*|/usr|/opt/*|/opt|/etc/*|/etc|/var/*|/var) true ;; *) false ;; esac; \
	}; then \
		echo; \
		echo "Privilege note: this system install ran as the trusted system account only"; \
		echo "to place new files under $(PREFIX). It did not change the ownership, group,"; \
		echo "or permissions of any file you already owned, and it did not modify files in"; \
		echo "your home directory. The newly written system files are owned by the system"; \
		echo "account (root), as is conventional for $(PREFIX); your own files are untouched."; \
	fi
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
	@if [ -z "$(DESTDIR)" ] && [ "$(PREFIX)" = "/usr/local" ] && [ "$$(id -u)" -ne 0 ]; then \
		base="$$HOME/.local"; \
		echo "User-local uninstall: removing from both $$base and $$base/sleela (no PREFIX given)."; \
		for prefix in "$$base" "$$base/sleela"; do \
			rt="$$prefix/lib/$(RUNTIME_NAME)"; dt="$$prefix/share/$(RUNTIME_NAME)"; bd="$$prefix/bin"; \
			if [ -f "$$rt/.sleela-install-manifest.json" ]; then \
				echo "==> removing install under $$prefix"; \
				python3 "$(REPO_ROOT)/install/install-runtime.py" uninstall "$$rt" "$$dt" "$$bd" || exit 1; \
			else \
				echo "==> no tracked install under $$prefix; skipping"; \
			fi; \
		done; \
	else \
		python3 "$(REPO_ROOT)/install/install-runtime.py" uninstall "$(RUNTIME_DIR)" "$(DATA_DIR)" "$(DESTDIR)$(BINDIR)"; \
	fi
	@echo "Configuration under $(CONFIG_DIR) is intentionally preserved; remove it manually if no longer needed."
