#!/usr/bin/env python3
"""
generate-modium.py — the Modium file-resolution recorder for the versioned VMs.

Idea System(TM) / Idea(TM). From Author; distributive.

For each VM version folder under virtual-machine/vm-version/NNN it records
EVERY file (no exceptions), capturing each file relative to itself, its parent,
and the full folder structure that led to it, plus SHA-256 and size. Two
artifacts per version, both DETERMINISTIC (sorted, stable) so regenerating from
unchanged files is byte-identical (idempotent):

  MODIUM.json  — the hash-scan manifest `sleela modium scan` verifies against:
                 { algorithm, root, generated_for, count, files: [ {path, name,
                 parent, ancestry[], depth, sha256, size}, ... ] }

  rebuild.sql  — a portable MySQL/MariaDB script carrying the metadata AND the
                 file content (base64) so an exact file can be reconstituted
                 when a hash fails. It is self-contained SQL (CREATE TABLE +
                 sorted INSERTs); it loads with `mysql < rebuild.sql`, and the
                 same rows are parseable without a server.

The MODIUM.json / rebuild.sql of a folder are themselves EXCLUDED from that
folder's record (a record never records itself), so the scan is stable.

Usage:
  python3 tools/generate-modium.py                 # all versions 001..010
  python3 tools/generate-modium.py --version 010   # one version
  python3 tools/generate-modium.py --check         # verify, write nothing (CI)
"""
from __future__ import annotations

import argparse
import base64
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
VM_VERSION_DIR = ROOT / "virtual-machine" / "vm-version"
# Artifacts a record never records about itself (kept out so the scan is stable).
SELF_ARTIFACTS = {"MODIUM.json", "rebuild.sql"}


def sha256_of(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


# The authoring party sealed into every entry's originality seal. A different
# author (or a tampered path/hash) yields a different seal, so a record that was
# tampered-then-regenerated under a different identity is detectable.
MODIUM_AUTHOR = "MEARVK LLC / the Author"


def originality_seal(version: str, rel: str, sha: str) -> str:
    """Originality seal = SHA-256(vm | path | sha256 | author). Ties the file's
    identity (where it is) to its content and the authoring party. Mirrors the
    registry's seal; it is what the `--seal` scan verifies (vs. `--verify`,
    which only checks file-vs-recorded-hash)."""
    seal_input = f"idea-vm-{version}|{rel}|{sha}|{MODIUM_AUTHOR}"
    return hashlib.sha256(seal_input.encode("utf-8")).hexdigest()


def record_file(version: str, version_root: Path, file_path: Path) -> dict:
    """Record one file relative to itself, its parent, and the whole folder
    structure that led to it (all paths POSIX, relative to the version root),
    with a content SHA-256 and an originality seal."""
    rel = file_path.relative_to(version_root).as_posix()
    parts = rel.split("/")
    name = parts[-1]
    parent = "/".join(parts[:-1]) if len(parts) > 1 else "."
    ancestry = parts[:-1]  # ordered folder chain from the version root to the file
    sha = sha256_of(file_path)
    return {
        "path": rel,
        "name": name,
        "parent": parent,
        "ancestry": ancestry,
        "depth": len(ancestry),
        "sha256": sha,
        "size": file_path.stat().st_size,
        "author": MODIUM_AUTHOR,
        "seal": originality_seal(version, rel, sha),
    }


def scan_version(version: str, version_root: Path) -> list[dict]:
    """Record EVERY file under the version root (no exceptions) except the
    two self-artifacts, sorted by path for determinism."""
    records = []
    for p in sorted(version_root.rglob("*")):
        if not p.is_file():
            continue
        if p.name in SELF_ARTIFACTS and p.parent == version_root:
            continue
        records.append(record_file(version, version_root, p))
    records.sort(key=lambda r: r["path"])
    return records


def manifest_seal(version: str, records: list[dict]) -> str:
    """A chain seal over every entry's seal (in sorted order), so the manifest
    as a whole is authenticity-checkable: adding/removing/reordering/retampering
    any entry changes this value."""
    h = hashlib.sha256()
    h.update(f"idea-vm-{version}|{MODIUM_AUTHOR}".encode("utf-8"))
    for r in records:
        h.update(("|" + r["seal"]).encode("utf-8"))
    return h.hexdigest()


def build_manifest(version: str, records: list[dict]) -> str:
    doc = {
        "algorithm": "sha256",
        "kind": "modium-file-resolution",
        "root": f"virtual-machine/vm-version/{version}",
        "generated_for": f"idea-vm-{version}",
        "author": MODIUM_AUTHOR,
        "count": len(records),
        "manifest_seal": manifest_seal(version, records),
        "files": records,
    }
    # sort_keys + fixed separators -> idempotent, stable bytes.
    return json.dumps(doc, indent=2, sort_keys=True) + "\n"


def sql_escape(s: str) -> str:
    return s.replace("\\", "\\\\").replace("'", "\\'")


def build_sql(version: str, version_root: Path, records: list[dict]) -> str:
    out = []
    out.append("-- rebuild.sql — Modium exact-reconstitution database for "
               f"idea-vm-{version}.")
    out.append("-- Idea System(TM) / Idea(TM). From Author; distributive.")
    out.append("-- Portable MySQL/MariaDB: loads with `mysql <db> < rebuild.sql`.")
    out.append("-- Deterministic (sorted rows) so regeneration is idempotent.")
    out.append("-- Each row carries a file's structural metadata + base64 content,")
    out.append("-- so an exact file can be reconstituted when a hash scan fails.")
    out.append("")
    out.append("CREATE TABLE IF NOT EXISTS modium_files (")
    out.append("  id        INT AUTO_INCREMENT PRIMARY KEY,")
    out.append(f"  vm        VARCHAR(32)  NOT NULL DEFAULT 'idea-vm-{version}',")
    out.append("  path      VARCHAR(1024) NOT NULL,")
    out.append("  name      VARCHAR(256)  NOT NULL,")
    out.append("  parent    VARCHAR(1024) NOT NULL,")
    out.append("  ancestry  VARCHAR(1024) NOT NULL,")
    out.append("  depth     INT           NOT NULL,")
    out.append("  sha256    CHAR(64)      NOT NULL,")
    out.append("  size      BIGINT        NOT NULL,")
    out.append("  seal      CHAR(64)      NOT NULL,")
    out.append("  content_b64 LONGTEXT    NOT NULL,")
    out.append("  UNIQUE KEY uq_vm_path (vm, path)")
    out.append(");")
    out.append("")
    for r in records:
        content = base64.b64encode((version_root / r["path"]).read_bytes()).decode("ascii")
        ancestry = "/".join(r["ancestry"])
        out.append(
            "INSERT INTO modium_files "
            "(vm, path, name, parent, ancestry, depth, sha256, size, seal, content_b64) VALUES ("
            f"'idea-vm-{version}', '{sql_escape(r['path'])}', '{sql_escape(r['name'])}', "
            f"'{sql_escape(r['parent'])}', '{sql_escape(ancestry)}', {r['depth']}, "
            f"'{r['sha256']}', {r['size']}, '{r['seal']}', '{content}');"
        )
    out.append("")
    return "\n".join(out)


def process(version: str, check: bool) -> tuple[bool, str]:
    version_root = VM_VERSION_DIR / version
    if not version_root.is_dir():
        return False, f"no such version folder: {version}"
    records = scan_version(version, version_root)
    manifest = build_manifest(version, records)
    sql = build_sql(version, version_root, records)
    man_path = version_root / "MODIUM.json"
    sql_path = version_root / "rebuild.sql"

    if check:
        ok = True
        msgs = []
        if not man_path.is_file() or man_path.read_text(encoding="utf-8") != manifest:
            ok = False; msgs.append("MODIUM.json out of date")
        if not sql_path.is_file() or sql_path.read_text(encoding="utf-8") != sql:
            ok = False; msgs.append("rebuild.sql out of date")
        return ok, f"{version}: " + (", ".join(msgs) if msgs else f"ok ({len(records)} files)")

    man_path.write_text(manifest, encoding="utf-8")
    sql_path.write_text(sql, encoding="utf-8")
    return True, f"{version}: wrote MODIUM.json + rebuild.sql ({len(records)} files)"


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--version", help="one version NNN (default: all 001..010)")
    ap.add_argument("--check", action="store_true", help="verify only; write nothing")
    args = ap.parse_args()

    versions = [args.version] if args.version else [f"{n:03d}" for n in range(1, 11)]
    all_ok = True
    for v in versions:
        ok, msg = process(v, args.check)
        print(("check: " if args.check else "modium: ") + msg)
        all_ok = all_ok and ok
    return 0 if all_ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
