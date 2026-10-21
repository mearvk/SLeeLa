#!/usr/bin/env python3
"""Generate a trusted SHA-256 manifest for the SLeeLa verified build gate.

The manifest is consumed by ``tools/verify-before-execution.py`` (and the
``verify-security`` target in ``impl/Makefile``), which fails closed unless every
listed file matches its recorded SHA-256 digest.

By default this records the SLeeLa-authored C/C++ source that the build actually
compiles (the execution core, the front end, the Nordshrift driver, the shared
catalog, the xclass loader, and the subject libraries) AND the SLeeLa standard
library source under ``lib/`` (every ``*.sleela`` plus its C/C++ helpers). The
runtime loads and executes ``lib/`` classes (e.g. the GUI Master Classes a
program pulls in with ``import gui.version-military;``), so they are part of the
trusted computing base and belong inside the integrity boundary. Vendored trees
(for example ``bash/``) and generated ``build/`` output are excluded, and
arbitrary user ``.sleela`` programs are deliberately NOT covered -- the gate
protects the toolchain and the standard library it runs, not the programs it
runs for the user.

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
    "wifi-common",
]
# The SLeeLa standard library the runtime loads and executes. Every *.sleela
# library class plus its native C/C++ helpers is a trusted input (a program's
# `import ...` runs these), so they are covered by the execution gate too.
LIBRARY_SOURCE_DIRS = [
    "lib",
]
# Suffixes recorded from LIBRARY_SOURCE_DIRS (source the runtime trusts). This is
# SOURCE_SUFFIXES plus the SLeeLa source extension.
LIBRARY_SUFFIXES = {".sleela", ".c", ".h", ".cpp", ".hpp", ".cc", ".hh"}
# Individual files outside those directories that the build also consumes.
DEFAULT_EXTRA_FILES = [
    # Idea System(TM) realtime canvas: the native-CPU render+physics core the
    # VM links by default so `sleela <program>` can drive it.
    "idea/realtime/idea_canvas.h",
    "idea/realtime/idea_canvas_internal.hpp",
    "idea/realtime/idea_canvas_sim.cpp",
    "idea/realtime/idea_canvas_gl_stub.cpp",
    "idea/realtime/idea_canvas_gpu.cpp",
    "idea/realtime/idea_canvas_ctx.h",
    "idea/realtime/idea_canvas_ctx_egl.cpp",
    "idea/realtime/idea_canvas_ctx_wgl.cpp",
    "idea/realtime/idea_canvas_ctx_cgl.mm",
    # Sleela-Native: clean-room original renderer/model/audio (MIT).
    "idea/sleela-native/sln_math.h",
    "idea/sleela-native/sln_scene.h",
    "idea/sleela-native/sln_audio.h",
    "idea/sleela-native/sln_audio.cpp",
    "idea/sleela-native/sln_gl_loader.h",
    "idea/sleela-native/sln_gl_loader.cpp",
    "idea/sleela-native/sln_render_gl.h",
    "idea/sleela-native/sln_render_gl.cpp",
    "idea/sleela-native/sln_demo.cpp",
    "Parameters.c",
    "runtime/garbage_collector.c",
    "runtime/garbage_collector.h",
    "runtime/security_supervisor.c",
    "runtime/security_supervisor.h",
    "runtime/Parameters.c",
    "runtime/Parameters.h",
    "bash/pixel_terminal.cpp",
    "bash/pixel_terminal.hpp",
    # Shared ledger core: compiled into the Nordshrift build (the compiler emits
    # a .ledger next to each .sleela artifact) and reused by the standalone
    # ledger tool. It lives outside the scanned impl/ dirs, so list it here.
    "ledger/ledger_core.cpp",
    "ledger/ledger_core.h",
    # SHEET.sheet is not compiled, but it is a TRUSTED INPUT the compiler reads
    # at compile time to resolve conducted methods (conduct/role/insight/route)
    # and the system invariants. Tampering with it silently changes program
    # semantics, so it belongs inside the integrity boundary alongside the
    # toolchain source. (User .sleela programs are deliberately NOT covered: the
    # gate protects the interpreter, not the arbitrary programs it runs.)
    "SHEET.sheet",
]
SOURCE_SUFFIXES = {".c", ".h", ".cpp", ".hpp", ".cc", ".hh", ".mm"}


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
    for d in LIBRARY_SOURCE_DIRS:
        base = root / d
        if not base.is_dir():
            continue
        for path in sorted(base.rglob("*")):
            if path.is_file() and path.suffix in LIBRARY_SUFFIXES:
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
