# SLeeLa repository build dispatcher
# Max Rupplin - MEARVK LLC - 2026
# Product-specific Makefiles remain authoritative. This root dispatcher only
# enters those existing build systems; it does not duplicate their source lists.

.PHONY: all build uninstall uninstall-all autocheck core java28 regex compiler decompiler vm scripting jetbrains connector connector-test connector-check-deps codecs codecs-test churn churn-test churn-data-check antivirus antivirus-test antivirus-clamav antivirus-integrity antivirus-integrity-check ballum ballum-test ballum-deps ballum-check-deps ballum-install ballum-uninstall ballum-clean lib-ballum lib-ballum-check lib-ballum-install install install-all install-local install-system install-check install-product clean-install advanced-build tutorial-check tests server config route common-rails registry premium-vms clean verify-security help guess install-guide

SLEELA_MANIFEST_CONSENT ?=
# Answer the "install the advanced (beautified/guided) build?" prompt
# non-interactively: yes runs install/sleela-clean-install.sh, no/skip keeps
# the plain make-driven build. Empty + a TTY prompts; empty + no TTY skips.
SLEELA_ADVANCED_BUILD ?=

PREFIX ?= /usr/local
DESTDIR ?=

all: core java28 regex compiler decompiler vm scripting jetbrains install-check tutorial-check tests config route common-rails codecs churn antivirus registry premium-vms ballum

# `build` is the human-facing entry point: it performs the full `make all`
# build and THEN offers, with a yes/no prompt, to simply install the advanced
# (beautified/guided) build instead of leaving you with only the plain,
# make-driven artifacts. `all` itself stays non-interactive so CI is unaffected.
# Answer non-interactively with SLEELA_ADVANCED_BUILD=yes|no (or skip); with no
# terminal the prompt is skipped and the plain build stands.
build: all advanced-build

# advanced-build: the yes/no gate. On yes it runs the calm, guided installer
# (install/sleela-clean-install.sh) which itself asks about scope and the fuller
# module groups; on no/skip (or no TTY) it does nothing and the plain `make`
# build you just produced is what you keep. TTY/env-aware, mirroring
# install/Makefile::manifest-prompt so automated builds never block.
advanced-build:
	@choice="$(SLEELA_ADVANCED_BUILD)"; \
	if [ -z "$$choice" ]; then \
	  if [ -t 0 ]; then \
	    printf 'Install the advanced (beautified/guided) build now instead of the plain make output? [yes/no]: '; \
	    read choice || choice=no; \
	  else \
	    choice=skip; \
	    echo "No terminal for the advanced-build prompt; keeping the plain make build (set SLEELA_ADVANCED_BUILD=yes to run the guided installer)."; \
	  fi; \
	fi; \
	case "$$choice" in \
	  y|Y|yes|YES|Yes) \
	    echo "Launching the SLeeLa advanced (guided) build..."; \
	    ./install/sleela-clean-install.sh ;; \
	  n|N|no|NO|No|skip|SKIP|Skip) \
	    echo "Keeping the plain make build; run 'make clean-install' later for the guided install." ;; \
	  *) echo "Unrecognized answer '$$choice'; expected yes, no, or skip." >&2; exit 2 ;; \
	esac

# Anchor verification to the directory containing this root Makefile, rather
# than an installed runtime tree such as ~/.local/sleela. The manifest remains
# overridable with SLEELA_SHA256_MANIFEST for deliberate trusted-manifest use.
SLEELA_REPO_ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
SLEELA_SHA256_MANIFEST ?= $(SLEELA_REPO_ROOT)/security/sha256-manifest.json
SLEELA_VERIFY_TOOL := $(SLEELA_REPO_ROOT)/tools/verify-before-execution.py
export SLEELA_SHA256_MANIFEST
# Anchor the COMPILER's own built-in SHA-256 gate to this checkout too. Child
# stages invoke `$(SLEELA) check`/`run` (common-rails, the lib example science
# harnesses, ...); that binary resolves its own verification root via
# findRepoRoot(), which honors $SLEELA_HOME FIRST, then falls back to the
# binary's installed layout (e.g. ~/.local/sleela). Without this export, a
# freshly built checkout would be verified against a STALE installed tree,
# producing "missing file"/"SHA-256 mismatch" failures for files the checkout
# added or changed since that install. Exporting SLEELA_HOME here makes the
# gate verify the checkout the manifest actually describes.
#
# This is an unconditional ':=' ON PURPOSE: a stale SLEELA_HOME inherited from
# the environment (the installer writes `export SLEELA_HOME=~/.local/sleela`
# into /etc/profile.d/sleela.sh and the per-user profile) must NOT hijack a
# build of THIS source tree -- when you build this checkout, the only correct
# verification root is this checkout. A '?=' here would let the inherited
# install path win and reintroduce the stale-tree "missing file" failures.
# To verify against a different trusted root, build from that root (or run the
# verifier directly with --root), rather than relying on the ambient variable.
SLEELA_HOME := $(SLEELA_REPO_ROOT)
export SLEELA_HOME

verify-security:
	@test -f "$(SLEELA_SHA256_MANIFEST)" || { echo "SLeeLa verification manifest not found: $(SLEELA_SHA256_MANIFEST)" >&2; exit 1; }
	@test -r "$(SLEELA_VERIFY_TOOL)" || { echo "SLeeLa verification tool not readable: $(SLEELA_VERIFY_TOOL)" >&2; exit 1; }
	python3 "$(SLEELA_VERIFY_TOOL)" --manifest "$(SLEELA_SHA256_MANIFEST)" --root "$(SLEELA_REPO_ROOT)"

autocheck:
	@sh ./scripts/autocheck-toolchain.sh

core: autocheck verify-security
	$(MAKE) -C impl all

java28:
	$(MAKE) -C java28 all

regex:
	$(MAKE) -C regex all

compiler:
	$(MAKE) -C lib/compiler all

decompiler:
	$(MAKE) -C lib/decompiler all

vm:
	$(MAKE) -C lib/vm all

scripting:
	$(MAKE) -C sleela-scripting all

jetbrains:
	$(MAKE) -C jetbrains all

# Optional native connectors have third-party client-library dependencies.
# Keep them out of the default build; invoke explicitly when those dependencies are installed.
connector:
	$(MAKE) -C lib/connector all

connector-test:
	$(MAKE) -C lib/connector test

connector-check-deps:
	$(MAKE) -C lib/connector check-deps

# Audio codec registry/plugins use the codecs package's own build and test rules.
codecs:
	$(MAKE) -C codecs all

codecs-test:
	$(MAKE) -C codecs test

# Churn data/model sets and their Java constraint-report tools.
churn:
	$(MAKE) -C churn all

churn-test:
	$(MAKE) -C churn test

churn-data-check:
	$(MAKE) -C churn data-check

# Sleelavirin's portable C core and self-tests (no third-party engine required).
antivirus:
	$(MAKE) -C antivirus all

antivirus-test:
	$(MAKE) -C antivirus test

antivirus-clamav:
	$(MAKE) -C antivirus clamav

antivirus-integrity:
	$(MAKE) -C antivirus integrity-manifest

antivirus-integrity-check:
	@test -n "$(MANIFEST)" || { echo "Usage: make antivirus-integrity-check MANIFEST=build/INTEGRITY-MANIFEST.json" >&2; exit 2; }
	$(MAKE) -C antivirus integrity-check MANIFEST="$(MANIFEST)"

# Ballum(TM) pixel-level display control. The native library links OS-centric
# display libraries (Linux X11+Xrandr; macOS CoreGraphics/ApplicationServices/
# Foundation; Windows gdi32/user32), provisioned/reported by the ballum/
# Makefile's own `deps`/`check-deps` targets. The top-level `ballum` target
# attempts the real per-OS build; if the OS development libraries are missing it
# falls back to the always-portable test rather than failing `make all` (mirror
# of how optional native families are kept non-fatal). Build the real OS library
# explicitly with `make ballum` on a host whose deps are present, or run
# `make ballum-deps` first.
ballum:
	@if $(MAKE) -C ballum check-deps >/dev/null 2>&1; then \
	  $(MAKE) -C ballum all; \
	else \
	  echo "ballum: OS display libraries missing -- building the portable test only."; \
	  echo "        run 'make ballum-deps' (host package manager) then 'make ballum' for the real OS library."; \
	  $(MAKE) -C ballum test; \
	fi

ballum-test:
	$(MAKE) -C ballum test

ballum-check-deps:
	$(MAKE) -C ballum check-deps

# Provision this OS's Ballum display libraries. Reports the install command by
# default; pass BALLUM_DEPS_INSTALL=1 to actually install (may use sudo on Linux).
ballum-deps:
	$(MAKE) -C ballum deps BALLUM_DEPS_INSTALL="$(BALLUM_DEPS_INSTALL)"

# Install the native Ballum library + headers under $(PREFIX).
ballum-install:
	$(MAKE) -C ballum install PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)"

ballum-uninstall:
	$(MAKE) -C ballum uninstall PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)"

ballum-clean:
	$(MAKE) -C ballum clean

# The SLeeLa-facing Ballum library (lib/ballum): validates the .sleela sources
# with the compiler and dispatches the native build/install/clean below it.
lib-ballum:
	$(MAKE) -C lib/ballum all
lib-ballum-check:
	$(MAKE) -C lib/ballum check
lib-ballum-install:
	$(MAKE) -C lib/ballum install PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)"

install-check:
	$(MAKE) -C install install-check

# Conventional installation. Scope-aware: a user-local PREFIX stays as the
# current user; a system PREFIX (with no DESTDIR) elevates via sudo for the
# system copy only. Every variant first offers to (re)generate and verify the
# base-code SHA-256 manifest (yes/no; answer non-interactively with
# SLEELA_MANIFEST_CONSENT=yes|no|skip).
install: core
	$(MAKE) -C install install PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" \
		SLEELA_MANIFEST_CONSENT="$(SLEELA_MANIFEST_CONSENT)"

# User-local install that never uses sudo (defaults PREFIX to $HOME/.local).
install-local: core
	$(MAKE) -C install install-local PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" \
		SLEELA_MANIFEST_CONSENT="$(SLEELA_MANIFEST_CONSENT)"

# System-wide install via the dedicated sudo script (system paths only).
install-system: core
	$(MAKE) -C install install-system PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)" \
		SLEELA_MANIFEST_CONSENT="$(SLEELA_MANIFEST_CONSENT)"

# Refresh EVERY standard location in one command: user-local (both layouts),
# /usr/local, and any stray binaries in EXTRA_BINDIRS (default /usr/bin).
install-all: core
	$(MAKE) -C install install-all SLEELA_MANIFEST_CONSENT="$(SLEELA_MANIFEST_CONSENT)"

# Remove files recorded by the conventional installer for this PREFIX.
# The installer removes only tracked SLeeLa files and preserves configuration.
uninstall:
	$(MAKE) -C install uninstall PREFIX="$(PREFIX)" DESTDIR="$(DESTDIR)"

# Remove SLeeLa from EVERY standard location (mirror of install-all).
uninstall-all:
	$(MAKE) -C install uninstall-all

# Interactive deployment of the compiled binaries; choose user-local or system-wide.
install-product: autocheck
	@./install/quick-safe-install.sh

# Clean, guided install of the main package only (binaries + standard library +
# protocols + support modules; no servers, IDEs, Wi-Fi, or religious/moral
# systems). One primary decision, then optional enhancement modules.
clean-install: autocheck
	@./install/sleela-clean-install.sh

tutorial-check:
	@./scripts/tutorial-check.sh

tests:
	$(MAKE) -C tests check

server:
	$(MAKE) -C api/server

route:
	@python3 -m json.tool route/ROUTE.DATA.json >/dev/null
	@echo "SLeeLa unified route data: route/ROUTE.DATA.json"

config:
	@make -C config test

common-rails:
	$(MAKE) -C common-rails check

# Build the registry core and materialize the baseline registry table (text
# store + MySQL export) a fresh install reviews. Part of `all`.
registry:
	./build-registry.sh

# Build the eleven premium SLVM generations as real runnable VMs. Each links
# the shared native Core engine archive (impl/build/libslcore.a, produced by
# `core`) and runs compiled .sleela artifacts; `sleela vm run premium <n> <prog>`
# boots the chosen generation. Depends on `core` for the Core archive.
premium-vms: core
	@for n in 001 002 003 004 005 006 007 008 009 010 011; do \
	  echo "=== premium SLVM $$n ==="; \
	  $(MAKE) -C sleela-virtual-machine/$$n all || exit 1; \
	done
	@echo "SLeeLa premium VMs built: sleela-virtual-machine/0NN/vm-build/slvm-NN"

clean:
	@for n in 001 002 003 004 005 006 007 008 009 010 011; do $(MAKE) -C sleela-virtual-machine/$$n clean; done
	$(MAKE) -C registry clean
	$(MAKE) -C impl clean
	$(MAKE) -C java28 clean
	$(MAKE) -C regex clean
	$(MAKE) -C lib/compiler clean
	$(MAKE) -C lib/decompiler clean
	$(MAKE) -C lib/vm clean
	$(MAKE) -C tests clean
	$(MAKE) -C codecs clean
	$(MAKE) -C churn clean
	$(MAKE) -C antivirus clean
	$(MAKE) -C ballum clean
	$(MAKE) -C lib/ballum clean
	@make -C config clean

# guess: show the build order. A plain-language walk-through for first timers
# ("what runs, in what order, and why") followed by the raw target graph for
# experts. Pure echo target -- it has no object-file prerequisites, so it works
# before anything is built and never trips the SHA-256 verification gate.
guess:
	@echo "SLeeLa build order -- 'make guess'"
	@echo "=================================================================="
	@echo
	@echo "THE ONE RULE THAT SHAPES EVERYTHING"
	@echo "------------------------------------------------------------------"
	@echo "  Nothing native compiles until the checkout passes its integrity"
	@echo "  gate. 'core' depends on 'autocheck' then 'verify-security', and"
	@echo "  verify-security is an order-only prerequisite of every object"
	@echo "  file. A stale security/sha256-manifest.json stops the build here"
	@echo "  BEFORE any compile. Regenerate it with:"
	@echo "      python3 tools/generate-sha256-manifest.py --root . \\"
	@echo "              --output security/sha256-manifest.json"
	@echo "      (cd impl && make check-manifest)   # should print OK"
	@echo
	@echo "FIRST TIMER -- the short path to a working compiler"
	@echo "------------------------------------------------------------------"
	@echo "  1) make core          Build just the native C/C++ runtime."
	@echo "                        Internally this runs, in order:"
	@echo "                          autocheck        toolchain + assets present"
	@echo "                          verify-security  SHA-256 gate (fail-closed)"
	@echo "                          make -C impl all compile -> link"
	@echo "                                           impl/build/sleela"
	@echo "  2) impl/build/sleela version    Confirm it runs."
	@echo "  3) make install-local           Install under \$$HOME/.local"
	@echo "                                  (never uses sudo)."
	@echo "  For the guided, narrated installation, run:  make install-guide"
	@echo
	@echo "EXPERT -- the full 'make all' order (left to right)"
	@echo "------------------------------------------------------------------"
	@echo "  core -> java28 -> regex -> compiler -> decompiler -> vm ->"
	@echo "  scripting -> jetbrains -> install-check -> tutorial-check ->"
	@echo "  tests -> config -> route -> common-rails -> codecs -> churn ->"
	@echo "  antivirus -> registry -> premium-vms -> ballum"
	@echo
	@echo "  Key dependency edges (prerequisite -> dependent):"
	@echo "    autocheck, verify-security  ->  core"
	@echo "    core                        ->  install / install-local /"
	@echo "                                    install-system / install-all"
	@echo "    core                        ->  premium-vms (links the Core"
	@echo "                                    archive impl/build/libslcore.a)"
	@echo "    verify-security (order-only) ->  every impl object file"
	@echo
	@echo "  What each stage produces:"
	@echo "    core         native sleela/sleelvac/nordshrift + libslcore.a"
	@echo "    java28       Java 28 runtime bridge pieces"
	@echo "    regex        regex engine + its test suites"
	@echo "    compiler     lib/compiler"
	@echo "    decompiler   lib/decompiler"
	@echo "    vm           lib/vm"
	@echo "    premium-vms  the 11 premium SLVM generations (need core first)"
	@echo "    ballum       native display library; falls back to the portable"
	@echo "                 test when OS display libs are missing (non-fatal)"
	@echo
	@echo "  Resolve the real recipe graph yourself with GNU make's own output:"
	@echo "      make -n all        dry run: print the commands in order"
	@echo "      make -pn all | less   full database (rules + prerequisites)"
	@echo
	@echo "  See also:  make help           every target, one line each"
	@echo "             make install-guide  installation explained as a process"

# install-guide: installation as a readable process, start to finish. This is
# the narrative companion to 'make help' (which is the flat target reference).
# Pure echo target; safe to run at any time.
install-guide:
	@echo "SLeeLa installation -- the process, end to end"
	@echo "=================================================================="
	@echo
	@echo "WHAT 'INSTALL' ACTUALLY DOES"
	@echo "------------------------------------------------------------------"
	@echo "  Every install variant is the same three movements:"
	@echo "    (A) BUILD     'install' depends on 'core', so the native"
	@echo "                  products are built (and the integrity gate"
	@echo "                  passes) before anything is copied."
	@echo "    (B) INTEGRITY A yes/no prompt offers to regenerate + verify the"
	@echo "                  base-code SHA-256 manifest. Decline and it still"
	@echo "                  VERIFIES the tree against the committed manifest,"
	@echo "                  so tampering is always caught. Answer without a"
	@echo "                  terminal using SLEELA_MANIFEST_CONSENT=yes|no|skip."
	@echo "    (C) INSTALL   Copy binaries + the selected standard library to"
	@echo "                  the destination prefix."
	@echo
	@echo "CHOOSE A SCOPE"
	@echo "------------------------------------------------------------------"
	@echo "  make install-local    User-local. Never uses sudo. Installs to"
	@echo "                        BOTH \$$HOME/.local and \$$HOME/.local/sleela"
	@echo "                        so the commands are found with no env setup."
	@echo "  make install-system   System-wide under /usr/local. Elevates with"
	@echo "                        sudo for the system paths only."
	@echo "  make install          Scope-aware: a user PREFIX stays local; a"
	@echo "                        system PREFIX (no DESTDIR) elevates."
	@echo "  make install-all      Refresh every standard location at once."
	@echo
	@echo "TUNE WHAT GETS INSTALLED  (passed at build time, not asked at runtime)"
	@echo "------------------------------------------------------------------"
	@echo "  PREFIX=<dir>                 destination root (e.g. /opt/sleela)"
	@echo "  DESTDIR=<dir>                staged/packaging root (no elevation)"
	@echo "  MODULES=all                  install every lib/ package (default)"
	@echo "  MODULES=\"crypto codecs regex\"  install only the named packages"
	@echo "  EXCLUDE_MODULES=\"ide wifi\"     drop these from whatever MODULES chose"
	@echo "  SERVERS=on|off               off drops all HTTP/web-server trees"
	@echo "  BINDIR/LIBDIR/DATADIR/SYSCONFDIR  override individual layout dirs"
	@echo
	@echo "GUIDED INSTALLERS (interactive, narrated)"
	@echo "------------------------------------------------------------------"
	@echo "  make clean-install    Guided install of the MAIN package only"
	@echo "                        (binaries + standard library + protocols;"
	@echo "                        no servers, IDEs, Wi-Fi, or moral systems),"
	@echo "                        then optional enhancement modules."
	@echo "  make install-product  Build, then interactively choose user-local"
	@echo "                        or system-wide for the compiled products."
	@echo
	@echo "A TYPICAL FIRST INSTALL"
	@echo "------------------------------------------------------------------"
	@echo "  make core                               # build + pass the gate"
	@echo "  make install-local SLEELA_MANIFEST_CONSENT=yes"
	@echo "  \$$HOME/.local/bin/sleela version          # confirm it works"
	@echo
	@echo "VERIFY / BACK OUT"
	@echo "------------------------------------------------------------------"
	@echo "  make install-check    Confirm the installer entry points exist."
	@echo "  make uninstall [PREFIX=...]   Remove one tracked install; config"
	@echo "                                files are preserved."
	@echo "  make uninstall-all            Remove from every standard location."
	@echo
	@echo "  Per-target detail:  make -C install install-help"
	@echo "  Flat target list:   make help"

help:
	@echo "SLeeLa repository build dispatcher"
	@echo "  make all       Build core, Java 28, regex, compiler, decompiler, VM, codecs, and tests"
	@echo "  make build     Run 'make all', then ask yes/no to install the advanced (guided) build"
	@echo "                 (SLEELA_ADVANCED_BUILD=yes|no answers it non-interactively)"
	@echo "  make guess     Show the build order (first-timer path + expert target graph)"
	@echo "  make install-guide  Installation explained as a process (companion to this list)"
	@echo "  make verify-security Verify the checkout rooted at this Makefile against its SHA-256 manifest"
	@echo "  make core      Build impl/"
	@echo "  make java28    Build java28/"
	@echo "  make regex     Build regex/ and its test suites"
	@echo "  make compiler  Build lib/compiler/"
	@echo "  make decompiler Build lib/decompiler/"
	@echo "  make vm        Build lib/vm/"
	@echo "  make codecs    Build the C/C++ codec registry and plugins"
	@echo "  make codecs-test Build and run codec registry/manager tests"
	@echo "  make churn     Compile the five churn constraint-report tools and validate data"
	@echo "  make churn-test Run all five churn constraint reports"
	@echo "  make churn-data-check Validate churn JSON/XML/CSV/TXT files"
	@echo "  make antivirus Build Sleelavirin C core and self-test executable"
	@echo "  make antivirus-test Build and run Sleelavirin C/native scanner self-tests"
	@echo "  make antivirus-clamav Check optional ClamAV scanner integration"
	@echo "  make antivirus-integrity Generate SHA-256 inventory for antivirus source and indexes"
	@echo "  make antivirus-integrity-check MANIFEST=PATH Verify the SHA-256 inventory"
	@echo "  make ballum    Build the native Ballum display library for this OS (falls back to the portable test if OS deps are missing)"
	@echo "  make ballum-deps [BALLUM_DEPS_INSTALL=1] Report/install this OS's Ballum display libraries (X11+Xrandr / Apple frameworks / gdi32)"
	@echo "  make ballum-check-deps Report whether this OS's Ballum display libraries are available"
	@echo "  make ballum-test Build and run the portable Ballum tests (no display libraries required)"
	@echo "  make ballum-install [PREFIX=/usr/local] Install the native Ballum library + headers"
	@echo "  make ballum-uninstall Remove the installed native Ballum library + headers"
	@echo "  make lib-ballum Validate the SLeeLa lib/ballum sources and build the native backend below it"
	@echo "  make lib-ballum-check Validate the lib/ballum .sleela sources with the compiler"
	@echo "  make jetbrains Show JetBrains source acquisition helpers"
	@echo "  make connector Build all optional native database/data-source connectors"
	@echo "  make connector-test Run available offline connector tests"
	@echo "  make connector-check-deps Report native connector dependencies"
	@echo "  make install-check Verify Quick and Safe installer entry points"
	@echo "  make install [PREFIX=/usr/local] [DESTDIR=...] Scope-aware install (local prefix stays local; system prefix elevates)"
	@echo "  make install-local [PREFIX=$HOME/.local] User-local install; never uses sudo"
	@echo "  make install-system [PREFIX=/usr/local] System install; uses sudo for system paths only"
	@echo "      (every variant first offers the base-code SHA-256 manifest step; SLEELA_MANIFEST_CONSENT=yes|no|skip answers it)"
	@echo "  make install-product  Build and interactively install compiled products (user-local or system-wide)"
	@echo "  make clean-install    Guided, beautiful install of the MAIN package only (no servers/IDEs/wifi/moral); then optional modules"
	@echo "  make tutorial-check Verify tutorial lesson sequence, example XML, and expected-evidence pairing"
	@echo "  make tests     Build and run tests/"
	@echo "  make server    Build api/server/"
	@echo "  make route     Validate unified route data"
	@echo "  make config    Validate configuration policy and run location/VM smoke tests"
	@echo "  make common-rails Build and run CommonRails C/C++/Java/Sleela/SST checks"
	@echo "  make premium-vms Build the 11 premium SLVM generations as real runnable VMs"
	@echo "  make clean     Remove outputs from dispatched build systems"
	@echo ""
	@echo "Product-specific build folders and Makefiles remain authoritative."
