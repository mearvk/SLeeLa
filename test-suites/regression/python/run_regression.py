#!/usr/bin/env python3
"""SLeeLa regression runner (C, C++ and Python suites).

    python3 test-suites/regression/python/run_regression.py            # everything
    python3 test-suites/regression/python/run_regression.py --fast     # skip mutation testing
    python3 test-suites/regression/python/run_regression.py --only native cli
    python3 test-suites/regression/python/run_regression.py --list

Groups
    native     builds and runs the C and C++ suites (strict flags, leak/heap-guard checked)
    ctypes     the C core driven from Python against Python oracles
    cli        the `sleela` compiler: language semantics, contract, transpilers
    integrity  SHA-256 gate tooling, committed manifest, /lib inventory
    mutation   injects defects into the code under test; every one must be caught

Exit status 0 only when every test passes.  KNOWN ISSUES (defects the suites
have found and that are still open) are listed at the end; they do not change the
exit status.  Environment: CC, CXX, SLEELA_BIN, REG_SANITIZE=1 (ASan/UBSan, when
the toolchain ships them), REG_SKIP_MUTATION=1.
"""
from __future__ import annotations

import argparse
import json
import os
import sys
import time
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))

import regression_support as rs  # noqa: E402

GROUPS = {
    "native": ["test_native_suites"],
    "ctypes": ["test_ctypes_differential"],
    "cli": ["test_cli_language", "test_cli_contract", "test_cli_transpilers"],
    "integrity": ["test_integrity_tools"],
    "mutation": ["test_mutation_kill"],
}


def describe_environment() -> None:
    import shutil
    import subprocess
    print("environment")
    print(f"  repository : {rs.ROOT}")
    for tool in (rs.CC, rs.CXX):
        found = shutil.which(tool)
        ver = ""
        if found:
            ver = subprocess.run([found, "--version"], capture_output=True, text=True).stdout.splitlines()[0]
        print(f"  {tool:<10} : {ver or 'NOT FOUND'}")
    binary = rs.sleela_binary()
    if binary:
        banner = subprocess.run([str(binary), "version"], capture_output=True, text=True, env=rs.cli_env()).stdout
        line = next((ln for ln in banner.splitlines() if "SLeeLa" in ln), "?")
        print(f"  sleela     : {binary}\n               {line}")
    else:
        print("  sleela     : NOT FOUND (CLI suites will be skipped; build impl/build/sleela)")
    print(f"  java/javac : {'yes' if shutil.which('javac') and shutil.which('java') else 'no (JVM differential skipped)'}")
    print(f"  sanitizers : {'ASan+UBSan requested' if os.environ.get('REG_SANITIZE') == '1' else 'off (REG_SANITIZE=1 to enable)'}")
    print()


def print_known_issues() -> None:
    if not rs.KNOWN_FILE.exists():
        return
    found = json.loads(rs.KNOWN_FILE.read_text())
    merged: dict = {}
    for e in found:
        cur = merged.setdefault(e["id"], {"open": False, "description": e["description"], "sources": set()})
        cur["open"] = cur["open"] or e["open"]
        cur["sources"].add(e["source"])
        if e["open"] and not e["description"].startswith("KNOWN-ISSUE"):
            cur["description"] = e["description"]
    if not merged:
        return
    print("\nKNOWN ISSUES (reported by the suites; they do not fail the run)")
    for ident in sorted(merged):
        e = merged[ident]
        state = "OPEN " if e["open"] else "FIXED"
        text = e["description"]
        if text.startswith("KNOWN-ISSUE"):
            text = text.split(":", 1)[-1].strip()
        print(f"  [{state}] {ident:<22} {text}")
        print(f"          found by: {', '.join(sorted(e['sources']))}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--only", nargs="+", choices=sorted(GROUPS), help="run only these groups")
    ap.add_argument("--fast", action="store_true", help="skip mutation testing")
    ap.add_argument("--sanitize", action="store_true", help="build with -fsanitize=address,undefined")
    ap.add_argument("--list", action="store_true", help="list tests and exit")
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("-k", dest="pattern", help="only tests whose id contains this text")
    ap.add_argument("--clean", action="store_true", help="delete build output and exit")
    args = ap.parse_args()

    if args.clean:
        import shutil
        shutil.rmtree(rs.BUILD, ignore_errors=True)
        return 0
    if args.fast:
        os.environ["REG_SKIP_MUTATION"] = "1"
    if args.sanitize:
        os.environ["REG_SANITIZE"] = "1"

    groups = args.only or [g for g in GROUPS if not (args.fast and g == "mutation")]
    loader = unittest.TestLoader()
    suite = unittest.TestSuite()
    for g in groups:
        for module in GROUPS[g]:
            suite.addTests(loader.loadTestsFromName(module))

    def flatten(s):
        for t in s:
            if isinstance(t, unittest.TestSuite):
                yield from flatten(t)
            else:
                yield t
    tests = [t for t in flatten(suite) if not args.pattern or args.pattern in t.id()]
    if args.list:
        for t in tests:
            print(t.id())
        print(f"{len(tests)} tests")
        return 0

    try:
        rs.sleela_binary()
    except rs.CompilerSelectionError as exc:
        print(f"run_regression: {exc}", file=sys.stderr)
        return 2

    rs.BUILD.mkdir(parents=True, exist_ok=True)
    if rs.KNOWN_FILE.exists():
        rs.KNOWN_FILE.unlink()
    describe_environment()
    sys.stdout.flush()

    selected = unittest.TestSuite(tests)
    started = time.time()
    result = unittest.TextTestRunner(verbosity=2 if args.verbose else 1, buffer=False).run(selected)
    elapsed = time.time() - started
    sys.stderr.flush()

    print_known_issues()
    rs.cleanup_scratch()
    ok = result.wasSuccessful()
    print(f"\nREGRESSION {'PASS' if ok else 'FAIL'}: {result.testsRun} tests, {len(result.failures)} failed, "
          f"{len(result.errors)} errors, {len(result.skipped)} skipped in {elapsed:.0f}s")
    for test, _ in result.failures + result.errors:
        print(f"  FAILED: {test.id()}")
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
