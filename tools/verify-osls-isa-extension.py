#!/usr/bin/env python3
"""Verify the frozen 155-opcode ISA and the append-only extension beyond it.

The first 155 opcodes (0-154) are frozen against tools/isa/osls-isa-baseline-155.txt
and OP_OSLS_CALL is pinned at 155. Everything from 156 onward is append-only
(OP_BIGMUL, OP_SLEEP, OP_BIGADD, OP_BIGDIVMOD, OP_BIGSUB, OP_BIGDIV,
OP_BIGISQRT, ...); this checker only requires that the native SLOp enum and the
source ISA registry agree on the WHOLE ordered list, not a fixed total count.
"""
from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "lib/vm/InstructionSet.sleela"
NATIVE = ROOT / "impl/core/sleela_core.h"
BASELINE = ROOT / "tools/isa/osls-isa-baseline-155.txt"

def fail(message: str) -> None:
    raise SystemExit(f"OSLS ISA verification failed: {message}")

source_text = SOURCE.read_text(encoding="utf-8")
source_names: list[str] = []
for line_number, line in enumerate(source_text.splitlines(), 1):
    match = re.match(r"^\s*<\s*(OP_[A-Z0-9_]+)\b(?:\s+(\d+))?", line)
    if not match:
        continue
    name, explicit_code = match.groups()
    if explicit_code is not None and int(explicit_code) != len(source_names):
        fail(f"{SOURCE.relative_to(ROOT)}:{line_number}: {name} is annotated {explicit_code}, expected {len(source_names)}")
    source_names.append(name)

native_text = NATIVE.read_text(encoding="utf-8")
enum_match = re.search(r"typedef\s+enum\s*\{([\s\S]*?)\}\s*SLOp\s*;", native_text)
if not enum_match:
    fail("could not locate the native SLOp enum")
enum_body = re.sub(r"/\*[\s\S]*?\*/|//[^\n]*", "", enum_match.group(1))
native_names = re.findall(r"\b(OP_[A-Z0-9_]+)\b", enum_body)
native_names = [name for name in native_names if name != "OP__COUNT"]

baseline_names = [
    line.strip() for line in BASELINE.read_text(encoding="utf-8").splitlines()
    if line.strip() and not line.lstrip().startswith("#")
]

if len(baseline_names) != 155:
    fail(f"baseline contains {len(baseline_names)} entries, expected 155")
if source_names[:155] != baseline_names:
    for index, (expected, actual) in enumerate(zip(baseline_names, source_names[:155])):
        if expected != actual:
            fail(f"legacy opcode {index} changed: expected {expected}, found {actual}")
    fail("source ISA is missing one or more legacy opcodes")
if len(source_names) < 156 or source_names[155] != "OP_OSLS_CALL":
    fail(f"source ISA must contain OP_OSLS_CALL at index 155 (>=156 entries); found {len(source_names)}")
if native_names != source_names:
    limit = min(len(native_names), len(source_names))
    for index in range(limit):
        if native_names[index] != source_names[index]:
            fail(f"native/source mismatch at opcode {index}: {native_names[index]} != {source_names[index]}")
    fail(f"native/source counts differ: {len(native_names)} != {len(source_names)}")
if source_names[103] != "OP_OS_PLATFORM" or source_names[123] != "OP_OS_PCLOSE":
    fail("legacy OS syscall range 103–123 has changed")

print(f"OSLS ISA verification: PASS (155 frozen legacy opcodes 0-154; OP_OS_* 103-123 unchanged; "
      f"OP_OSLS_CALL at 155; {len(source_names)} total opcodes, native and source in agreement)")
