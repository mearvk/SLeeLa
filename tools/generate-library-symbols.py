#!/usr/bin/env python3
"""Generate lib/LIBRARY.SYMBOLS.md from the actual /lib tree.

The /lib filesystem is the single source of truth for the SLeeLa class
vocabulary (see lib/LIBRARY.INDEX.md and impl/nordshrift/SST.SYMBOLS.md). This
tool mirrors that tree into the authoritative, machine-parsable symbol manifest:
one `package<TAB>symbol<TAB>path<TAB>kind` row per `.sleela` unit, under a header
whose counts are derived from the tree (never hand-edited).

Classification:
  - `SLPackage.sleela`  -> kind `facade`  (a package module facade)
  - every other `.sleela` -> kind `source`

Usage:
  python3 tools/generate-library-symbols.py            # write lib/LIBRARY.SYMBOLS.md
  python3 tools/generate-library-symbols.py --print-counts   # just print the counts
"""
import argparse
import os
import sys
from pathlib import Path


def repo_root() -> Path:
    # tools/ lives directly under the repository root.
    return Path(__file__).resolve().parent.parent


def collect(lib: Path):
    """Return (rows, packages) where rows is a list of (package, symbol, path, kind)."""
    rows = []
    packages = set()
    for dirpath, _dirnames, filenames in os.walk(lib):
        for name in filenames:
            if not name.endswith(".sleela"):
                continue
            full = Path(dirpath) / name
            rel = full.relative_to(lib.parent)  # e.g. lib/core/SLAny.sleela
            # Top-level package family = first component under lib/.
            package = rel.parts[1]
            packages.add(package)
            kind = "facade" if name == "SLPackage.sleela" else "source"
            rows.append((package, name, rel.as_posix(), kind))
    # Deterministic order: by package, then by path.
    rows.sort(key=lambda r: (r[0], r[2]))
    return rows, packages


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--print-counts", action="store_true",
                    help="print derived counts and exit without writing")
    args = ap.parse_args()

    root = repo_root()
    lib = root / "lib"
    if not lib.is_dir():
        print(f"error: {lib} not found", file=sys.stderr)
        return 2

    rows, packages = collect(lib)
    source_rows = [r for r in rows if r[3] == "source"]
    facade_rows = [r for r in rows if r[3] == "facade"]

    n_packages = len(packages)
    n_sources = len(source_rows)
    n_facades = len(facade_rows)
    n_total = len(rows)  # every .sleela unit is one symbol record

    if args.print_counts:
        print(f"packages={n_packages} sources={n_sources} "
              f"facades={n_facades} symbols={n_total}")
        return 0

    out = lib / "LIBRARY.SYMBOLS.md"
    lines = []
    lines.append("# SLeeLa /lib Symbol Manifest")
    lines.append("schema: SLeeLa-Library-Symbols-1")
    lines.append("collection-revision: 2.1")
    lines.append(f"library-source-files: {n_sources}")
    lines.append(f"library-packages: {n_packages}")
    lines.append(f"module-facade-symbols: {n_facades}")
    lines.append(f"total-symbol-records: {n_total}")
    lines.append("")
    lines.append("package\tsymbol\tpath\tkind")
    for package, symbol, path, kind in rows:
        lines.append(f"{package}\t{symbol}\t{path}\t{kind}")
    out.write_text("\n".join(lines) + "\n")
    print(f"wrote {out} : packages={n_packages} sources={n_sources} "
          f"facades={n_facades} symbols={n_total}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
