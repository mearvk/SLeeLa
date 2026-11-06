# Cryptography examples — what to learn here

These examples teach the **integrity-verification workflow** that SLeeLa itself
relies on, and — just as importantly — where the line is between a real
cryptographic check and a toy one. **None of these files implement
cryptography.** Real hashing in this repository is performed by the approved
SHA-256 verifier against `security/sha256-manifest.json`; the examples teach the
concepts and the discipline around it.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-integrity-workflow.sleela`](BASIC-integrity-workflow.sleela) | The five-step verifier workflow: choose → compute → compare → fail-closed → record. |
| Moderate | [`MODERATE-checksum-not-crypto.sleela`](MODERATE-checksum-not-crypto.sleela) | What a digest *shape* is (fixed output from variable input) — and why an additive checksum is **not** secure. |
| Advanced | [`ADVANCED-manifest-verify-sim.sleela`](ADVANCED-manifest-verify-sim.sleela) | A fail-closed manifest verifier: walk entries, compare digests, stop at the first mismatch. |

The original [`CLASSIC-hash-integrity-workflow.sleela`](CLASSIC-hash-integrity-workflow.sleela)
is the shortest starter and states the same workflow.

## What we want you to take away

1. **Fail closed.** The most important habit in integrity work: when a digest
   does not match, you stop — you do not build, run, or ship. Both the basic
   workflow and the advanced simulation make this the decisive step. This mirrors
   the real SLeeLa build gate, which refuses to compile or execute when any file
   fails its SHA-256 check.
2. **A checksum is not a hash.** The moderate example sums bytes modulo 256 and
   then shows that swapping two bytes produces the *same* checksum (181 in both
   cases). A cryptographic hash must make such collisions infeasible to find.
   Recognizing the difference is the whole lesson — never substitute a checksum
   for a security hash.
3. **Verification is data-driven.** A manifest is just a table of (file, trusted
   digest) pairs. The advanced example models it as an array of objects and
   walks it, which is exactly how a real verifier is structured — the crypto is
   one function call inside that loop, everything else is bookkeeping.

## Practical notes for SLeeLa 1.7

- **No crypto intrinsics are used here on purpose.** The teaching files stay
  self-contained and deterministic. Production code calls the verified SHA-256
  tooling (see `lib/examples/antivirus/` and `security/sha256-manifest.json`),
  not hand-rolled arithmetic.
- **String comparison** uses `==` / `!=` (the only operators String supports at
  this level). The advanced verifier compares `trusted` against `measured`
  strings directly.
- **Fail-closed loop control**: the advanced example stops early by setting the
  loop index to the array length when it finds a mismatch — a simple way to
  "break" without a `break` statement in these teaching files.

## Run them

```sh
sleela check lib/examples/crypto/ADVANCED-manifest-verify-sim.sleela
sleela run   lib/examples/crypto/ADVANCED-manifest-verify-sim.sleela
```

Output is deterministic: the moderate example reports a checksum collision
(181 == 181), and the advanced verifier REJECTS the tree at `payload.bin`.
