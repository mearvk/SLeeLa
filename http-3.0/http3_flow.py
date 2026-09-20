"""Python reference for the HTTP 3.0 data flow (spec §4/§5/§7/§9).

This mirrors the C reference (http3_envelope.c / http3_naming.c /
http3_pipeline.c) at the level the spec actually defines: the compact
envelope, fast naming, the response model, and retry classes. It is
dependency-free so a Python connector can speak the protocol without
becoming a SLeeLa runtime (spec §20). Crypto is out of scope here; that
lives in the C substrate (crypto_openssl.c et al.).

Every packet also carries a NONCE (per-connection monotonic counter, covered by
the MAC, for replay rejection), a per-packet DIGEST (a 64-bit KEYED MAC,
SipHash-2-4, over the header + basket + payload under a per-connection secret
key), an INTACTX id (64-bit system-specific host-integrity identity), and the
fixed BASKET of 14 goods & services (atomic-bound to the US capitalism system,
ISO USD per gram) as a canonical block covered by the MAC. The keyed DIGEST
resists deliberate forgery, not just accidental corruption. A receiver rejects a
packet whose DIGEST does not verify under the shared key, RESETs the exchange
when a packet's INTACTX variance exceeds the tamper threshold, and rejects a
packet whose NONCE is not ahead of its high-water mark (a replay).

Textual wire forms (must match the C implementation byte-for-byte):

    envelope:  H3 <ver> <flags> <service_id> <op_id> <request_id> <nonce> <digest> <intactx> <basket-hex> <len>:<payload>\\n
    response:  H3R <status> <request_id> <len>:<result>\\n
"""
from __future__ import annotations

import os
import platform
import struct
from dataclasses import dataclass
from enum import IntEnum
from typing import Callable, Dict, Optional, Tuple

ENVELOPE_VERSION = 3
MAX_PAYLOAD = 4096

# ---- 64-bit FNV-1a (matches env_fnv1a in http3_envelope.c) ------------------
_FNV64_OFFSET = 0xCBF29CE484222325
_FNV64_PRIME = 0x100000001B3
_U64 = 0xFFFFFFFFFFFFFFFF


def _fnv1a(data: bytes, h: int = _FNV64_OFFSET) -> int:
    for byte in data:
        h ^= byte
        h = (h * _FNV64_PRIME) & _U64
    return h


# ---- SipHash-2-4 keyed MAC (matches http3_mac.c byte-for-byte) --------------
MAC_KEY_BYTES = 16


def _rotl64(x: int, b: int) -> int:
    return ((x << b) | (x >> (64 - b))) & _U64


def siphash24(key: bytes, data: bytes) -> int:
    """64-bit SipHash-2-4 keyed MAC. `key` is 16 bytes; returns a 64-bit int.

    Reference-conformant (Aumasson & Bernstein): 2 compression rounds per 8-byte
    block, 4 finalization rounds; key and message read little-endian.
    """
    if len(key) != MAC_KEY_BYTES:
        raise ValueError("SipHash key must be 16 bytes")
    k0 = int.from_bytes(key[0:8], "little")
    k1 = int.from_bytes(key[8:16], "little")
    v0 = 0x736F6D6570736575 ^ k0
    v1 = 0x646F72616E646F6D ^ k1
    v2 = 0x6C7967656E657261 ^ k0
    v3 = 0x7465646279746573 ^ k1

    def sipround(v0: int, v1: int, v2: int, v3: int):
        v0 = (v0 + v1) & _U64; v1 = _rotl64(v1, 13); v1 ^= v0; v0 = _rotl64(v0, 32)
        v2 = (v2 + v3) & _U64; v3 = _rotl64(v3, 16); v3 ^= v2
        v0 = (v0 + v3) & _U64; v3 = _rotl64(v3, 21); v3 ^= v0
        v2 = (v2 + v1) & _U64; v1 = _rotl64(v1, 17); v1 ^= v2; v2 = _rotl64(v2, 32)
        return v0, v1, v2, v3

    n = len(data)
    end = n - (n % 8)
    for off in range(0, end, 8):
        m = int.from_bytes(data[off:off + 8], "little")
        v3 ^= m
        v0, v1, v2, v3 = sipround(v0, v1, v2, v3)
        v0, v1, v2, v3 = sipround(v0, v1, v2, v3)
        v0 ^= m

    b = (n & 0xFF) << 56
    tail = data[end:]
    for i, byte in enumerate(tail):
        b |= byte << (8 * i)

    v3 ^= b
    v0, v1, v2, v3 = sipround(v0, v1, v2, v3)
    v0, v1, v2, v3 = sipround(v0, v1, v2, v3)
    v0 ^= b

    v2 ^= 0xFF
    for _ in range(4):
        v0, v1, v2, v3 = sipround(v0, v1, v2, v3)
    return (v0 ^ v1 ^ v2 ^ v3) & _U64


# ---- Basket of goods & services (mirrors http3_basket.{h,c} and BASKET.docx) --
# A fixed, carefully-selected set of 14 goods/services, atomic-bound to the US
# capitalism system. Each item: (atomic_number, value_micro_usd_per_gram, name).
# The name does NOT travel on the wire; only the number and per-gram value do.
BASKET_ITEMS = 14
BASKET_ISO_CURRENCY = "USD"
BASKET_ISO_NUMERIC = 840
BASKET_BLOCK_SIZE = 4 + BASKET_ITEMS * 12  # iso(2)+count(2) + per item number(4)+value(8)

BASKET = (
    (1, 56249759, "Bleached pulp paper"),
    (2, 34243099, "Broadband gigabyte (svc)"),
    (3, 69987683, "Cane sugar"),
    (4, 43104945, "Cobalt metal"),
    (5, 29864648, "Cured tobacco leaf"),
    (6, 17981345, "Freight ton-mile (svc)"),
    (7, 6610716, "Gold bullion"),
    (8, 12259381, "Legal counsel hour (svc)"),
    (9, 65041790, "Managed cloud-compute (svc)"),
    (10, 31566795, "Natural rubber"),
    (11, 24592204, "Portland cement"),
    (12, 65211966, "Raw cotton"),
    (13, 35009362, "Roasted coffee"),
    (14, 35480545, "Structural steel"),
)


def basket_serialize() -> bytes:
    """Canonical basket block: iso(2) + count(2), then per item number(4)+value(8).
    Big-endian; matches http3_basket_serialize() in C byte-for-byte."""
    out = struct.pack(">HH", BASKET_ISO_NUMERIC, BASKET_ITEMS)
    for number, value, _name in BASKET:
        out += struct.pack(">IQ", number & 0xFFFFFFFF, value & _U64)
    return out


def basket_parse(block: bytes):
    """Parse a canonical basket block -> (iso_numeric, [(number, value), ...])."""
    if len(block) < 4:
        raise ValueError("short basket block")
    iso, count = struct.unpack(">HH", block[:4])
    if len(block) < 4 + count * 12:
        raise ValueError("short basket block")
    items = []
    for i in range(count):
        number, value = struct.unpack(">IQ", block[4 + i * 12:4 + i * 12 + 12])
        items.append((number, value))
    return iso, items


class Flag(IntEnum):
    NONE = 0x00
    BINARY = 0x01
    COMPRESSED = 0x02
    STREAM = 0x04
    IDEMPOTENT = 0x08
    RESET = 0x10  # packet reset: host tampered/untrusted


class Status(IntEnum):
    OK = 0
    APP_ERROR = 1
    UNKNOWN_SERVICE = 2
    UNKNOWN_OPERATION = 3
    BAD_ENVELOPE = 4
    TOO_LARGE = 5
    RETRY_DENIED = 6
    BAD_DIGEST = 7  # per-packet DIGEST did not verify
    TAMPERED = 8    # INTACTX variance exceeded threshold
    REPLAYED = 9    # NONCE not ahead of high-water: replay


class RetryClass(IntEnum):
    READ = 0
    IDEMPOTENT = 1
    MUTATING = 2
    STREAM = 3


# ---- INTACTX host-integrity identity (mirrors http3_intactx.{c,h}) ----------
INTACTX_VARIANCE_SHIFT = 48
INTACTX_VARIANCE_MASK = 0xFFFF
INTACTX_IDENTITY_MASK = 0x0000FFFFFFFFFFFF
INTACTX_TAMPER_THRESHOLD = 0x0400  # 1024 of 65535
INTACTX_DEFAULT_BASELINE_PATH = ".http3_intactx_baseline"


def _fnv1a_str(h: int, s: Optional[str]) -> int:
    return _fnv1a((s or "").encode("utf-8"), h)


def _sample_identity() -> int:
    """Stable OS/identity hash: changes only on re-image/clone/rename/user swap."""
    h = _FNV64_OFFSET
    u = platform.uname()
    h = _fnv1a_str(h, u.system)
    h = _fnv1a_str(h, u.release)
    h = _fnv1a_str(h, u.version)
    h = _fnv1a_str(h, u.machine)
    h = _fnv1a_str(h, u.node)
    h = _fnv1a_str(h, platform.node())
    h = _fnv1a_str(h, os.environ.get("USER"))
    h = _fnv1a_str(h, os.environ.get("LOGNAME"))
    h = _fnv1a_str(h, os.environ.get("HOME"))
    return h


def _sample_use_normality(identity: int) -> int:
    """How the host is being used right now; small drift expected, big jump = anomaly."""
    h = identity
    h = _fnv1a_str(h, os.environ.get("SHELL"))
    h = _fnv1a_str(h, os.environ.get("PWD"))
    h = _fnv1a_str(h, os.environ.get("TERM"))
    h = _fnv1a_str(h, os.environ.get("LANG"))
    return h


def _variance_from(current: int, baseline: int) -> int:
    dist = bin((current ^ baseline) & _U64).count("1")  # Hamming distance 0..64
    scaled = dist * 1024
    return min(scaled, INTACTX_VARIANCE_MASK)


class Intactx:
    """Computes the 64-bit INTACTX value for outgoing packets and judges tampering.

    Layout: bits[63..48] = 16-bit variance from baseline, bits[47..0] = identity.
    A larger environmental change yields a statically larger number.
    """

    def __init__(self, baseline_path: Optional[str] = None) -> None:
        self.baseline_path = baseline_path or INTACTX_DEFAULT_BASELINE_PATH
        self.loaded = False
        try:
            with open(self.baseline_path, "r", encoding="utf-8") as f:
                self.baseline = int(f.read().strip())
            self.loaded = True
        except (OSError, ValueError):
            # Baseline captures the same shape as an emit sample so a healthy,
            # unchanged host measures near-zero variance.
            self.baseline = _sample_use_normality(_sample_identity())
            self._write_baseline()

    def _write_baseline(self) -> None:
        try:
            with open(self.baseline_path, "w", encoding="utf-8") as f:
                f.write(f"{self.baseline:020d}\n")
        except OSError:
            pass

    def compute(self) -> int:
        identity = _sample_identity()
        sample = _sample_use_normality(identity)
        variance = _variance_from(sample, self.baseline)
        return ((variance << INTACTX_VARIANCE_SHIFT) |
                (identity & INTACTX_IDENTITY_MASK)) & _U64

    def reset_baseline(self) -> None:
        self.baseline = _sample_use_normality(_sample_identity())
        self.loaded = True
        self._write_baseline()

    @staticmethod
    def variance(intactx: int) -> int:
        return (intactx >> INTACTX_VARIANCE_SHIFT) & INTACTX_VARIANCE_MASK

    @staticmethod
    def is_tampered(intactx: int, threshold: int = 0) -> bool:
        if threshold == 0:
            threshold = INTACTX_TAMPER_THRESHOLD
        return Intactx.variance(intactx) >= threshold


@dataclass
class Envelope:
    """The §5 compact application envelope, with NONCE, DIGEST, and INTACTX."""

    service_id: int
    op_id: int
    request_id: int
    payload: bytes = b""
    flags: int = Flag.NONE
    version: int = ENVELOPE_VERSION
    nonce: int = 0
    intactx: int = 0
    digest: int = 0
    basket: bytes = None  # canonical basket block; defaults to the fixed basket

    def __post_init__(self) -> None:
        if self.basket is None:
            self.basket = basket_serialize()

    def compute_digest(self, key: bytes) -> int:
        """Keyed MAC (SipHash-2-4) over canonical header + basket + payload.

        Must match http3_envelope_compute_digest() in the C reference exactly.
        The transport-only BINARY flag is excluded so text and binary forms of
        the same logical envelope share a digest; `key` is the 16-byte
        per-connection secret. The NONCE and the BASKET are inside the MAC.
        """
        logical_flags = self.flags & ~int(Flag.BINARY)
        hdr = struct.pack(
            ">BBIIQQQI",
            self.version & 0xFF,
            logical_flags & 0xFF,
            self.service_id & 0xFFFFFFFF,
            self.op_id & 0xFFFFFFFF,
            self.request_id & _U64,
            self.nonce & _U64,
            self.intactx & _U64,
            len(self.payload) & 0xFFFFFFFF,
        )
        return siphash24(key, hdr + self.basket + self.payload)

    def seal(self, key: bytes) -> "Envelope":
        """Stamp the keyed DIGEST over the current contents; returns self for chaining."""
        self.digest = self.compute_digest(key)
        return self

    def verify_digest(self, key: bytes) -> bool:
        return self.compute_digest(key) == self.digest

    def pack_text(self, key: Optional[bytes] = None) -> bytes:
        if len(self.payload) > MAX_PAYLOAD:
            raise ValueError("payload too large")
        if key is not None:
            self.seal(key)
        basket_hex = self.basket.hex()
        head = (
            f"H3 {self.version} {self.flags} {self.service_id} {self.op_id} "
            f"{self.request_id} {self.nonce} {self.digest} {self.intactx} "
            f"{basket_hex} {len(self.payload)}:"
        )
        return head.encode("utf-8") + self.payload + b"\n"

    @staticmethod
    def unpack_text(wire: bytes) -> "Envelope":
        # Split off the fixed prefix up to the ':' after the length field.
        head, sep, rest = wire.partition(b":")
        if not sep:
            raise ValueError("malformed envelope")
        parts = head.decode("utf-8").split(" ")
        if len(parts) != 11 or parts[0] != "H3":
            raise ValueError("malformed envelope")
        _, ver, flags, sid, oid, rid, nonce, digest, intactx, basket_hex, plen = parts
        n = int(plen)
        if n > MAX_PAYLOAD:
            raise ValueError("payload too large")
        payload = rest[:n]
        if len(payload) != n:
            raise ValueError("short payload")
        return Envelope(
            service_id=int(sid), op_id=int(oid), request_id=int(rid),
            payload=payload, flags=int(flags), version=int(ver),
            nonce=int(nonce), intactx=int(intactx), digest=int(digest),
            basket=bytes.fromhex(basket_hex),
        )


@dataclass
class Response:
    """The §7 response model: STATUS | REQUEST-ID | RESULT."""

    status: Status
    request_id: int
    result: bytes = b""

    def pack_text(self) -> bytes:
        head = f"H3R {int(self.status)} {self.request_id} {len(self.result)}:"
        return head.encode("utf-8") + self.result + b"\n"

    @staticmethod
    def unpack_text(wire: bytes) -> "Response":
        head, sep, rest = wire.partition(b":")
        if not sep:
            raise ValueError("malformed response")
        parts = head.decode("utf-8").split(" ")
        if len(parts) != 4 or parts[0] != "H3R":
            raise ValueError("malformed response")
        _, status, rid, rlen = parts
        n = int(rlen)
        return Response(status=Status(int(status)), request_id=int(rid), result=rest[:n])


class Naming:
    """§4 fast naming: connection-local name <-> compact id, cached on first use."""

    def __init__(self) -> None:
        self._services: Dict[str, int] = {}
        self._ops: Dict[Tuple[int, str], int] = {}
        self._next_service = 1

    def intern_service(self, name: str) -> int:
        if name not in self._services:
            self._services[name] = self._next_service
            self._next_service += 1
        return self._services[name]

    def intern_op(self, service: str, op: str) -> int:
        sid = self.intern_service(service)
        key = (sid, op)
        if key not in self._ops:
            existing = [o for (s, o) in self._ops if s == sid]
            self._ops[key] = len(existing) + 1
        return self._ops[key]

    def lookup_service(self, name: str) -> int:
        return self._services.get(name, 0)


Handler = Callable[[bytes, object], Tuple[Status, bytes]]


class Pipeline:
    """§19 pipeline: register services/ops, then dispatch envelopes to handlers."""

    def __init__(self, intactx_threshold: int = 0, mac_key: bytes = b"\x00" * MAC_KEY_BYTES) -> None:
        self.naming = Naming()
        self._bindings: Dict[Tuple[int, int], Tuple[RetryClass, Handler]] = {}
        self._ctx: Dict[int, object] = {}
        self.requests_handled = 0
        self.intactx_threshold = intactx_threshold or INTACTX_TAMPER_THRESHOLD
        self.digest_rejects = 0
        self.tamper_resets = 0
        self.replays_rejected = 0
        self.nonce_high_water = 0
        self.set_mac_key(mac_key)

    def set_mac_key(self, key: bytes) -> None:
        """Install the 16-byte per-connection MAC key used to verify each
        packet's keyed-MAC DIGEST (the shared secret from key agreement)."""
        if len(key) != MAC_KEY_BYTES:
            raise ValueError("MAC key must be 16 bytes")
        self.mac_key = bytes(key)

    def reset_replay_window(self, start: int = 0) -> None:
        """Reset the NONCE high-water mark; the next accepted packet must carry
        a NONCE strictly greater than `start`."""
        self.nonce_high_water = start

    def register(self, service: str, op: str, retry: RetryClass, handler: Handler,
                 ctx: object = None) -> Tuple[int, int]:
        sid = self.naming.intern_service(service)
        oid = self.naming.intern_op(service, op)
        self._bindings[(sid, oid)] = (retry, handler)
        if ctx is not None:
            self._ctx[sid] = ctx
        return sid, oid

    def retry_class(self, sid: int, oid: int) -> Optional[RetryClass]:
        b = self._bindings.get((sid, oid))
        return b[0] if b else None

    def dispatch(self, env: Envelope) -> Response:
        # §19: service-id lookup then op-id lookup.
        if not any(s == env.service_id for (s, _o) in self._bindings):
            return Response(Status.UNKNOWN_SERVICE, env.request_id)
        binding = self._bindings.get((env.service_id, env.op_id))
        if binding is None:
            return Response(Status.UNKNOWN_OPERATION, env.request_id)
        _retry, handler = binding
        status, result = handler(env.payload, self._ctx.get(env.service_id))
        self.requests_handled += 1
        return Response(status, env.request_id, result)

    def handle_wire(self, wire: bytes) -> bytes:
        # §19: minimal parse -> integrity gate -> dispatch -> pack response.
        try:
            env = Envelope.unpack_text(wire)
        except ValueError:
            return Response(Status.BAD_ENVELOPE, 0).pack_text()
        # 1. Per-packet DIGEST (keyed MAC) must verify under the shared key --
        #    else the packet was corrupted OR forged without the secret.
        if not env.verify_digest(self.mac_key):
            self.digest_rejects += 1
            return Response(Status.BAD_DIGEST, env.request_id).pack_text()
        # 2. INTACTX variance below threshold -- else the host is tampered: RESET.
        if Intactx.is_tampered(env.intactx, self.intactx_threshold):
            self.tamper_resets += 1
            return Response(Status.TAMPERED, env.request_id, b"RESET").pack_text()
        # 3. NONCE must be strictly ahead of the high-water mark -- else replay.
        #    The NONCE is MAC'd, so it cannot be bumped to evade this check.
        if env.nonce <= self.nonce_high_water:
            self.replays_rejected += 1
            return Response(Status.REPLAYED, env.request_id).pack_text()
        self.nonce_high_water = env.nonce
        return self.dispatch(env).pack_text()



# ---- HTTP "colors": named configuration profiles (mirrors http3_profile.{h,c})
# A SLeeLa program names a COLOR from source; the color bundles wire form,
# default flags, and the integrity profile (MAC / INTACTX / replay / basket).
# Colors are advisory selectors over already-supported behavior; naming a color
# never changes the on-the-wire envelope layout.
COLOR_NAME_MAX = 24


class WireForm(IntEnum):
    TEXT = 0
    BINARY = 1


@dataclass
class Profile:
    """A resolved HTTP color (see http3_colors.conf / HTTP-COLORS.md)."""

    name: str
    wire: WireForm
    flags: int
    mac_required: bool
    intactx_enabled: bool
    intactx_threshold: int  # 0 => library default
    replay_guard: bool
    basket_required: bool


# The shipped palette, kept in sync with http3_profile.c and http3_colors.conf.
PALETTE = (
    Profile("green", WireForm.TEXT,   Flag.NONE,                         False, False, 0, False, False),
    Profile("amber", WireForm.TEXT,   Flag.NONE,                         True,  False, 0, True,  False),
    Profile("red",   WireForm.BINARY, int(Flag.BINARY),                  True,  True,  0, True,  True),
    Profile("black", WireForm.BINARY, int(Flag.BINARY) | int(Flag.COMPRESSED),
                                                                          True,  True,  0, True,  True),
)


def profile_by_color(color: str) -> Profile:
    """Resolve a shipped color by name. Raises KeyError if unknown."""
    for p in PALETTE:
        if p.name == color:
            return p
    raise KeyError(f"unknown HTTP color: {color!r}")


def profile_load(path: str, color: str) -> Profile:
    """Load a color from an http3_colors.conf-format file. Raises KeyError if
    the color is not present, ValueError if a matching line is malformed."""
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) != 8 or parts[0] != color:
                continue
            name, wire, flags, mac, intactx, threshold, replay, basket = parts
            if wire not in ("text", "binary"):
                raise ValueError(f"bad wire form: {wire!r}")
            return Profile(
                name=name,
                wire=WireForm.TEXT if wire == "text" else WireForm.BINARY,
                flags=int(flags),
                mac_required=bool(int(mac)),
                intactx_enabled=bool(int(intactx)),
                intactx_threshold=int(threshold),
                replay_guard=bool(int(replay)),
                basket_required=bool(int(basket)),
            )
    raise KeyError(f"color {color!r} not found in {path}")
