# SLeeLa 3.0 API edition

**Current language edition:** SLeeLa syntax 3.0  
**Normative specification:** [`SLEELA.syntax`](../../../SLEELA.syntax)  
**Compiler gate:** `impl/frontend/version.h` — supported source syntax 1.0–3.0

The 3.0 API edition documents the current `/lib` source tree. Its generated
snapshot is stored in an immutable numbered directory:

- [Version 050 — latest generated HTML API snapshot](version-050/index.html)
- [Version 014 — earlier generated HTML API snapshot](version-014/index.html)
- [Version 003 — earlier generated HTML API snapshot](version-003/index.html)
- [Version 002 — earlier generated HTML API snapshot](version-002/index.html)
- [Version 001 — initial generated HTML API snapshot](version-001/index.html)

Hand-authored API references for language-level features (specified in
`SLEELA.syntax`, not as a `/lib` class):

- [Session additions — index](SESSION-ADDITIONS-API.md)
- [Loop-timing API — `@loopDelay` / `@loopBudget` / `sleep(ms)`](LOOP_TIMING_API.md)

If the snapshot has not yet been generated in a checkout, run:

```sh
make api-docs
```

To create a later snapshot without overwriting an existing one:

```sh
API_SNAPSHOT=version-051 make api-docs
```

The API generator reports each source file's actual `#sleela` declaration.
The 3.0 specification edition does not automatically relabel legacy source
files; compatibility declarations remain part of the API record.
