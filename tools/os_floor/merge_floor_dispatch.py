#!/usr/bin/env python3
"""merge_floor_dispatch.py -- merge a sleela-os-floor dispatch payload into the
repository OS-floor catalog.

Consumes the `client_payload` of a GitHub `repository_dispatch` event whose
`event_type` is `sleela-os-floor` (posted by http-3.0/os_floor_report.py) and
appends/deduplicates the reported floors into a catalog JSON kept in the repo.

Dedupe key is `floor_id`. On a repeat, the catalog entry's `seen_count` is
incremented by the reported count, `last_seen` advances, `report_count` bumps,
and the bonded tier is upgraded (never silently downgraded). The merge is
deterministic and side-effect-free apart from writing the catalog file.

Only the whitelisted, non-identifying descriptors are stored -- matching the
reporter's sanitized payload (no secrets, no PII).

Usage:
    merge_floor_dispatch.py --payload event_payload.json --catalog CATALOG.json
    # or read the payload from stdin:
    cat payload.json | merge_floor_dispatch.py --catalog CATALOG.json
"""
from __future__ import annotations

import argparse
import json
import sys
import time
from pathlib import Path

CATALOG_VERSION = 1
_ALLOWED = {
    "floor_id", "os_name", "os_release", "filesystem",
    "bonded_tier", "bonded_tier_name", "responder",
    "seen_count", "first_seen", "last_seen",
}
_TIER_ORDER = {"NONE": 0, "T1_PRESENT": 1, "T2_STORAGE": 2, "T3_CONTEXT": 3, "T4_RECOVERY": 4}


def _now_iso() -> str:
    return time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())


def _sanitize(floor: dict) -> dict:
    return {k: floor[k] for k in _ALLOWED if k in floor}


def load_catalog(path: Path) -> dict:
    if path.exists():
        try:
            doc = json.loads(path.read_text(encoding="utf-8"))
            if isinstance(doc, dict) and isinstance(doc.get("floors"), list):
                return doc
        except (json.JSONDecodeError, OSError):
            pass
    return {"catalog_version": CATALOG_VERSION, "generated": _now_iso(), "floors": []}


def merge(catalog: dict, floors: list) -> tuple:
    """Merge reported floors into the catalog. Returns (added, updated)."""
    by_id = {f.get("floor_id"): f for f in catalog["floors"] if f.get("floor_id")}
    added = updated = 0
    now = _now_iso()
    for raw in floors:
        f = _sanitize(raw)
        fid = f.get("floor_id")
        if not fid:
            continue
        f.setdefault("seen_count", 1)
        existing = by_id.get(fid)
        if existing is None:
            f["report_count"] = 1
            f.setdefault("first_seen", now)
            f.setdefault("last_seen", now)
            catalog["floors"].append(f)
            by_id[fid] = f
            added += 1
        else:
            existing["seen_count"] = int(existing.get("seen_count", 0)) + int(f.get("seen_count", 1))
            existing["report_count"] = int(existing.get("report_count", 1)) + 1
            existing["last_seen"] = f.get("last_seen") or now
            # upgrade tier if the new report attests higher
            new_rank = _TIER_ORDER.get(f.get("bonded_tier_name", "NONE"), 0)
            old_rank = _TIER_ORDER.get(existing.get("bonded_tier_name", "NONE"), 0)
            if new_rank > old_rank:
                existing["bonded_tier"] = f.get("bonded_tier", existing.get("bonded_tier"))
                existing["bonded_tier_name"] = f.get("bonded_tier_name")
                existing["responder"] = f.get("responder", existing.get("responder"))
            updated += 1
    catalog["floors"].sort(key=lambda x: (-_TIER_ORDER.get(x.get("bonded_tier_name", "NONE"), 0),
                                          x.get("floor_id", "")))
    catalog["generated"] = now
    catalog["catalog_version"] = CATALOG_VERSION
    catalog["floor_count"] = len(catalog["floors"])
    return added, updated


def extract_floors(payload: dict) -> list:
    """Pull the floors list from a repository_dispatch client_payload."""
    cp = payload.get("client_payload", payload)
    if cp.get("kind") not in (None, "sleela.os_floor.discovery"):
        return []
    floors = cp.get("floors")
    return floors if isinstance(floors, list) else []


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description="Merge a sleela-os-floor dispatch into the catalog.")
    ap.add_argument("--payload", help="event payload JSON file (default: stdin)")
    ap.add_argument("--catalog", required=True, help="catalog JSON file to update")
    args = ap.parse_args(argv)

    raw = Path(args.payload).read_text(encoding="utf-8") if args.payload else sys.stdin.read()
    try:
        payload = json.loads(raw) if raw.strip() else {}
    except json.JSONDecodeError as exc:
        print(f"merge: bad payload JSON: {exc}", file=sys.stderr)
        return 2

    floors = extract_floors(payload)
    catalog_path = Path(args.catalog)
    catalog = load_catalog(catalog_path)
    added, updated = merge(catalog, floors)
    catalog_path.parent.mkdir(parents=True, exist_ok=True)
    catalog_path.write_text(json.dumps(catalog, indent=2) + "\n", encoding="utf-8")
    print(f"merge: {added} added, {updated} updated; catalog now holds "
          f"{catalog['floor_count']} floor(s) at {catalog_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
