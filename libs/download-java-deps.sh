#!/usr/bin/env bash
# =============================================================================
# download-java-deps.sh -- fetch the Maven/JavaFX artifacts SLeeLa's GUI
# modules depend on, into libs/java/. Only artifacts <= 50 MB are kept.
#
# Dependencies come from: gui/pom.xml, audio/gui/pom.xml,
# telephony-skya/javafx/pom.xml. Source: Maven Central.
#
# Requires network access to repo1.maven.org. In an offline sandbox this will
# fail at the download step (by design) -- run it where Maven Central is
# reachable. Uses `mvn` if present, otherwise falls back to curl/wget.
# =============================================================================
set -euo pipefail

HERE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
DEST="$HERE/java"
MAX_BYTES=$((50 * 1024 * 1024))      # 50 MB hard cap per artifact
REPO="https://repo1.maven.org/maven2"
mkdir -p "$DEST"

# JavaFX versions used across the three poms.
JFX_VERSIONS=("21.0.6" "21.0.8")
# JavaFX modules needed (controls -> graphics -> base; fxml for skya). Both the
# plain jar and the :linux native classifier are fetched.
JFX_MODULES=("javafx-base" "javafx-graphics" "javafx-controls" "javafx-fxml")
# Build plugins referenced by the poms.
# format: groupPath|artifact|version|packaging
PLUGINS=(
  "org/apache/maven/plugins|maven-compiler-plugin|3.13.0|jar"
  "org/apache/maven/plugins|maven-surefire-plugin|3.5.2|jar"
  "org/openjfx|javafx-maven-plugin|0.0.8|jar"
)

# size_ok FILE -- true if FILE exists and is <= MAX_BYTES; otherwise remove it.
size_ok() {
  local f="$1" sz
  [ -f "$f" ] || return 1
  sz=$(wc -c < "$f" | tr -d ' ')
  if [ "$sz" -gt "$MAX_BYTES" ]; then
    echo "  SKIP (> 50 MB: ${sz} bytes): $(basename "$f")" >&2
    rm -f "$f"
    return 1
  fi
  echo "  ok   ($(( (sz + 1023) / 1024 )) KiB): $(basename "$f")"
  return 0
}

# fetch URL OUT -- download URL to OUT using curl or wget.
fetch() {
  local url="$1" out="$2"
  if command -v curl >/dev/null 2>&1; then
    curl -fL --retry 3 -o "$out" "$url"
  elif command -v wget >/dev/null 2>&1; then
    wget -q -O "$out" "$url"
  else
    echo "error: neither curl nor wget is available" >&2
    return 127
  fi
}

# fetch_maven groupPath artifact version packaging [classifier]
fetch_maven() {
  local gp="$1" art="$2" ver="$3" pkg="$4" cls="${5:-}"
  local name="$art-$ver${cls:+-$cls}.$pkg"
  local url="$REPO/$gp/$art/$ver/$name"
  local out="$DEST/$name"
  echo "fetch: $url"
  if fetch "$url" "$out"; then size_ok "$out" || true; else echo "  FAILED: $url" >&2; fi
}

echo "=== SLeeLa Java/Maven dependencies -> $DEST (<= 50 MB each) ==="

# Preferred path: let Maven resolve + copy the whole dependency graph. This also
# pulls transitive deps and caches them. Requires the three poms.
if command -v mvn >/dev/null 2>&1; then
  echo "[maven] resolving via mvn dependency:copy-dependencies"
  for pom in "$HERE/../gui/pom.xml" "$HERE/../audio/gui/pom.xml" "$HERE/../telephony-skya/javafx/pom.xml"; do
    [ -f "$pom" ] || continue
    echo "  pom: $pom"
    mvn -q -f "$pom" \
      org.apache.maven.plugins:maven-dependency-plugin:3.8.1:copy-dependencies \
      -DoutputDirectory="$DEST" -Dmdep.useRepositoryLayout=false || \
      echo "  (mvn resolve failed for $pom; direct download below still runs)" >&2
  done
  # Enforce the size cap on whatever Maven copied.
  for f in "$DEST"/*.jar; do [ -e "$f" ] && size_ok "$f" || true; done
else
  echo "[maven] mvn not found; using direct Maven Central downloads"
fi

# Direct downloads (also a fallback when mvn is unavailable or offline-partial).
for ver in "${JFX_VERSIONS[@]}"; do
  for mod in "${JFX_MODULES[@]}"; do
    fetch_maven "org/openjfx/$mod" "$mod" "$ver" "jar"
    fetch_maven "org/openjfx/$mod" "$mod" "$ver" "jar" "linux"
  done
done
for p in "${PLUGINS[@]}"; do
  IFS='|' read -r gp art ver pkg <<< "$p"
  fetch_maven "$gp" "$art" "$ver" "$pkg"
done

echo "=== done. Fetched artifacts in: $DEST ==="
ls -la "$DEST" 2>/dev/null || true
