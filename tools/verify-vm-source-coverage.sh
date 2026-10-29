#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
ISA="$ROOT/lib/vm/InstructionSet.sleela"
HDR="$ROOT/impl/core/sleela_core.h"
python3 - "$ISA" "$HDR" <<'PY'
import re,sys
isa,hdr=map(open,sys.argv[1:])
source_text=isa.read()
header_text=hdr.read()
# Canonical SLeeLa ISA records use the angle-bracket house style. Accept one
# opcode record per line, with an optional numeric code and -- description.
source=re.findall(
    r'^[ \\t]*<[ \\t]*(OP_[A-Z][A-Z0-9_]*)(?:[ \\t]+[0-9]+)?(?:[ \\t]+--.*)?[ \\t]*>$',
    source_text, re.M
)
# Ignore opcode names in C comments and the OP__COUNT sentinel.
header_text=re.sub(r'/\\*.*?\\*/|//[^\\n]*', '', header_text, flags=re.S)
enum_match=re.search(r'typedef enum \\{(.*?)\\}\\s*SLOp;', header_text, re.S)
if not enum_match:
    raise SystemExit("native SLOp enum missing")
native=[op for op in re.findall(r'\\bOP_[A-Z][A-Z0-9_]*\\b',enum_match.group(1))
        if op != "OP__COUNT"]
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
