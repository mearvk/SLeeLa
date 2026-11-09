#!/bin/sh
set -eu

repo_root="$(CDPATH= cd -- "$(dirname -- "$0")/../../.." && pwd)"
runtime="${1:-$repo_root/impl/build/sleela}"

if [ ! -x "$runtime" ]; then
    echo "exception source smoke: runtime is not executable: $runtime" >&2
    exit 2
fi

output="$(cd "$repo_root" && "$runtime" run impl/tests/exceptions/exception_source_smoke.sleela 2>&1)" || {
    printf '%s\n' "$output" >&2
    echo "exception source smoke: SLeeLa execution failed" >&2
    exit 1
}

for expected in \
    EXCEPTION-SOURCE-CAUGHT \
    73 \
    19 \
    EXCEPTION-SOURCE-NORMAL
do
    printf '%s\n' "$output" | grep -Fq "$expected" || {
        printf '%s\n' "$output" >&2
        echo "exception source smoke: missing expected output: $expected" >&2
        exit 1
    }
done

if printf '%s\n' "$output" | grep -Fq EXCEPTION-SOURCE-UNEXPECTED-CATCH; then
    printf '%s\n' "$output" >&2
    echo "exception source smoke: catch body ran after normal completion" >&2
    exit 1
fi

echo "exception source smoke: PASS"
