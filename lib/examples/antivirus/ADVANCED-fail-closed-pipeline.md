# Antivirus example (advanced): fail-closed in a pipeline, and the scanner boundary

This example shows how to wire the integrity preflight into an automated
pipeline so that verification failure *stops* the build — and where the real
malware scanner fits as a separate, contracted integration.

## Gate the build on the preflight

Run the preflight first and let its exit status decide whether the rest runs.
Because the commands are chained with `&&`, a non-zero exit from the verifier
prevents the build and tests from starting:

```sh
python3 tools/verify-before-execution.py \
  --manifest security/sha256-manifest.json \
  --root . \
&& make -C impl \
&& ./impl/build/sleela run lib/examples/sleela/CLASSIC-hello-world.sleela
```

If verification fails, nothing downstream executes. That is "fail closed": the
safe state is *not* to proceed. This mirrors the compiler's own built-in gate,
which refuses to build or run on a manifest mismatch.

## The scanner is a contract, not a product here

Repository integrity (what the preflight does) is **not** the same as malware
scanning. The antivirus package documents the scanner/ClamAV host bridge as an
*integration contract* — a boundary you wire a real engine into — not a complete
bundled antivirus product.

Rules for that boundary:

- Follow the documented ClamAV adapter setup in `lib/antivirus/README.md` to
  attach a real scanner.
- **Never treat an unavailable scanner or an engine error as a clean verdict.**
  No engine ≠ no threat.
- Keep the two concerns distinct in your pipeline: integrity preflight (digests
  match) and malware scan (content judged safe by an engine) answer different
  questions.

## What a good pipeline step reports

1. Which manifest and root were checked.
2. PASS, or the exact file(s) that mismatched.
3. A clear stop on any failure or error — never a silent continue.
