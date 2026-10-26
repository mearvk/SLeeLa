# MANUAL.md — using Munction™

A practical guide to building, running, and writing **Munction™** reaches. For the normative
language definition see [`MUNCTION.md`](./MUNCTION.md); for the compiler gate see
[`COMPILER_NOTE.md`](./COMPILER_NOTE.md).

---

## 1. What Munction™ is, in one minute

A Munction™ is **one sane sentence of reach**: a bounded fluent chain that opens a named reach,
connects a **system-method channel** by URI scheme (pipe, file, internet, private packet, or
crypto), moves a datum, takes back what returns, holds the reached state, and **closes with a
receipt**. Every reach is *receivable* (always returns a receipt) and every send is *coherent*
(offered vs. acknowledged bytes are accounted). A sentence is bounded to **4..16 calls**; breaking
the shape **stops at the boundary** and records the fact rather than inventing meaning.

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

---

## 2. Folder layout

```text
munction/
  MUNCTION.md            normative language spec (this feature's reference)
  MANUAL.md              this guide
  COMPILER_NOTE.md       how the compiler recognizes and gates Munction™
  examples/
    reach_xyx.sleela         the canonical sentence (sdps://)
    reach_all_methods.sleela one grammar over every system method
    EXPECTED_OUTPUT.txt      reference program output
  java/com/mearvk/sleela/munction/
    Munction.java            the fluent engine (bound + sequence enforced)
    MunctionChannel.java     the receivability + coherent-send contract
    MunctionChannels.java    scheme -> channel registry
    MunctionReceipt.java     the witnessed receipt
    Interims.java  Reception.java  SendCoherence.java
    ChannelReceipt.java  MunctionDigest.java
    MunctionDemo.java        Java launcher for the same sentences
    channels/                AbstractChannel + pipe · file · tcp · http · sdps · crypto
```

The **C runtime core** and **C++ compiler gate** live in the implementation tree, not here:
`impl/core/sleela_munction.c` + `sleela_munction.h`, the smoke test `impl/core/munction_smoke.c`,
and the recognition in `impl/frontend/compiler.cpp`.

---

## 3. Running the SLeeLa examples

Munction™ is a syntax-1.3 feature, recognized by the source-built compiler (the prebuilt
`working-binaries/sleela` may predate it; build from source to be sure).

```sh
# 1) Build the compiler (needs g++, X11/fontconfig/freetype dev libs)
cd impl
SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela

# 2) Check and run the canonical reach
./build/sleela check ../munction/examples/reach_xyx.sleela
./build/sleela run   ../munction/examples/reach_xyx.sleela
# expected:  reach xyx closed with receipt

# 3) The grammar over every system method
./build/sleela run   ../munction/examples/reach_all_methods.sleela
# expected:  all reaches closed with receipts
```

Reference output for both programs is in
[`examples/EXPECTED_OUTPUT.txt`](./examples/EXPECTED_OUTPUT.txt).

### Run the C runtime smoke test

```sh
cd impl
make test-munction
# === Munction reach-composition smoke ===
```

---

## 4. Running the Java reference engine

The Java engine is pure JDK (no native libraries, no JNI). The source root is `munction/java`;
the launcher is `com.mearvk.sleela.munction.MunctionDemo`.

```sh
cd munction/java
javac -d /tmp/munction-classes $(find com -name '*.java')
java  -cp /tmp/munction-classes com.mearvk.sleela.munction.MunctionDemo
```

`MunctionDemo` prints a `MunctionReceipt` for three reaches (a `file:` reach, the canonical
`sdps://` reach, and a `crypto:pipe:` reach). Digests, addresses, and timestamps vary per run; the
**shape** of the output is shown in `EXPECTED_OUTPUT.txt`.

> Requires a JDK new enough for records/pattern-matching (`instanceof` patterns are used, e.g. in
> `Munction.enable`). JDK 17+ is a safe baseline.

---

## 5. Writing a Munction™ in your own `.sleela`

### 5.1 Declare the version
Put the pragma on the first non-blank, non-comment line of the Wrapper™ source:

```text
#sleela 1.3
```

Under `#sleela 1.2` or lower the compiler **rejects** a `Munction` chain — the meaning is
version-gated.

### 5.2 Write one sane sentence
Keep it to **4..16 calls**: exactly one `start` first, one closer last, every data verb after a
`connect`, `latch` at most once. The minimum sane sentence is four calls:

```text
Munction.start("audit").connect("file:./ledger.log").send(entry).closeWithReceipt();
```

### 5.3 Pick a system method by scheme

| Reach | `connect(...)` |
|-------|----------------|
| file I/O | `connect("file:./ledger.log")` |
| OS pipe / FIFO | `connect("pipe:/tmp/reach.fifo")` |
| internet (TCP) | `connect("tcp://127.0.0.1:19866")` |
| internet (HTTP) | `connect("http://host/path")` |
| private packets | `connect("sdps://tmcf:19866")` |
| crypto over a transport | `connect("crypto:tcp://clearing:8443")` then `enable("crypto:<keyref>")` |

### 5.4 Read the receipt
`closeWithReceipt()` returns a `MunctionReceipt`. Check `outcome()` (`REACHED` / `CONTAINED` /
`ABORTED`), `fullyCoherent()`, and `bumps()` for residual facts. A reach that hit a transport bump
or fell outside 4..16 calls still closes — as `CONTAINED`, with the reason recorded — rather than
crashing.

---

## 6. Common boundary stops (and the fix)

| Symptom | Cause | Fix |
|---------|-------|-----|
| `Munction requires #sleela 1.3` | version pragma too low / missing | add `#sleela 1.3` (or higher) as the first line |
| `connect must immediately follow start` | a verb between `start` and `connect` | put `connect` right after `start` |
| `<verb> requires a connect first` | a data verb before `connect` | `connect(...)` before any `send`/`consume`/`propagate` |
| `latch may appear at most once` | two `latch()` calls | keep a single `latch`, before the closer |
| `more than 16 calls in one Munction sentence` | sentence too long | split the intent into two Munctions |
| outcome `CONTAINED` with a bump | call count outside 4..16, or a transport bump | read `bumps()`; shorten/complete the sentence or handle the transport residual |

Every one of these is a **boundary stop**: recorded on the receipt, never silently repaired.
See [`MUNCTION.md` §7](./MUNCTION.md#7-boundary-not-repair).

---

## 7. Security notes

- **Keys are referenced, never literal.** A crypto reach takes a key *reference* via
  `enable("crypto:<keyref>")`; the key bytes never appear in the URI, the witness, or the receipt.
- **The witness/receipt record only `host:port`/path** — query strings and user-info are stripped,
  so secrets cannot leak into the audit trail.
- **The crypto envelope is authenticated**: a tampered or truncated payload fails to open (a
  recorded bump), rather than being accepted as plaintext.

---

## 8. Where to go next

- [`MUNCTION.md`](./MUNCTION.md) — the normative spec (verb ladder, bound, channels, receipt).
- [`COMPILER_NOTE.md`](./COMPILER_NOTE.md) — how the compiler gates it (no new grammar tokens).
- `impl/core/sleela_munction.c` — the C runtime core.
- `examples/` — runnable reaches and their expected output.

---

<!-- ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- -->
<!--
metaVersion: 1.3.0
metaId: none
metaAuthor: Max Rupplin
metaDate: 2026-10-25
metaCopyright: (c) 2026 MEARVK LLC
metaTrademark: Munction (TM)
metaCorporation: MEARVK LLC
metaOrigin: National (United States)
metaBirthDate: 2026
metaScienceRating: 6
metaNationalSchedule: none (project-internal user manual)
-->
