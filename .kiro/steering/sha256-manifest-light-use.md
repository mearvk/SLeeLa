---
inclusion: always
---

# SHA-256 build manifest: use the rebuild script lightly

`tools/generate-sha256-manifest.py` rewrites `security/sha256-manifest.json`,
the execution gate that `verify-security` (impl/Makefile) and every
`sleela run/check` verify before they build or execute. Regenerate it
**lightly**: only when it is actually needed, and once per batch of edits.

## When to run it

- **Once, after a batch of intentional edits** to files the gate covers
  (`lib/**/*.sleela`, the lib C/C++ helpers, `impl/` core and frontend source),
  just before the next build or `sleela run/check` that needs them.
- **Once before committing** those edits, so the committed manifest matches the
  committed tree.
- **Once on a fresh clone** if `cd impl && make check-manifest` already reports
  mismatches with a clean HEAD (pre-existing drift), to get a buildable baseline.

## When not to run it

- **Not after every small edit or experiment.** Finish the edit set, then
  regenerate once. Do not put it in a loop around each try.
- **Not for files outside the gate:** `*.md`, `examples/**`,
  `working-binaries/**`, `.kiro/**`, tool scripts. Editing only these never needs
  a regeneration.
- **Not to silence a mismatch you did not cause.** A mismatch on a file you did
  not edit means someone else changed it. Find out why before you re-sign it.
- **Not on throwaway edits.** If you change a gated file only to probe
  something, revert it (`git checkout -- <file>`). Reverting restores the old
  digests, so no regeneration is needed.

## How

```
python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json
cd impl && make check-manifest     # must print OK
```

Commit the regenerated manifest in the same commit as the source it covers.
`SHA256-DIGESTS.md` (whole-repo digests, `tools/generate-all-sha256-md.sh`) is a
separate document. Refresh it only when asked, or when refreshing
`working-binaries/`.
