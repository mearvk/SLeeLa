# HTTP 2.1 Data Flow — Sketch

This is a **sketch** of the SLeeLa HTTP 2.1 application protocol, a sibling to
[`../http-3.0/`](../http-3.0/). HTTP 2.1 shares HTTP 3.0's **basic design goals**
but is an **earlier generation**: it carries the clean core only and omits the
3.0-era per-packet integrity substrate.

## Same design goals as HTTP 3.0

| Goal | HTTP 2.1 | HTTP 3.0 |
|---|---|---|
| Compact application envelope | ✅ core fields | ✅ core + integrity fields |
| Fast service/operation naming | ✅ | ✅ |
| Response model (transport ≠ application) | ✅ | ✅ |
| Retry classes | ✅ (READ/IDEMPOTENT/MUTATING) | ✅ (+ STREAM) |
| Traceable processing pipeline | ✅ | ✅ |

## What HTTP 2.1 deliberately omits (3.0-era additions)

- Per-packet keyed-MAC **DIGEST** (SipHash-2-4)
- **INTACTX** host-integrity identity / tamper reset
- Replay **NONCE**
- Goods & services **BASKET**

So a 2.1 packet is just: `VERSION | FLAGS | SERVICE-ID | OP-ID | REQUEST-ID | PAYLOAD`.

## The data, and where it lives

| Concept | Type / symbol | File |
|---|---|---|
| Compact envelope | `h21_envelope_t` | `h21_envelope.h` |
| Textual/binary pack/unpack | `h21_envelope_pack_text` / `_binary` (+ unpack) | `h21_envelope.c` |
| Fast-naming dictionary | `h21_naming_t` | `h21_naming.h` |
| Response model | `h21_response_t` / `h21_status_t` | `h21_envelope.h` |
| Retry classes | `h21_retry_class_t` | `h21_pipeline.h` |
| Processing pipeline | `h21_pipeline_dispatch` / `_handle_wire` | `h21_pipeline.c` |
| Python parity | `h21_flow.py` | `h21_flow.py` |

## Wire forms

```text
envelope (textual):  H21 <ver> <flags> <service_id> <op_id> <request_id> <len>:<payload>\n
response (textual):  H21R <status> <request_id> <len>:<result>\n
envelope (binary) :  [ver:1][flags:1][service_id:4][op_id:4][request_id:8][payload_len:4][payload:N]
```

## Hot path (no integrity gate)

```text
   receive -> minimal parse (auto-detect textual "H21 " vs binary)
           -> service-id lookup -> operation-id lookup
           -> dispatch -> business logic -> pack response
```

Unlike HTTP 3.0, there is **no integrity gate** between parse and dispatch: 2.1
has no per-packet MAC, host-identity, replay, or basket checks.

## Status of this sketch

- **Implemented:** the envelope and response pack/unpack (textual + binary) in
  both C (`h21_envelope.c`) and Python (`h21_flow.py`).
- **Stubs (TODO):** `h21_naming.c` and `h21_pipeline.c` bodies, and the Python
  `Naming`/`Pipeline` methods — sketched with signatures and TODO markers to be
  filled in to mirror the HTTP 3.0 core (minus the integrity substrate).

## Build

```sh
cd http-2.0
make proto     # compiles the protocol-core objects (compile check)
make syntax    # syntax-only check of all C sources
```


## Port and Stream Multiplexing

HTTP 2.1 uses the SLeeLa logical-port model above the HTTP/2 stream layer.

```text
one transport connection
        |
        +-- HTTP/2 stream 1 -- logical port -- service/op -- payload
        +-- HTTP/2 stream 2 -- logical port -- service/op -- payload
        +-- HTTP/2 stream N -- logical port -- service/op -- payload
```

The important separation is:

- **Transport endpoint:** the native TCP/TLS endpoint.
- **HTTP/2 stream:** the independently multiplexed request/response stream.
- **SLeeLa logical port:** the application-level service/channel/route identifier.
- **SERVICE-ID / OP-ID:** the compact dispatch identity carried by the SLeeLa envelope.

A logical port therefore does not require one operating-system socket per service. Multiple logical ports can share the same HTTP/2 connection and its stream pool.

HTTP/2 stream identifiers and SLeeLa logical ports are different namespaces and must not be conflated. The stream provides transport multiplexing; the logical port provides application routing.
