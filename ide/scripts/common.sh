#!/usr/bin/env bash
# Shared helpers for the SLeeLa IDE build scripts.
# Sourced by the other scripts in this directory; not meant to be run directly.

set -euo pipefail

# --- paths -------------------------------------------------------------------
# scripts/ lives at <repo>/ide/scripts, so the repo root is two levels up.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
IDE_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
REPO_ROOT="$(cd "$IDE_DIR/.." && pwd)"
PLUGIN_DIR="$IDE_DIR/sleela-intellij"

# The IntelliJ Platform Gradle Plugin 2.19.0 (see build.gradle.kts) requires
# Gradle 9.0.0+. Keep this in sync with gradle-wrapper.properties.
GRADLE_VERSION="9.8.0"

# --- logging -----------------------------------------------------------------
log()  { printf '\033[1;32m[sleela-ide]\033[0m %s\n' "$*"; }
warn() { printf '\033[1;33m[sleela-ide]\033[0m %s\n' "$*" >&2; }
die()  { printf '\033[1;31m[sleela-ide]\033[0m %s\n' "$*" >&2; exit 1; }

# --- JDK 21 discovery --------------------------------------------------------
# The IntelliJ Platform Gradle Plugin 2.x and its bundled Kotlin compiler reject
# JDK 25+, so the build is pinned to JDK 21. Honour an explicit JAVA_HOME if it
# already points at a 21.x JDK; otherwise try to locate one.
java_major() {
  # Prints the major version of the java at $1, or nothing on failure.
  "$1" -version 2>&1 | awk -F'"' '/version/ {split($2,a,"."); print (a[1]=="1")?a[2]:a[1]; exit}'
}

ensure_jdk21() {
  if [ -n "${JAVA_HOME:-}" ] && [ -x "$JAVA_HOME/bin/java" ]; then
    if [ "$(java_major "$JAVA_HOME/bin/java")" = "21" ]; then
      log "Using JAVA_HOME=$JAVA_HOME (JDK 21)"
      export PATH="$JAVA_HOME/bin:$PATH"
      return
    fi
    warn "JAVA_HOME ($JAVA_HOME) is not JDK 21; searching for a 21.x JDK."
  fi

  local candidates=()
  # mise-managed JDKs (sandbox default), then common system locations.
  if [ -d "$HOME/.local/share/mise/installs/java" ]; then
    while IFS= read -r d; do candidates+=("$d"); done \
      < <(find "$HOME/.local/share/mise/installs/java" -maxdepth 1 -type d -name '21*' 2>/dev/null | sort -r)
  fi
  for base in /usr/lib/jvm /Library/Java/JavaVirtualMachines /opt; do
    [ -d "$base" ] || continue
    while IFS= read -r d; do candidates+=("$d"); done \
      < <(find "$base" -maxdepth 2 -type d -iname '*21*' 2>/dev/null | sort -r)
  done

  for c in "${candidates[@]}"; do
    local home="$c"
    [ -x "$home/bin/java" ] || home="$c/Contents/Home"      # macOS layout
    [ -x "$home/bin/java" ] || continue
    if [ "$(java_major "$home/bin/java")" = "21" ]; then
      export JAVA_HOME="$home"
      export PATH="$JAVA_HOME/bin:$PATH"
      log "Found JDK 21 at $JAVA_HOME"
      return
    fi
  done

  die "No JDK 21 found. Install one (e.g. 'mise install java@21') or set JAVA_HOME to a 21.x JDK."
}

# --- gradle launcher ---------------------------------------------------------
# Prefer the project wrapper (reproducible 9.8.0). Fall back to a system gradle
# only when the wrapper jar has not been generated yet.
gradle_cmd() {
  # Prefer the wrapper whenever its jar exists. Invoke via `sh gradlew` so a
  # missing execute bit (git may not preserve it on checkout) does not force a
  # fallback to a system gradle — which would not have the IntelliJ Platform
  # plugin's tasks (e.g. buildPlugin) and would fail with "task not found".
  if [ -f "$PLUGIN_DIR/gradle/wrapper/gradle-wrapper.jar" ] && [ -f "$PLUGIN_DIR/gradlew" ]; then
    chmod +x "$PLUGIN_DIR/gradlew" 2>/dev/null || true
    echo "sh $PLUGIN_DIR/gradlew"
  elif command -v gradle >/dev/null 2>&1; then
    warn "Wrapper jar missing; using system 'gradle' ($(command -v gradle))." >&2
    warn "The IntelliJ plugin tasks (buildPlugin/runIde) need the pinned wrapper;" >&2
    warn "run gen-gradle-wrapper.sh first if 'buildPlugin' is reported missing." >&2
    echo "gradle"
  else
    die "Neither ./gradlew (wrapper jar) nor a system 'gradle' is available. Run gen-gradle-wrapper.sh first."
  fi
}
