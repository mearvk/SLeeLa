# HTTP Colors — specifying HTTP behavior from SLeeLa source

A SLeeLa program specifies its HTTP behavior by naming a **color**. A color is a
named bundle of protocol knobs — wire form, default flags, and the per-packet
integrity profile (keyed-MAC **DIGEST**, **INTACTX** tamper gate, replay
**NONCE**, and the goods & services **BASKET**) — so source code can say
"use color `green`" instead of setting each switch by hand.

Colors are **advisory selectors** over behavior the protocol already supports.
Naming a color never changes the on-the-wire envelope layout; it only chooses
which optional protections a connection turns on. The mechanism is additive.

## Where colors live

| Artifact | Role |
|---|---|
| [`http3_colors.conf`](http3_colors.conf) | Authoritative palette (one color per line) |
| [`http3_profile.h`](http3_profile.h) / [`http3_profile.c`](http3_profile.c) | C: `http3_profile_t`, resolver, loader, pipeline apply |
| [`http3_flow.py`](http3_flow.py) | Python parity: `Profile`, `PALETTE`, `profile_by_color`, `profile_load` |
| [`http3_service.example.conf`](http3_service.example.conf) | Example per-service → color mapping |
| [`http_colors.example.sleela`](http_colors.example.sleela) | Example of naming colors from SLeeLa source |

## The shipped palette

| Color | Wire | Flags | MAC | INTACTX | Replay | Basket | Use |
|---|---|---|:--:|:--:|:--:|:--:|---|
| `green` | text | — | ✗ | ✗ | ✗ | ✗ | Lightest, most interoperable; core only |
| `amber` | text | — | ✓ | ✗ | ✓ | ✗ | Balanced: authenticity + replay guard |
| `red` | binary | BINARY | ✓ | ✓ | ✓ | ✓ | Strict: full per-packet integrity |
| `black` | binary | BINARY+COMPRESSED | ✓ | ✓ | ✓ | ✓ | Strict, with compressed payloads |

The palette is identical across `http3_colors.conf`, `http3_profile.c`, and
`http3_flow.py`.

## Specifying colors from SLeeLa source

A Wrapper (`.sleela` source file) declares its HTTP colors declaratively:

```sleela
http {
    default color green;

    service "orders"    color red;     // binary + full integrity + basket
    service "billing"   color black;   // red, plus compressed payloads
    service "catalog"   color green;   // textual, core only (lightest)
    service "telemetry" color amber;   // textual, MAC + replay guard
}
```

Under the hood the connector resolves and applies the color:

```c
http3_profile_t p;
http3_profile_by_color("red", &p);   /* or http3_profile_load(path,"red",&p) */
http3_profile_apply(&p, &pipe);      /* sets the pipeline's INTACTX threshold */
/* then send/verify per p.wire, p.mac_required, p.replay_guard, ... */
```

```python
from http3_flow import profile_by_color, profile_load
p = profile_by_color("red")          # or profile_load("http3_colors.conf", "red")
```

## Config-file format (`http3_colors.conf`)

One color per line; `#` comments and blank lines ignored:

```text
# name    wire    flags  mac  intactx  threshold  replay  basket
green     text    0      0    0        0          0       0
amber     text    0      1    0        0          1       0
red       binary  1      1    1        0          1       1
black     binary  3      1    1        0          1       1
```

- `wire` — `text` or `binary`
- `flags` — envelope FLAGS bitfield (1=BINARY, 2=COMPRESSED, 4=STREAM, 8=IDEMPOTENT, 16=RESET)
- `mac` / `intactx` / `replay` / `basket` — `0`/`1` toggles
- `threshold` — INTACTX variance threshold (`0` = library default)

## HTTP 2.1 note

The HTTP 2.1 sketch ([`../http-2.0/`](../http-2.0/)) has no per-packet integrity
substrate, so its colors reduce to **wire form + flags** only (the MAC / INTACTX
/ replay / basket columns do not apply). The same "name a color from source"
surface is intended there; when the 2.1 core is fleshed out, a trimmed palette
(`green`/`amber` as text/binary + flags) would mirror this document.
