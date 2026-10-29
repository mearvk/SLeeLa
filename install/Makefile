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
# The invoking user's home, resolved correctly even under sudo (where $HOME is
# root's). install-all / uninstall-all use this so the user-local layouts under
# ~/.local are always found, whether the target is run as the user or elevated.
USER_HOME := $(shell if [ "$$(id -u)" -eq 0 ] && [ -n "$$SUDO_USER" ]; then getent passwd "$$SUDO_USER" | cut -d: -f6; else printf '%s' "$$HOME"; fi)

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

# When set (e.g. SLEELA_INSTALL_FORCE=1), do-install adopts and overwrites an
# existing *untracked* SLeeLa install under the target prefix (one with binaries
# but no .sleela-install-manifest.json, e.g. from quick-safe-install) instead of
# refusing. install-all sets this so it can refresh every location in one run.
SLEELA_INSTALL_FORCE ?=

# -----------------------------------------------------------------------------
# Module selection for the staged standard library. The installer is BUILT with
# these options rather than asking at install time:
#
#   MODULES=all                 install every lib/ package (default).
#   MODULES="crypto codecs ..."  install only the named packages ("individually"
#                                expressed as data -- the explicit set to keep).
#   EXCLUDE_MODULES="ide wifi"   drop these packages from whatever MODULES chose.
#
# "All" is simply the default (MODULES=all). "Individually" is MODULES set to the
# chosen subset. Selection is non-interactive and is resolved by
# install-runtime.py stage-lib, so every install path (install, install-local,
# install-system, install-all) and any DESTDIR package build honor it.
MODULES ?= all
EXCLUDE_MODULES ?=

# Convenience group: the HTTP / web server library trees. SERVERS=off drops
# every one of these in a single flag instead of naming all of them in
# EXCLUDE_MODULES. SERVERS=on (the default) keeps them. Whatever SERVERS does is
# COMPOSED with any explicit EXCLUDE_MODULES you also pass, via EFFECTIVE_EXCLUDE
# below -- so e.g. `SERVERS=off EXCLUDE_MODULES="ide"` drops the servers AND ide.
SERVERS ?= on
SERVER_MODULES := http http-1.0 http-2.0 http-3.0 http-4.0 http-5.0 http-6.0 \
  http-7.0 http-8.0 http-9.0 http-servers server-edition web-server-technology \
  website
# The exclude set actually handed to the staging step: the user's
# EXCLUDE_MODULES, plus the server group when SERVERS=off.
ifeq ($(SERVERS),off)
  EFFECTIVE_EXCLUDE := $(strip $(EXCLUDE_MODULES) $(SERVER_MODULES))
else
  EFFECTIVE_EXCLUDE := $(strip $(EXCLUDE_MODULES))
endif

.PHONY: all install install-all install-local install-system do-install refresh-extra-bins install-check install-help uninstall uninstall-all remove-extra-bins manifest-prompt

# Extra standard bin directories that may hold a SLeeLa command from a prior or
# packaged install but are not a prefix this Makefile installs a full runtime
# into (e.g. /usr/bin alongside the /usr/local system prefix). install-all
# refreshes any sleela/sleelvac/nordshrift ALREADY present in these dirs so they
# stop going stale, without inventing a new runtime tree for them.
EXTRA_BINDIRS ?= /usr/bin
all: install-check

install-help:
	@echo "SLeeLa installation targets"
	@echo "  make -C install install-local  [PREFIX=\$$HOME/.local]   User-local install; never uses sudo"
	@echo "  make -C install install-system [PREFIX=/usr/local]      System install; uses sudo for system paths only"
	@echo "  make -C install install        [PREFIX=... DESTDIR=...]  Scope-aware: local prefix stays local, system prefix elevates"
	@echo "  make -C install install-all                             Refresh EVERY location: user-local (both layouts) +"
	@echo "                                                          /usr/local + any present EXTRA_BINDIRS (default /usr/bin)"
	@echo "  make -C install uninstall      [PREFIX=/usr/local]      Remove one tracked install (single prefix)"
	@echo "  make -C install uninstall-all                           Remove from EVERY location (mirror of install-all)"
	@echo "  BINDIR, LIBDIR, DATADIR, SYSCONFDIR, EXTRA_BINDIRS may be overridden."
	@echo
	@echo "Module selection (the installer is built with these, not asked at runtime):"
	@echo "  MODULES=all                      install every lib/ package (default)"
	@echo "  MODULES=\"crypto codecs regex\"     install only the named packages"
	@echo "  EXCLUDE_MODULES=\"ide wifi\"        drop these from whatever MODULES selected"
	@echo "  SERVERS=on|off                   on (default) keeps the HTTP/web server trees;"
	@echo "                                   off drops them all in one flag:"
	@echo "                                   http, http-1.0..9.0, http-servers,"
	@echo "                                   server-edition, web-server-technology, website"
	@echo "  Example: make -C install install MODULES=\"crypto codecs compression regex\""
	@echo "  Example: make -C install install SERVERS=off          (everything except servers)"
	@echo "  Example: make -C install install SERVERS=off EXCLUDE_MODULES=\"ide\"  (also drop ide)"
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
				MODULES="$(MODULES)" EXCLUDE_MODULES="$(EFFECTIVE_EXCLUDE)" SERVERS=on \
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
				SYSCONFDIR="$$HOME/.config" \
				MODULES="$(MODULES)" EXCLUDE_MODULES="$(EFFECTIVE_EXCLUDE)" SERVERS=on \
				SLEELA_INSTALL_ELEVATED=1 || exit 1; \
		done; \
	else \
		$(MAKE) --no-print-directory -C "$(CURDIR)" do-install \
			PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" BINDIR="$(BINDIR)" \
			LIBDIR="$(LIBDIR)" DATADIR="$(DATADIR)" SYSCONFDIR="$(SYSCONFDIR)" \
			MODULES="$(MODULES)" EXCLUDE_MODULES="$(EFFECTIVE_EXCLUDE)" SERVERS=on \
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
			SYSCONFDIR="$$HOME/.config" \
			MODULES="$(MODULES)" EXCLUDE_MODULES="$(EFFECTIVE_EXCLUDE)" SERVERS=on \
			SLEELA_INSTALL_ELEVATED=1 || exit 1; \
	done

# install-system: system-wide install via the dedicated sudo script. The script
# runs the manifest prompt as the invoking user first, then elevates only for
# the privileged copy into system locations.
install-system:
	@PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" BINDIR="$(BINDIR)" \
		LIBDIR="$(LIBDIR)" DATADIR="$(DATADIR)" SYSCONFDIR="$(SYSCONFDIR)" \
		SLEELA_MANIFEST_CONSENT="$(SLEELA_MANIFEST_CONSENT)" \
		sh "$(CURDIR)/system-install.sh"

# install-all: refresh EVERY standard SLeeLa install location in one command,
# so a single invocation updates all the directories `whereis sleela` reports.
# It performs, in order:
#   1. the user-local DUAL install ($HOME/.local and $HOME/.local/sleela);
#   2. the system install at /usr/local (via sudo, system paths only);
#   3. an in-place refresh of any stray binaries already present in the
#      EXTRA_BINDIRS (e.g. /usr/bin), so none are left stale.
# Directories are created on first install. No environment variables required.
install-all:
	@uh="$(USER_HOME)"; \
	if [ -z "$$uh" ]; then echo "Could not resolve the user's home directory." >&2; exit 1; fi; \
	echo "== install-all: refreshing every standard SLeeLa location =="; \
	echo "   user home: $$uh (root=$$( [ "$$(id -u)" -eq 0 ] && echo yes || echo no ))"; \
	echo; \
	echo "-- (1/3) user-local layouts $$uh/.local and $$uh/.local/sleela --"; \
	for p in "$$uh/.local" "$$uh/.local/sleela"; do \
		echo "==> installing into $$p"; \
		if [ "$$(id -u)" -eq 0 ] && [ -n "$$SUDO_USER" ]; then RUNAS="sudo -u $$SUDO_USER"; else RUNAS=""; fi; \
		$$RUNAS $(MAKE) --no-print-directory -C "$(CURDIR)" do-install \
			PREFIX="$$p" DESTDIR="" \
			BINDIR="$$p/bin" LIBDIR="$$p/lib" DATADIR="$$p/share" \
			SYSCONFDIR="$$uh/.config" \
			MODULES="$(MODULES)" EXCLUDE_MODULES="$(EFFECTIVE_EXCLUDE)" SERVERS=on \
			SLEELA_INSTALL_ELEVATED=1 SLEELA_INSTALL_FORCE=1 || exit 1; \
	done; \
	echo; \
	echo "-- (2/3) system prefix /usr/local --"; \
	if [ "$$(id -u)" -eq 0 ]; then SUDO=""; else SUDO="sudo"; fi; \
	$$SUDO $(MAKE) --no-print-directory -C "$(CURDIR)" do-install \
		PREFIX=/usr/local DESTDIR="" BINDIR=/usr/local/bin \
		LIBDIR=/usr/local/lib DATADIR=/usr/local/share SYSCONFDIR=/etc \
		MODULES="$(MODULES)" EXCLUDE_MODULES="$(EFFECTIVE_EXCLUDE)" SERVERS=on \
		SLEELA_INSTALL_ELEVATED=1 SLEELA_INSTALL_FORCE=1 || exit 1; \
	echo; \
	echo "-- (3/3) refresh stray binaries in $(EXTRA_BINDIRS) --"; \
	$(MAKE) --no-print-directory -C "$(CURDIR)" refresh-extra-bins || exit 1; \
	echo; \
	echo "install-all complete. Updated user-local (both layouts), /usr/local, and any present $(EXTRA_BINDIRS) copies."; \
	echo "Tip: run ../scripts/which-sleela.sh to confirm which 'sleela' your PATH resolves."

# uninstall-all: the mirror of install-all. Removes the tracked SLeeLa install
# from EVERY standard location -- the user-local dual layout ($HOME/.local and
# $HOME/.local/sleela), the /usr/local system prefix, and any stray binaries in
# EXTRA_BINDIRS -- regardless of whether it is run as the user or under sudo
# (the user's home is resolved via SUDO_USER when elevated). Each location is
# skipped cleanly if it has no tracked install.
uninstall-all:
	@uh="$(USER_HOME)"; \
	if [ -z "$$uh" ]; then echo "Could not resolve the user's home directory." >&2; exit 1; fi; \
	echo "== uninstall-all: removing SLeeLa from every standard location =="; \
	echo "   user home: $$uh"; \
	echo; \
	echo "-- user-local layouts --"; \
	for p in "$$uh/.local" "$$uh/.local/sleela"; do \
		rt="$$p/lib/$(RUNTIME_NAME)"; dt="$$p/share/$(RUNTIME_NAME)"; bd="$$p/bin"; \
		if [ -f "$$rt/.sleela-install-manifest.json" ]; then \
			echo "==> removing install under $$p"; \
			if [ "$$(id -u)" -eq 0 ] && [ -n "$$SUDO_USER" ]; then RUNAS="sudo -u $$SUDO_USER"; else RUNAS=""; fi; \
			$$RUNAS python3 "$(REPO_ROOT)/install/install-runtime.py" uninstall "$$rt" "$$dt" "$$bd" || exit 1; \
		else \
			echo "==> no tracked install under $$p; skipping"; \
		fi; \
	done; \
	echo; \
	echo "-- /usr/local system prefix --"; \
	rt=/usr/local/lib/$(RUNTIME_NAME); dt=/usr/local/share/$(RUNTIME_NAME); bd=/usr/local/bin; \
	if [ -f "$$rt/.sleela-install-manifest.json" ]; then \
		if [ "$$(id -u)" -eq 0 ]; then SUDO=""; else SUDO="sudo"; fi; \
		echo "==> removing install under /usr/local"; \
		$$SUDO python3 "$(REPO_ROOT)/install/install-runtime.py" uninstall "$$rt" "$$dt" "$$bd" || exit 1; \
	else \
		echo "==> no tracked install under /usr/local; skipping"; \
	fi; \
	echo; \
	echo "-- stray binaries in $(EXTRA_BINDIRS) --"; \
	$(MAKE) --no-print-directory -C "$(CURDIR)" remove-extra-bins || exit 1; \
	echo; \
	echo "uninstall-all complete. Configuration dirs are preserved; remove them manually if desired."

# remove-extra-bins: delete sleela/sleelvac/nordshrift from EXTRA_BINDIRS when
# present. These are stray commands with no tracked runtime tree, so they are
# removed directly (with sudo for system-owned dirs). Mirrors refresh-extra-bins.
remove-extra-bins:
	@for d in $(EXTRA_BINDIRS); do \
		for p in sleela sleelvac nordshrift; do \
			if [ -e "$$d/$$p" ]; then \
				if [ "$$(id -u)" -eq 0 ] || [ -w "$$d" ]; then SUDO=""; else SUDO="sudo"; fi; \
				echo "==> removing $$d/$$p"; \
				$$SUDO rm -f "$$d/$$p" || exit 1; \
			else \
				echo "==> $$d/$$p not present; nothing to remove"; \
			fi; \
		done; \
	done

# refresh-extra-bins: overwrite sleela/sleelvac/nordshrift in EXTRA_BINDIRS, but
# ONLY where that command already exists (never create a new stray command).
# Uses sudo for system-owned directories. This keeps e.g. /usr/bin/sleela from
# lingering at an old version after a newer compile.
refresh-extra-bins:
	@test -x "$(REPO_ROOT)/impl/build/sleela" || { echo "Missing impl/build/sleela; run make -C impl all first." >&2; exit 1; }
	@for d in $(EXTRA_BINDIRS); do \
		for p in sleela sleelvac nordshrift; do \
			if [ -e "$$d/$$p" ]; then \
				src="$(REPO_ROOT)/impl/build/$$p"; \
				if [ -w "$$d" ] && [ "$$(id -u)" -eq 0 -o -w "$$d/$$p" ]; then SUDO=""; else SUDO="sudo"; fi; \
				echo "==> refreshing $$d/$$p"; \
				$$SUDO install -m 755 "$$src" "$$d/$$p" || exit 1; \
			else \
				echo "==> $$d/$$p not present; leaving it absent (install-all never creates a new stray command)"; \
			fi; \
		done; \
	done

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
	@if { [ -e "$(DESTDIR)$(BINDIR)/sleela" ] || [ -e "$(DESTDIR)$(BINDIR)/sleelvac" ] || [ -e "$(DESTDIR)$(BINDIR)/nordshrift" ]; } && [ ! -f "$(RUNTIME_DIR)/.sleela-install-manifest.json" ]; then \
		if [ -n "$(SLEELA_INSTALL_FORCE)" ]; then \
			echo "Note: adopting an existing untracked SLeeLa install under $(PREFIX) (SLEELA_INSTALL_FORCE set); it will be overwritten and tracked from now on."; \
		else \
			echo "Refusing to overwrite existing command(s) not tracked as a SLeeLa install. Inspect them, choose another PREFIX, or re-run with SLEELA_INSTALL_FORCE=1 to adopt and overwrite them (install-all does this automatically)." >&2; exit 1; \
		fi; \
	fi
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
	@echo "Staging standard library (MODULES=$(MODULES); SERVERS=$(SERVERS); EXCLUDE=$(EFFECTIVE_EXCLUDE))"
	python3 "$(REPO_ROOT)/install/install-runtime.py" stage-lib "$(REPO_ROOT)" "$(RUNTIME_DIR)/lib" "$(MODULES)" "$(EFFECTIVE_EXCLUDE)"
	install -m 644 "$(REPO_ROOT)/tools/verify-before-execution.py" "$(RUNTIME_DIR)/tools/"
	install -m 644 "$(REPO_ROOT)/security/sha256-manifest.json" "$(RUNTIME_DIR)/security/"
	if [ -f "$(REPO_ROOT)/SHEET.sheet" ]; then install -m 644 "$(REPO_ROOT)/SHEET.sheet" "$(RUNTIME_DIR)/SHEET.sheet"; fi
	python3 "$(REPO_ROOT)/install/install-runtime.py" copy-manifest "$(REPO_ROOT)" "$(RUNTIME_DIR)" "$(MODULES)" "$(EFFECTIVE_EXCLUDE)"
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
