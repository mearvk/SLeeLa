#!/usr/bin/env python3
"""Optional ClamAV adapter for Sleelavirin.

Uses the installed clamscan executable; it does not install ClamAV or update
its signature database. Exit codes: 0=no detection, 1=detection, 2=scan error.
"""
import argparse
import shutil
import subprocess
import sys


def main() -> int:
    parser = argparse.ArgumentParser(description="Scan files using the optional ClamAV engine.")
    parser.add_argument("--clamscan", default="clamscan", help="clamscan executable path")
    parser.add_argument("paths", nargs="+", help="file or directory paths to scan")
    args = parser.parse_args()

    executable = shutil.which(args.clamscan)
    if executable is None:
        print(f"ERROR: ClamAV executable not found: {args.clamscan}", file=sys.stderr)
        return 2

    command = [executable, "--infected", "--no-summary", "--", *args.paths]
    try:
        result = subprocess.run(command, check=False)
    except OSError as exc:
        print(f"ERROR: unable to start ClamAV: {exc}", file=sys.stderr)
        return 2

    if result.returncode == 0:
        print("CLAMAV_NO_DETECTION (not a guarantee that files are safe)")
        return 0
    if result.returncode == 1:
        print("CLAMAV_DETECTION")
        return 1
    print(f"CLAMAV_ERROR exit={result.returncode}", file=sys.stderr)
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
