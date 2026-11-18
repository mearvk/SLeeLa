# Java converter input/output idempotence contract

Date: 2026-11-14  
Scope: `sleelactl/convert` and `lib/java/converter`. This contract is separate
from semantic coverage: deterministic conversion does not make an unsupported
Java construct convertible.

## Required meaning

For a fixed tuple of (source-tree bytes, relative source paths, JDK release
identity, conversion-model bytes/revision, converter revision, and explicit
options), conversion must produce the same sorted output path set and exactly
the same output bytes on every run and host supported by the converter.

The operation must be idempotent:

```
convert(snapshot, options) -> output
convert(snapshot, options) -> identical output bytes and paths
```

Unchanged output files must not be rewritten merely because conversion ran
again. Their modification times should remain unchanged. A source deletion or
a source that now fails closed must remove only outputs previously owned by the
converter; unrelated files must remain untouched.

## Input contract

1. Discover source paths in a deterministic lexical order using normalized
   relative paths and a fixed case policy.
2. Reject symlinks, non-regular source entries, path traversal, duplicate
   normalized paths, and unreadable inputs before mutating output.
3. Read every source exactly once into an immutable snapshot. Project indexing
   and translation must consume the same bytes; never index one version and
   translate a later version after a concurrent edit.
4. Pin the source identity to the actual JDK release/source archive, not merely
   a user-supplied `--jdk-version 28` label. Record the source archive/release
   identity and per-file content digests in a machine-readable input manifest.
5. Pin the conversion model by content digest as well as declared revision.
   A revision string alone is not a trust boundary.
6. Normalize only what Java defines as semantically irrelevant. Preserve
   source encoding/Unicode semantics, line endings where diagnostics depend on
   them, package/module paths, and licence notices. Do not silently rewrite
   malformed UTF-8 or Unicode escapes.

## Output contract

1. Generate into a staging tree; do not destroy the previous successful output
   before the new run has passed input validation and generation.
2. Sort output paths and report entries. Avoid timestamps, random IDs, host
   paths, locale-dependent formatting, and unstable map iteration in generated
   output. If provenance dates are required, derive them from pinned source
   metadata or keep them out of generated source.
3. Compare bytes before replacing an existing owned output. If bytes are equal,
   leave the file and its modification time unchanged.
4. Atomically publish changed files and the ownership manifest only after
   generation succeeds. On failure, preserve the previous committed output
   set; write diagnostics separately.
5. Remove stale outputs only when they are listed in the previous ownership
   manifest and absent from the newly committed output set. Refuse unsafe
   manifest paths and symlink escapes.
6. Emit a deterministic provenance manifest containing source digest(s),
   JDK identity, model digest/revision, converter revision, options, sorted
   input/output paths, and output digests. The manifest itself must be stable
   for identical inputs.

## Status of this implementation pass

### C++ converter

- Implemented deterministic lexical ordering of discovered input paths.
- Implemented a one-read immutable source snapshot shared by project indexing
  and translation.
- Implemented refusing symlink and non-regular source files before output
  mutation.
- Implemented byte comparison before writing generated files, preserving
  modification times when output bytes are unchanged.
- Implemented a sorted ownership manifest and deferred stale-output cleanup.
- Added `CONVERSION-PROVENANCE.json`: SHA-256 digests cover the exact model
  bytes, immutable input snapshot, available converter source files, and
  emitted output bytes. `converterSourceComplete` records whether all expected
  source files were present; the compiled binary identity is not recorded.
  The manifest has no timestamps or absolute checkout paths and labels the JDK
  version as a declared label rather than asserting release authenticity.
- Whole-run staging/publication is implemented by `OutputTransaction`; it
  preserves unowned files, refuses unsafe prior-output entries, restores the
  previous generation on publication failure, and writes reports/manifests
  before the directory swap.
- The immutable source snapshot, exact model digest, converter-source inventory,
  and output digests are recorded. The provenance intentionally does not claim
  official JDK authenticity from `--jdk-version`; it binds the actual source
  snapshot instead. `tests/javaconv_differential_test.sh` now independently
  validates provenance and checks repeat-run output mtimes in its transaction
  fixture. These tests have been added but have not yet been executed in this
  session.

### SLeeLa-native converter

- The walker sorts directory entries. The emitter now writes each output to a
  same-directory `.jconv-tmp`, compares it with the existing file, preserves
  unchanged mtimes, and renames a complete changed file into place. The report
  TSV and summary use the same staged compare/publish pattern. Model descriptions
  omit absolute paths, and generated `metaDate`/`metaBirthDate` are now `unknown`
  rather than wall-clock values, so identical inputs do not vary by run date.
- Per-file output-open and staged-publication failures are now reported as
  failed files rather than successful conversions.
- Implemented in the launcher: sibling whole-run staging, preservation of the
  prior published directory until `DONE`, symlink/special-file refusal in the
  prior output tree, deterministic SHA-256 provenance for sorted inputs, exact
  model bytes, native converter sources and outputs, and same-parent rename
  publication with rollback on publish failure.
- Remaining: the native converter still lacks an ownership manifest for safe
  stale-output deletion; existing outputs are copied forward. JDK identity is
  marked unverified unless a source `release` file or explicit `JCONV_JDK_ID`
  is present. `JCONV_REVISION` must be supplied to bind the manifest to a
  declared source revision. No runtime/test result is claimed for this launcher
  change yet.
- Its per-member-stub policy remains a different acceptance result from the
  C++ converter's per-file fail-closed policy. Reports must keep these outcomes
  distinct; a stubbed output is not a complete mapping.

## Required conformance tests

For each converter independently, run the following against a pinned JDK 28
source snapshot:

- Convert twice into the same output directory; compare sorted path lists and
  SHA-256 digests, and verify unchanged file mtimes on the second run.
- Change one source file; verify only dependent outputs and provenance change.
- Delete one source file; verify only its previously owned outputs are removed.
- Make one source unsupported; verify no partial output for it and no loss of
  unrelated owned outputs.
- Add an unrelated file to the output directory; verify it is never overwritten
  or deleted.
- Mutate a source during conversion; verify the converter uses one captured
  snapshot or aborts before publishing.
- Introduce a symlink source and a symlinked output parent; verify safe refusal.
- Run from two different absolute checkout paths and supported locales; verify
  generated source bytes are identical.
- Run with the same source but a different model digest, converter revision, or
  option; verify provenance changes and the result is not misidentified as the
  same conversion.
- For every accepted source, compile/run the result and compare observable
  behavior with the same pinned JDK. Determinism is not a substitute for
  equivalence.

## Next implementation order

1. Run the added C++ provenance/repeat-run transaction regression and the
   native repeat-run/rollback integration regression; fix any failures.
2. Add native ownership tracking and safe stale-output cleanup without deleting
   unowned files; test interrupted batch/resume and publication rollback.
3. Pin the exact JDK 28 source archive/revision used for the conformance corpus.
4. Complete native overload-aware project indexing, generic invocation
   inference and generic SAM substitution to close the semantic-parity gap.
5. Align accepted/refused semantics and add JDK differential fixtures for each
   newly shared behavior.
6. Run both converters independently; report complete, partial/stubbed,
   refused, and infrastructure-failed counts separately.
