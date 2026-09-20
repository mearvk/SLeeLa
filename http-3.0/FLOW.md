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
| ├ NONCE (replay guard) | §5 | `nonce` (MAC-covered; pipeline high-water mark) | `http3_envelope.h` / `http3_pipeline.c` |
| ├ DIGEST (per-packet keyed MAC) | §5 | `digest` + `http3_envelope_compute_digest` (SipHash-2-4, `http3_mac.{h,c}`) | `http3_envelope.{h,c}` |
| ├ INTACTX (host-integrity id) | §5 | `intactx` (`http3_intactx_t`) | `http3_intactx.{h,c}` |
| ├ BASKET (goods & services) | §5 | `basket` (fixed 14-item block, ISO USD/g) | `http3_basket.{h,c}` |
| └ PAYLOAD | §5 | `payload`, `payload_len` | `http3_envelope.h` |
| Textual pack/unpack | §5 | `http3_envelope_pack_text` / `_unpack_text` | `http3_envelope.c` |
| Binary pack/unpack | §5 | `http3_envelope_pack_binary` / `_unpack_binary` | `http3_envelope.c` |
| Fast-naming dictionary | §4 | `http3_naming_t` | `http3_naming.h` |
| Name → compact id (cached) | §4 | `http3_naming_intern_service` / `_intern_op` | `http3_naming.c` |
| Id → name (dispatch/diag) | §4 | `http3_naming_service_name` / `_op_name` | `http3_naming.c` |
| Integrity gate (digest + tamper + replay) | §19 | `http3_pipeline_handle_wire` (verify + reset + replay) | `http3_pipeline.c` |
| INTACTX variance / tamper check | §5 | `http3_intactx_variance` / `_is_tampered` | `http3_intactx.c` |
| Replay window (NONCE high-water) | §19 | `nonce_high_water` + `http3_pipeline_reset_replay_window` | `http3_pipeline.c` |
| Timing: max speed / on time / balance / carrier certainty | §19 | `http3_timing_t` + `http3_pipeline_observe_timing` | `http3_timing.{h,c}` / `http3_pipeline.c` |
| Response model | §7 | `http3_response_t` (`status`, `request_id`, `result`) | `http3_envelope.h` |
| Application status | §7 | `http3_status_t` (incl. `BAD_DIGEST`, `TAMPERED`, `REPLAYED`) | `http3_envelope.h` |
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
        ▼  §5 build the compact envelope (NONCE + INTACTX, seals keyed DIGEST)
   intactx = http3_intactx_compute(&ix)
   http3_envelope_init(env, service_id=1, op_id=1, request_id=1001,
                       flags=0, nonce=1, intactx, mac_key, payload="20,22")
        │
        ▼  §5 pack to wire (textual OR binary). NONCE + DIGEST + INTACTX +
           BASKET (fixed 14-item block) all travel too.
   textual:  H3 3 0 1 1 1001 <nonce> <digest> <intactx> <basket-hex> 5:20,22
   binary :  03 01 00000001 00000001 00000000000003e9
             <nonce:8> <digest:8> <intactx:8> <basket:172> 00000005 32302c3232
        │
        ▼  §19 receive + minimal parse (auto-detect wire form)
   http3_pipeline_handle_wire(pipe, wire, len, out, ...)
        │      └─ http3_envelope_unpack_text/_binary -> env
        ▼  §19 integrity gate (BEFORE dispatch)
   verify keyed MAC   -> mismatch => BAD_DIGEST (corrupted OR forged; dropped)
   INTACTX variance   -> over threshold => TAMPERED + RESET (not dispatched)
   NONCE <= high-water => REPLAYED (replay of a valid packet; dropped)
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
  The per-packet DIGEST is a keyed MAC (SipHash-2-4) computed over a canonical
  big-endian field layout, so it is identical for both wire forms and reproduced
  byte-for-byte by the Python reference under the same key.
- **Integrity precedes dispatch** (§19): every received packet is checked before
  any business logic runs. Its DIGEST is a *keyed* MAC, so a corrupted **or
  forged** packet (one rewritten by a party without the per-connection key)
  fails verification and returns `BAD_DIGEST`; a packet from a tampered/changed
  host carries an INTACTX whose variance exceeds the threshold and is answered
  with `TAMPERED` + a `RESET` body, never reaching a handler.
- **INTACTX magnitude tracks drift** (§5): INTACTX packs a 16-bit variance in
  its high bits over a 48-bit identity, so a larger environmental change from the
  persisted baseline produces a statically larger number. A healthy, unchanged
  host emits variance 0.
- **Replay is refused** (§19): each packet carries a monotonic NONCE inside the
  MAC. The receiver keeps a high-water mark and admits a packet only if its
  NONCE is strictly greater, so re-sending a previously valid packet — even with
  a perfectly valid MAC — returns `REPLAYED` and never reaches a handler. Since
  the NONCE is MAC-covered, an attacker cannot bump it to slip a replay through
  without breaking the DIGEST.
- **The basket rides every packet** (§5): a fixed 14-item basket of goods &
  services (atomic number + ISO USD micro-value per gram, `http3_basket.{h,c}`)
  is serialized into a 172-byte canonical block carried on every packet and
  covered by the MAC, so it is authenticated end to end. The C block, the Python
  block, and the human-readable `BASKET.docx` all agree byte-for-byte.
- **Timing is measured, not enforced on the wire** (§19): a connection-level
  timing layer (`http3_timing.{h,c}`) watches each arrival for **max speed** (a
  minimum inter-arrival gap), **on time** (arrival within a deadline + grace),
  and **temporal balance** (inter-arrival jitter within a band), and keeps a
  running **carrier certainty** in [0,1] — the fraction of recent packets that
  were clean. It is **advisory**: nothing new travels on the wire and no packet
  is rejected by timing; the pipeline records `late_packets`, `over_rate_packets`,
  and `unbalanced_packets`. C and Python compute identical flags and certainty.

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

The one place the two layers *touch by value* (not by build) is the per-packet
DIGEST key: the keyed MAC (`http3_mac.{h,c}`, SipHash-2-4) is a dependency-free
part of the protocol core, but its 16-byte key is a per-connection secret that a
deployment supplies from the crypto substrate's key agreement
(`crypto_key_agreement.*`). The protocol core never links OpenSSL; it just
accepts the key bytes, so the separation of builds is preserved while packets
gain authenticity end to end.
