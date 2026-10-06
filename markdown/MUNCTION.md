# Munction™ — the Reach Composition Language for SLeeLa

    Language element: Munction™ (a reach-composition fluent form)
    Syntax version:   1.3   (the .sleela grammar version that introduces Munction™)
    Compiler:         Sleelvac™ 1.4+ (accepts .sleela syntax 1.0 .. 1.3)
    Surface:          Java-like fluent chain
    Companions:       BODI.md (witness/verb model), NETWORK.md (TCP layer),
                      FILEIO.md / impl/FILEIO.md (file I/O), SHEET.sheet
    Status:           Normative for the Munction™ surface
    Attention:        Max Rupplin — MEARVK LLC — 2026

---

## 0. What Munction™ is

A **Munction™** is a single, sane sentence of *reach*: a fluent chain that
starts at an addressed system, **connects** over one of the known system
methods, moves a datum through **interim** stages, **consumes** what returns,
**latches** the reached state, and **closes with a receipt**. It is SLeeLa's
uniform grammar for reaching *anything the operating system can reach* — a pipe,
a file, the internet, a private packet, or a crypto channel — while protecting
one invariant above all:

> **The structure of Reach is what we protect here.** A Munction™ may traverse
> supposed network hatches or bumps and still continue to reach; what it must
> never do is lose the shape of the reach. Every Munction™ is receivable and
> every send is coherent, or the chain stops at its boundary and reports.

The canonical sentence:

```sleela
Munction.start("xyx")
        .connect("sdps://tmcf:19866")
        .send(data)
        .thatch(interims)
        .consume()
        .consume()
        .latch()
        .closeWithReceipt();
```

Read aloud: *start a reach named "xyx"; connect to a private-packet endpoint;
send the datum; thatch the interims into a covered path; consume, consume; latch
the reached state; close and hand back a receipt.*

---

## 1. The Munction™ sentence

### 1.1 Shape

A Munction™ is a chain of method calls on a **reach in progress**. It always
has three parts:

```text
opener  →  one or more middle verbs  →  closer
```

| Part | Calls | Meaning |
|---|---|---|
| **opener** | `Munction.start(name)` | name and begin an addressed reach |
| **middles** | `connect` `send` `thatch` `consume` `latch` … | move the reach forward |
| **closer** | `closeWithReceipt()` (or `close()`, `abort()`) | end the reach; emit a receipt |

### 1.2 Sanity bound — 4 to 16 calls

A Munction™ sentence must contain **between 4 and 16 function calls inclusive**,
counting the opener, every middle, and the closer. Fewer than four cannot
express a complete reach (name → connect → move → close); more than sixteen is
past the point where one sentence stays sane and should be split into two
Munctions joined by a latch/receipt.

```text
4  <=  call_count(Munction)  <=  16
```

The bound is enforced by the builder at compose time and by the compiler's
Munction gate (§7). A chain outside the bound is rejected — it is not silently
truncated or extended. The sentence may be *arbitrarily long within the bound*:
any 4..16 arrangement whose verbs are legal in sequence (§3) is a sane sentence.

### 1.3 Legibility

A Munction™ is one sentence. It reads left to right, each verb depends only on
the state the previous verb left, and the closer is always reached (even on
failure the closer is `abort()` producing a negative receipt). There is no
hidden control flow inside a Munction™; branching lives in the SLeeLa program
around it.

---

## 2. Reach, receivability, and coherent send

Three properties are guaranteed for every Munction™ object.

### 2.1 Reach

**Reach** is the live, shaped path from the opener to the current verb. It
carries: the address (`name`), the bound channel, the sequence position, the
datum in flight, the accumulated interims, and the witness. Reach is *protected*
— network hatches (NAT, proxies, gateways) and bumps (drops, retries, partial
reads) may perturb transport, but the Reach structure is preserved or the chain
contains at its boundary. A Munction™ never invents meaning to paper over a
bump; it records the residual fact (BODI.md §5) in the receipt.

### 2.2 Receivability

Every Munction™ object is **receivable**: at any point the reach can answer
"what has been received?" and, at the close, it yields a **receipt** — a
witnessed, immutable record of what was actually reached. Receivability is not
optional. A channel that cannot produce a receipt is not a Munction™ channel.

### 2.3 Coherent send

Every `send` is **coherent**: the bytes offered to the channel are the bytes the
channel accounts for, or the discrepancy is recorded. A coherent send reports
the datum's identity (a content digest), the count offered, and the count the
channel acknowledges. Coherence is what lets a receipt be trusted across a
lossy transport: a bump that drops bytes is visible, never silent.

---

## 3. The verb ladder (mapped to BODI™)

Munction™ verbs are drawn from — and ordered by — the BODI™ build-up / teardown
ladder (BODI.md §7). Positive verbs advance the reach; negative verbs retire it.

| Munction™ verb | BODI™ verb | Arity | Effect on the reach |
|---|---|---|---|
| `start(name)` | `register` + `bind` | 1 | name and open a reach (opener; exactly once, first) |
| `connect(uri)` | `connect` | 1 | bind the reach to a channel resolved from the URI scheme (§4) |
| `open()` | `open` | 0 | explicitly open the resource (implied by `connect`; optional) |
| `enable(policy)` | `enable` | 1 | permit an optional capability (e.g. `retry`, `crypto`, `compress`) |
| `send(datum)` | `push` | 1 | coherently push a datum onto the channel |
| `thatch(interims)` | `attach` | 1 | weave interim stages over the path (a thatched roof over the reach) |
| `consume()` | `pull` | 0 | pull one available unit of what the channel returns (repeatable) |
| `observe()` | `observe` | 0 | record channel state into the witness without consuming |
| `propagate(uri)` | `propagate` | 1 | carry the reach onward to a further endpoint (federated, degree ≤ 4) |
| `latch()` | `commit` | 0 | hold/commit the reached state so it survives the close |
| `contain()` | `contain` | 0 | stop propagation at the boundary (safe partial close) |
| `close()` | `close`+`disconnect`+…+`unregister` | 0 | orderly teardown, plain closer |
| `closeWithReceipt()` | `close` + `commit(witness)` | 0 | teardown that returns the receipt (preferred closer) |
| `abort()` | `contain` + `rollback` | 0 | negative closer: stop now, receipt records the residual |

### 3.1 Legal sequence

```text
start
  ( connect ( open? enable* ) )
  ( send | thatch | consume | observe | propagate | latch )+
  ( closeWithReceipt | close | abort )
```

Rules the sequence enforces:

1. `start` is first and appears exactly once.
2. `connect` precedes any `send` / `consume` / `propagate` (you cannot move on
   an unbound reach). `open` and `enable` may only appear between `connect` and
   the first data verb.
3. `send` is coherent (§2.3); `thatch` must precede the `consume` stages it
   covers; `consume` is repeatable (`.consume().consume()`), each pulls one unit.
4. `latch` may appear at most once and, when present, precedes the closer.
5. Exactly one closer, last. `closeWithReceipt` is preferred; `abort` is the
   negative closer used when a bump makes the reach unsafe to complete.
6. `propagate` is bounded by the Constitution: federated depth ≤ 4, reach depth
   ≤ 3024 (SLEELA.syntax §8). Exceeding it is a boundary stop, not an error to
   hide.

A sequence that violates any rule is rejected at compose time (builder) and at
compile time (Munction gate), never repaired silently.

---

## 4. System-method channels — reaching everything

`connect(uri)` resolves a channel from the URI **scheme**. Munction™ reaches
through **all known system methods** with one grammar; only the scheme changes.

| Scheme(s) | Channel | System method | Notes |
|---|---|---|---|
| `pipe:` | `PipeChannel` | OS pipe / FIFO / stdio | named FIFO or anonymous pipe pair; line- or block-framed |
| `file:` | `FileChannel` | file I/O | `send` appends/writes, `consume` reads; honors the VM file table |
| `tcp:` `net:` | `NetChannel` | internet (TCP) | built on the NETWORK.md six-primitive TCP layer |
| `http:` `https:` | `HttpChannel` | internet (HTTP) | request/response over the connector HTTP path |
| `sdps:` | `PacketChannel` | private packets | **Secure Datum Packet Stream** — framed private packets (§4.1) |
| `crypto:` | `CryptoChannel` | crypto | an envelope wrapping another channel; seals/opens the datum (§4.2) |

A channel is any object that implements the receivability + coherent-send
contract (§5). New system methods are added by registering a scheme → channel;
the Munction™ grammar does not change.

### 4.1 `sdps://` — Secure Datum Packet Stream (private packets)

`sdps://host:port` is the private-packet channel used by the canonical sentence
(`"sdps://tmcf:19866"`). It frames each `send` as a **private packet**:

```text
+----------+-----------+------------------+-------------+
| magic    | length    | datum (opaque)   | packet-mac  |
| "SDPS"   | uint32 BE | length bytes     | 32 bytes    |
+----------+-----------+------------------+-------------+
```

- Each packet is length-prefixed and MAC-tagged, so a bump that truncates a
  packet is detected (coherent send).
- Packets are private: the payload is opaque on the wire; framing metadata never
  carries the datum's meaning.
- Delivery is receivable: every packet sent yields a per-packet acknowledgment
  folded into the receipt.

### 4.2 `crypto:` — the crypto envelope

`crypto:` does not name a transport; it **wraps** one. `connect("crypto:tcp://…")`
binds a `NetChannel` inside a `CryptoChannel` that seals each `send` and opens
each `consume`. The envelope contract:

- **Seal on send:** the datum is sealed (authenticated encryption) before it
  reaches the inner channel; the seal's tag participates in coherent-send
  accounting.
- **Open on consume:** returned bytes are opened and verified before the reach
  sees them; a failed verification is a bump recorded in the receipt, never a
  silently accepted plaintext.
- **Keying is out of band:** Munction™ carries no key material in the URI. Keys
  are supplied through `enable("crypto:<keyref>")`, a key reference resolved by
  the host, never a literal secret in the sentence.

---

## 5. The channel contract (receivability + coherent send)

Every channel a Munction™ can `connect` to implements one small contract, so
the same sentence works over pipes, files, the internet, private packets, and
crypto.

```text
MunctionChannel
  scheme()                 -> String                 // e.g. "sdps"
  open(address)            -> void                    // bind the OS resource
  send(datum)              -> SendCoherence            // coherent push (§2.3)
  consume()                -> Reception                // pull one unit (receivable)
  thatch(interims)         -> void                     // register interim stages
  observe()                -> ChannelState             // read state, no consume
  latch()                  -> void                     // hold reached state
  close()                  -> ChannelReceipt           // teardown + partial receipt
```

- **`SendCoherence`** = `{ datumDigest, offered, acknowledged, coherent }`.
  `coherent` is true iff `offered == acknowledged` (or the channel's framing
  otherwise proves full acceptance).
- **`Reception`** = `{ present, datum, digest, sequence }`. `present` is false at
  end-of-reach; a Munction™ never blocks a `consume` forever — it honors the
  channel's bound (NETWORK.md bounded reads, VM file table, etc.).
- **`ChannelReceipt`** is the channel's contribution to the Munction™ receipt.

A channel that cannot satisfy this contract is not receivable and must not be
registered as a system method.

---

## 6. The receipt

`closeWithReceipt()` returns a **`MunctionReceipt`**: an immutable, witnessed
record of what the reach actually did. It is the durable proof of receivability.

```text
MunctionReceipt
  name           the reach name from start(...)
  scheme         the connected system method
  address        the resolved endpoint (no secrets)
  sent           list of SendCoherence (per send)
  received       list of Reception     (per consume)
  interims       the thatched interim stages that ran
  latched        whether the reached state was committed
  outcome        REACHED | CONTAINED | ABORTED
  bumps          residual facts: retries, partial reads, verify failures
  witness        BODI™ witness: system, sequence, verb, datum, boundary, man
  startedAt / closedAt
```

- **`REACHED`** — the sentence completed and latched (or closed cleanly).
- **`CONTAINED`** — a bump stopped propagation at the boundary; what was reached
  is real and recorded, the rest was safely not attempted.
- **`ABORTED`** — the reach was retired early; the receipt records the residual
  (BODI.md §5: no fictional rollback).

A receipt never claims a send that was not acknowledged, and never claims a
reception that did not arrive. That is the whole point: the receipt is the
protected structure of the Reach.

---

## 7. Compiler integration (syntax 1.3)

Munction™ is a **syntax 1.3** feature. A Wrapper™ using it declares:

```text
#sleela 1.3
```

Following the NETWORK.md model, Munction™ needs **no new lexer or parser
keywords**: `Munction.start(...)` is an ordinary qualified call and the rest of
the chain is ordinary fluent member-call syntax the parser already accepts
(SLEELA.syntax §5). Recognition and gating happen at the compiler's built-in
dispatch point:

1. The compiler recognizes a chain rooted at the contextual name `Munction`.
2. It checks the **4..16 call bound** (§1.2) and the **legal sequence** (§3.1).
3. It verifies exactly one opener first and one closer last, and that data verbs
   follow a `connect`.
4. Under an explicitly declared `#sleela 1.2` (or lower) program, a `Munction`
   chain is rejected — the *meaning* of the recognized name is version-gated,
   exactly as the network built-ins are (NETWORK.md §10).

Like the network layer, Munction™ keeps the grammar stable while growing the
runtime vocabulary through compiler/core coordination. The channel set (§4) is a
registration table, so a new system method is a channel registration, not a
grammar change.

---

## 8. Worked sentences (4..16 calls)

**Minimum (4 calls) — reach a file and receipt it:**

```sleela
#sleela 1.3
Munction.start("audit").connect("file:./ledger.log").send(entry).closeWithReceipt();
```

**Canonical private-packet reach (8 calls):**

```sleela
#sleela 1.3
Munction.start("xyx")
        .connect("sdps://tmcf:19866")
        .send(data)
        .thatch(interims)
        .consume()
        .consume()
        .latch()
        .closeWithReceipt();
```

**Crypto-over-TCP with retry and propagation (13 calls, still sane):**

```sleela
#sleela 1.3
Munction.start("settle")
        .connect("crypto:tcp://clearing:8443")
        .enable("crypto:kref-2026")
        .enable("retry")
        .open()
        .send(order)
        .thatch(interims)
        .consume()
        .observe()
        .propagate("sdps://mirror:19866")
        .consume()
        .latch()
        .closeWithReceipt();
```

Each of these is one sane sentence within the 4..16 bound; each is receivable
and closes with a receipt.

---

## 9. Design ordinance (why Munction™ is shaped this way)

1. **One grammar, every system method.** Pipes, files, internet, private
   packets, and crypto are reached by the same sentence; only the scheme differs.
2. **Reach is the protected structure.** Transport may bump; the reach's shape
   and the truth of the receipt may not.
3. **Receivable by construction.** No channel is admitted that cannot produce a
   receipt; no `consume` fabricates a reception.
4. **Coherent send by construction.** Every send accounts offered vs.
   acknowledged bytes; loss is visible.
5. **Bounded and sane.** 4..16 calls, a legal verb ladder, one opener, one
   closer. Longer intent is two Munctions joined by a latch/receipt.
6. **Witnessed.** Every Munction™ sits inside the BODI™ mitigative circumference
   and yields a witness in its receipt.

---

Munction™ (syntax 1.3) — the reach-composition sentence for SLeeLa: one fluent
grammar that reaches every system method, protects the structure of Reach, and
always closes with a receipt. — Max Rupplin — MEARVK LLC — 2026.
