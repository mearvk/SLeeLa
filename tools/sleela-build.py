#!/usr/bin/env python3
"""
SLeeLa build lifecycle driver.

This tool is intentionally a thin, auditable layer over the repository's
existing native Makefile. It does not replace the native build system.

Commands:
  init      Create a local project skeleton/manifest when absent.
  check     Validate repository/build prerequisites without building.
  build     Run the native build and record provenance.
  test      Run the native test suite and record provenance.
  run       Run the native SLeeLa executable with remaining arguments.
  package   Create a deterministic source/build provenance bundle.
  install   Install already-built binaries to an explicit directory.
  clean     Remove native build output through make clean.
  doctor    Produce a diagnostic report.
  version   Report the repository development version.

The driver uses only Python's standard library.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import platform
import shutil
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
IMPL = ROOT / "impl"
BUILD = IMPL / "build"
TOOLS = ROOT / "tools"
STATE = ROOT / ".sleela"
MANIFEST = STATE / "build-manifest.json"
PROJECT = ROOT / "sleela.project.json"
LOCK = ROOT / "sleela.lock.json"


def utc_now() -> str:
    return datetime.now(timezone.utc).replace(microsecond=0).isoformat().replace("+00:00", "Z")


def run(cmd: list[str], cwd: Path = ROOT, check: bool = True) -> subprocess.CompletedProcess[str]:
    print("+", " ".join(cmd))
    return subprocess.run(cmd, cwd=cwd, text=True, check=check)


def command_version(command: str) -> str:
    path = shutil.which(command)
    if not path:
        return "NOT FOUND"
    try:
        p = subprocess.run([command, "--version"], text=True, capture_output=True, check=False)
        text = (p.stdout or p.stderr).strip().splitlines()
        return text[0] if text else "VERSION UNKNOWN"
    except OSError as exc:
        return f"ERROR: {exc}"


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def executable(name: str) -> Path | None:
    p = BUILD / name
    return p if p.exists() else None


def write_json(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2, sort_keys=True) + "\n", encoding="utf-8")


def collect_provenance() -> dict:
    artifacts = {}
    for name in ("sleela", "sleela.exe", "nordshrift", "nordshrift.exe"):
        p = BUILD / name
        if p.is_file():
            artifacts[str(p.relative_to(ROOT))] = {
                "bytes": p.stat().st_size,
                "sha256": sha256_file(p),
            }

    return {
        "schema": "SLeeLa-Build-Provenance-1",
        "timestamp_utc": utc_now(),
        "repository_root": str(ROOT),
        "host": {
            "system": platform.system(),
            "release": platform.release(),
            "machine": platform.machine(),
            "python": sys.version.splitlines()[0],
        },
        "toolchain": {
            "cc": os.environ.get("CC", "gcc"),
            "cc_version": command_version(os.environ.get("CC", "gcc")),
            "cxx": os.environ.get("CXX", "g++"),
            "cxx_version": command_version(os.environ.get("CXX", "g++")),
            "make": command_version("make"),
            "python": sys.version.splitlines()[0],
        },
        "artifacts": artifacts,
    }


def cmd_init(_: argparse.Namespace) -> int:
    if not PROJECT.exists():
        write_json(PROJECT, {
            "schema": "SLeeLa-Project-1",
            "name": "SLeeLa",
            "version": "0.1.0-dev",
            "language": "SLeeLa-Complete",
            "nordshrift": "Complete",
            "targets": ["linux-amd64", "windows-10+-amd64", "macos-amd64-or-arm64"],
            "native_build": "impl/Makefile",
            "test_command": "make -C impl test",
        })
        print(f"created {PROJECT}")
    else:
        print(f"exists {PROJECT}")

    if not LOCK.exists():
        write_json(LOCK, {
            "schema": "SLeeLa-Lock-1",
            "generated_by": "tools/sleela-build.py",
            "policy": "toolchain-and-build-input record; no dependency is silently upgraded",
            "build_tools": {
                "make": shutil.which("make"),
                "cc": shutil.which(os.environ.get("CC", "gcc")),
                "cxx": shutil.which(os.environ.get("CXX", "g++")),
                "python": sys.executable,
            },
            "versions": {
                "make": command_version("make"),
                "cc": command_version(os.environ.get("CC", "gcc")),
                "cxx": command_version(os.environ.get("CXX", "g++")),
                "python": sys.version.splitlines()[0],
            },
        })
        print(f"created {LOCK}")
    else:
        print(f"exists {LOCK}")
    return 0


def cmd_check(_: argparse.Namespace) -> int:
    required = ["make", os.environ.get("CC", "gcc"), os.environ.get("CXX", "g++"), sys.executable]
    missing = [x for x in required if shutil.which(x) is None and x != sys.executable]
    if not IMPL.is_dir():
        missing.append("impl/")
    if missing:
        print("CHECK: FAIL")
        for item in missing:
            print("  missing:", item)
        return 1

    print("CHECK: PASS")
    print("  repository:", ROOT)
    for cmd in required:
        if cmd == sys.executable:
            print("  python:", sys.version.splitlines()[0])
        else:
            print(f"  {cmd}:", command_version(cmd))
    return 0


def repo_lib_inventory() -> tuple[list[Path], dict[str, list[Path]]]:
    lib = ROOT / "lib"
    files = sorted(p for p in lib.rglob("*.sleela") if p.is_file())
    symbols: dict[str, list[Path]] = {}
    import re
    for path in files:
        try:
            source = path.read_text(encoding="utf-8", errors="replace")
        except OSError:
            continue
        for match in re.finditer(r"\\bclass\\s+([A-Za-z_][A-Za-z0-9_]*)", source):
            symbols.setdefault(match.group(1), []).append(path)
    return files, symbols


def cmd_inventory_lib(_: argparse.Namespace) -> int:
    files, symbols = repo_lib_inventory()
    duplicates = {name: paths for name, paths in symbols.items() if len(paths) > 1}
    verifier = TOOLS / "verify-vm-source-coverage.sh"
    if not verifier.is_file():
        print("LIBRARY INVENTORY: FAIL; ISA/source verifier is missing", file=sys.stderr)
        return 1
    result = run(["bash", str(verifier)], cwd=ROOT, check=False)
    if result.returncode != 0:
        print("LIBRARY INVENTORY: FAIL; ISA coverage gate failed")
        return result.returncode
    packages = {p.relative_to(ROOT / "lib").parts[0] for p in files if len(p.relative_to(ROOT / "lib").parts) > 1}
    print("LIBRARY INVENTORY: PASS")
    print("  source files:", len(files))
    print("  packages:", len(packages))
    print("  class symbols:", len(symbols))
    print("  duplicate class symbols:", len(duplicates))
    if duplicates:
        for name, paths in sorted(duplicates.items()):
            print("  DUPLICATE:", name, "=>", ", ".join(str(p.relative_to(ROOT)) for p in paths))
    return 0


def cmd_compile(args: argparse.Namespace) -> int:
    source = Path(args.source).resolve()
    output = Path(args.output).resolve()
    if not source.is_file() or source.suffix != ".sleela":
        print(f"COMPILE: source is not a readable .sleela file: {source}", file=sys.stderr)
        return 2
    inventory_rc = cmd_inventory_lib(argparse.Namespace())
    if inventory_rc != 0:
        return inventory_rc
    exe = executable("sleela")
    if exe is None:
        print("COMPILE: no built native compiler; run 'python3 tools/sleela-build.py build' first", file=sys.stderr)
        return 2
    output.parent.mkdir(parents=True, exist_ok=True)
    result = run([str(exe), "compile", str(source), "-o", str(output)], cwd=ROOT, check=False)
    if result.returncode != 0:
        print("COMPILE: FAIL")
        return result.returncode
    print(f"COMPILE: PASS -> {output}")
    return 0


def cmd_build(_: argparse.Namespace) -> int:
    result = run(["make", "all"], cwd=IMPL, check=False)
    if result.returncode != 0:
        print("BUILD: FAIL")
        return result.returncode
    write_json(MANIFEST, collect_provenance())
    print(f"BUILD: PASS; provenance written to {MANIFEST}")
    return 0


def cmd_test(_: argparse.Namespace) -> int:
    result = run(["make", "test"], cwd=IMPL, check=False)
    if result.returncode != 0:
        print("TEST: FAIL")
        return result.returncode
    write_json(MANIFEST, collect_provenance() | {"test_result": "PASS"})
    print(f"TEST: PASS; provenance updated at {MANIFEST}")
    return 0


def cmd_run(args: argparse.Namespace) -> int:
    exe = executable("sleela")
    if platform.system().lower().startswith("windows"):
        exe = executable("sleela.exe")
    if exe is None:
        print("RUN: no built sleela executable; run 'build' first", file=sys.stderr)
        return 2
    return run([str(exe), *args.args], cwd=ROOT, check=False).returncode


def cmd_package(_: argparse.Namespace) -> int:
    package_dir = BUILD / "package-provenance"
    package_dir.mkdir(parents=True, exist_ok=True)
    provenance = collect_provenance()
    write_json(package_dir / "build-manifest.json", provenance)
    for name in ("VERSION.md", "BUILD.md", "BUILD.SYSTEM.md"):
        src = ROOT / name
        if src.exists():
            shutil.copy2(src, package_dir / name)
    print(f"PACKAGE: provenance bundle written to {package_dir}")
    return 0


def cmd_install(args: argparse.Namespace) -> int:
    target = Path(args.directory).expanduser().resolve()
    target.mkdir(parents=True, exist_ok=True)
    names = ["sleela", "nordshrift"]
    if platform.system().lower().startswith("windows"):
        names = ["sleela.exe", "nordshrift.exe"]
    for name in names:
        src = BUILD / name
        if not src.exists():
            print(f"INSTALL: missing {src}", file=sys.stderr)
            return 2
        shutil.copy2(src, target / name)
    print(f"INSTALL: PASS -> {target}")
    return 0


def cmd_clean(_: argparse.Namespace) -> int:
    return run(["make", "clean"], cwd=IMPL, check=False).returncode


def cmd_doctor(_: argparse.Namespace) -> int:
    report = collect_provenance()
    report["checks"] = {
        "impl_directory": IMPL.is_dir(),
        "project_manifest": PROJECT.exists(),
        "lock_file": LOCK.exists(),
        "native_makefile": (IMPL / "Makefile").exists(),
        "build_directory": BUILD.is_dir(),
        "sleela_binary": any((BUILD / n).is_file() for n in ("sleela", "sleela.exe")),
        "nordshrift_binary": any((BUILD / n).is_file() for n in ("nordshrift", "nordshrift.exe")),
    }
    write_json(STATE / "doctor.json", report)
    failed = [k for k, v in report["checks"].items() if not v]
    print(json.dumps(report["checks"], indent=2))
    print("DOCTOR:", "FAIL" if failed else "PASS")
    return 1 if failed else 0


def cmd_version(_: argparse.Namespace) -> int:
    print("SLeeLa 0.1.0-dev")
    return 0


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(prog="sleela-build", description="SLeeLa build lifecycle driver")
    sub = p.add_subparsers(dest="command", required=True)
    for name, func in [
        ("init", cmd_init), ("check", cmd_check), ("inventory-lib", cmd_inventory_lib), ("build", cmd_build),
        ("test", cmd_test), ("compile", cmd_compile), ("package", cmd_package), ("install", cmd_install),
        ("clean", cmd_clean), ("doctor", cmd_doctor), ("version", cmd_version),
    ]:
        sp = sub.add_parser(name)
        if name == "install":
            sp.add_argument("directory")
        if name == "compile":
            sp.add_argument("source")
            sp.add_argument("output")
        sp.set_defaults(func=func)
    runp = sub.add_parser("run")
    runp.add_argument("args", nargs=argparse.REMAINDER)
    runp.set_defaults(func=cmd_run)
    return p


if __name__ == "__main__":
    raise SystemExit(parser().parse_args().func(parser().parse_args()))
