# SLeeLa API documentation build

This directory is the contributor-facing HTML API site generated from the
current `/lib` source tree. Open [`index.html`](index.html) to browse package,
class, field, and method documentation. The current normative language edition
is [SLeeLa syntax 3.2](../../SLEELA.syntax).

## Build all API destinations

From the repository root:

```sh
make api-docs
```

The build uses `tools/generate-sleeladoc-api.py` to produce:

1. `/sleeladoc-api` — the live generated reference.
2. `/docs/api` — this contributor-facing copy.
3. `/api/sleela/3.2/version-NNN` — an immutable, versioned API snapshot
   (first: [`version-001`](../../api/sleela/3.2/version-001/index.html)). The 3.1 and 3.0
   snapshots are preserved and no longer written to.

The generator reads the checked-in source comments and declarations. It reports
each unit's actual `#sleela` pragma and does not rewrite older source versions
when a new language specification edition is published.

See the [API catalogue](../../api/README.md), [SLeeLa 3.2 edition](../../api/sleela/3.2/README.md),
and [SleelaDoc source convention](../../manifesto/SLEELADOC.md).

## Language-level additions (not `/lib` classes)

Language/compiler features specified in `SLEELA.syntax` are documented by
hand (they are not `/lib` package pages). Under the current 3.2 edition:

- [Language API — character, array, labels, `finally`](../../api/sleela/3.2/LANGUAGE-API.md)
- [Java tier API — what the Java to SLeeLa conversion adds](../../api/sleela/3.2/JAVA-TIER-API.md)

Under the 3.0 edition, still current for these features:

- [Session additions — index](../../api/sleela/3.0/SESSION-ADDITIONS-API.md)
- [Loop-timing API — `@loopDelay` / `@loopBudget` / `sleep(ms)`](../../api/sleela/3.0/LOOP_TIMING_API.md)

The `OP_SLEEP` (157) and `OP_BIGMUL` (156) opcodes appear on this site's
generated [`vm` package page](package/vm.html) as `SLISASleep` / `SLISABigMul`.
