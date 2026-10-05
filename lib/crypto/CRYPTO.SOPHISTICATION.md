<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa Crypto Sophistication Classes

**Family:** `lib/crypto`
**Revision:** 0.1
**Native bridge:** `native/include/sleela_crypto.h`, `native/src/sleela_crypto.cpp`

This note documents the general-cryptography sophistication classes added to the
SLeeLa `lib/crypto` family. They model the ordered mixed-radix process used by
the reference AES2 obfuscation module in
`mearvk/Java.Web.Server.Telnet.Front.Java.21`
(`source/encryption/module/aes/two/EncryptionModule.java`), expressed as a
measurable SLeeLa object model with its value primitives behind an explicit
VM/OS bridge.

> **Security note.** Like the reference module, these transforms are
> deterministic, reversible mixed-radix conversions ORed with constants. This is
> **obfuscation, not cryptography.** Do not use it to protect sensitive data; use
> a vetted AEAD cipher (AES-256-GCM, ChaCha20-Poly1305).

## The process, in order

The pipeline enforces a strict order with three carried attributes on every
ordered step — **Order** (position), **Orientation** (role), and **Respect**
(precedence rank honoured on ties):

| Stage | Class | Orientation |
|------:|-------|-------------|
| 0 | `SLPlainTextField` → `SLPlainText` | init — one or more initial plaintext fields (1..255) |
| 1 | `SLIntermixPrimary` | secondary intermix, internal |
| 2 | `SLIntermixSecondary` | secondary intermix, internal, again |
| 3 | `SLCryptoSeries` of `SLCryptoBlock` | main general intermix, Order 1..255 |
| 4 | `SLCryptoResult` | result |
| 5 | `SLCryptoComparator` | compare |
| 6 | `SLNationalRegister` | call to national register |
| 7 | `SLFinalResult` | final standard result of all of it |

`SLCryptoPipeline` orchestrates stages 0–7 and guards each precondition
(at least one plaintext field; at least one ordered block).

## Radix converter — bases 1 to 2055

`SLRadix` is a single configurable converter covering the whole span the user
requested, replacing the reference module's one-class-per-base family (Radix6,
Radix11, Radix12, Radix13, Radix17, Radix18, ...):

- **Base 1** — unary (tally) encoding.
- **Bases 2–36** — positional with the `0-9A-Z` alphabet.
- **Bases 37–2055** — positional in a delimited decimal-digit form `d1.d2.d3`
  (most-significant first), so every base up to 2055 is text-representable
  without 2055 distinct glyphs.

`SLRadixTable` is a bounded registry that hands out configured converters by
base to the intermix stages and the ordered series.

## Ordered blocks (Order 1..255)

`SLCryptoBlock` is the base unit of one ordered step and defines the ordering
contract. Concrete steps mirror the reference passes:

- `SLCryptoBlockOne` — pass one, the initial pad (base-12, mask `0x88034321`).
- `SLCryptoBlockTwo` — pass two, symmetry-row intermix (base-18/13/6).
- `SLCryptoBlockThree` — pass three, the lightning rounds (drives the two
  intermix stages in their respected internal order).

`SLCryptoSeries` holds up to 255 blocks, arranges them by Order then Respect, and
advances the running text 1,2,3,...,x,y,z. Richer step classes
(`SLCryptoBlockFour` ...) can be added without changing the driver.

## Native bridge

The deterministic value primitives live in C/C++:

- `sleela_crypto_radix_to_base` / `_from_base` — radix 1..2055 round-trip.
- `sleela_crypto_block_advance` — ordered radix-then-OR step.
- `sleela_crypto_block_two_rows` — symmetry-row intermix.
- `sleela_crypto_intermix_primary` / `_secondary` — the two internal weaves.
- `sleela_crypto_compare` — non-short-circuiting comparison gate.
- `sleela_crypto_national_register` — register call (fail-closed on empty).

Build and self-test:

```sh
cd native
make test
```

**Max Rupplin — MEARVK LLC — 2026**
