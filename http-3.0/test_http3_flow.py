"""Tests for the HTTP 3.0 data-flow reference (spec §4/§5/§7/§9).

Runs with either pytest or plain `python3 test_http3_flow.py`. Exercises the
pure-data protocol surface (envelope, naming, response, pipeline, retry
classes) implemented in http3_flow.py. Crypto is intentionally out of scope
here — it lives in the C substrate (crypto_openssl.c et al.).
"""
from __future__ import annotations

from http3_flow import (
    Envelope, Flag, Intactx, Naming, Pipeline, Response, RetryClass, Status,
    INTACTX_VARIANCE_SHIFT, INTACTX_VARIANCE_MASK, INTACTX_TAMPER_THRESHOLD,
)


def test_envelope_text_round_trip() -> None:
    env = Envelope(service_id=7, op_id=3, request_id=1001, payload=b"20,22")
    wire = env.pack_text()
    # Wire now carries DIGEST and INTACTX between REQUEST-ID and the length.
    # digest is 0 here since intactx defaults to 0; assert the exact framing.
    assert wire == f"H3 3 0 7 3 1001 {env.digest} 0 5:".encode() + b"20,22\n"
    back = Envelope.unpack_text(wire)
    assert (back.service_id, back.op_id, back.request_id, back.payload) == (7, 3, 1001, b"20,22")
    assert back.digest == env.digest and back.intactx == 0


def test_digest_verifies_and_detects_corruption() -> None:
    env = Envelope(service_id=1, op_id=1, request_id=5, payload=b"20,22").seal()
    assert env.verify_digest()
    back = Envelope.unpack_text(env.pack_text())
    assert back.verify_digest()
    # Corrupt the payload after sealing: digest must no longer verify.
    back.payload = b"90,22"
    assert not back.verify_digest()


def test_intactx_variance_layout_and_tamper() -> None:
    # Variance sits in the high bits; a larger variance is a larger number.
    low = (1 << INTACTX_VARIANCE_SHIFT) | 0xABC
    high = (INTACTX_VARIANCE_MASK << INTACTX_VARIANCE_SHIFT) | 0xABC
    assert high > low
    assert Intactx.variance(high) == INTACTX_VARIANCE_MASK
    assert not Intactx.is_tampered(low, INTACTX_TAMPER_THRESHOLD)
    assert Intactx.is_tampered(high, INTACTX_TAMPER_THRESHOLD)


def test_pipeline_rejects_bad_digest_and_resets_tampered() -> None:
    pipe = Pipeline()

    def echo(payload: bytes, ctx: object):
        return Status.OK, payload

    sid, oid = pipe.register("svc", "echo", RetryClass.READ, echo)

    # A packet with a wrong digest is rejected before dispatch.
    env = Envelope(sid, oid, 7001, b"hi").seal()
    env.digest ^= 0xFF  # tamper the digest field
    assert Response.unpack_text(pipe.handle_wire(env.pack_text())).status == Status.BAD_DIGEST
    assert pipe.digest_rejects == 1

    # A packet from a tampered host (max variance) is RESET, not dispatched.
    tampered_intactx = (INTACTX_VARIANCE_MASK << INTACTX_VARIANCE_SHIFT) | 0x1234
    env2 = Envelope(sid, oid, 7002, b"hi", flags=int(Flag.RESET), intactx=tampered_intactx).seal()
    resp = Response.unpack_text(pipe.handle_wire(env2.pack_text()))
    assert resp.status == Status.TAMPERED and resp.result == b"RESET"
    assert pipe.tamper_resets == 1


def test_envelope_payload_is_length_prefixed_and_binary_safe() -> None:
    # A payload containing spaces and a newline must survive the round trip.
    payload = b"a b\nc:d"
    env = Envelope(service_id=1, op_id=1, request_id=5, payload=payload)
    back = Envelope.unpack_text(env.pack_text())
    assert back.payload == payload


def test_response_round_trip_echoes_request_id() -> None:
    resp = Response(Status.OK, request_id=1001, result=b"sum=42")
    back = Response.unpack_text(resp.pack_text())
    assert back.status == Status.OK
    assert back.request_id == 1001
    assert back.result == b"sum=42"


def test_fast_naming_caches_ids_on_first_use() -> None:
    naming = Naming()
    assert naming.intern_service("orders") == 1
    assert naming.intern_service("orders") == 1  # cached, same id
    assert naming.intern_service("billing") == 2
    # op ids are local to a service and start at 1
    assert naming.intern_op("orders", "calculate") == 1
    assert naming.intern_op("orders", "place") == 2
    assert naming.intern_op("orders", "calculate") == 1  # cached


def test_pipeline_dispatch_and_retry_classes() -> None:
    pipe = Pipeline()

    def calculate(payload: bytes, ctx: object):
        a, b = (int(x) for x in payload.decode().split(","))
        return Status.OK, f"sum={a + b}".encode()

    def place(payload: bytes, ctx: object):
        ctx["n"] += 1
        return Status.OK, f"order_id={ctx['n']}".encode()

    sid, calc = pipe.register("orders", "calculate", RetryClass.READ, calculate)
    _, place_id = pipe.register("orders", "place", RetryClass.MUTATING, place, ctx={"n": 0})

    # §19 over the wire, textual.
    out = pipe.handle_wire(Envelope(sid, calc, 1001, b"20,22").pack_text())
    resp = Response.unpack_text(out)
    assert resp.status == Status.OK and resp.request_id == 1001 and resp.result == b"sum=42"

    # §9 retry classes.
    assert pipe.retry_class(sid, calc) == RetryClass.READ
    assert pipe.retry_class(sid, place_id) == RetryClass.MUTATING

    # §7 posture: unknown service / operation.
    assert pipe.dispatch(Envelope(999, 1, 2002)).status == Status.UNKNOWN_SERVICE
    assert pipe.dispatch(Envelope(sid, 999, 2003)).status == Status.UNKNOWN_OPERATION

    # bad envelope -> typed status, no crash.
    assert Response.unpack_text(pipe.handle_wire(b"not-an-envelope")).status == Status.BAD_ENVELOPE


def _run_all() -> int:
    failures = 0
    for name, fn in sorted(globals().items()):
        if name.startswith("test_") and callable(fn):
            try:
                fn()
                print(f"  [ok]   {name}")
            except AssertionError as exc:  # pragma: no cover - manual runner
                print(f"  [FAIL] {name}: {exc}")
                failures += 1
    print("HTTP 3.0 flow tests:", "PASS" if failures == 0 else f"FAIL ({failures})")
    return failures


if __name__ == "__main__":
    raise SystemExit(_run_all())
