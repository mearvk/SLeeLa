#!/usr/bin/env python3
"""Generate a trusted SHA-256 manifest for the SLeeLa verified build gate.

The manifest is consumed by ``tools/verify-before-execution.py`` (and the
``verify-security`` target in ``impl/Makefile``), which fails closed unless every
listed file matches its recorded SHA-256 digest.

By default this records the SLeeLa-authored C/C++ source that the build actually
compiles: the execution core, the front end, the Nordshrift driver, the shared
catalog, the xclass loader, and the subject libraries. Vendored trees (for
example ``bash/``) and generated ``build/`` output are excluded.

Usage:
    # Regenerate the default committed manifest (paths relative to repo root):
    python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json

    # Print to stdout instead of writing a file:
    python3 tools/generate-sha256-manifest.py --root . --stdout
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys

# Source directories whose C/C++/header files are inputs to the verified build.
DEFAULT_SOURCE_DIRS = [
    "impl/core",
    "impl/frontend",
    "impl/nordshrift",
    "impl/catalog",
    "impl/xclass",
    "impl/subjects",
]
# Individual files outside those directories that the build also consumes.
DEFAULT_EXTRA_FILES = [
    "Parameters.c",
    "runtime/garbage_collector.c",
    "runtime/garbage_collector.h",
    "runtime/security_supervisor.c",
    "runtime/security_supervisor.h",
    "runtime/Parameters.c",
    "runtime/Parameters.h",
    "bash/pixel_terminal.cpp",
    "bash/pixel_terminal.hpp",
    # SHEET.sheet is not compiled, but it is a TRUSTED INPUT the compiler reads
    # at compile time to resolve conducted methods (conduct/role/insight/route)
    # and the system invariants. Tampering with it silently changes program
    # semantics, so it belongs inside the integrity boundary alongside the
    # toolchain source. (User .sleela programs are deliberately NOT covered: the
    # gate protects the interpreter, not the arbitrary programs it runs.)
    "SHEET.sheet",
]
SOURCE_SUFFIXES = {".c", ".h", ".cpp", ".hpp", ".cc", ".hh"}


def sha256_of(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def collect(root: Path) -> list[dict[str, str]]:
    root = root.resolve()
    seen: set[Path] = set()
    entries: list[dict[str, str]] = []

    def add(path: Path) -> None:
        rp = path.resolve()
        if rp in seen or not rp.is_file():
            return
        seen.add(rp)
        rel = rp.relative_to(root).as_posix()
        entries.append({"path": rel, "sha256": sha256_of(rp)})

    for d in DEFAULT_SOURCE_DIRS:
        base = root / d
        if not base.is_dir():
            continue
        for path in sorted(base.rglob("*")):
            if path.is_file() and path.suffix in SOURCE_SUFFIXES:
                add(path)
    for f in DEFAULT_EXTRA_FILES:
        p = root / f
        if p.is_file():
            add(p)

    entries.sort(key=lambda e: e["path"])
    return entries


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate the SLeeLa SHA-256 build manifest.")
    parser.add_argument("--root", type=Path, default=Path("."),
                        help="Repository root (default: current directory).")
    parser.add_argument("--output", type=Path, default=Path("security/sha256-manifest.json"),
                        help="Output manifest path (default: security/sha256-manifest.json).")
    parser.add_argument("--stdout", action="store_true",
                        help="Write the manifest to stdout instead of --output.")
    args = parser.parse_args()

    entries = collect(args.root)
    if not entries:
        print("generate: no source files found; check --root", file=sys.stderr)
        return 1

    manifest = {"algorithm": "sha256", "files": entries}
    text = json.dumps(manifest, indent=2) + "\n"

    if args.stdout:
        sys.stdout.write(text)
    else:
        out = args.output
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(text, encoding="utf-8")
        print(f"generate: wrote {len(entries)} entries to {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
