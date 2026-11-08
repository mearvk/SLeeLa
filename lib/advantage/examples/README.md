artifact;
pure;
gasoline;

# lib/advantage/examples — worked SLeeLa examples, by level

Source examples that exercise the `advantage` package (the Sequitur onion model
and its `math/`, `physics/`, `integration/`, `grading/`, `output/` subjects)
from SLeeLa itself.

The examples are organized by **level of sophistication** — the estimated
experience a reader needs to follow the example comfortably — and within each
level by **topic**, grouping examples that work the same kind of material.

| Level | Reader | Folders |
|-------|--------|---------|
| [`novice/`](novice/) | first contact with the model — outcomes before controls | `onion-model/`, `consumer-outcomes/` |
| [`medium/`](medium/) | one subject at a time, a little machinery | `trade-economics/`, `output-routing/`, `supply-chains/` |
| [`advanced/`](advanced/) | richer, multi-concept subjects and analogies | `trust-fields/`, `vertical-integration/` |
| [`professional/`](professional/) | graded, ranked, role-cast work | `sophistication-grading/` |

Every example is a complete, self-validating `.sleela` unit that ends in
`ALL CHECKS PASSED`. Run any of them from the repository root, e.g.:

```sh
./working-binaries/sleela run lib/advantage/examples/novice/onion-model/onion-primer.sleela
```

Each example names the subject classes it needs; the driver resolves them from
`lib/advantage/<subject>/` via its `/lib` identifier scan, so no imports are
required. These are illustrative source examples, not part of the library's
class vocabulary surface.

All documents carry the `artifact; pure; gasoline;` preonem. They are
demonstrations of the package's implemented models — not financial, medical,
legal, or investment advice, and the trust-field "physics" is a mathematical
modelling analogy.
