"""Python reference for the HTTP 3.0 data flow (spec §4/§5/§7/§9).

This mirrors the C reference (http3_envelope.c / http3_naming.c /
http3_pipeline.c) at the level the spec actually defines: the compact
envelope, fast naming, the response model, and retry classes. It is
dependency-free so a Python connector can speak the protocol without
becoming a SLeeLa runtime (spec §20). Crypto is out of scope here; that
lives in the C substrate (crypto_openssl.c et al.).

Textual wire forms (must match the C implementation byte-for-byte):

    envelope:  H3 <ver> <flags> <service_id> <op_id> <request_id> <len>:<payload>\\n
    response:  H3R <status> <request_id> <len>:<result>\\n
"""
from __future__ import annotations

from dataclasses import dataclass, field
from enum import IntEnum
from typing import Callable, Dict, Optional, Tuple

ENVELOPE_VERSION = 3
MAX_PAYLOAD = 4096


class Flag(IntEnum):
    NONE = 0x00
    BINARY = 0x01
    COMPRESSED = 0x02
    STREAM = 0x04
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
    STREAM = 3


@dataclass
class Envelope:
    """The §5 compact application envelope."""

    service_id: int
    op_id: int
    request_id: int
    payload: bytes = b""
    flags: int = Flag.NONE
    version: int = ENVELOPE_VERSION

    def pack_text(self) -> bytes:
        if len(self.payload) > MAX_PAYLOAD:
            raise ValueError("payload too large")
        head = f"H3 {self.version} {self.flags} {self.service_id} {self.op_id} {self.request_id} {len(self.payload)}:"
        return head.encode("utf-8") + self.payload + b"\n"

    @staticmethod
    def unpack_text(wire: bytes) -> "Envelope":
        # Split off the fixed prefix up to the ':' after the length field.
        head, sep, rest = wire.partition(b":")
        if not sep:
            raise ValueError("malformed envelope")
        parts = head.decode("utf-8").split(" ")
        if len(parts) != 7 or parts[0] != "H3":
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

    def __init__(self) -> None:
        self.naming = Naming()
        self._bindings: Dict[Tuple[int, int], Tuple[RetryClass, Handler]] = {}
        self._ctx: Dict[int, object] = {}
        self.requests_handled = 0

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
        # §19: minimal parse -> dispatch -> pack response.
        try:
            env = Envelope.unpack_text(wire)
        except ValueError:
            return Response(Status.BAD_ENVELOPE, 0).pack_text()
        return self.dispatch(env).pack_text()
