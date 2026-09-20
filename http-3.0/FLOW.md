# HTTP 3.0 Data Flow — Spec → Code Map

This maps the data that drives the HTTP 3.0 spec
(`../http/spec/HTTP-3.0-SPEC.md`) to the source in this directory, and traces
one request end to end. Companion: [`STATUS.md`](STATUS.md).

## The data, and where it lives

| Spec concept | Section | Type / symbol | File |
|---|---|---|---|
| Compact envelope | §5 | `http3_envelope_t` | `http3_envelope.h` |
| ├ VERSION / FLAGS | §5 | `version`, `flags` (`http3_envelope_flag_t`) | `http3_envelope.h` |
| ├ SERVICE-ID / OP-ID | §4/§5 | `service_id`, `op_id` | `http3_envelope.h` |
| ├ REQUEST-ID | §6 | `request_id` | `http3_envelope.h` |
| ├ DIGEST (per-packet integrity) | §5 | `digest` + `http3_envelope_compute_digest` | `http3_envelope.{h,c}` |
| ├ INTACTX (host-integrity id) | §5 | `intactx` (`http3_intactx_t`) | `http3_intactx.{h,c}` |
| └ PAYLOAD | §5 | `payload`, `payload_len` | `http3_envelope.h` |
| Textual pack/unpack | §5 | `http3_envelope_pack_text` / `_unpack_text` | `http3_envelope.c` |
| Binary pack/unpack | §5 | `http3_envelope_pack_binary` / `_unpack_binary` | `http3_envelope.c` |
| Fast-naming dictionary | §4 | `http3_naming_t` | `http3_naming.h` |
| Name → compact id (cached) | §4 | `http3_naming_intern_service` / `_intern_op` | `http3_naming.c` |
| Id → name (dispatch/diag) | §4 | `http3_naming_service_name` / `_op_name` | `http3_naming.c` |
| Integrity gate (digest + tamper) | §19 | `http3_pipeline_handle_wire` (verify + reset) | `http3_pipeline.c` |
| INTACTX variance / tamper check | §5 | `http3_intactx_variance` / `_is_tampered` | `http3_intactx.c` |
| Response model | §7 | `http3_response_t` (`status`, `request_id`, `result`) | `http3_envelope.h` |
| Application status | §7 | `http3_status_t` (incl. `BAD_DIGEST`, `TAMPERED`) | `http3_envelope.h` |
| Retry classes | §9 | `http3_retry_class_t` | `http3_pipeline.h` |
| Dispatch table | §19 | `http3_service_binding_t` / `http3_op_binding_t` | `http3_pipeline.h` |
| Processing pipeline | §19 | `http3_pipeline_dispatch` / `http3_pipeline_handle_wire` | `http3_pipeline.c` |
| Governing principle (any client) | §20 | `http3_flow.py` (Python parity) | `http3_flow.py` |

## End-to-end trace (§19 hot path)

```text
                         REQUEST
   name: service="orders", operation="calculate", payload="20,22"
        │
        ▼  §4 fast naming — resolve + cache compact ids
   http3_naming_intern_service("orders") -> 1
   http3_naming_intern_op("orders","calculate") -> 1
        │
        ▼  §5 build the compact envelope (stamps INTACTX, seals DIGEST)
   intactx = http3_intactx_compute(&ix)
   http3_envelope_init(env, service_id=1, op_id=1, request_id=1001,
                       flags=0, intactx, payload="20,22")
        │
        ▼  §5 pack to wire (textual OR binary). DIGEST + INTACTX now travel too.
   textual:  H3 3 0 1 1 1001 <digest> <intactx> 5:20,22
   binary :  03 01 00000001 00000001 00000000000003e9
             <digest:8> <intactx:8> 00000005 32302c3232
        │
        ▼  §19 receive + minimal parse (auto-detect wire form)
   http3_pipeline_handle_wire(pipe, wire, len, out, ...)
        │      └─ http3_envelope_unpack_text/_binary -> env
        ▼  §19 integrity gate (BEFORE dispatch)
   verify DIGEST      -> mismatch => BAD_DIGEST (packet dropped)
   INTACTX variance   -> over threshold => TAMPERED + RESET (not dispatched)
        │
        ▼  §19 service-id lookup -> op-id lookup (dispatch table)
   http3_pipeline_dispatch(pipe, env, resp)
        │      └─ find_binding(service_id=1) -> find_op(op_id=1)
        ▼  §19 SLeeLa dispatch -> business logic
   handler(payload="20,22") -> status=OK, result="sum=42"
        │
        ▼  §7 pack response — STATUS | REQUEST-ID | RESULT
   H3R 0 1001 6:sum=42
                         RESPONSE
```

Key invariants surfaced by the flow:

- **REQUEST-ID is echoed** (§6/§7): the response carries `request_id=1001` from
  the request, so it correlates without relying on ordering.
- **Transport ≠ application** (§7): a parsed envelope can still yield
  `UNKNOWN_SERVICE`, `UNKNOWN_OPERATION`, or `APP_ERROR`; success is the
  handler's status, not the mere fact of delivery.
- **Compact ids drive dispatch** (§4/§19): the hot path keys on
  `service_id`/`op_id`, never on a re-parsed name.
- **Same logical envelope, two wires** (§5): textual and binary unpack to an
  identical `http3_envelope_t`; the demo runs both and gets `sum=42` each time.
  The per-packet DIGEST is computed over a canonical big-endian field layout, so
  it is identical for both wire forms and reproduced byte-for-byte by the Python
  reference.
- **Integrity precedes dispatch** (§19): every received packet is checked before
  any business logic runs. A corrupted packet fails its DIGEST and returns
  `BAD_DIGEST`; a packet from a tampered/changed host carries an INTACTX whose
  variance exceeds the threshold and is answered with `TAMPERED` + a `RESET`
  body, never reaching a handler.
- **INTACTX magnitude tracks drift** (§5): INTACTX packs a 16-bit variance in
  its high bits over a 48-bit identity, so a larger environmental change from the
  persisted baseline produces a statically larger number. A healthy, unchanged
  host emits variance 0.

## Run it

```sh
cd http-3.0
make demo     # builds + runs the C flow demo (http3_pipeline_demo)
make test     # C demo + Python flow tests (test_http3_flow.py)
python3 test_http3_flow.py   # Python-only; verifies byte-identical wire
```

## Relationship to the crypto substrate

The protocol core (`http3_protocol.h`: envelope + naming + pipeline) is
independent of the cryptographic layer (`http3_crypto.h`: capsules, KDS, route
sessions). They compose at deployment: the envelope/PAYLOAD is what a
capsule/session would protect, and SERVICE/OP naming is what a route decision
would carry — but neither layer depends on the other's build. This keeps the
data-flow model and the security model separable, per the spec's boundary
between application protocol (§1–§19) and transport/security infrastructure
(§2, §12).
