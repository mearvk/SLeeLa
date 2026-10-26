# MUNCTION.md — the Munction™ reach-composition language (normative spec)

**Attention:** Max Rupplin — MEARVK LLC — 2026
**Syntax version introducing Munction™:** 1.3 (supported range 1.3 .. current)
**Status:** Normative. This is the specification the Munction™ sources cite as
`MUNCTION.md §N` (the C core `impl/core/sleela_munction.c`, the C++ compiler gate in
`impl/frontend/compiler.cpp`, and the Java reference engine under
[`java/com/mearvk/sleela/munction/`](./java/com/mearvk/sleela/munction)).

> Companion documents: [`COMPILER_NOTE.md`](./COMPILER_NOTE.md) (how the compiler gates it) and
> [`MANUAL.md`](./MANUAL.md) (how to build and run it).

---

## 0. What a Munction™ is

A **Munction™** is **one sane sentence of _reach_**: a single, bounded, fluent chain that opens
an addressed reach, moves a datum over a **system-method channel** (pipe, file, internet, private
packet, or crypto — chosen by the URI scheme), takes back what returns, holds the reached state,
and closes with a **receipt**. It is the reach-composition counterpart of the six-primitive
network layer (`NETWORK.md`): where the network layer is the raw verbs, a Munction™ is the one
composed sentence that uses them responsibly.

Two invariants hold for every Munction™:

- **Receivability.** A Munction™ *always* yields a [`MunctionReceipt`](#6-the-receipt-6). It never
  claims a send that was not acknowledged nor a reception that did not arrive.
- **Coherent send.** Every `send` is accounted: offered bytes vs. acknowledged bytes
  ([`SendCoherence`](#52-sendcoherence)). An unacknowledged or partial send is recorded as a
  **bump**, never papered over.

The guiding discipline is **BODI™** — a fixed verb ladder with a boundary stop: when a sentence
violates its shape, the engine **stops at the boundary and records the fact**, rather than
inventing meaning or silently repairing the chain.

---

## 1. The sentence and its bound

### 1.1 Shape
A Munction™ sentence is a fluent chain rooted at the contextual name `Munction`:

```text
Munction.start(name)        <- opener (exactly once, first)
        .connect(uri)        <- bind a channel from the URI scheme
        [ .open() ]          <- optional, explicit resource open
        [ .enable(policy) ]  <- optional capability (retry, crypto:<keyref>)
        .send(datum)         <- coherent push  (a data verb)
        [ .thatch(interims) ]<- weave interim stages over the path
        .consume()           <- pull one returned unit (repeatable)
        [ .observe() ]       <- record state without consuming
        [ .propagate(uri) ]  <- carry the reach onward (degree <= 4)
        [ .latch() ]         <- hold/commit reached state (at most once)
        .closeWithReceipt()  <- closer (exactly once, last)
```

### 1.2 The sanity bound (§1.2)
A sane reach sentence contains **4 to 16 calls inclusive**, counting the opener, every middle,
and the closer (`MIN_CALLS = 4`, `MAX_CALLS = 16`).

- Fewer than 4 calls is not a complete reach (name → connect → move → close is the minimum).
- More than 16 calls is **not one sane sentence**; longer intent must split into two Munctions.
- A sentence whose call count falls outside 4..16 **does not fail destructively**: teardown still
  happens, the receipt records a bump, and the outcome is `CONTAINED` (never silently truncated or
  extended).

---

## 2. The BODI™ verb ladder

### 2.1 The verbs by role

| Role | Verb | Meaning |
|------|------|---------|
| **Opener** | `start(name)` | Open a named reach (register + bind). Exactly once, first. |
| Middle | `connect(uri)` | Bind the reach to a channel resolved from the URI scheme. Must immediately follow `start`. |
| Middle | `open()` | Explicitly open the resource (already implied by `connect`; optional). |
| Middle | `enable(policy)` | Permit a capability, e.g. `retry` or `crypto:<keyref>`. |
| Middle (data) | `send(datum)` | Coherently push a datum (`byte[]` or UTF-8 `String`). |
| Middle | `thatch(interims)` | Weave named interim stages over the covered path; precedes `consume`. |
| Middle (data) | `consume()` | Pull one available returned unit. Repeatable. |
| Middle | `observe()` | Record channel state into the witness without consuming. |
| Middle (data) | `propagate(uri)` | Carry the reach onward to a further endpoint (federated degree ≤ 4). |
| Middle | `latch()` | Hold/commit the reached state so it survives the close. At most once. |
| **Closer** | `closeWithReceipt()` | Preferred closer: orderly teardown, returns the receipt. |
| Closer | `close()` | Plain closer (alias of `closeWithReceipt`; receipt discarded by caller). |
| Closer | `abort()` | Negative closer: retire the reach now; the receipt records the residual. |

### 2.2 Data verbs require a connect (§2.2)
Every **data verb** (`send`, `consume`, `propagate`) is legal only after a `connect` has bound a
channel. A data verb issued while the sentence is still merely `STARTED` is a boundary stop.

### 2.3 Capability placement (§2.3)
`open` and `enable` are legal only **between `connect` and the first data verb** (while the reach
is `CONNECTED` but not yet `MOVING`). A `crypto:<keyref>` capability resolves a key **reference**
(never a secret) into the crypto envelope; the keyref is recorded, the key bytes never are.

---

## 3. Legal sequence (§3) and the phase machine (§3.1)

A Munction™ progresses through four phases; each verb is policed against the current phase:

```text
STARTED ──connect──▶ CONNECTED ──(send|consume|thatch|propagate)──▶ MOVING ──close──▶ CLOSED
```

### 3.1 Sequence rules (§3.1)
1. **Exactly one opener** (`start`), first.
2. **Exactly one closer** (`closeWithReceipt` / `close` / `abort`), last.
3. `connect` must **immediately follow** `start`.
4. Every **data verb** (`send`, `consume`, `propagate`) follows a `connect`.
5. `open` / `enable` only between `connect` and the first data verb.
6. `latch` **at most once**, and before the closer.
7. Any verb after `CLOSED` is a boundary stop ("the sentence is already closed").

A violation of any rule is a **BODI™ boundary stop**: it raises a semantic/boundary error, records
the reason as a bump, and never silently repairs the chain.

---

## 4. System methods — the channel set (§4)

A Munction™ reaches a **system method** chosen by the URI **scheme**. Reaching a new system method
is a **channel registration**, not a grammar change: the sentence stays fixed while the runtime
vocabulary grows. The scheme → channel registry is `MunctionChannels` (Java) / the channel table
in the C core.

| Scheme(s) | System method | Channel | Notes |
|-----------|---------------|---------|-------|
| `pipe:` | OS pipe / FIFO / stdio | `PipeChannel` | in-process framed buffer in the reference core |
| `file:` | file I/O | `FileChannel` | mirrors to disk; the minimum sane sentence targets this |
| `tcp:` / `net:` | internet (TCP) | `NetChannel` | a real connected socket; bounded 4096-byte reads |
| `http:` / `https:` | internet (HTTP) | `HttpChannel` | request/response over HTTP |
| `sdps:` | private packets (SDPS) | `PacketChannel` | magic + length + MAC framing |
| `crypto:` | crypto envelope | `CryptoChannel` | wraps an inner channel (see §4.2) |

### 4.1 URI resolution (§4.1)
- **Scheme** is the text before the first `:` (lower-cased).
- **Address** is everything after `scheme:`, with a leading `//` stripped (`tcp://host:port` →
  `host:port`).
- A URI with no scheme is rejected: `connect` requires a scheme.
- The **witness** and receipt record only `host:port`/path — never query strings or user-info that
  could carry a secret.

### 4.2 The crypto envelope (§4.2)
`crypto:` does **not** name a transport; it **wraps** one. `connect("crypto:tcp://host:port")`
binds the inner `tcp` channel inside a crypto envelope that **seals** each `send` before it reaches
the inner channel and **opens** each `consume` after the inner channel returns. The seal is
**authenticated**: a per-datum tag is verified on open, so a tampered or truncated payload **fails
to open** (recorded as a bump) rather than being silently accepted as plaintext. The key is
supplied out of band via `enable("crypto:<keyref>")`; absent a resolved key, a clearly-marked
development seal is used so the reach still demonstrably seals/opens end to end.

---

## 5. The channel contract (§5)

Every system-method channel implements one contract
([`MunctionChannel`](./java/com/mearvk/sleela/munction/MunctionChannel.java)), so a single sentence
reaches all system methods uniformly:

| Method | Obligation |
|--------|-----------|
| `scheme()` | the URI scheme served |
| `open(address)` | bind the underlying OS resource |
| `send(datum)` → `SendCoherence` | coherent push: account offered vs. acknowledged bytes |
| `thatch(interims)` | register interim stages woven over the flow |
| `consume()` → `Reception` | pull one unit; an **absent** reception marks end-of-reach |
| `observe()` → state string | record state without consuming |
| `latch()` | hold/commit the reached state |
| `close()` → `ChannelReceipt` | teardown; the channel's contribution to the receipt |

A channel that cannot satisfy **receivability** (truthful `consume`/`close`) and **coherent send**
is not a Munction™ channel and must not be registered.

### 5.1 Reception
A `Reception` is one returned unit: its bytes, a digest, and a monotone sequence number — or
**`Reception.absent()`**, the truthful end-of-reach marker. The engine never fabricates a present
reception to fill a chain.

### 5.2 SendCoherence
A `SendCoherence` is the send accounting: the datum digest, the **offered** byte count, the
**acknowledged** byte count, and a `coherent()` flag (offered == acknowledged and the send
succeeded). An incoherent send is recorded as a bump on the receipt.

### 5.3 Interims
`Interims` are named interim stages (e.g. `frame`, `checksum`, `seal`) woven over the covered path
with `thatch`. They transform the datum deterministically and their names are recorded on the
receipt.

---

## 6. The receipt (§6)

`closeWithReceipt()` returns an **immutable, witnessed** `MunctionReceipt` — the durable proof of
receivability. It records:

| Field | Meaning |
|-------|---------|
| `name` | the reach name from `start` |
| `scheme`, `address` | the bound system method and its safe address |
| `sent` | the list of `SendCoherence` accountings |
| `received` | the list of `Reception`s (present and absent, truthfully) |
| `interims` | the names of woven interim stages |
| `latched` | whether the reach committed its state |
| `outcome` | `REACHED`, `CONTAINED`, or `ABORTED` (see below) |
| `bumps` | every recorded residual fact (failed/incoherent send, boundary, close bump) |
| `witness` | the `munction:<name>#<call>:<verb>` witness trail (addresses/secrets excluded) |
| `startedAt`, `closedAt` | the reach's time bounds |

**Outcomes:**

| Outcome | Meaning |
|---------|---------|
| `REACHED` | completed and latched, or closed cleanly |
| `CONTAINED` | a bump stopped propagation at the boundary; what was reached is real |
| `ABORTED` | retired early via `abort()`; the receipt records the residual (no fictional rollback) |

`fullyCoherent()` is true only when every recorded send was coherent **and** no bump was noted.

---

## 7. Boundary, not repair

The single most important behavioral rule, applied everywhere above: **a shape violation is a
boundary stop, recorded, never invented past.** The engine will:

- stop at the first illegal verb and raise a boundary error (the message is recorded as a bump),
- never silently truncate a too-long chain or extend a too-short one,
- never fabricate a reception, acknowledge a send that was not acknowledged, or hide a transport
  bump — all of these become truthful residual facts on the receipt.

This is what makes a Munction™ receipt trustworthy: it is the protected structure of the reach, not
a success story.

---

## 8. Canonical examples (§8)

The reference programs live in [`examples/`](./examples); their reference output is
[`examples/EXPECTED_OUTPUT.txt`](./examples/EXPECTED_OUTPUT.txt).

**The canonical sentence** ([`reach_xyx.sleela`](./examples/reach_xyx.sleela), 8 calls over
`sdps://`):

```text
Munction.start("xyx")
        .connect("sdps://tmcf:19866")
        .send(data)
        .thatch(interims)
        .consume()
        .consume()
        .latch()
        .closeWithReceipt();
```

**One grammar over every system method**
([`reach_all_methods.sleela`](./examples/reach_all_methods.sleela)) — the same sentence shape,
only the `connect()` scheme changes, from the 4-call minimum (`file:`) to a 13-call crypto-over-TCP
reach. Every reach is receivable and closes with a receipt.

---

## 9. Version awareness

| Item | Value |
|------|-------|
| Syntax version introducing Munction™ | **1.3** |
| New lexer / parser tokens | **none** (ordinary qualified + fluent member calls) |
| Recognition point | the compiler built-in dispatch (`tryEmitBuiltin`, `impl/frontend/compiler.cpp`) |
| Rejection under `#sleela 1.2` or lower | **required** (the meaning of `Munction` is version-gated) |
| Call-count bound | **4 .. 16** inclusive |
| Channel set | registration table, not grammar — see §4 |

See [`COMPILER_NOTE.md`](./COMPILER_NOTE.md) for the compiler-gate detail.

---

<!-- ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- -->
<!--
metaVersion: 1.3.0
metaId: none
metaAuthor: Max Rupplin
metaDate: 2026-10-25
metaCopyright: (c) 2026 MEARVK LLC
metaTrademark: Munction (TM), BODI (TM)
metaCorporation: MEARVK LLC
metaOrigin: National (United States)
metaBirthDate: 2026
metaScienceRating: 6
metaNationalSchedule: none (project-internal normative spec; cites NETWORK.md and SLEELA.syntax)
-->
