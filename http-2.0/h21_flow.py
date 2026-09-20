"""Python reference for the HTTP 2.1 data flow (SKETCH).

Mirrors the C sketch (h21_envelope.c / h21_naming.c / h21_pipeline.c) at the
level HTTP 2.1 defines: the compact envelope, fast naming, the response model,
and retry classes. HTTP 2.1 shares HTTP 3.0's basic design goals but is the
earlier generation -- it carries the clean core only, with NO per-packet
integrity substrate (no keyed-MAC DIGEST, INTACTX, NONCE, or goods/services
BASKET). Dependency-free so a Python connector can speak the protocol.

Textual wire forms (must match the C implementation byte-for-byte):

    envelope:  H21 <ver> <flags> <service_id> <op_id> <request_id> <len>:<payload>\\n
    response:  H21R <status> <request_id> <len>:<result>\\n

STATUS: the Envelope/Response pack/unpack are implemented; Naming and Pipeline
are sketched as stubs (see TODO markers) to be filled in to mirror the 3.0 core.
"""
from __future__ import annotations

import struct
from dataclasses import dataclass
from enum import IntEnum
from typing import Callable, Dict, Optional, Tuple

ENVELOPE_VERSION = 2
MAX_PAYLOAD = 4096


class Flag(IntEnum):
    NONE = 0x00
    BINARY = 0x01
    COMPRESSED = 0x02
    IDEMPOTENT = 0x08


class Status(IntEnum):
    OK = 0
    APP_ERROR = 1
    UNKNOWN_SERVICE = 2
    UNKNOWN_OPERATION = 3
    BAD_ENVELOPE = 4
    TOO_LARGE = 5
    RETRY_DENIED = 6


class RetryClass(IntEnum):
    READ = 0
    IDEMPOTENT = 1
    MUTATING = 2


@dataclass
class Envelope:
    """The HTTP 2.1 compact application envelope (core only)."""

    service_id: int
    op_id: int
    request_id: int
    payload: bytes = b""
    flags: int = Flag.NONE
    version: int = ENVELOPE_VERSION

    def pack_text(self) -> bytes:
        if len(self.payload) > MAX_PAYLOAD:
            raise ValueError("payload too large")
        head = (
            f"H21 {self.version} {self.flags} {self.service_id} {self.op_id} "
            f"{self.request_id} {len(self.payload)}:"
        )
        return head.encode("utf-8") + self.payload + b"\n"

    @staticmethod
    def unpack_text(wire: bytes) -> "Envelope":
        head, sep, rest = wire.partition(b":")
        if not sep:
            raise ValueError("malformed envelope")
        parts = head.decode("utf-8").split(" ")
        if len(parts) != 7 or parts[0] != "H21":
            raise ValueError("malformed envelope")
        _, ver, flags, sid, oid, rid, plen = parts
        n = int(plen)
        if n > MAX_PAYLOAD:
            raise ValueError("payload too large")
        payload = rest[:n]
        if len(payload) != n:
            raise ValueError("short payload")
        return Envelope(
            service_id=int(sid), op_id=int(oid), request_id=int(rid),
            payload=payload, flags=int(flags), version=int(ver),
        )


@dataclass
class Response:
    """The HTTP 2.1 response model: STATUS | REQUEST-ID | RESULT."""

    status: Status
    request_id: int
    result: bytes = b""

    def pack_text(self) -> bytes:
        head = f"H21R {int(self.status)} {self.request_id} {len(self.result)}:"
        return head.encode("utf-8") + self.result + b"\n"

    @staticmethod
    def unpack_text(wire: bytes) -> "Response":
        head, sep, rest = wire.partition(b":")
        if not sep:
            raise ValueError("malformed response")
        parts = head.decode("utf-8").split(" ")
        if len(parts) != 4 or parts[0] != "H21R":
            raise ValueError("malformed response")
        _, status, rid, rlen = parts
        n = int(rlen)
        return Response(status=Status(int(status)), request_id=int(rid), result=rest[:n])


class Naming:
    """HTTP 2.1 fast naming (SKETCH -- stubs to fill in)."""

    def __init__(self) -> None:
        self._services: Dict[str, int] = {}
        self._ops: Dict[Tuple[int, str], int] = {}
        self._next_service = 1

    def intern_service(self, name: str) -> int:
        raise NotImplementedError("TODO(2.1): name -> compact id, cached")

    def intern_op(self, service: str, op: str) -> int:
        raise NotImplementedError("TODO(2.1): service-local op id, cached")


Handler = Callable[[bytes, object], Tuple[Status, bytes]]


class Pipeline:
    """HTTP 2.1 pipeline (SKETCH -- stubs to fill in).

    Note: 2.1 has NO per-packet integrity gate; handle_wire goes straight from
    parse to dispatch (no MAC/INTACTX/NONCE/basket checks).
    """

    def __init__(self) -> None:
        self.naming = Naming()
        self.requests_handled = 0

    def register(self, service: str, op: str, retry: RetryClass, handler: Handler,
                 ctx: object = None) -> Tuple[int, int]:
        raise NotImplementedError("TODO(2.1): register service/op -> (sid, oid)")

    def dispatch(self, env: Envelope) -> Response:
        raise NotImplementedError("TODO(2.1): svc-id -> op-id -> handler dispatch")

    def handle_wire(self, wire: bytes) -> bytes:
        raise NotImplementedError("TODO(2.1): parse -> dispatch -> pack response")
