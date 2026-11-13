#!/usr/bin/env python3
"""Validate the 1:1 rule of /lib/java/jdk-28.

  * every JDK 28 type in the inventory has exactly one SLeeLa file;
  * every SLeeLa file under lib/java/jdk-28/<module>/ is listed in INDEX.tsv (no extras);
  * each file sits at the online API path and names its Java Source URL.

usage: python3 lib/java/jdk-28/tools/validate_jdk28.py   (exit 1 on any violation)
"""
from __future__ import annotations

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ONLINE = "https://download.java.net/java/early_access/jdk28/docs/api/"


def main() -> int:
    rows = [l.split("\t") for l in (ROOT / "INDEX.tsv").read_text(encoding="utf-8").splitlines()
            if l and not l.startswith("#")]
    errors = []
    types = [r[0] for r in rows]
    if len(set(types)) != len(types):
        errors.append("a Java type is listed more than once in INDEX.tsv")
    listed = set()
    for fq, module, _kind, sfile, src, _m in rows:
        rel = Path(sfile).relative_to("lib/java/jdk-28")
        listed.add(rel.as_posix())
        want = ONLINE + rel.with_suffix(".html").as_posix()
        if src != want:
            errors.append(f"{fq}: Source {src} does not match the file path ({want})")
        p = ROOT / rel
        if not p.is_file():
            errors.append(f"{fq}: missing {sfile}")
            continue
        text = p.read_text(encoding="utf-8")
        if f'String javaType = "{fq}";' not in text or f'String javaSource = "{src}";' not in text:
            errors.append(f"{fq}: {sfile} does not name its Java type and Source")
    on_disk = {p.relative_to(ROOT).as_posix() for p in ROOT.rglob("*.sleela")
               if p.relative_to(ROOT).parts[0] != "tools"}
    for extra in sorted(on_disk - listed):
        errors.append(f"extra file not in INDEX.tsv: lib/java/jdk-28/{extra}")
    for e in errors[:50]:
        print("FAIL:", e)
    print(f"jdk-28 validate: {len(rows)} Java types, {len(on_disk)} SLeeLa files, "
          f"{'1:1 OK' if not errors else str(len(errors)) + ' violation(s)'}")
    return 1 if errors else 0


if __name__ == "__main__":
    sys.exit(main())
