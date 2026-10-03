#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname "$0")/../.." && pwd)
fail=0

check_grade() {
    path="$1"
    label="$2"
    if [ ! -d "$ROOT/$path" ]; then
        echo "WARN: $label directory not present: $path"
        return 0
    fi
    c=$(find "$ROOT/$path" -maxdepth 1 -type f -name '*.c' | wc -l)
    cpp=$(find "$ROOT/$path" -maxdepth 1 -type f \( -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' \) | wc -l)
    build="$ROOT/$path/build"
    if [ ! -d "$build" ]; then
        echo "FAIL: $label has no build/: $path"
        fail=1
    else
        echo "OK: $label build/: $path/build"
    fi
    echo "    C sources: $c; C++ sources: $cpp"
}

for g in 7.0 8.0 9.0; do
    check_grade "http/$g" "HTTP/$g"
done

for d in http-1.0 http-2.0 http-3.0 http-4.0 http-5.0 http-6.0; do
    if [ -d "$ROOT/$d" ]; then
        if [ ! -d "$ROOT/$d/build" ]; then
            echo "FAIL: $d has no build/"
            fail=1
        else
            echo "OK: $d/build present"
        fi
        if [ -f "$ROOT/$d/Makefile" ]; then
            echo "OK: $d has legacy/authoritative Makefile"
        else
            echo "INFO: $d uses its per-grade build wrapper"
        fi
    fi
done

if [ -f "$ROOT/http/8.0/http80_crypto.h" ] &&
   [ -f "$ROOT/http/8.0/http80_crypto.c" ] &&
   [ -f "$ROOT/http/8.0/http80_crypto.hpp" ] &&
   [ -f "$ROOT/http/8.0/http80_crypto.cpp" ]; then
    echo "OK: HTTP/8.0 C/C++ crypto adapter pair"
else
    echo "FAIL: HTTP/8.0 crypto C/C++ adapter pair incomplete"
    fail=1
fi

if [ -f "$ROOT/http/9.0/http90.h" ] &&
   [ -f "$ROOT/http/9.0/http90.c" ] &&
   [ -f "$ROOT/http/9.0/http90.hpp" ] &&
   [ -f "$ROOT/http/9.0/http90.cpp" ] &&
   [ -f "$ROOT/http/9.0/HTTP90.conf" ]; then
    echo "OK: HTTP/9.0 C/C++ implementation and configuration"
else
    echo "FAIL: HTTP/9.0 implementation/configuration incomplete"
    fail=1
fi

if [ "$fail" -ne 0 ]; then
    echo "HTTP BUILD AUDIT: FAIL"
    exit 1
fi
echo "HTTP BUILD AUDIT: PASS"
