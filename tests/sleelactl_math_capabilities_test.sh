#!/usr/bin/env sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
out=$(NO_COLOR=1 sh "$ROOT/bin/sleelactl" info math)
printf '%s\n' "$out"
for marker in \
  "native math capability descriptor" \
  "source_facade=lib/math/SLNativeMathEngine.sleela" \
  "native_demo=lib/math/examples/NativeMathEngineDemo.sleela" \
  "vm_opcode=OP_MATH_CALL:179" \
  "vm_abi=1.7" \
  "vector=add;subtract;scale;dot;norm;normalize;cross3" \
  "matrix=multiply;transpose;inverse;determinant;thin QR" \
  "complex=add;subtract;multiply;divide;magnitude;phase;conjugate" \
  "fourier=radix-2 complex FFT"
do
  printf '%s\n' "$out" | grep -Fq "$marker" || {
    printf 'missing sleelactl math capability marker: %s\n' "$marker" >&2
    exit 1
  }
done
printf 'sleelactl math capability test: PASS\n'
