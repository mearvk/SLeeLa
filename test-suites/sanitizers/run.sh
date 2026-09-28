#!/usr/bin/env bash
set -u
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
CXX=c++
BUILD="$ROOT/test-suites/.sanitizers"
mkdir -p "$BUILD"
if ! command -v "$CXX" >/dev/null 2>&1; then echo "SKIP: C++ compiler unavailable"; exit 0; fi
"$CXX" -std=c++17 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer -I"$ROOT"   "$ROOT/test-suites/cpp/test_annotations.cpp"   "$ROOT/impl/annotation/Annotation.cpp"   "$ROOT/impl/annotation/AnnotationForwarder.cpp"   "$ROOT/impl/annotation/ForwardingAnnotation.cpp"   "$ROOT/impl/annotation/AnnotationInterpreter.cpp"   -o "$BUILD/asan_ubsan"
"$BUILD/asan_ubsan"
echo "TSan is an explicit opt-in because support varies by platform."
