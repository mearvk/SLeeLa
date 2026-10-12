#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# gen-gradle-wrapper.sh
#
# Generates the Gradle wrapper jar for the SLeeLa IntelliJ plugin.
#
# The repo intentionally does NOT commit gradle/wrapper/gradle-wrapper.jar (it is
# a binary materialized on demand; see sleela-intellij/.gitignore). This script
# regenerates it pinned to the version the repo expects (8.14.3).
#
# Why not just `cd sleela-intellij && gradle wrapper`? Because settings.gradle.kts
# references the IntelliJ Platform plugin, so evaluating the real project needs
# the IntelliJ SDK from the network. To stay offline-friendly we run the wrapper
# task in a throwaway project and copy the jar in — the jar is version-agnostic;
# the pinned version lives in gradle-wrapper.properties.
#
# Requires: a system 'gradle' on PATH and a JDK 21. No network needed.
# -----------------------------------------------------------------------------
set -euo pipefail
source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/common.sh"

ensure_jdk21

command -v gradle >/dev/null 2>&1 \
  || die "A system 'gradle' is required to generate the wrapper jar (e.g. 'mise install gradle')."

WRAPPER_DIR="$PLUGIN_DIR/gradle/wrapper"
JAR="$WRAPPER_DIR/gradle-wrapper.jar"

if [ -f "$JAR" ] && [ "${FORCE:-0}" != "1" ]; then
  log "Wrapper jar already present: $JAR (set FORCE=1 to regenerate)."
  exit 0
fi

SCRATCH="$(mktemp -d)"
trap 'rm -rf "$SCRATCH"' EXIT

log "Generating Gradle $GRADLE_VERSION wrapper in a scratch project..."
printf 'rootProject.name = "wrapgen"\n' > "$SCRATCH/settings.gradle.kts"

# The wrapper jar is identical regardless of the pinned version; the version is
# enforced later by gradle-wrapper.properties. We just need any gradle to emit a
# jar. Strategy, in order of preference:
#   1. Plain `wrapper` — works on every Gradle version. When online it validates
#      the distribution URL against services.gradle.org and passes.
#   2. If that fails (e.g. offline URL check) AND this gradle supports it, retry
#      with --no-validate-url (added in Gradle 8.2). Older gradle never gets here
#      with an unknown-option error because we don't pass the flag up front.
gen_wrapper() {
  ( cd "$SCRATCH" && gradle wrapper --gradle-version "$GRADLE_VERSION" "$@" )
}

if ! gen_wrapper 2>"$SCRATCH/err.log"; then
  if grep -q 'validate\|distribution url\|services.gradle.org' "$SCRATCH/err.log" \
     && gradle help --task :wrapper 2>/dev/null | grep -q -- '--no-validate-url'; then
    warn "Distribution-URL validation failed (offline?); retrying with --no-validate-url."
    gen_wrapper --no-validate-url || { cat "$SCRATCH/err.log" >&2; die "gradle wrapper failed; see output above."; }
  else
    cat "$SCRATCH/err.log" >&2
    die "gradle wrapper failed; see output above."
  fi
fi

[ -f "$SCRATCH/gradle/wrapper/gradle-wrapper.jar" ] \
  || die "Wrapper jar was not produced; check the gradle output above."

mkdir -p "$WRAPPER_DIR"
cp "$SCRATCH/gradle/wrapper/gradle-wrapper.jar" "$JAR"

# Keep the repo's committed gradle-wrapper.properties (it pins $GRADLE_VERSION and
# keeps validateDistributionUrl=true). Only create one if it is somehow missing.
if [ ! -f "$WRAPPER_DIR/gradle-wrapper.properties" ]; then
  cp "$SCRATCH/gradle/wrapper/gradle-wrapper.properties" "$WRAPPER_DIR/"
fi

log "Installed wrapper jar: $JAR ($(wc -c < "$JAR") bytes)"
log "Done. You can now run ./gradlew from $PLUGIN_DIR."
