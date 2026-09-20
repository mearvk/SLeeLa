# TODO — A Recipe

A recipe for the work ahead. Each numbered course is prepared the same way:
**Ingredients** (what it needs), **Method** (how to make it), and a **Moral**
(the principle that governs the dish). Cook them in order or to taste; every
course must satisfy its Moral before it is served.

---

## 1. The "WE WILL" line for the base README

**Ingredients:** the exact wording to follow "WE WILL"; [`README.md`](README.md).
**Method:** take the phrase as given, fold it into the README verbatim, let it
rest under a clear heading, then push to `master` and `main`.
**Moral — Say only what you mean.** A declaration is written in the author's
own words, never invented on their behalf.

---

## 2. Runnable capstone under `capstone/`

**Ingredients:** [`tutorial/12-capstone.md`](tutorial/12-capstone.md); the
verified `nordshrift` toolchain.
**Method:** create `capstone/src/Ledgerize.sleela` and `capstone/build.sst`,
build them, and confirm the artifact and its `.ledger` verify.
**Moral — Proof over promise.** A tutorial that claims it runs must actually run.

---

## 3. HTTP 2.1 core: fill the sketch stubs

**Ingredients:** [`http-2.0/`](http-2.0/) naming/pipeline stubs; the 3.0 core
as a reference.
**Method:** implement `h21_naming.c` and `h21_pipeline.c` (minus the integrity
substrate), add a demo + parity tests, keep the `H21` wire byte-identical
across C and Python.
**Moral — Finish what you frame.** A sketch is a debt; honor it before adding
new rooms to the house.

---

## 4. Deadline on the wire (optional, breaking)

**Ingredients:** the advisory timing layer ([`http-3.0/http3_timing.{h,c}`](http-3.0/));
agreement that a wire change is wanted.
**Method:** add a MAC-covered `deadline` field to the envelope, make late
arrivals a hard reject, and version the change so peers negotiate it.
**Moral — Change the contract in the open.** A breaking change is announced and
negotiated, never slipped in.

---

## 5. Replay window that survives reordering

**Ingredients:** the NONCE high-water mark in the pipeline.
**Method:** replace the single high-water mark with a sliding replay window
(a bitmap of recently seen nonces), keyed per sender.
**Moral — Guard the common case without punishing the honest one.** Security
must not reject legitimate, merely out-of-order, traffic.

---

## 6. QoS worked example in the RMI demo

**Ingredients:** [`QOS.md`](QOS.md); the Java RMI client
([`rmi/java/.../SleelaRmiClient.java`](rmi/java/com/mearvk/sleela/rmi/SleelaRmiClient.java)).
**Method:** wrap a remote call with an explicit timeout and an `isHealthy()`
back-off, and show the latency/certainty reading it produces.
**Moral — Advice earns trust by example.** A guideline is worth more when it is
demonstrated than when

 it is merely stated.

---

## 7. HTTP colors for the 2.1 sketch

**Ingredients:** [`http-3.0/HTTP-COLORS.md`](http-3.0/HTTP-COLORS.md); the 2.1
core once it exists (course 3).
**Method:** define a trimmed 2.1 palette (wire form + flags only — no MAC/
INTACTX/NONCE/basket tiers), mirror the loader, and document it.
**Moral — Adapt the pattern, do not copy it whole.** A younger generation
inherits the shape of a standard, not the parts it cannot yet bear.

---

## 8. Cross-link the term-care note

**Ingredients:** [`GLOSSARY.md`](GLOSSARY.md) §B.2; [`NUMERAL-INTENT.md`](NUMERAL-INTENT.md);
[`FIDUCIARY.md`](FIDUCIARY.md), [`BRITISH.md`](BRITISH.md), [`politico/SAKES.md`](politico/SAKES.md).
**Method:** add a one-line reference to the Celebrity term-care standard from
each document that touches the term, so the care travels with the word.
**Moral — Keep the guardrail where the road turns.** A safeguard belongs at
every place the hazard appears, not only where it was first written.

---

## 9. Ledger the whole tutorial tree

**Ingredients:** [`ledger/ledger.py`](ledger/ledger.py); the 12 demo sources.
**Method:** build one `.ledger` chain over all tutorial demos, publish the QR
insignia set, and add a `make`-able verify step.
**Moral — Account for everything you ship.** What is worth teaching is worth
sealing against silent change.

---

## 10. A verification pass across the docs

**Ingredients:** every `.md` touched this session; the link checker in your head.
**Method:** confirm each cross-reference resolves, each claim matches the code,
and each honest caveat (no synced clock, UTC-not-RFC-3161, structural-only QR
validation) is still stated where relevant.
**Moral — Tell the truth about your own work.** A document earns belief by
naming its limits as plainly as its features.

---

*Serve in order for a full meal, or take any single course. No dish is done
until its Moral is met.*
