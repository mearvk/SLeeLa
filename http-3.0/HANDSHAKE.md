# The SLeeLa HTTP 3.0 capability handshake

Two peers — a client and a router/server — agree on how much of the protocol
they will use before exchanging traffic. Each advertises the **capability
levels** it supports as a neutral bitmask; the handshake deterministically picks
the **highest level both peers support**, and falls back to a modest **baseline**
when one side is not yet updated.

The bitmask names **supported protocol tiers only**. It carries no national,
geographic, or identity meaning.

## Capability levels (cumulative tiers)

| Level | Bit | Meaning |
|---|---|---|
| **L1 BASELINE** | `0x01` | envelope + integrity gate (always supported) |
| **L2 PER-LEG** | `0x02` | per-leg MSS / path-MTU sizing for goodput |
| **L3 PACING** | `0x04` | timing-aware pacing (max-speed / balance; see [`QOS.md`](../QOS.md)) |
| **L4 ECHO** | `0x08` | echo-acknowledged, deadline-aware delivery |

Levels are cumulative: a peer that supports L4 also supports L1–L3. A conforming
peer **always** includes the baseline (`L1`).

## The negotiation (the "calculus")

Given each peer's advertised set, the agreed level is the **highest single level
present in both**:

```text
common = local_caps & remote_caps
agreed = L4 if (common & L4) else
         L3 if (common & L3) else
         L2 if (common & L2) else
         L1                          # modest baseline fallback
```

If either side fails to include the baseline (non-conforming), the result is
`NONE`. Otherwise the result is always at least `L1`. This is deterministic and
symmetric — both ends compute the same agreed level from the same two offers.

## Baseline until the router is updated

The key property for **routers**: a router whose software is not yet updated
advertises only the levels it knows — at minimum `L1`. Negotiation then caps the
pair at that level, so the exchange still works over the modest baseline. When
the router's software becomes **apt to update** and it starts advertising higher
levels, the very next handshake moves the pair up automatically. No flag day, no
coordinated cutover:

```text
full peer <-> older router (L1 only)  ->  L1_BASELINE   (works today)
   ... router software updated, now advertises L1..L3 ...
full peer <-> updated router (L1..L3) ->  L3_PACING     (moves up automatically)
full peer <-> full peer               ->  L4_ECHO
```

## Integrity: offers are MAC-backed

Each offer carries a keyed MAC (SipHash-2-4, [`http3_mac.h`](http3_mac.h)) over
its canonical bytes — a fixed domain tag `H3CP` followed by the 4-byte
big-endian capability mask:

```text
offer.mac = SipHash24(key, "H3CP" || big_endian_u32(capabilities))
```

The domain tag keeps an offer MAC distinct from an envelope MAC under the same
key. `http3_handshake_resolve()` verifies **both** peers' offer MACs before
negotiating, so an attacker cannot silently downgrade (or upgrade) a peer's
advertised capabilities in flight without invalidating the tag.

## API

C ([`http3_handshake.h`](http3_handshake.h)):

```c
http3_cap_offer_t offer;
http3_handshake_make_offer(&offer, HTTP3_CAP_ALL, key);      /* advertise */
http3_handshake_verify_offer(&offer, key);                    /* 1 = authentic */

uint32_t agreed;
http3_handshake_resolve(&local, &remote, key, &agreed);       /* verify + negotiate */
http3_handshake_level_name(agreed);                           /* e.g. "L3_PACING" */
```

Python ([`http3_flow.py`](http3_flow.py)):

```python
full = CapOffer.make(CAP_ALL, key)
old  = CapOffer.make(CAP_L1_BASE, key)          # a not-yet-updated router
level = handshake_resolve(full, old, key)       # -> CAP_L1_BASE
```

## Cross-language parity

C and Python compute **identical** offer MACs and identical negotiated levels
for the same inputs (verified: offer MAC of `CAP_ALL` matches byte-for-byte, and
`full↔old → L1`, `full↔L3 → L3`, `full↔full → L4` in both). The demo
(`make demo`) and the flow tests (`make test`) exercise all three cases.
