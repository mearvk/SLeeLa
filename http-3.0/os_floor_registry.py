"""os_floor_registry.py -- SLeeLa install-time "tidiest fit" OS-floor discovery.

When a SLeeLa executable installs on a host, it wants the *tidiest fit*: it
looks at the operating-system "floor" it landed on (OS + filesystem), confers
with any capability responder present (TAC3 or another, via the bonding collar),
and records what it found. Over time SLeeLa keeps a growing **registry of OS
floors / filesystems** it has seen, with rollup statistics, and can *advance*
new or updated discoveries to a known GitHub endpoint (see os_floor_report.py).

DESIGN STANCE (matches the bonding collar):
  - Observe, don't misrepresent. A floor's bonded tier is exactly what the
    collar attested -- never guessed.
  - Fail closed / no leakage. Detection makes no privileged calls and records
    only non-identifying descriptors (OS name, filesystem hint, bonded tier,
    a derived fingerprint). No secrets, no user PII, no host-proprietary
    internals (the collar already guarantees the last point).
  - Portable. Uses only the standard library so it runs on any host SLeeLa
    reaches, including a new/unknown OS.
"""
from __future__ import annotations

import hashlib
import json
import os
import platform
import time
from dataclasses import dataclass, field, asdict
from typing import Dict, List, Optional

from bonding_collar import (
    BondingCollar, CollarOffer, probe_env_responder,
    CAP_ALL, tier_name, MAC_KEY_BYTES,
)

REGISTRY_VERSION = 1


def _now_iso() -> str:
    return time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())


# --------------------------------------------------------------------------
# A single observed OS floor
# --------------------------------------------------------------------------
@dataclass
class OsFloor:
    """One OS/filesystem floor SLeeLa has landed on, and the advantage it confers."""
    floor_id: str                 # stable fingerprint (dedupe key)
    os_name: str                  # e.g. "Linux", "Darwin", "Windows", or "Unknown"
    os_release: str               # kernel/OS release string (non-identifying)
    filesystem: str               # detected/declared fs hint, or "unknown"
    bonded_tier: int              # bonding-collar tier (0 = NONE / unknown)
    bonded_tier_name: str
    responder: str                # "tac3", "generic", or "none"
    first_seen: str = ""
    last_seen: str = ""
    seen_count: int = 0
    notes: str = ""

    def to_dict(self) -> dict:
        return asdict(self)


def _fingerprint(os_name: str, os_release: str, filesystem: str, responder: str) -> str:
    """Stable, non-identifying dedupe key for a floor (12 hex chars)."""
    h = hashlib.sha256(f"{os_name}|{os_release}|{filesystem}|{responder}".encode("utf-8"))
    return "floor-" + h.hexdigest()[:12]


def detect_filesystem_hint() -> str:
    """A conservative, non-privileged guess at the host filesystem 'floor'.

    Reads only what the platform readily exposes; never mounts, never calls a
    privileged tool. Returns "unknown" when nothing safe is available.
    """
    # A host (or a responder like TAC3) may declare the floor explicitly.
    declared = os.environ.get("SLEELA_FS_FLOOR")
    if declared:
        return declared.strip()[:64]
    sysname = platform.system()
    if sysname == "Linux":
        # /proc/mounts is world-readable and non-privileged where present.
        try:
            root_fs = None
            with open("/proc/mounts", "r", encoding="utf-8", errors="replace") as fh:
                for line in fh:
                    parts = line.split()
                    if len(parts) >= 3 and parts[1] == "/":
                        root_fs = parts[2]
                        break
            if root_fs:
                return root_fs
        except OSError:
            pass
    elif sysname == "Darwin":
        return "apfs?"       # best-effort hint without privileged calls
    elif sysname == "Windows":
        return "ntfs?"
    return "unknown"


def confer_with_host(key: bytes, *, env: Optional[dict] = None) -> tuple:
    """Confer via the bonding collar. Returns (bonded_tier, tier_name, responder).

    responder is "tac3" when the host declares SLEELA_OS_RESPONDER=tac3 alongside
    a valid attestation, "generic" for any other attested responder, else "none".
    """
    e = env if env is not None else os.environ
    collar = BondingCollar(local_caps=CAP_ALL, key=key)
    offer = probe_env_responder(key, env=e)
    result = collar.confer(offer)
    if not result.bonded:
        return (result.tier, result.tier_name, "none")
    responder = (e.get("SLEELA_OS_RESPONDER") or "generic").strip().lower()[:32]
    if responder not in ("tac3", "generic"):
        responder = "generic"
    return (result.tier, result.tier_name, responder)


def observe_current_floor(key: bytes, *, env: Optional[dict] = None) -> OsFloor:
    """Detect the current OS floor and confer for its bonded advantage tier."""
    e = env if env is not None else os.environ
    os_name = platform.system() or "Unknown"
    os_release = (platform.release() or "")[:96]
    fs_hint = detect_filesystem_hint()
    tier, tname, responder = confer_with_host(key, env=e)
    fid = _fingerprint(os_name, os_release, fs_hint, responder)
    now = _now_iso()
    return OsFloor(
        floor_id=fid, os_name=os_name, os_release=os_release, filesystem=fs_hint,
        bonded_tier=tier, bonded_tier_name=tname, responder=responder,
        first_seen=now, last_seen=now, seen_count=1,
    )


# --------------------------------------------------------------------------
# The registry (deduplicated, persistent)
# --------------------------------------------------------------------------
class OsFloorRegistry:
    """A growing, deduplicated list of OS floors/filesystems SLeeLa has seen."""

    def __init__(self, path: Optional[str] = None) -> None:
        self.path = path
        self._floors: Dict[str, OsFloor] = {}
        if path and os.path.exists(path):
            self.load(path)

    # ---- persistence ----
    def load(self, path: Optional[str] = None) -> None:
        p = path or self.path
        with open(p, "r", encoding="utf-8") as fh:
            doc = json.load(fh)
        self._floors = {}
        for entry in doc.get("floors", []):
            f = OsFloor(**entry)
            self._floors[f.floor_id] = f

    def save(self, path: Optional[str] = None) -> None:
        p = path or self.path
        if not p:
            raise ValueError("no path to save to")
        tmp = p + ".tmp"
        with open(tmp, "w", encoding="utf-8") as fh:
            fh.write(self.to_json())
        os.replace(tmp, p)

    # ---- mutation ----
    def record(self, floor: OsFloor) -> tuple:
        """Add or update a floor. Returns (floor, is_new).

        Dedupe key is floor_id. On re-observation we bump seen_count/last_seen
        and upgrade the bonded tier if the host now attests to a higher one.
        """
        existing = self._floors.get(floor.floor_id)
        if existing is None:
            self._floors[floor.floor_id] = floor
            return (floor, True)
        existing.seen_count += 1
        existing.last_seen = floor.last_seen
        if floor.bonded_tier > existing.bonded_tier:
            existing.bonded_tier = floor.bonded_tier
            existing.bonded_tier_name = floor.bonded_tier_name
            existing.responder = floor.responder
        return (existing, False)

    # ---- access ----
    def floors(self) -> List[OsFloor]:
        return sorted(self._floors.values(), key=lambda f: (-f.bonded_tier, f.floor_id))

    def get(self, floor_id: str) -> Optional[OsFloor]:
        return self._floors.get(floor_id)

    def stats(self) -> dict:
        """Rollup statistics over the registry (non-identifying)."""
        floors = list(self._floors.values())
        by_tier: Dict[str, int] = {}
        by_os: Dict[str, int] = {}
        by_fs: Dict[str, int] = {}
        by_responder: Dict[str, int] = {}
        total_observations = 0
        bonded = 0
        for f in floors:
            by_tier[f.bonded_tier_name] = by_tier.get(f.bonded_tier_name, 0) + 1
            by_os[f.os_name] = by_os.get(f.os_name, 0) + 1
            by_fs[f.filesystem] = by_fs.get(f.filesystem, 0) + 1
            by_responder[f.responder] = by_responder.get(f.responder, 0) + 1
            total_observations += f.seen_count
            if f.bonded_tier > 0:
                bonded += 1
        return {
            "distinct_floors": len(floors),
            "total_observations": total_observations,
            "bonded_floors": bonded,
            "unbonded_floors": len(floors) - bonded,
            "by_tier": by_tier,
            "by_os": by_os,
            "by_filesystem": by_fs,
            "by_responder": by_responder,
        }

    def to_json(self, *, indent: int = 2) -> str:
        doc = {
            "registry_version": REGISTRY_VERSION,
            "generated": _now_iso(),
            "floor_count": len(self._floors),
            "stats": self.stats(),
            "floors": [f.to_dict() for f in self.floors()],
        }
        return json.dumps(doc, indent=indent)


if __name__ == "__main__":
    # No-network self-demo: observe this host's floor and print it.
    key = b"\x3c" * MAC_KEY_BYTES
    reg = OsFloorRegistry()
    floor = observe_current_floor(key)
    f, is_new = reg.record(floor)
    print(f"observed floor: {f.os_name} / {f.filesystem} -> tier={f.bonded_tier_name} "
          f"responder={f.responder} (new={is_new})")
    print(reg.to_json())
