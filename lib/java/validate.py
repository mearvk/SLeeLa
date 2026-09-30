#!/usr/bin/env python3
"""Validate the SLeeLa /lib/java native library inventory.

Run from the repository root:
    python3 lib/java/validate.py
"""

from __future__ import annotations

import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
LIB = ROOT / "lib" / "java"
MANIFEST_PATH = LIB / "MANIFEST.json"
EXPECTED_COUNT = 8988


def fail(message: str) -> None:
    print(f"FAIL: {message}", file=sys.stderr)
    raise SystemExit(1)


def git_files() -> list[str]:
    proc = subprocess.run(
        ["git", "ls-files", "--cached", "--", "lib/java"],
        cwd=ROOT,
        text=True,
        capture_output=True,
        check=False,
    )
    if proc.returncode != 0:
        fail(proc.stderr.strip() or "git ls-files failed")
    return [line for line in proc.stdout.splitlines() if line]


def main() -> int:
    if not LIB.is_dir():
        fail("lib/java directory is missing")
    if not MANIFEST_PATH.is_file():
        fail("lib/java/MANIFEST.json is missing")

    try:
        manifest = json.loads(MANIFEST_PATH.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        fail(f"cannot read manifest: {exc}")

    if manifest.get("schema") != "sleela-java-library-manifest-v1":
        fail("unexpected manifest schema")
    if manifest.get("root") != "/lib/java":
        fail("manifest root must be /lib/java")

    inventory = manifest.get("inventory", {})
    recorded = inventory.get("count")
    if recorded != EXPECTED_COUNT:
        fail(f"manifest count is {recorded!r}; expected {EXPECTED_COUNT}")

    files = git_files()
    actual = len(files)
    if actual != EXPECTED_COUNT:
        fail(
            f"/lib/java contains {actual} Git-indexed files; "
            f"expected baseline is {EXPECTED_COUNT}"
        )

    language_bridge = manifest.get("language_bridge", {})
    if language_bridge.get("source_language") != "SLeeLa":
        fail("manifest source_language must be SLeeLa")
    if language_bridge.get("parallel_runtime") != "Java":
        fail("manifest parallel_runtime must be Java")

    native_vm = language_bridge.get("native_vm_foundation", [])
    if "C" not in native_vm or "C++" not in native_vm:
        fail("manifest must identify both C and C++ as VM foundations")

    mapping = manifest.get("mapping", {})
    required = {
        "java_owner",
        "sleela_symbol",
        "source_role",
        "execution_boundary",
        "loader_visibility",
        "compatibility_surface",
    }
    if set(mapping.get("required_concepts", [])) != required:
        fail("manifest mapping.required_concepts is incomplete")

    print(f"PASS: /lib/java validated ({actual} Git-indexed files)")
    print("PASS: SLeeLa/Java bridge metadata validated")
    print("PASS: C/C++ VM foundation metadata validated")
    return 0


if __name__ == "__main__":
    sys.exit(main())
