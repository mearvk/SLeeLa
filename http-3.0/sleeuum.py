"""sleeuum.py -- Sleeuum(TM), the packet ledger client for SLeeLa executables.

Sleeuum(TM) is the client-side module a SLeeLa executable uses to KEEP TRACK of
the HTTP 3.0 / HTTP 2.0+ packets it sends and receives -- together with their
DATES and their NUMBERS. It does not move bytes itself; it observes the bytes
that the packet builder (http3_transport.py) and the H3 pipeline (http3_flow.py)
already produce, and records a durable, queryable ledger of them.

What Sleeuum(TM) tracks, per the request:

  * PACKETS -- one ledger entry per observed H3 packet, tagged with its HTTP
        carrier (HTTP/1.1, HTTP/2, or raw H3), direction (sent / received),
        service-id, op-id, request-id, wire form (text / binary), and byte size.
  * DATES   -- an ISO-8601 UTC timestamp on every entry, plus the session's
        first-seen and last-seen instants and total duration.
  * NUMBERS -- a monotonic Sleeuum sequence number per entry, the H3 NONCE and
        DIGEST, request-ids, running byte counters, per-status tallies, and the
        BASKET NUMERALS (atomic-number item ids) carried in each packet.

Design stance matches the rest of the stack: observe, don't misrepresent. The
ledger never alters a packet, never forges an integrity field, and reports the
H3 status exactly as the pipeline returned it.

Trademark: "Sleeuum(TM)" -- rendered SLEEUUM_TM below; use the unicode mark
where the display supports it.
"""
from __future__ import annotations

import json
from dataclasses import dataclass, field, asdict
from datetime import datetime, timezone
from typing import Dict, List, Optional

from http3_flow import Envelope, Response, Status, basket_parse
import http3_transport as transport

# Trademark rendering. Prefer the unicode symbol; the ASCII form is a fallback.
SLEEUUM_TM = "Sleeuum\u2122"          # Sleeuum™
SLEEUUM_TM_ASCII = "Sleeuum(TM)"
MODULE_NAME = SLEEUUM_TM


def _now_iso() -> str:
    """Current instant as an ISO-8601 UTC string with a trailing 'Z'."""
    return datetime.now(timezone.utc).isoformat(timespec="microseconds").replace("+00:00", "Z")


# --------------------------------------------------------------------------
# Ledger entry -- one observed packet
# --------------------------------------------------------------------------

@dataclass
class PacketRecord:
    """A single tracked HTTP 3.0 / HTTP 2.0+ packet: its dates and its numbers."""

    # --- Sleeuum numbering ---
    seq: int                       # monotonic Sleeuum sequence number (1-based)
    # --- date ---
    at: str                        # ISO-8601 UTC timestamp of observation
    # --- packet identity ---
    direction: str                 # "sent" | "received"
    carrier: str                   # "http/1.1" | "http/2" | "h3-raw"
    kind: str                      # "request" | "response"
    wire_form: str                 # "text" | "binary"
    # --- H3 numbers (request packets) ---
    service_id: Optional[int] = None
    op_id: Optional[int] = None
    request_id: Optional[int] = None
    nonce: Optional[int] = None
    digest: Optional[int] = None
    intactx: Optional[int] = None
    # --- H3 numbers (response packets) ---
    status: Optional[int] = None
    status_name: Optional[str] = None
    # --- sizes ---
    payload_bytes: int = 0
    wire_bytes: int = 0
    # --- basket numerals carried in this packet (atomic-number item ids) ---
    numerals: List[int] = field(default_factory=list)

    def to_dict(self) -> dict:
        return {k: v for k, v in asdict(self).items() if v is not None}


# --------------------------------------------------------------------------
# The Sleeuum ledger
# --------------------------------------------------------------------------

@dataclass
class SleeuumStats:
    """Rolled-up NUMBERS across everything the ledger has seen."""
    packets: int = 0
    sent: int = 0
    received: int = 0
    requests: int = 0
    responses: int = 0
    total_payload_bytes: int = 0
    total_wire_bytes: int = 0
    by_carrier: Dict[str, int] = field(default_factory=dict)
    by_status: Dict[str, int] = field(default_factory=dict)
    first_seen: Optional[str] = None
    last_seen: Optional[str] = None

    def duration_seconds(self) -> float:
        if not self.first_seen or not self.last_seen:
            return 0.0
        a = datetime.fromisoformat(self.first_seen.replace("Z", "+00:00"))
        b = datetime.fromisoformat(self.last_seen.replace("Z", "+00:00"))
        return (b - a).total_seconds()


class Sleeuum:
    """Sleeuum(TM) -- client ledger of HTTP 3.0 / HTTP 2.0+ packets for a SLeeLa exe.

    Usage:

        led = Sleeuum(name="my-exe")
        wire = transport.build_http2(env, key, include_preface=True)
        led.track_sent(env, carrier=transport.Carrier.HTTP2, wire=wire)
        ...
        led.track_received_wire(response_wire, carrier="http/2")
        print(led.report())
    """

    def __init__(self, name: str = "sleela-exe") -> None:
        self.name = name
        self._records: List[PacketRecord] = []
        self._seq = 0
        self.stats = SleeuumStats()

    # ---- numbering / dating helpers -------------------------------------
    def _next_seq(self) -> int:
        self._seq += 1
        return self._seq

    def _stamp(self, rec: PacketRecord) -> PacketRecord:
        self._records.append(rec)
        s = self.stats
        s.packets += 1
        s.total_payload_bytes += rec.payload_bytes
        s.total_wire_bytes += rec.wire_bytes
        s.by_carrier[rec.carrier] = s.by_carrier.get(rec.carrier, 0) + 1
        if rec.direction == "sent":
            s.sent += 1
        else:
            s.received += 1
        if rec.kind == "request":
            s.requests += 1
        else:
            s.responses += 1
            if rec.status_name:
                s.by_status[rec.status_name] = s.by_status.get(rec.status_name, 0) + 1
        if s.first_seen is None:
            s.first_seen = rec.at
        s.last_seen = rec.at
        return rec

    @staticmethod
    def _numerals_of(env: Envelope) -> List[int]:
        """Basket numerals (atomic-number item ids) carried in this packet.

        basket_parse() returns (iso_numeric, [(number, value), ...]); the numeral
        is each item's `number` (its atomic-number id, per NUMERAL.md).
        """
        try:
            _iso, items = basket_parse(env.basket)
            return [int(number) for (number, _value) in items]
        except Exception:
            return []

    # ---- tracking API ----------------------------------------------------
    def track_sent(self, env: Envelope, *, carrier: str, wire: bytes) -> PacketRecord:
        """Record a request packet the executable is sending."""
        return self._stamp(PacketRecord(
            seq=self._next_seq(), at=_now_iso(), direction="sent", carrier=carrier,
            kind="request",
            wire_form=("binary" if (env.flags & 0x01) else "text"),
            service_id=env.service_id, op_id=env.op_id, request_id=env.request_id,
            nonce=env.nonce, digest=env.digest, intactx=env.intactx,
            payload_bytes=len(env.payload), wire_bytes=len(wire),
            numerals=self._numerals_of(env),
        ))

    def track_received_wire(self, wire: bytes, *, carrier: str = "h3-raw") -> PacketRecord:
        """Record a response packet received (raw H3R bytes or HTTP-wrapped)."""
        raw = wire
        # If it looks like an HTTP message, extract the embedded H3 bytes first.
        if wire[:5] == b"HTTP/" or wire[:3] in (b"GET", b"POS", b"PRI") or b" HTTP/1." in wire[:64]:
            try:
                raw = transport.unwrap(wire)
            except Exception:
                raw = wire
        status_val = None
        status_name = None
        rid = None
        rlen = 0
        try:
            resp = Response.unpack_text(raw)
            status_val = int(resp.status)
            status_name = Status(resp.status).name
            rid = resp.request_id
            rlen = len(resp.result)
        except Exception:
            pass
        return self._stamp(PacketRecord(
            seq=self._next_seq(), at=_now_iso(), direction="received", carrier=carrier,
            kind="response",
            wire_form=("text" if raw[:3] == b"H3R" else "binary"),
            request_id=rid, status=status_val, status_name=status_name,
            payload_bytes=rlen, wire_bytes=len(wire),
        ))

    # ---- reporting -------------------------------------------------------
    def records(self) -> List[PacketRecord]:
        return list(self._records)

    def to_json(self, *, indent: int = 2) -> str:
        """Serialize the full ledger (dates + numbers + packets) to JSON."""
        doc = {
            "module": SLEEUUM_TM,
            "trademark": SLEEUUM_TM,
            "client": self.name,
            "generated": _now_iso(),
            "stats": {
                **asdict(self.stats),
                "duration_seconds": round(self.stats.duration_seconds(), 6),
            },
            "packets": [r.to_dict() for r in self._records],
        }
        return json.dumps(doc, indent=indent, sort_keys=False)

    def report(self) -> str:
        """Short human-readable summary line."""
        s = self.stats
        return (
            f"{SLEEUUM_TM} [{self.name}] "
            f"packets={s.packets} (sent={s.sent} recv={s.received}) "
            f"bytes={s.total_wire_bytes} "
            f"carriers={s.by_carrier or '{}'} statuses={s.by_status or '{}'} "
            f"window={s.first_seen}..{s.last_seen}"
        )


if __name__ == "__main__":
    # Tiny self-demonstration (no network): build a couple of packets and track.
    import http3_flow as f
    key = b"\x11" * f.MAC_KEY_BYTES
    led = Sleeuum(name="demo")
    e = f.Envelope(service_id=7, op_id=3, request_id=1, payload=b"hi", nonce=1)
    e.seal(key)
    w = transport.build_http2(e, key, include_preface=True)
    led.track_sent(e, carrier=transport.Carrier.HTTP2, wire=w)
    led.track_received_wire(f.Response(status=f.Status.OK, request_id=1, result=b"ok").pack_text())
    print(led.report())
