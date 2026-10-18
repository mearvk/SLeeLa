#!/bin/sh
# SLeeLa system-wide installer (sudo for system paths only).
# MEARVK LLC - 2026
#
# This script performs the privileged, system-wide install. It is the sudo
# counterpart to the never-elevating user-local install (`make install-local`).
#
# Flow:
#   1. Run the base-code SHA-256 manifest step as the INVOKING user (yes/no).
#   2. Elevate with sudo ONLY to copy SLeeLa into system locations.
#   3. State plainly that the user's own files are not re-owned or altered.
#
# Honored environment variables (all optional):
#   PREFIX DESTDIR BINDIR LIBDIR DATADIR SYSCONFDIR  - install destinations
#   SLEELA_MANIFEST_CONSENT=yes|no|skip              - answer the manifest prompt
set -eu

SCRIPT_DIR="$(cd -- "$(dirname -- "$0")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

PREFIX="${PREFIX:-/usr/local}"
DESTDIR="${DESTDIR:-}"
BINDIR="${BINDIR:-$PREFIX/bin}"
LIBDIR="${LIBDIR:-$PREFIX/lib}"
DATADIR="${DATADIR:-$PREFIX/share}"
SYSCONFDIR="${SYSCONFDIR:-/etc}"
SLEELA_MANIFEST_CONSENT="${SLEELA_MANIFEST_CONSENT:-}"

echo "SLeeLa system-wide install"
echo "Prefix: $PREFIX  (binaries: $BINDIR, runtime: $LIBDIR/sleela, config: $SYSCONFDIR/sleela)"
echo

# Refuse a non-system prefix here: that is what `make install-local` is for, and
# it must never involve sudo.
case "$PREFIX" in
  /usr/*|/usr|/opt/*|/opt|/etc/*|/etc|/var/*|/var) : ;;
  *)
    echo "PREFIX '$PREFIX' is not a system location." >&2
    echo "For a user-local install (no sudo), run: make install-local PREFIX='$PREFIX'" >&2
    exit 1 ;;
esac

# Step 1: manifest step as the invoking (unprivileged) user, before any sudo.
echo "== Base-code integrity (SHA-256 manifest) =="
make --no-print-directory -C "$SCRIPT_DIR" manifest-prompt \
  SLEELA_MANIFEST_CONSENT="$SLEELA_MANIFEST_CONSENT"
echo

# Decide how to elevate.
if [ "$(id -u)" -eq 0 ]; then
  SUDO=""
  echo "Already running as root; proceeding with the system copy."
elif command -v sudo >/dev/null 2>&1; then
  SUDO="sudo"
  echo "Requesting administrator privileges (sudo) for the system copy only."
  echo "sudo writes new files under $PREFIX as the trusted system account; it does"
  echo "NOT change the owner, group, or permissions of files you already own."
else
  echo "System install needs root privileges, but sudo is not available." >&2
  echo "Re-run as root, or use 'make install-local' for a user-local install." >&2
  exit 1
fi
echo

# Step 2: privileged copy. do-install does no prompting and makes no elevation
# decision -- it just performs the install as whatever account runs it.
# SLEELA_INSTALL_ELEVATED=1 marks that consent/scope are already handled.
exec $SUDO make --no-print-directory -C "$SCRIPT_DIR" do-install \
  PREFIX="$PREFIX" DESTDIR="$DESTDIR" BINDIR="$BINDIR" \
  LIBDIR="$LIBDIR" DATADIR="$DATADIR" SYSCONFDIR="$SYSCONFDIR" \
  SLEELA_INSTALL_ELEVATED=1
