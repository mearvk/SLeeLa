# Bonding collar — conferring with an OS capability layer

`bonding_collar.py` is the SLeeLa-side **bonding collar**: the careful
negotiator a SLeeLa executable uses to discover, on a **new or otherwise
unknown operating system**, whether the host confers a real advantage — and to
bond with it only as far as both sides can *prove*.

It is the client half of a capability confer. A conforming host responder — for
example a **TAC3-class** filesystem/OS layer — may attest to the capability
*names* below; the collar independently verifies that attestation and decides
the tier. TAC3 is the operator's own creation; this collar contains **none of
its internals** and does not depend on it.

## What it does

1. **Probe** — look for a host capability responder; never assume one exists.
2. **Confer** — exchange MAC-backed capability offers (same keyed-MAC primitive,
   `siphash24`, as the HTTP 3.0 capability handshake).
3. **Bond** — agree on the highest tier **both** sides attest to, under a shared
   secret.
4. **Fail closed** — on any unknown, unattested, or untrusted host, drop to
   `NONE` and run in SLeeLa's safe, host-agnostic mode.

## Capability tiers (negotiated by name, never by host internals)

| Tier | Name | Meaning (advantage conferred) |
|---|---|---|
| `0x00` | `NONE` | No attested advantage — safe baseline / unknown OS |
| `0x01` | `T1_PRESENT` | A capability responder is present and attested |
| `0x02` | `T2_STORAGE` | Durable / redundant storage advantage available |
| `0x04` | `T3_CONTEXT` | Contextual-identity / health metadata available |
| `0x08` | `T4_RECOVERY` | Protected boot/recovery + integrity attestation |

Tiers are **cumulative** and describe *what* advantage a host confers, never
*how* it implements it. `CAP_BASELINE = T1_PRESENT`.

## Protecting our advantage (the design contract)

This is deliberately the **client side only**, and it embeds **no proprietary
OS/filesystem internals** — no on-disk format, no kernel structures, no
host-specific field layouts or magic numbers. It negotiates purely by neutral
capability *names* plus an opaque, MAC-attested tag. Therefore:

- On a **trusted, attested** host, the collar unlocks the advantage tiers.
- On a **new / unknown / hostile** host, the collar reveals nothing proprietary
  and drops to `NONE` — so probing can never leak our advantage.

The collar tells SLeeLa **whether** an advantage is present and **which tier** to
use. It never encodes how the host provides it.

### Fail-closed rules

`confer()` returns `NONE` (not bonded) when:

- there is **no responder** / no offer (unknown OS);
- the offer's **protocol** does not match;
- the offer's **attestation MAC fails** (untrusted host, or caps tampered after
  signing);
- there is **no common baseline** capability.

Otherwise it bonds at the highest mutually-attested tier — and **never above the
local collar's own declared support** (we never advertise more than we have).

## Usage

```python
from bonding_collar import BondingCollar, probe_env_responder, CAP_ALL, CAP_T2_STORAGE

collar = BondingCollar(local_caps=CAP_ALL, key=shared_secret)   # 16-byte key
result = collar.confer(probe_env_responder(shared_secret))

if result.bonded and result.tier >= CAP_T2_STORAGE:
    ...  # safe to use the durable-storage advantage
else:
    ...  # unknown/untrusted host -> run host-agnostic baseline
print(result.tier_name, "-", result.reason)
```

The bundled probe `probe_env_responder()` is intentionally conservative: it
reads an attestation from the environment (`SLEELA_OS_CAPS`,
`SLEELA_OS_CAP_MAC`), makes **no privileged calls, mounts nothing, and loads no
host code**, and returns `None` when the host provides nothing (→ fail closed).
A deployment may add richer responders (a well-known device node or socket); the
contract is unchanged — return a `CollarOffer` the collar will independently
MAC-verify, or `None`.

## Run the self-demo

```bash
cd http-3.0 && python3 bonding_collar.py
```

It shows the three canonical outcomes: unknown OS → fail closed; attested host →
bond at the highest common tier; forged host (wrong key) → fail closed.

## Relationship to the HTTP 3.0 handshake

The collar mirrors the shape of the H3 capability handshake in
[`http3_flow.py`](http3_flow.py) (`CapOffer` / `handshake_negotiate`) — MAC-backed
offers, a cumulative tier bitmask, and a baseline fallback — but negotiates
**OS/host advantage** rather than transport behavior, with its own domain tag
(`SLBC`) so the two handshakes can never be confused.
