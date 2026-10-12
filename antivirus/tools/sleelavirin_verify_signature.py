#!/usr/bin/env python3
"""Verify a detached OpenSSL signature for a release artifact or signature index.

Example:
  python3 tools/sleelavirin_verify_signature.py --public-key release-public.pem \
      --signature native-sha256.txt.sig --file native-sha256.txt

The trusted public key must be obtained through an independent trusted channel.
"""
import argparse
import shutil
import subprocess
import sys
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--public-key", required=True, type=Path)
    parser.add_argument("--signature", required=True, type=Path)
    parser.add_argument("--file", required=True, type=Path)
    parser.add_argument("--openssl", default="openssl")
    args = parser.parse_args()
    for label, path in (("public key", args.public_key), ("signature", args.signature), ("artifact", args.file)):
        if not path.is_file():
            print("INVALID: missing " + label + ": " + str(path), file=sys.stderr)
            return 2
    openssl = shutil.which(args.openssl)
    if not openssl:
        print("ERROR: OpenSSL executable not found: " + args.openssl, file=sys.stderr)
        return 2
    try:
        result = subprocess.run(
            [openssl, "dgst", "-sha256", "-verify", str(args.public_key),
             "-signature", str(args.signature), str(args.file)],
            check=False, capture_output=True, text=True, timeout=30,
        )
    except (OSError, subprocess.TimeoutExpired) as exc:
        print("ERROR: signature verification could not complete: " + str(exc), file=sys.stderr)
        return 2
    if result.returncode != 0 or "Verified OK" not in result.stdout:
        print("INVALID: detached signature verification failed", file=sys.stderr)
        return 1
    print("VALID: detached SHA-256 signature verified for " + str(args.file))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
