#!/usr/bin/env python3
"""Unified Sleelavirin scan entry point with native and optional ClamAV engines."""
import argparse
from pathlib import Path
import subprocess
import sys


def main() -> int:
    tools_dir = Path(__file__).resolve().parent
    package_dir = tools_dir.parent
    parser = argparse.ArgumentParser(description="Scan files with Sleelavirin's selected backend.")
    parser.add_argument("--engine", choices=("native", "clamav"), default="native",
                        help="native SHA-256 signatures (default) or installed ClamAV")
    parser.add_argument("--database", default=str(package_dir / "signatures" / "native-sha256.txt"),
                        help="native SHA-256 signature database")
    parser.add_argument("--clamscan", default="clamscan", help="ClamAV executable when --engine clamav")
    parser.add_argument("paths", nargs="+", help="file or directory paths to scan")
    args = parser.parse_args()

    if args.engine == "native":
        executable = package_dir / "build" / "sleelavirin-scan"
        if not executable.is_file():
            print("ERROR: native scanner is not built; run 'make -C antivirus' first.", file=sys.stderr)
            return 2
        command = [str(executable), "--database", args.database, *args.paths]
    else:
        adapter = tools_dir / "sleelavirin_clamav.py"
        command = [sys.executable, str(adapter), "--clamscan", args.clamscan, *args.paths]

    try:
        return subprocess.run(command, check=False).returncode
    except OSError as exc:
        print(f"ERROR: unable to start selected scanner: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
