# HTTP 2.1 — Status (Sketch)

**Scope:** a sketch of the SLeeLa HTTP 2.1 application protocol in
[`http-2.0/`](.), a sibling of the fuller [`../http-3.0/`](../http-3.0/) core.
HTTP 2.1 has the same **basic design goals** as HTTP 3.0 but is the **earlier
generation** — the clean application core, without the 3.0-era per-packet
integrity substrate.

## Design-goal parity with HTTP 3.0

| Goal | 2.1 | Notes |
|---|:--:|---|
| Compact envelope | ✅ | `VERSION\|FLAGS\|SERVICE-ID\|OP-ID\|REQUEST-ID\|PAYLOAD` |
| Fast naming | ✅ | name ↔ compact id (sketch stubs) |
| Response model | ✅ | `STATUS\|REQUEST-ID\|RESULT`; transport ≠ application |
| Retry classes | ✅ | READ / IDEMPOTENT / MUTATING |
| Processing pipeline | ✅ | receive → parse → svc/op lookup → dispatch → pack |
| Textual **and** binary wire | ✅ | tags `H21` / `H21R`; binary header 22 bytes |

## Intentionally NOT in 2.1 (added by HTTP 3.0)

| 3.0 feature | Present in 2.1? |
|---|:--:|
| Keyed-MAC DIGEST (SipHash-2-4) | ❌ |
| INTACTX host identity / tamper RESET | ❌ |
| Replay NONCE | ❌ |
| Goods & services BASKET | ❌ |
| Per-packet integrity gate in the pipeline | ❌ |

## Implementation status

| Component | State |
|---|---|
| `h21_envelope.{h,c}` | **Implemented** — init + textual/binary pack/unpack + response + status names |
| `h21_flow.py` (Envelope/Response) | **Implemented** — byte-compatible wire with the C sketch |
| `h21_naming.{h,c}` | **Sketch/STUB** — types + signatures; bodies TODO |
| `h21_pipeline.{h,c}` | **Sketch/STUB** — types + signatures; dispatch/register bodies TODO |
| `h21_flow.py` (Naming/Pipeline) | **Sketch/STUB** — `NotImplementedError` with TODO markers |
| `h21_protocol.h` | **Implemented** — aggregation header |
| `Makefile` | **Implemented** — `make proto` / `make syntax` |

## Next steps to grow the sketch into a working core

1. Fill in `h21_naming.c` (name→id caching, id→name reverse lookup).
2. Fill in `h21_pipeline.c` (register service/op, dispatch, handle_wire).
3. Mirror those in `h21_flow.py` and add a demo + parity tests, matching the
   HTTP 3.0 core minus the integrity substrate.

*Companion: [`FLOW.md`](FLOW.md).*


## Port-Multiplexing Status

The HTTP 2.1 architecture explicitly defines three independent addressing layers:

| Layer | Purpose |
|---|---|
| Native transport endpoint | Physical/network connection endpoint |
| HTTP/2 stream | Multiplexed request/response channel |
| SLeeLa logical port | Application service/channel/route identifier |

This permits many SLeeLa services to operate over one HTTP/2 connection without allocating a separate native socket for each logical service.

The logical-port namespace is an application construct. Native TCP/UDP port limits therefore remain relevant only to the underlying transport binding, not to the number of logical SLeeLa routes that can be represented.


## Download Mode Status

HTTP 2.0/2.1 supports the common SLeeLa DOWNLOAD mode for files over 50 MB. HTTP/2 multiplexes transfer operations while SLeeLa transfer metadata provides durable resume semantics.

`SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE`
