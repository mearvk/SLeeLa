# SLeeLa HTTP 3.0 — Station Signaling Specification

**Series:** J-STA-0001 · **Status:** Normative specification, rev 1.0.0
**Layer:** below the HTTP 3.0 application protocol; rides on TCP/IP control
semantics.

## 0. Purpose

Before two clients exchange any HTTP 3.0 payload, they must be able to
**negotiate what they will actually send**, and a Router on the path must be
able to **witness and record** that negotiation as an *International Station*.

This specification defines how that negotiation is signaled using TCP/IP
control semantics (the URG/ACK/SYN/RST/FIN/PSH flag family) plus a hop-distance
encoding, how a Router classifies each signal into a **condition**, how the
negotiation advances through a three-phase **premise**, and how the Station
witnesses and stores the outcome — including the terminal states **UNCLEAR**,
**HUNG-UP**, and **DISASTROUS**.

Because setting raw TCP control flags requires privileged, kernel-level access,
the normative behavior is split:

- **A kernel driver** (`http-3.0/kernel/`) inspects/marks TCP control flags on
  signaling segments via a Netfilter hook and exposes a character-device ioctl
  API — the *known API* for direct calls.
- **Userland** (`http-3.0/station/`) drives the negotiation state machine
  through that API, or through an equivalent in-process **simulation backend**
  when raw-socket privileges are unavailable.

Both use the **same classifier** so the Station's judgment is identical whether
it runs in the kernel hook or in userland.

## 1. Roles

| Role | Who | Responsibility |
|---|---|---|
| **Party A / Party B** | the two negotiating clients | emit signaling segments stating intent |
| **Station** | a Router on the path | classify signals, advance premises, witness + store, decide terminal state |

The Station is an **intermediary and witness**, not a party: it never invents a
premise, it records what the parties signal.

## 2. The wire: TCP control flags as signals

Signaling segments carry no application payload; meaning is in the flags
(RFC 793 bit positions; see `station_abi.h`):

| Flag | Bit | Signaling use |
|---|---|---|
| `SYN` | 0x02 | begin a negotiation turn |
| `ACK` | 0x10 | acknowledge / accept the peer's premise |
| `URG` | 0x20 | this segment carries a premise signal; magnitude in the URGENT count |
| `PSH` | 0x08 | request the Station to witness/record now |
| `RST` | 0x04 | abort → **DISASTROUS** |
| `FIN` | 0x01 | orderly close of a party's participation |

### 2.1 The URGENT count (the "8 URG")

The URGENT pointer field carries a small integer `urg_units` in `0..8`
(`SL_STATION_URG_MAX = 8`). It selects the **premise bank** and intensity:

- `urg_units == 0` — no premise carried (pure ACK/keepalive).
- `1..8` — a premise signal; **8 URG** is the full-scale signal that, combined
  with the hop verb, selects among the three premises (§4).

## 3. HOP distance encoding (DIST±N)

Every Station knows its own hop index on the path, **DIST**. A signaling
segment carries a hop field; the **relation** of that field to DIST is the verb
(see `sl_hop_verb_t`):

| Encoding | Verb | Meaning |
|---|---|---|
| `HOP = DIST + 0` | **HOLD** | witness/record at this station; do not advance the premise |
| `HOP = DIST + 1` | **ADVANCE** | move the premise forward one phase |
| `HOP = DIST − 1` | **CANCEL** | withdraw / roll back the current premise |

`hop_delta = hop − DIST` is what the driver reports; only −1, 0, +1 are
meaningful here.

## 4. The three-phase premise

A negotiation turn moves through three premises (`sl_premise_t`):

1. **START** — a party's opening premise ("here is what I propose to send").
2. **INTERMEDIARY** — a mediated or counter premise (the middle of the manualog).
3. **CANCELLING** — withdrawal of a premise.

The premise selected by a signal is a function of the **8-URG signal** and the
**hop verb**:

| urg_units | hop verb | premise |
|---|---|---|
| 1..8 | ADVANCE (DIST+1) | **START** if none open, else **INTERMEDIARY** |
| 1..8 | HOLD (DIST+0) | re-assert the current premise (witnessed, not advanced) |
| any | CANCEL (DIST−1) | **CANCELLING** |
| 0 | any | no premise (bare ACK) |

So the triple *start premise → intermediary premise → cancelling premise* is
exactly: an ADVANCE that opens START, a later ADVANCE that yields INTERMEDIARY,
and a CANCEL that yields CANCELLING.

## 5. Conditions

The Station classifies each premise-bearing signal into a **condition**
(`sl_condition_t`), from the flag combination:

| Condition | Signaled by | Meaning |
|---|---|---|
| **INTENTIONAL** | `URG` set, `urg_units≥1`, ADVANCE/HOLD | a deliberate premise step |
| **LEGAL** | `URG+ACK` with full-scale `urg_units==8` | a legally-framed assertion (legal dressing attaches) |
| **WARNING** | `URG+PSH` | a caution; the peer must ACK before further advance |
| **EMERGENCY** | `URG+PSH+ACK`, `urg_units==8` | time-critical; expedited witnessing |
| **NONE** | no `URG` | not a premise signal (bare ACK/keepalive) |

A condition is descriptive evidence, not authority: a LEGAL condition records a
legal *framing*, it does not adjudicate legality (consistent with the HTTP 3.0
guiding principle that context is never authority).

## 6. Manualog

The negotiation is a **manualog**: a two-way, subsystem-backed dialog in which
**both parties and their subsystems participate**. Each party maintains its own
premise state; the Station maintains the joint witnessed view. A turn is:

```text
Party A --signal(cond,premise,verb)--> Station --witness--> record
Party B --signal(cond,premise,verb)--> Station --witness--> record
        ... alternating, until agreement or a terminal state ...
```

Agreement (**CLEAR**) requires both parties to have advanced to a matching
premise and each to have `ACK`ed the other (§7).

## 7. Witnessed state machine

The Station tracks a session state (`sl_state_t`):

```text
IDLE ──open──> OPEN ──both parties ACK a matching INTERMEDIARY──> CLEAR
                │
                ├── a party sends CANCELLING then no re-open ──> UNCLEAR
                ├── a party FIN / goes silent past timeout ────> HUNG_UP
                └── RST, or contradictory/illegal premises ────> DISASTROUS
```

- **CLEAR** — both parties agreed on what to send.
- **UNCLEAR** — ended without a definite agreement (e.g. dangling CANCELLING).
- **HUNG-UP** — a party stopped responding (FIN or silence past timeout).
- **DISASTROUS** — `RST`/abort, or a party asserts a premise that contradicts a
  LEGAL condition already witnessed.

## 8. Witness and storage

For every accepted signal the Station stores a `sl_record_t`
(session, party, condition, premise, hop verb, timestamp, monotonic sequence).
The sealed record of a session is the Station's evidence of the negotiation.
Storage rules:

1. Records are append-only within a session; sequence numbers are monotonic.
2. A session is **sealed** on CLOSE; sealed records are not mutated.
3. The stored terminal state is authoritative for "what the Station witnessed",
   distinct from any claim about what the parties *intended* (evidence, not
   truth).

## 9. Kernel / userland split (the "known API")

`station_abi.h` is the contract. The driver exposes `/dev/sleela_station` with:

| ioctl | Direction | Effect |
|---|---|---|
| `SL_IOC_OPEN` | IOWR `sl_session_view_t` | open a witnessed session; kernel assigns `session_id` |
| `SL_IOC_SIGNAL` | IOWR `sl_signal_t` | feed one signaling event; returns the classified `sl_record_t` |
| `SL_IOC_QUERY` | IOWR `sl_session_view_t` | read a session's current witnessed view |
| `SL_IOC_CLOSE` | IOW `uint64_t` | seal and close a session |

The kernel Netfilter hook parses TCP headers of signaling segments into
`sl_signal_t` and runs the shared classifier; userland may instead submit
`sl_signal_t` directly through `SL_IOC_SIGNAL` (or the simulation backend),
yielding identical classification.

## 10. Governing principle

> The Station makes a negotiation **legible and witnessed** without becoming a
> party to it. It records conditions, premises, and outcomes as evidence — never
> as adjudication — so two clients can agree on what to send, and a Router can
> stand as an International Station that remembers how they got there.

## Reference layout

```
http-3.0/
  station/
    station_abi.h              the driver<->userland contract (this spec's numbers)
    station_classifier.{h,c}   shared Station judgment (kernel + userland)
    station_session.{h,c}      session/state machine + witnessed store
    station_backend.{h,c}      device (ioctl) OR simulation transport
    station_demo.c             two-party manualog trace
    test_station.py            Python parity/behavior tests
  kernel/
    sleela_station.c           the Linux kernel module (char dev + Netfilter)
    Kbuild, Makefile           kernel build
```
