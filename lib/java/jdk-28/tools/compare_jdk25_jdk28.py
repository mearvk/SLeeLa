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
    member_add_samples, member_remove_samples = [], []
    for name in changed_types:
        before = Counter(json.dumps(m, sort_keys=True) for m in old_members.get(name, []))
        after = Counter(json.dumps(m, sort_keys=True) for m in new_members.get(name, []))
        added = list((after - before).elements())
        removed = list((before - after).elements())
        member_added += len(added)
        member_removed += len(removed)
        # Same erased Java declaration key but different generic/return/modifier
        # metadata is called out separately as a changed declaration.
        old_keys = Counter(member_key(m) for m in old_members.get(name, []))
        new_keys = Counter(member_key(m) for m in new_members.get(name, []))
        member_changed += sum((old_keys & new_keys).values()) if added and removed else 0
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
        f"- Same-name/parameter-key changed-member candidates (overlap estimate): {member_changed}", "",
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
