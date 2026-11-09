#!/usr/bin/env python3
"""Validate the tracked SLeeLa Java library inventory and bridge manifest.

Run from any working directory:
    python3 lib/java/validate.py

The Git index is authoritative: untracked build products and local files do not
change the published library inventory. The manifest count is an exact count,
not a lower-bound floor.
"""
from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path
from typing import Any

EXPECTED_SCHEMA = "sleela-java-library-manifest-v1"
REQUIRED_MAPPING = {
    "java_owner",
    "sleela_symbol",
    "source_role",
    "execution_boundary",
    "loader_visibility",
    "compatibility_surface",
}


class ValidationError(Exception):
    """A repository or manifest invariant failed."""


def tracked_files(root: Path) -> list[str]:
    proc = subprocess.run(
        ["git", "ls-files", "--cached", "--", "lib/java"],
        cwd=root,
        text=True,
        capture_output=True,
        check=False,
    )
    if proc.returncode:
        raise ValidationError(proc.stderr.strip() or "git ls-files failed")
    return sorted(line for line in proc.stdout.splitlines() if line)


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValidationError(message)


def validate(root: Path) -> list[str]:
    root = root.resolve()
    lib = root / "lib" / "java"
    manifest_path = lib / "MANIFEST.json"
    require(lib.is_dir(), "lib/java directory is missing")
    require(manifest_path.is_file(), "lib/java/MANIFEST.json is missing")

    try:
        manifest: Any = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise ValidationError(f"cannot read manifest: {exc}") from exc

    require(isinstance(manifest, dict), "manifest root must be a JSON object")
    require(manifest.get("schema") == EXPECTED_SCHEMA, "unexpected manifest schema")
    require(manifest.get("root") == "/lib/java", "manifest root must be /lib/java")

    inventory = manifest.get("inventory")
    require(isinstance(inventory, dict), "manifest inventory must be an object")
    expected_count = inventory.get("count")
    require(
        isinstance(expected_count, int) and not isinstance(expected_count, bool)
        and expected_count >= 0,
        "manifest inventory.count must be a non-negative integer",
    )
    require(
        inventory.get("count_type") == "library-source-baseline",
        "manifest inventory.count_type must be library-source-baseline",
    )
    controls = inventory.get("control_files")
    require(isinstance(controls, list) and controls, "inventory.control_files must be a non-empty list")
    require(all(isinstance(p, str) and p.startswith("lib/java/") for p in controls),
            "every control_files entry must be a repository-relative lib/java path")
    require(len(controls) == len(set(controls)), "inventory.control_files contains duplicates")

    files = tracked_files(root)
    file_set = set(files)
    missing_controls = sorted(set(controls) - file_set)
    require(not missing_controls, "control files are not tracked: " + ", ".join(missing_controls))
    require("lib/java/MANIFEST.json" in controls, "MANIFEST.json must be excluded from its own inventory")
    require("lib/java/validate.py" in controls, "validate.py must be excluded from its own inventory")

    source_files = [path for path in files if path not in set(controls)]
    actual_count = len(source_files)
    require(
        actual_count == expected_count,
        f"exact inventory mismatch: manifest records {expected_count}, Git index contains {actual_count} library files",
    )
    require(all((root / path).is_file() for path in files),
            "one or more Git-indexed lib/java paths are missing from the working tree")

    bridge = manifest.get("language_bridge")
    require(isinstance(bridge, dict), "manifest language_bridge must be an object")
    require(bridge.get("source_language") == "SLeeLa", "source_language must be SLeeLa")
    require(bridge.get("parallel_runtime") == "Java", "parallel_runtime must be Java")
    native_vm = bridge.get("native_vm_foundation")
    require(isinstance(native_vm, list) and {"C", "C++"}.issubset(native_vm),
            "native_vm_foundation must identify both C and C++")

    mapping = manifest.get("mapping")
    require(isinstance(mapping, dict), "manifest mapping must be an object")
    concepts = mapping.get("required_concepts")
    require(isinstance(concepts, list) and set(concepts) == REQUIRED_MAPPING,
            "manifest mapping.required_concepts is incomplete or contains unknown concepts")

    roles = manifest.get("source_roles")
    boundaries = manifest.get("execution_boundaries")
    require(isinstance(roles, list) and roles and all(isinstance(x, str) and x for x in roles),
            "manifest source_roles must be a non-empty string list")
    require(isinstance(boundaries, list) and boundaries and all(isinstance(x, str) and x for x in boundaries),
            "manifest execution_boundaries must be a non-empty string list")

    validation = manifest.get("validation")
    require(isinstance(validation, dict), "manifest validation must be an object")
    require(validation.get("validator") == "lib/java/validate.py",
            "manifest validator path does not match")
    return [
        f"tracked files: {len(files)}",
        f"control files excluded: {len(controls)}",
        f"library inventory: {actual_count} (exact match)",
        "bridge and mapping metadata: valid",
    ]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[2],
                        help="repository root (defaults to the parent repository)")
    args = parser.parse_args()
    try:
        details = validate(args.root)
    except ValidationError as exc:
        print(f"FAIL: {exc}", file=sys.stderr)
        return 1
    for detail in details:
        print(f"PASS: {detail}")
    print("PASS: /lib/java inventory validated")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
