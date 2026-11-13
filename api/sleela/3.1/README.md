# SLeeLa 3.1 API edition

**Current language edition:** SLeeLa syntax 3.1, also written `3.01` (the minor is an integer)  
**Normative specification:** [`SLEELA.syntax`](../../../SLEELA.syntax), sections 44, 45 and 46  
**Compiler gate:** `impl/frontend/version.h` — supported source syntax 1.0–3.1  
**Java tier:** [`lib/java/JAVA.SPEC.md`](../../../lib/java/JAVA.SPEC.md) (specified apart from the language)

This edition documents the current `/lib` source tree. The generated HTML snapshots are immutable
numbered directories (`version-001`, `version-002`, ...). Each build adds the next number; an existing
snapshot is never rewritten.

Hand-authored API references for language-level features (specified in `SLEELA.syntax`, not as a `/lib` class):

- [Language API — character, array, labels, `finally`](LANGUAGE-API.md)
- [Java tier API — what the Java to SLeeLa conversion adds](JAVA-TIER-API.md)

Earlier editions are preserved and are not rewritten: [3.0](../3.0/README.md), [2.9](../2.9/README.md).

To build:

```sh
make api-docs                        # writes sleeladoc-api/, docs/api/ and the next api/sleela/3.1/version-NNN/
API_SNAPSHOT=version-007 make api-docs   # choose the number
API_EDITION=3.0 make api-docs        # write into another edition's directory (not normally used)
```

The generator reports each source file's actual `#sleela` declaration. The 3.1 edition does not relabel
legacy source files; a unit that declares `#sleela 3.0` (or older) is shown as such and still runs.
