#!/bin/sh
set -eu
exec "$(dirname "$0")/sleela-http-install.sh" "$@"
