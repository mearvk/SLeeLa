# Lesson 10 — The `.ledger` Class

> Series: [index](README.md) · prev: [09](09-verification.md) · next: [11 — HTTP 3.0 & HTTP colors](11-http3-and-colors.md)

Where Lesson 09's gate protects the *toolchain*, the **`.ledger`** gives per-file
integrity for *artifacts*. A `.ledger` is a tamper-evident chain of records, one
per file, binding three things:

- a **QR insignia** (an SVG QR encoding the record's identity),
- a **SHA-256** of the file, **chained** to the previous record's hash,
- an **ISO-8601 UTC timestamp**.

Full spec: [`../ledger/LEDGER.md`](../ledger/LEDGER.md).

## Two ways to produce one

**1. Standalone** — build a ledger over any set of files:

```sh
# Python
python3 ledger/ledger.py build --out /tmp/demo.ledger \
    --insignia-dir /tmp/insignia --root . \
    examples/symmetry/src/core/Message.sleela

# or the C++ tool
cd ledger && make ledger_tool
./ledger_tool build --out /tmp/demo.ledger --insignia-dir /tmp/insignia --root . ledger_core.h
```

**2. By the compiler** — Nordshrift emits `<artifact>.sleela.ledger` (and a
`.qr.svg`) next to each `.sleela` artifact automatically. You saw that line in
Lesson 05.

## Verifying a ledger

```sh
python3 ledger/ledger.py verify /tmp/demo.ledger
# ledger: OK — N records, chain intact
```

The verifier recomputes each record's hash and checks every `prev` link. Edit or
reorder any record and it fails closed:

```text
ledger: record 1 record_hash MISMATCH
ledger: record 2 prev-link BROKEN
```

## A record, up close

```json
{"seq":1,"path":"...Demo.sleela","sha256":"ea85…8f01","prev":"0000…0000",
 "timestamp_utc":"2026-09-20T18:24:01Z","timestamp_kind":"utc-iso8601",
 "qr_svg":"...Demo.sleela.qr.svg","record_hash":"5c9c…d012"}
```

Each `record_hash` is a SHA-256 over the record's canonical bytes (excluding the
hash itself); the next record's `prev` points at it — that's the chain.

## Honest scope

The timestamp is a **UTC ISO-8601** clock reading — the international *format* —
not an RFC-3161 notarized token (that needs network the offline build lacks); the
format reserves a field where a real token would attach. The C and Python
implementations produce byte-identical SHA-256 and QR SVG, and each verifies the
other's ledgers.

**Next:** [Lesson 11 — HTTP 3.0 & HTTP colors](11-http3-and-colors.md)
