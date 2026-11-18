#!/usr/bin/env python3
"""Compare exact reflected JDK 25 and JDK 28 public API inventories."""
from __future__ import annotations

import argparse
import json
from collections import Counter
from pathlib import Path


def records(path: Path):
    return [json.loads(line) for line in path.read_text(encoding="utf-8").splitlines() if line.strip()]


def member_key(member):
    return (
        member.get("name", ""),
        tuple(p[0] for p in member.get("params", [])),
    )


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--jdk25-api", required=True)
    ap.add_argument("--jdk25-signatures", required=True)
    ap.add_argument("--jdk28-api", required=True)
    ap.add_argument("--jdk28-signatures", required=True)
    ap.add_argument("--markdown", required=True)
    args = ap.parse_args()

    old_api, new_api = records(Path(args.jdk25_api)), records(Path(args.jdk28_api))
    old_sig, new_sig = records(Path(args.jdk25_signatures)), records(Path(args.jdk28_signatures))
    old_types = {r["name"]: r for r in old_api}
    new_types = {r["name"]: r for r in new_api}
    old_members = {r["name"]: r.get("members", []) for r in old_sig}
    new_members = {r["name"]: r.get("members", []) for r in new_sig}

    added_types = sorted(new_types.keys() - old_types.keys())
    removed_types = sorted(old_types.keys() - new_types.keys())
    changed_types = sorted(old_types.keys() & new_types.keys())
    member_added = member_removed = member_changed = 0
    field_added = field_removed = field_changed = 0
    type_metadata_changed = 0
    member_add_samples, member_remove_samples = [], []
    for name in changed_types:
        old_record, new_record = old_types[name], new_types[name]
        if any(old_record.get(k) != new_record.get(k) for k in ("kind", "super", "interfaces", "abstract")):
            type_metadata_changed += 1

        before = Counter(json.dumps(m, sort_keys=True) for m in old_members.get(name, []))
        after = Counter(json.dumps(m, sort_keys=True) for m in new_members.get(name, []))
        added = list((after - before).elements())
        removed = list((before - after).elements())
        member_added += len(added)
        member_removed += len(removed)
        old_by_key, new_by_key = {}, {}
        for member in old_members.get(name, []):
            old_by_key.setdefault(member_key(member), Counter())[json.dumps(member, sort_keys=True)] += 1
        for member in new_members.get(name, []):
            new_by_key.setdefault(member_key(member), Counter())[json.dumps(member, sort_keys=True)] += 1
        for key in old_by_key.keys() & new_by_key.keys():
            changed_out = sum((old_by_key[key] - new_by_key[key]).values())
            changed_in = sum((new_by_key[key] - old_by_key[key]).values())
            member_changed += min(changed_out, changed_in)

        old_fields = {field[0]: tuple(field) for field in old_record.get("fields", [])}
        new_fields = {field[0]: tuple(field) for field in new_record.get("fields", [])}
        field_added += len(new_fields.keys() - old_fields.keys())
        field_removed += len(old_fields.keys() - new_fields.keys())
        field_changed += sum(old_fields[k] != new_fields[k] for k in old_fields.keys() & new_fields.keys())

        if added and len(member_add_samples) < 20:
            member_add_samples.extend(f"{name}: {json.loads(x).get('name')}({', '.join(p[0] for p in json.loads(x).get('params', []))})" for x in added[:20-len(member_add_samples)])
        if removed and len(member_remove_samples) < 20:
            member_remove_samples.extend(f"{name}: {json.loads(x).get('name')}({', '.join(p[0] for p in json.loads(x).get('params', []))})" for x in removed[:20-len(member_remove_samples)])

    missing_sig_types = sorted(new_types.keys() - new_members.keys())
    lines = [
        "# JDK 25 → JDK 28 reflected API comparison", "",
        "This report compares the checked-in JDK 25 reflection baseline with fresh JDK 28 runtime reflection generated during the campaign.",
        "It is a structural API comparison, not a claim that the generated SLeeLa stubs implement Java behavior.", "",
        "## Totals", "",
        f"- JDK 25 reflected types: {len(old_types)}",
        f"- JDK 28 reflected types: {len(new_types)}",
        f"- Types added in JDK 28: {len(added_types)}",
        f"- Types absent from JDK 28: {len(removed_types)}",
        f"- JDK 28 types without a signature record: {len(missing_sig_types)}",
        f"- Member declarations added: {member_added}",
        f"- Member declarations removed: {member_removed}",
        f"- Existing member keys with changed generic/return/modifier metadata: {member_changed}",
        f"- Public/protected fields added: {field_added}",
        f"- Public/protected fields removed: {field_removed}",
        f"- Public/protected fields changed: {field_changed}",
        f"- Type metadata changes (kind/super/interfaces/abstract): {type_metadata_changed}", "",
        "## Added types", "",
        *(f"- {x}" for x in added_types[:100]), "",
        "## Removed types", "",
        *(f"- {x}" for x in removed_types[:100]), "",
        "## Added member samples", "",
        *(f"- {x}" for x in member_add_samples), "",
        "## Removed member samples", "",
        *(f"- {x}" for x in member_remove_samples), "",
        "## Signature-index completeness", "",
        *(f"- Missing JDK 28 signature record: {x}" for x in missing_sig_types[:100]),
        "" if missing_sig_types else "- Every reflected JDK 28 type has a signature record.",
    ]
    Path(args.markdown).write_text("\n".join(lines) + "\n", encoding="utf-8")
    print("\n".join(lines[:18]))
    if missing_sig_types:
        raise SystemExit(f"JDK 28 signature reflection is incomplete for {len(missing_sig_types)} types")


if __name__ == "__main__":
    main()
