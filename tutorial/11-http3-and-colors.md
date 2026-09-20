# Lesson 11 — HTTP 3.0 & HTTP Colors

> Series: [index](README.md) · prev: [10](10-ledger.md) · next: [12 — Putting it together](12-capstone.md)

SLeeLa ships an application-protocol core under [`../http-3.0/`](../http-3.0/)
(with an earlier sketch in [`../http-2.0/`](../http-2.0/)). This lesson shows the
per-packet integrity model and how a program **names its HTTP behavior from
source** using *colors*.

## The compact envelope

Every HTTP 3.0 packet carries, in order:

```text
VERSION | FLAGS | SERVICE-ID | OP-ID | REQUEST-ID | NONCE | DIGEST | INTACTX | BASKET | PAYLOAD
```

The last four are the per-packet integrity substrate:

| Field | Purpose |
|---|---|
| **DIGEST** | keyed MAC (SipHash-2-4) — authenticity + tamper detection |
| **INTACTX** | host-integrity fingerprint — RESET if the host looks tampered |
| **NONCE** | monotonic counter — replay rejection |
| **BASKET** | fixed goods/services basket carried per packet (MAC-covered) |

The receive path checks them **before** dispatch: a corrupted/forged packet ->
`BAD_DIGEST`, a tampered host -> `TAMPERED`+RESET, a replay -> `REPLAYED`.
See [`../http-3.0/FLOW.md`](../http-3.0/FLOW.md).

## Try the references

```sh
cd http-3.0
make demo    # C end-to-end demo (prints each integrity check)
make test    # C demo + Python flow tests
```

The C and Python references agree byte-for-byte on the wire and the keyed MAC.

## HTTP colors — behavior from source

Rather than flip each switch by hand, a SLeeLa program **names a color**. A color
bundles wire form, flags, and the integrity profile. From
[`../http-3.0/http_colors.example.sleela`](../http-3.0/http_colors.example.sleela):

```sleela
http {
    default color green;
    service "orders"    color red;     // binary + full integrity + basket
    service "catalog"   color green;   // textual, core only (lightest)
    service "telemetry" color amber;   // textual, MAC + replay guard
}
```

The shipped palette (see [`../http-3.0/HTTP-COLORS.md`](../http-3.0/HTTP-COLORS.md)):

| Color | Wire | MAC | INTACTX | Replay | Basket |
|---|---|:--:|:--:|:--:|:--:|
| `green` | text | ✗ | ✗ | ✗ | ✗ |
| `amber` | text | ✓ | ✗ | ✓ | ✗ |
| `red` | binary | ✓ | ✓ | ✓ | ✓ |
| `black` | binary (+compressed) | ✓ | ✓ | ✓ | ✓ |

Under the hood the connector resolves and applies the color:

```c
http3_profile_t p;
http3_profile_by_color("red", &p);   /* or http3_profile_load(path, "red", &p) */
http3_profile_apply(&p, &pipe);
```

Colors are **advisory selectors** over already-supported behavior — naming one
never changes the wire layout, only which protections a connection turns on.

**Next:** [Lesson 12 — Putting it together](12-capstone.md)
