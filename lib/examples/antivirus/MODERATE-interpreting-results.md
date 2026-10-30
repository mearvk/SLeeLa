# Antivirus example (moderate): interpret results and coverage

A verdict is only useful if you read it correctly. This example covers the three
things people most often get wrong about the integrity preflight.

## 1. PASS is scoped to the manifest

The check compares files **listed in the manifest** against their recorded
digests. Files that are not in the manifest are simply not checked. So:

- PASS = "all *covered* files are unchanged."
- PASS ≠ "the whole tree is verified" and ≠ "there is no malware."

Know what your manifest covers before you rely on a PASS.

## 2. A mismatch names the file

When a covered file changed, the tool reports which one. Two honest responses:

- If the change was **unintended**, treat it as a red flag and investigate.
- If the change was **intentional** (you edited a covered source file),
  regenerate the manifest with the repository's manifest tool, then re-run the
  preflight so it matches again.

## 3. An error is not a PASS

A tool error, a missing manifest, or an unavailable scanner is **not** a clean
verdict. Never interpret "could not check" as "safe." Fix the error and re-run.

## Try it

```sh
python3 tools/verify-before-execution.py \
  --manifest security/sha256-manifest.json \
  --root .
```

Expect a PASS on an unmodified checkout. Edit any covered file and re-run to see
the mismatch report (then restore the file or regenerate the manifest).
