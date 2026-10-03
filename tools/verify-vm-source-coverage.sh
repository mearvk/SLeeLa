#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ISA="$ROOT/lib/vm/InstructionSet.sleela"
HDR="$ROOT/impl/core/sleela_core.h"
test -s "$ISA" && test -s "$HDR"
python3 - "$ISA" "$HDR" <<'PY'
import re,sys
isa,hdr=map(open,sys.argv[1:])
classes=re.findall(r'\bclass\s+SLISA\w+\s*\{\s*String\s+name;',isa.read())
h=hdr.read()
body=re.search(r'typedef enum \{(.*?)\} SLOp;',h,re.S).group(1)
ops=[]
for x in re.findall(r'\bOP_[A-Z][A-Z0-9_]*\b',body):
    if x not in ops: ops.append(x)
if len(classes) != len(ops):
    raise SystemExit(f"ISA source/native count mismatch: {len(classes)} != {len(ops)}")
print(f"ISA source/native count: PASS ({len(ops)} opcodes)")
PY
COUNT="$(find "$ROOT/lib" -type f -name '*.sleela' | wc -l)"
test "$COUNT" -gt 0
printf 'SLeeLa /lib source inventory: PASS (%s .sleela files)\n' "$COUNT"
for n in $(seq 1 11); do test -f "$ROOT/sleela-virtual-machine/$n/docs/SOURCE-TO-VM.md"; done
printf 'SLVM generation contracts: PASS (1-11)\n'
