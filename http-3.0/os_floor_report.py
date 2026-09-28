"""os_floor_report.py -- advance new/updated OS floors to a known GitHub endpoint.

SLeeLa discovers OS floors (os_floor_registry.py) and can *advance* new or
updated discoveries to a known GitHub endpoint via an HTTP POST, so the catalog
of OS floors/filesystems it has met grows in one shared place.

SAFETY / CONSENT CONTRACT (this is telemetry, so it is explicit, not covert):
  - OPT-IN. Disabled unless BOTH an endpoint and a token are configured. With
    nothing configured, report() is a no-op that returns a "disabled" result.
  - NON-IDENTIFYING PAYLOAD. Sends only the floor descriptors the registry
    already holds (OS name, OS release string, filesystem hint, bonded tier,
    responder, fingerprint, counts). It NEVER sends secrets, tokens, host names,
    IPs, usernames, paths, or any host-proprietary internals.
  - NO HARDCODED CREDENTIALS. The token and endpoint come from configuration or
    the environment at call time; nothing is baked in.
  - FAIL CLOSED. Any network/config error is caught and reported as a failed
    (but non-fatal) result; discovery and the local registry are never blocked
    by the reporter.

Configuration (env, or passed to ReporterConfig):
  SLEELA_FLOOR_REPORT_URL    full GitHub API URL to POST to, e.g.
                             https://api.github.com/repos/OWNER/REPO/dispatches
  SLEELA_FLOOR_REPORT_TOKEN  a GitHub token with the minimum needed scope
  SLEELA_FLOOR_REPORT_EVENT  (optional) repository_dispatch event_type
                             (default: "sleela-os-floor")

The default shape targets GitHub's repository_dispatch REST endpoint, which is a
clean, low-privilege way for automated software to post a structured event to a
known repo. A custom transport can be injected for testing (see `transport=`).
"""
from __future__ import annotations

import json
import os
from dataclasses import dataclass
from typing import Callable, List, Optional

from os_floor_registry import OsFloor, OsFloorRegistry


@dataclass
class ReporterConfig:
    url: Optional[str] = None
    token: Optional[str] = None
    event_type: str = "sleela-os-floor"

    @staticmethod
    def from_env(env: Optional[dict] = None) -> "ReporterConfig":
        e = env if env is not None else os.environ
        return ReporterConfig(
            url=e.get("SLEELA_FLOOR_REPORT_URL"),
            token=e.get("SLEELA_FLOOR_REPORT_TOKEN"),
            event_type=e.get("SLEELA_FLOOR_REPORT_EVENT", "sleela-os-floor"),
        )

    def enabled(self) -> bool:
        return bool(self.url and self.token)


@dataclass
class ReportResult:
    ok: bool
    status: str            # "disabled" | "sent" | "error" | "nothing-to-send"
    detail: str = ""
    posted: int = 0        # number of floors included in the payload


def _sanitize(floor: OsFloor) -> dict:
    """Whitelist exactly the non-identifying fields we are willing to advance."""
    return {
        "floor_id": floor.floor_id,
        "os_name": floor.os_name,
        "os_release": floor.os_release,
        "filesystem": floor.filesystem,
        "bonded_tier": floor.bonded_tier,
        "bonded_tier_name": floor.bonded_tier_name,
        "responder": floor.responder,
        "seen_count": floor.seen_count,
        "first_seen": floor.first_seen,
        "last_seen": floor.last_seen,
    }


def build_payload(floors: List[OsFloor], event_type: str) -> dict:
    """GitHub repository_dispatch payload carrying the sanitized floor list."""
    return {
        "event_type": event_type,
        "client_payload": {
            "kind": "sleela.os_floor.discovery",
            "schema": 1,
            "floors": [_sanitize(f) for f in floors],
        },
    }


# Transport is injectable so tests can run without real network access.
# Signature: transport(url, headers: dict, body: bytes) -> (status_code, text)
Transport = Callable[[str, dict, bytes], tuple]


def _urllib_transport(url: str, headers: dict, body: bytes) -> tuple:
    import urllib.request
    import urllib.error
    req = urllib.request.Request(url, data=body, headers=headers, method="POST")
    try:
        with urllib.request.urlopen(req, timeout=15) as resp:
            return (resp.status, resp.read().decode("utf-8", "replace"))
    except urllib.error.HTTPError as e:
        return (e.code, e.read().decode("utf-8", "replace"))


def report_floors(
    floors: List[OsFloor],
    config: Optional[ReporterConfig] = None,
    *,
    transport: Optional[Transport] = None,
) -> ReportResult:
    """Advance the given floors to the configured GitHub endpoint.

    Fail-closed: returns a ReportResult describing the outcome and never raises
    for network/config problems (so discovery is never blocked by reporting).
    """
    cfg = config or ReporterConfig.from_env()
    if not cfg.enabled():
        return ReportResult(False, "disabled",
                            "reporting is opt-in; set SLEELA_FLOOR_REPORT_URL and "
                            "SLEELA_FLOOR_REPORT_TOKEN to enable")
    if not floors:
        return ReportResult(True, "nothing-to-send", "no floors to advance", 0)

    payload = build_payload(floors, cfg.event_type)
    body = json.dumps(payload).encode("utf-8")
    headers = {
        "Authorization": f"Bearer {cfg.token}",
        "Accept": "application/vnd.github+json",
        "Content-Type": "application/json",
        "User-Agent": "sleela-os-floor-reporter",
        "X-GitHub-Api-Version": "2022-11-28",
    }
    send = transport or _urllib_transport
    try:
        status, text = send(cfg.url, headers, body)
    except Exception as exc:  # fail closed on any transport error
        return ReportResult(False, "error", f"transport error: {exc}", 0)
    if 200 <= status < 300:
        return ReportResult(True, "sent", f"HTTP {status}", len(floors))
    return ReportResult(False, "error", f"HTTP {status}: {text[:200]}", 0)


def advance_new_floors(
    registry: OsFloorRegistry,
    new_floor_ids: List[str],
    config: Optional[ReporterConfig] = None,
    *,
    transport: Optional[Transport] = None,
) -> ReportResult:
    """Convenience: advance only the floors whose ids are in `new_floor_ids`."""
    floors = [registry.get(fid) for fid in new_floor_ids]
    floors = [f for f in floors if f is not None]
    return report_floors(floors, config, transport=transport)


if __name__ == "__main__":
    # No-network self-demo: show disabled-by-default, then a mocked successful POST.
    from os_floor_registry import observe_current_floor, MAC_KEY_BYTES
    key = b"\x3c" * MAC_KEY_BYTES
    reg = OsFloorRegistry()
    floor = observe_current_floor(key)
    reg.record(floor)

    print("unconfigured ->", report_floors(reg.floors()).__dict__)

    captured = {}
    def mock(url, headers, body):
        captured["url"] = url
        captured["auth_present"] = "Authorization" in headers
        captured["body"] = json.loads(body.decode())
        return (204, "")
    cfg = ReporterConfig(url="https://api.github.com/repos/OWNER/REPO/dispatches",
                        token="MOCK", event_type="sleela-os-floor")
    print("mocked POST  ->", report_floors(reg.floors(), cfg, transport=mock).__dict__)
    print("payload kind ->", captured["body"]["client_payload"]["kind"],
          "| floors:", len(captured["body"]["client_payload"]["floors"]))
