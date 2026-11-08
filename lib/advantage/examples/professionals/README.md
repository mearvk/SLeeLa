artifact;
pure;
gasoline;

# lib/advantage/examples/professionals — examples cast for professional roles

Careful, choice `advantage` examples, each cast for a professional role and
each exercising a different subject of the package. Every example is a complete,
self-validating `.sleela` unit (ends in `ALL CHECKS PASSED`).

| Example | Professional | Subject used | What it shows |
|---------|--------------|--------------|---------------|
| `professionals-demo.sleela` | (a cohort) | `grading/` | scores six professional roles and ranks winners A–D |
| `trader-example.sleela` | Trader | `math/` | finds the joint-profit optimum `Q*` and strikes a Nash-fair wholesale price (Q*=20, surplus=800, P*=60 splitting 400/400) |
| `healer-example.sleela` | Healer | `physics/` | restores trust over time and measures a breach, via the CIR trust-decay SDE (reverts 0.3→0.76 toward 0.8; breach drops 0.8→0.4) |
| `helper-example.sleela` | Helper | `integration/` | stands up a supply→depot→storefront chain and flows 8 units one hop downstream |
| `consumer-example.sleela` | Consumer | onion (user's model) | nudges three plain dials (grow/shrink/rethink) to a net advantage of 30 (value 50→80) |

Run any example from the repository root, e.g.:

```sh
./working-binaries/sleela run lib/advantage/examples/professionals/trader-example.sleela
```

Each example names the subject classes it needs; the driver resolves them from
`lib/advantage/<subject>/` via its `/lib` identifier scan, so no imports are
required. All documents carry the `artifact; pure; gasoline;` preonem.

These are illustrative demonstrations of the package's implemented models — not
financial, medical, legal, or investment advice, and the trust-field "physics"
is a mathematical modelling analogy.
