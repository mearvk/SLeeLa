#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ISA="$ROOT/lib/vm/InstructionSet.sleela"
HDR="$ROOT/impl/core/sleela_core.h"
DISPATCH="$ROOT/impl/core/sleela_core.c"
python3 - "$ISA" "$HDR" "$DISPATCH" <<'PY'
import collections,re,sys
isa,hdr,dispatch=map(open,sys.argv[1:])
source_text=isa.read()
header_text=hdr.read()
dispatch_text=dispatch.read()
# Canonical SLeeLa ISA records use one angle-bracket record per line.
# Optional numeric codes and -- descriptions belong to that record.
source_records=re.findall(
    r'^[ \t]*<[ \t]*(OP_[A-Z][A-Z0-9_]*)(?:[ \t]+([0-9]+))?(?:[ \t]+--.*)?[ \t]*>$',
    source_text, re.M
)
for i,(op,code) in enumerate(source_records):
    if code and int(code) != i:
        print(f"source ISA code mismatch at index {i}: {op} declares {code}",file=sys.stderr)
        raise SystemExit(1)
source=[op for op,_code in source_records]
# Ignore C comments and the OP__COUNT sentinel in the native enum.
header_text=re.sub(r'/\*.*?\*/|//[^\n]*', '', header_text, flags=re.S)
enum_match=re.search(r'typedef enum \{(.*?)\}\s*SLOp;', header_text, re.S)
if not enum_match:
    raise SystemExit("native SLOp enum missing")
native=[op for op in re.findall(r'\bOP_[A-Z][A-Z0-9_]*\b',enum_match.group(1))
        if op != "OP__COUNT"]
if source != native:
    print("ISA source/native mismatch",file=sys.stderr)
    for i,(a,b) in enumerate(zip(source,native)):
        if a!=b: print(f"first difference at {i}: source={a} native={b}",file=sys.stderr); break
    print(f"source={len(source)} native={len(native)}",file=sys.stderr)
    raise SystemExit(1)
dispatch_ops=re.findall(r'\bcase\s+(OP_[A-Z][A-Z0-9_]*)\s*:',dispatch_text)
dispatch_counts=collections.Counter(dispatch_ops)
missing=[op for op in native if dispatch_counts[op] == 0]
extra=[op for op in dispatch_counts if op not in native]
duplicates=[op for op,count in dispatch_counts.items() if count != 1]
if missing or extra or duplicates:
    print(f"native dispatch mismatch: missing={missing}, extra={extra}, non-unique={duplicates}",file=sys.stderr)
    raise SystemExit(1)
print(f"ISA source/native/dispatch coverage: PASS ({len(source)} ordered opcodes; {len(dispatch_ops)} dispatch cases)")
PY
COUNT="$(find "$ROOT/lib" -type f -name '*.sleela' | wc -l)"
test "$COUNT" -gt 0
printf 'SLeeLa /lib source inventory: PASS (%s .sleela files)\n' "$COUNT"
for n in $(seq 1 11); do printf -v version "%03d" "$n"; test -f "$ROOT/sleela-virtual-machine/$version/docs/SOURCE-TO-VM.md"; done
printf 'SLVM generation contracts: PASS (1-11)\n'
