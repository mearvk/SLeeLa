#!/bin/sh
set -eu
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
"$ROOT/build/linux/build.sh"
exec "$ROOT/build/linux/skya" --client
