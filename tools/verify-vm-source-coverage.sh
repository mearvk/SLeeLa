#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ISA="$ROOT/lib/vm/InstructionSet.sleela"
HDR="$ROOT/impl/core/sleela_core.h"
python3 - "$ISA" "$HDR" <<'PY'
import re,sys
isa,hdr=map(open,sys.argv[1:])
source=re.findall(r'^// (OP_[A-Z0-9_]+)$',isa.read(),re.M)
body=re.search(r'typedef enum \{(.*?)\} SLOp;',hdr.read(),re.S).group(1)
native=[]
for op in re.findall(r'\bOP_[A-Z][A-Z0-9_]*\b',body):
    if op not in native: native.append(op)
if source != native:
    print("ISA source/native mismatch",file=sys.stderr)
    for i,(a,b) in enumerate(zip(source,native)):
        if a!=b: print(f"first difference at {i}: source={a} native={b}",file=sys.stderr); break
    raise SystemExit(1)
print(f"ISA source/native coverage: PASS ({len(source)} ordered opcodes)")
PY
COUNT="$(find "$ROOT/lib" -type f -name '*.sleela' | wc -l)"
test "$COUNT" -gt 0
printf 'SLeeLa /lib source inventory: PASS (%s .sleela files)\n' "$COUNT"
for n in $(seq 1 11); do test -f "$ROOT/sleela-virtual-machine/$n/docs/SOURCE-TO-VM.md"; done
printf 'SLVM generation contracts: PASS (1-11)\n'
