#!/usr/bin/env bash
# Build and run both Java 28 SecureJDK memory-integration channels
# (port + JNI) and show that Sleela drives Java-resident objects by handle.
#
# Usage: java28/examples/run_demo.sh
set -euo pipefail
HERE="$(cd "$(dirname "$0")/.." && pwd)"
cd "$HERE"

echo ">> make all"
make all >/dev/null

echo ">> make demo"
make demo

echo
echo "Both channels produced the same result: Sleela executed against the"
echo "Java 28 SecureJDK memory model, holding only opaque handles."
