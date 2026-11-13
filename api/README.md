# SLeeLa API documentation

The repository maintains three complementary API documentation surfaces:

- [Current generated SleelaDoc site](../sleeladoc-api/index.html) — browsable
  reference generated from the live `/lib` tree.
- [Contributor API site](../docs/api/index.html) — the documentation-facing
  entry point, generated from the same source inventory.
- [SLeeLa language API editions](sleela/README.md) — versioned language API
  snapshots. The current edition is [SLeeLa 3.1](sleela/3.1/README.md) (also written 3.01).

Native C and C++ API references are catalogued under [`native/`](native/).
The historical SLeeLa 2.1, 2.6, 2.8, and 2.9 material remains available and
is not rewritten as part of the 3.0 or 3.1 editions.

The standard-library **`advantage`** package (advantaging a dividend, with its
`math/`, `physics/`, `integration/`, `grading/`, and `output/` subjects) is
included in all three generated surfaces — see its package page in the
generated sites and the package-level docs under
[`../lib/advantage/`](../lib/advantage/README.md).

## Build

From the repository root:

```sh
make api-docs
```

This generates the live site, the contributor-facing site, and a new immutable
3.1 snapshot under `api/sleela/3.1/version-NNN/`. The first build uses
`version-001`; subsequent snapshots must use the next unused number:

```sh
API_SNAPSHOT=version-002 make api-docs
```

All three destinations are generated from the same `/lib` source tree using
`tools/generate-sleeladoc-api.py`. The generator records each source unit's
actual `#sleela` declaration; publishing syntax 3.1 does not relabel older
source units or remove their compatibility history.
