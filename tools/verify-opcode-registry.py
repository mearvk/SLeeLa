#!/usr/bin/env python3
"""Verify the SLeeLa 3.0 opcode catalog against both executable ISA registries.

The ordered source registry remains authoritative. This catalog records the
family allocation and status of assigned/reserved IDs; it may not add runtime
instructions or renumber existing ones.
"""
from __future__ import annotations

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / "tools/isa/opcode-registry-3.0.json"
SOURCE = ROOT / "lib/vm/InstructionSet.sleela"
NATIVE = ROOT / "impl/core/sleela_core.h"


def fail(message: str) -> int:
    print("OPCODE REGISTRY: FAIL: " + message, file=sys.stderr)
    return 1


def main() -> int:
    try:
        catalog = json.loads(CATALOG.read_text(encoding="utf-8"))
        source = SOURCE.read_text(encoding="utf-8")
        native = NATIVE.read_text(encoding="utf-8")
    except (OSError, json.JSONDecodeError) as exc:
        return fail(str(exc))

    if catalog.get("schema") != "SLeeLa-Opcode-Registry-1":
        return fail("unsupported catalog schema")
    if catalog.get("authority") != "lib/vm/InstructionSet.sleela":
        return fail("catalog must identify the authoritative source registry")

    source_names = re.findall(
        r"^[ \\t]*<[ \\t]*(OP_[A-Z0-9_]+)(?:[ \\t]+[0-9]+(?:[ \\t]+--[^\\n]*)?)?[ \\t]*>$",
        source, re.MULTILINE,
    )
    enum_match = re.search(r"typedef enum \\{([\\s\\S]*?)\\}\\s*SLOp;", native)
    if not enum_match:
        return fail("native SLOp enum not found")
    enum_body = re.sub(r"/\\*[\\s\\S]*?\\*/|//[^\\n]*", "", enum_match.group(1))
    native_names = [name for name in re.findall(r"\\b(OP_[A-Z0-9_]+)\\b", enum_body)
                    if name != "OP__COUNT"]
    if source_names != native_names:
        limit = min(len(source_names), len(native_names))
        mismatch = next((i for i in range(limit) if source_names[i] != native_names[i]), limit)
        return fail(f"source/native order mismatch at index {mismatch}: "
                    f"source={len(source_names)} native={len(native_names)}")

    assigned = catalog.get("assigned")
    if not isinstance(assigned, list):
        return fail("assigned must be an array")
    if len(assigned) != len(source_names):
        return fail(f"catalog has {len(assigned)} assigned entries; source has {len(source_names)}")
    for code, (record, source_name) in enumerate(zip(assigned, source_names)):
        if record.get("code") != code or record.get("name") != source_name:
            return fail(f"assigned record {code} does not match authoritative source")
        if record.get("status") not in {"implemented", "reserved-fail-closed"}:
            return fail(f"invalid implementation status for {source_name}")
        if record.get("family") != family_for(catalog, code):
            return fail(f"wrong family for {source_name} ({code})")
        if not isinstance(record.get("operand_contract"), dict):
            return fail(f"missing operand_contract record for {source_name}")

    families = catalog.get("families")
    if not isinstance(families, list) or not families:
        return fail("family allocations are missing")
    covered: dict[int, str] = {}
    for family in families:
        start, end, family_id = family.get("start"), family.get("end"), family.get("id")
        if not isinstance(start, int) or not isinstance(end, int) or not isinstance(family_id, str):
            return fail("malformed family range")
        if start < 0 or end < start:
            return fail(f"invalid range for family {family_id}")
        for code in range(start, end + 1):
            if code in covered:
                return fail(f"overlapping family ranges at {code}")
            covered[code] = family_id
    if set(covered) != set(range(282)):
        return fail("family ranges must cover every ID from 0 through 281 exactly once")

    reserved = catalog.get("reserved")
    expected_reserved = [code for code in range(282) if code >= len(assigned)]
    if not isinstance(reserved, list):
        return fail("reserved must be an array")
    if [entry.get("code") for entry in reserved] != expected_reserved:
        return fail("reserved IDs must be the complete ordered unassigned tail through 281")
    for entry in reserved:
        code = entry["code"]
        if entry.get("family") != covered[code] or entry.get("status") != "reserved-not-executable":
            return fail(f"invalid reserved record for {code}")
    if not any(entry.get("code") == 196 and entry.get("name") == "OP_MATH_ARRAY_CALL"
               for entry in reserved):
        return fail("196 must remain reserved for OP_MATH_ARRAY_CALL")
    if catalog.get("assigned_count") != len(assigned):
        return fail("assigned_count does not match assigned entries")
    if catalog.get("executable_high_watermark") != len(assigned) - 1:
        return fail("executable high-watermark does not match assigned entries")
    if catalog.get("allocated_high_watermark") != 281:
        return fail("allocated high-watermark must remain 281")

    print(f"OPCODE REGISTRY: PASS ({len(assigned)} ordered instructions; "
          f"{len(reserved)} reserved IDs; allocation 0..281)")
    return 0


def family_for(catalog: dict, code: int) -> str | None:
    for family in catalog.get("families", []):
        if family.get("start", 1) <= code <= family.get("end", 0):
            return family.get("id")
    return None


if __name__ == "__main__":
    raise SystemExit(main())
