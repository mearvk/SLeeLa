#!/bin/sh
set -eu
SOURCE_ROOT=$(pwd)
PREFIX=/usr/local/share/sleela/http
VERSION=
ALL=0
NO_BUILD=0
while [ $# -gt 0 ]; do
  case "$1" in
    --source-root) shift; SOURCE_ROOT=$1 ;;
    --prefix) shift; PREFIX=$1 ;;
    --version) shift; VERSION=$1 ;;
    --all) ALL=1 ;;
    --no-build) NO_BUILD=1 ;;
    -h|--help) echo 'Usage: --version X.Y [--source-root PATH] [--prefix PATH] [--all] [--no-build]'; exit 0 ;;
    *) echo "Unknown option: $1" >&2; exit 2 ;;
  esac
  shift
done
case "$(uname -s)" in Linux) OS=linux ;; Darwin) OS=macos ;; *) echo 'Unsupported host' >&2; exit 1 ;; esac
ARCH=$(uname -m)
mkdir -p "$PREFIX"
install_one() {
  v=$1; module="$SOURCE_ROOT/http-$v"; [ -d "$module" ] || { echo "Missing module: $module" >&2; return 1; }
  if [ "$NO_BUILD" -eq 0 ] && [ -f "$module/build/Makefile" ]; then make -C "$module/build" syntax; fi
  dest="$PREFIX/$v"; rm -rf "$dest"; mkdir -p "$dest"; cp -R "$module/." "$dest/"
  { echo "sleela-http-version=$v"; echo "os=$OS"; echo "arch=$ARCH"; echo "source-root=$SOURCE_ROOT"; } > "$dest/INSTALL.MANIFEST"
  echo "Installed SLeeLa HTTP $v -> $dest"
}
if [ "$ALL" -eq 1 ]; then
  for d in "$SOURCE_ROOT"/http-*.0; do [ -d "$d" ] || continue; b=$(basename "$d"); install_one "${b#http-}"; done
else
  [ -n "$VERSION" ] || { echo 'Specify --version X.Y or --all' >&2; exit 2; }; install_one "$VERSION"
fi
