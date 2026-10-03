#!/usr/bin/env bash
# =============================================================================
# download-all.sh -- fetch every external library SLeeLa depends on that is
# <= 50 MB, into relative libs/ subdirectories.
#
#   libs/java/    Maven/JavaFX artifacts   (download-java-deps.sh)
#   libs/native/  native C/C++ libraries   (download-native-deps.sh)
#
# See libs/MANIFEST.md for the full list, sizes, and sources.
# Requires network access; in an offline sandbox the sub-scripts fail by design.
# =============================================================================
set -euo pipefail
HERE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

echo "########## SLeeLa external library downloader (<= 50 MB each) ##########"
rc=0
bash "$HERE/download-java-deps.sh"   || { echo "java deps: FAILED" >&2;   rc=1; }
bash "$HERE/download-native-deps.sh" || { echo "native deps: FAILED" >&2; rc=1; }

echo
echo "########## summary ##########"
for d in java native; do
  if [ -d "$HERE/$d" ]; then
    n=$(find "$HERE/$d" -type f 2>/dev/null | wc -l | tr -d ' ')
    echo "  libs/$d: $n file(s)"
  fi
done
exit "$rc"
