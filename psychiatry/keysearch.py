#!/usr/bin/env python3
"""keysearch.py -- ping GitHub to check whether the local Secret.key matches
the one published in the repository.

Invoked from the SLeeLa terminal as:

    $> SLeeLa keysearch

It performs a **read-only HTTPS GET** of the repository's Secret.key and compares
it to the local copy. To avoid ever transmitting or logging the key material, the
comparison is done by **SHA-256 digest**, not by sending the raw key anywhere:

  * the local key is hashed locally;
  * the remote key is fetched and hashed;
  * only the two 64-hex-char digests are compared and (optionally) shown.

Result:
    MATCH      local and remote keys are byte-identical (same SHA-256)
    MISMATCH   both present but differ
    NOT FOUND  the remote key could not be fetched (missing/offline/HTTP error)
    ERROR      the local key is missing or unreadable

Safety stance (matches the bonding collar / OS-floor reporter):
  * GET only -- it never uploads or POSTs the secret anywhere.
  * Digest comparison -- the raw key never leaves the machine and is never logged.
  * Fail closed -- any network/HTTP problem yields NOT FOUND, never a false MATCH.

Configuration:
  SLEELA_KEY_URL   override the remote URL (default: the repo's main branch raw URL)
  SLEELA_KEY_PATH  override the local key path (default: alongside this script)
"""
from __future__ import annotations

import hashlib
import os
import sys
from pathlib import Path
from typing import Callable, Optional, Tuple

DEFAULT_URL = "https://raw.githubusercontent.com/mearvk/SLeeLa/main/psychiatry/Secret.key"
DEFAULT_LOCAL = Path(__file__).resolve().parent / "Secret.key"

# Transport is injectable so tests can run without real network access.
# Signature: fetch(url) -> (status_int, body_bytes).  Raise on transport failure.
Fetch = Callable[[str], Tuple[int, bytes]]


def _urllib_fetch(url: str) -> Tuple[int, bytes]:
    import urllib.request
    import urllib.error
    req = urllib.request.Request(url, method="GET",
                                 headers={"User-Agent": "sleela-keysearch"})
    try:
        with urllib.request.urlopen(req, timeout=15) as resp:
            return (resp.status, resp.read())
    except urllib.error.HTTPError as e:
        return (e.code, b"")


def _sha256_hex(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _normalize(raw: bytes) -> bytes:
    # Compare on the key's hex content, ignoring a trailing newline / surrounding
    # whitespace so a stored "...\n" matches a fetched "..." of the same digits.
    return raw.strip()


def keysearch(url: Optional[str] = None,
              local_path: Optional[Path] = None,
              *,
              fetch: Optional[Fetch] = None) -> Tuple[str, str]:
    """Return (result, detail). result in {MATCH, MISMATCH, NOT FOUND, ERROR}."""
    url = url or os.environ.get("SLEELA_KEY_URL", DEFAULT_URL)
    local_path = Path(local_path or os.environ.get("SLEELA_KEY_PATH", DEFAULT_LOCAL))

    # Local key
    try:
        local = _normalize(local_path.read_bytes())
    except OSError as e:
        return ("ERROR", f"local key unreadable: {e}")
    if not local:
        return ("ERROR", f"local key is empty: {local_path}")
    local_digest = _sha256_hex(local)

    # Remote key (GET only; fail closed on any problem)
    send = fetch or _urllib_fetch
    try:
        status, body = send(url)
    except Exception as e:  # noqa: BLE001 -- fail closed on any transport error
        return ("NOT FOUND", f"fetch failed: {e}")
    if status < 200 or status >= 300 or not body:
        return ("NOT FOUND", f"remote unavailable (HTTP {status})")
    remote_digest = _sha256_hex(_normalize(body))

    if local_digest == remote_digest:
        return ("MATCH", f"sha256={local_digest} ({len(local)} hex digits)")
    return ("MISMATCH", f"local={local_digest} remote={remote_digest}")


def main(argv=None) -> int:
    argv = list(sys.argv[1:] if argv is None else argv)
    url = argv[0] if argv else None
    result, detail = keysearch(url=url)
    print(f"SLeeLa keysearch: {result}")
    print(f"  {detail}")
    # Exit code: 0 match, 1 mismatch, 2 not found, 3 error (scriptable).
    return {"MATCH": 0, "MISMATCH": 1, "NOT FOUND": 2, "ERROR": 3}.get(result, 3)


if __name__ == "__main__":
    raise SystemExit(main())
