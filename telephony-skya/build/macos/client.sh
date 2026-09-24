#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
"$ROOT/build/macos/build.sh"
exec "$ROOT/build/macos/skya" --client
