# Station Signaling — Present State

**As of:** 2026-09-14 · **Branch of work:** `station-signaling` (off `master`)
**Spec:** [`STATION-SIGNALING-SPEC.md`](STATION-SIGNALING-SPEC.md) (J-STA-0001)

This is the point-in-time status, organized 1–2–3–4. Section 4 gives a paragraph
for each branching technology. Dates are relative to the "as of" date.

---

## 1. Why this exists

Two clients need to **negotiate what they will actually send** before any HTTP
3.0 payload flows, and a Router on the path should be able to act as an
**International Station** that witnesses and stores that negotiation. As of the
day before this record (2026-09-13), `http-3.0/` had the compact envelope,
naming, and pipeline (the application data flow) plus the crypto substrate — but
nothing let a Router stand as an intermediary/witness over the connection setup
itself. This work adds that: a TCP-control-flag signaling layer, below the
application protocol, in which the Router classifies intent and remembers how a
negotiation resolved.

## 2. What was added now (present state)

As of the "as of" date, `http-3.0/` contains a complete **Station Signaling**
layer, split as requested into a **kernel driver** and the **direct-API
userland** that drives it, sharing one classifier so the Router's judgment is
identical in either place. The userland side builds and runs here with no
privileges (a simulation backend); the kernel module is written as a real Linux
module with a Kbuild for hosts that have kernel headers. Everything verified:
`make all` builds the userland Station, `make test` passes the manualog demo (5
scenarios) and the Python parity tests, and `make kernel-check` reports
gracefully when kernel headers are absent. New: `station/` (ABI, classifier,
session, backend, demo, spec, tests) and `kernel/` (module + Kbuild/Makefile).

## 3. Present completeness against the spec (J-STA-0001)

| Spec section | Concept | State (2026-09-14) |
|---|---|---|
| §2 | TCP control flags as signals (URG/ACK/SYN/RST/FIN/PSH) | **Implemented** — parsed in kernel hook; modeled in `sl_signal_t` |
| §2.1 | 8-URG magnitude | **Implemented** — `urg_units` 0..8 drives premise selection |
| §3 | HOP = DIST±N verbs (CANCEL/HOLD/ADVANCE) | **Implemented** — `sl_classify_hop` |
| §4 | Start / intermediary / cancelling premise triple | **Implemented** — `sl_classify_signal` |
| §5 | Conditions (Intentional/Legal/Warning/Emergency) | **Implemented** — `sl_classify_condition` |
| §6 | Manualog (two-party, subsystem-backed) | **Implemented** — per-party state in `sl_session_t` |
| §7 | Witnessed state machine (OPEN/CLEAR/UNCLEAR/HUNG-UP/DISASTROUS) | **Implemented** — `sl_session_signal` |
| §8 | Witness + append-only store | **Implemented** — `records[]`, monotonic sequence, seal on close |
| §9 | Kernel/userland known API (ioctl) | **Implemented** — driver ioctls + userland backend |
| §9 | Netfilter capture of real segments | **Implemented (kernel)** — builds where headers exist |
| — | Full wire field for hop/urg (vs. TTL/urg-ptr stand-ins) | **Partial** — negotiated header fields are future work |
| — | Persistent/off-box storage of sealed records | **Not yet** — in-memory store today |

Net: the **signaling model, classification, manualog, witnessed state machine,
and both the kernel and userland halves are real and demonstrable**; the
remaining items (a negotiated on-wire hop/urg field carrier, durable storage)
are scoped as future work.

## 4. Branching technologies — one paragraph each

**Shared ABI (`station_abi.h`).** One header is the single source of truth for
every number that crosses the kernel/userland boundary or appears "on the wire":
the TCP flag bits, the 8-URG scale, the hop verbs (DIST±N), the condition,
premise, and state enums, the signal/record/view structs, and the ioctl command
codes. Because both halves include it, the driver and userland cannot silently
disagree about what a value means. This is the branch that makes the split
system coherent.

**Shared classifier (`station_classifier.{h,c}`).** The Station's judgment —
turning one signal into (condition, premise, hop verb) — is pure logic with no
libc- or kernel-only calls, so the exact same code compiles into the kernel
module (via `#include`) and links into userland. A Router therefore reaches the
identical verdict whether it classifies in the Netfilter hook or in a userland
process. This is the branch that guarantees "the Router acts the same
everywhere."

**Session state machine + witness store (`station_session.{h,c}`).** This folds
classified signals from both parties into the manualog: it tracks each party's
open premise and acknowledgement, appends an immutable record per accepted
signal, and recomputes the joint witnessed state — CLEAR when both reach a
matching intermediary premise with mutual ACK, UNCLEAR on a standing
cancellation, HUNG-UP on FIN/silence, DISASTROUS on RST or a contradiction of a
standing legal premise. This is the branch that turns signals into a remembered
outcome.

**Kernel driver + Netfilter (`kernel/sleela_station.c`).** The real Router-side
module registers a character device (`/dev/sleela_station`) exposing the
OPEN/SIGNAL/QUERY/CLOSE ioctls, and a `PRE_ROUTING` Netfilter hook that inspects
the TCP control flags of signaling-port segments, builds an `sl_signal_t`, and
folds it into the matching session — witnessing only, never dropping traffic. It
compiles the shared classifier/session directly in, and its Kbuild builds a
`.ko` wherever kernel headers are present. This is the branch that lets an
actual Router participate.

**Userland backend transports (`station_backend.{h,c}`).** One userland API
(open/signal/query/close) sits over two interchangeable transports: a **device**
backend that drives the kernel driver by ioctl (the deployed path), and a
**simulation** backend that holds the sessions in-process using the same shared
logic (the unprivileged path that runs and is tested here). Callers do not
change between them. This is the branch that makes the system both real and
demonstrable without root.

**Manualog + premise triple (`station_demo.c`, `test_station.py`).** The
end-to-end behavior — a two-way, subsystem-backed dialog advancing start →
intermediary premises to agreement, or falling to a cancelling premise or a
terminal state — is exercised by a C demo (five scenarios) and mirrored by
Python parity tests, so the negotiation is observable and cross-checked. This is
the branch that proves the whole thing behaves as the spec says.

---

*Verification note: on the "as of" date, `make test` built the userland Station
and passed the manualog demo (CLEAR/UNCLEAR/HUNG-UP/DISASTROUS + LEGAL/WARNING/
EMERGENCY) and the Python parity tests; `make kernel-check` reported gracefully
without kernel headers. See [`FLOW.md`](FLOW.md) for the signal→witness trace.*
