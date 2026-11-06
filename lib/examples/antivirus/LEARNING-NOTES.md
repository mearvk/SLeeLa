# Antivirus examples — what to learn here

These examples teach **repository integrity verification** — the SHA-256
preflight that confirms covered files are unchanged — and, importantly, the line
between that and real malware scanning. They are operational guides (`.md`),
because the subject is a workflow and its correct interpretation.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-integrity-preflight.md`](BASIC-integrity-preflight.md) | Run the preflight; understand that PASS = "covered files unchanged." |
| Moderate | [`MODERATE-interpreting-results.md`](MODERATE-interpreting-results.md) | Coverage, intentional-vs-unintended changes, and "an error is not a PASS." |
| Advanced | [`ADVANCED-fail-closed-pipeline.md`](ADVANCED-fail-closed-pipeline.md) | Gate a build on the preflight, and treat the malware scanner as a separate contract. |

The original [`SAFE-integrity-preflight.md`](SAFE-integrity-preflight.md) is the
shortest starter and states the same scope caveats.

## What we want you to take away

1. **Integrity ≠ antivirus.** The preflight answers "did the covered files
   change?" A malware scanner answers "is this content dangerous?" These are
   different questions; a PASS on the first says nothing about the second.
2. **Scope your trust.** PASS is scoped to what the manifest covers. Files not in
   the manifest are not checked. Always know your coverage before relying on a
   green result.
3. **Fail closed.** The safe default when something is wrong — a mismatch, a
   missing manifest, an unavailable scanner — is to **stop**, not proceed. The
   advanced example wires this into a pipeline with `&&` so a failure halts
   everything downstream, exactly like the compiler's own integrity gate.
4. **Intentional changes mean regenerate.** If you deliberately edited a covered
   file, the mismatch is expected; regenerate the manifest with the repository's
   tool and re-check. If you did *not* edit it, treat the mismatch as a signal to
   investigate.

## The scanner boundary

The ClamAV host bridge is documented as an integration contract in
`lib/antivirus/README.md`, not shipped as a complete antivirus product. Attach a
real engine there, and never read "no engine available" as "clean."

## Try it

```sh
python3 tools/verify-before-execution.py \
  --manifest security/sha256-manifest.json \
  --root .
```

Expect PASS on an unmodified checkout.
