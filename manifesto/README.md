# manifesto/ — A Manifesto to Thinking Kind

This directory holds a long-form work written *as* a runnable SLeeLa program and
documented in the **SleelaDoc** brand.

| File | What it is |
|------|------------|
| [`manifesto.sleela`](manifesto.sleela) | The 25-page **Manifesto to Thinking Kind** — a runnable Wrapper™. Every page is real, compiling SLeeLa; `main()` performs the argument on the C/C++ core and prints a verdict on itself. |
| [`SLEELADOC.md`](SLEELADOC.md) | The SleelaDoc documentation-brand spec (block forms and tags) used throughout the manifesto. |

## What it argues

That a mind — of any substrate — is measured not by the speed of its answers
but by the honesty of its doubt, the traceability of its reasons, and the care
with which it separates what it *observed* from what it *assumed*. The code does
not merely describe this discipline; it **enacts** it: doubt is a method that
lowers unearned confidence to its evidence ceiling, inference is a method that
propagates doubt, revision yields to better evidence, and the document keeps an
honest tally of which of its own demonstrations held.

The "146+ IQ" framing is answered directly on page 23: here **IQ means
system Insight/Quality** (as in SLeeLa's IQ Conservator) — how well a mind
understands and expresses *itself* — not a psychometric score. The manifesto
computes that score from proofs it actually performed and must clear its own
bar of 146 to reach a verdict of **KEPT**.

## Run it

```sh
cd impl && make                 # build the toolchain (build/sleela)
cd ..
export SLEELA_SHEET="$PWD/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
./impl/build/sleela run manifesto/manifesto.sleela
```

Expected tail of the output:

```
 VERDICT: KEPT.  Every demonstration held this run.
 proven=11  failed=0  insight-IQ=152
```

It is both the manifesto and its own proof-of-work: if it prints `KEPT`, the
document met its own standard on that run.
