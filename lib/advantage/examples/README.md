artifact;
pure;
gasoline;

# lib/advantage/examples — worked SLeeLa examples, by level

Source examples that exercise the `advantage` package (the Sequitur onion model
and its `math/`, `physics/`, `integration/`, `grading/`, `output/`, `network/`
subjects) from SLeeLa itself.

The examples are organized by **level of sophistication** — the estimated
experience a reader needs to follow the example comfortably — and within each
level by **topic**, grouping examples that work the same kind of material.

| Level | Reader | Folders |
|-------|--------|---------|
| [`novice/`](novice/) | first contact with the model — outcomes before controls | `onion-model/`, `consumer-outcomes/`, `network-trades/` |
| [`medium/`](medium/) | one subject at a time, a little machinery | `trade-economics/`, `output-routing/`, `supply-chains/`, `network-trades/` |
| [`advanced/`](advanced/) | richer, multi-concept subjects and analogies | `trust-fields/`, `vertical-integration/`, `network-trades/` |
| [`professional/`](professional/) | graded, ranked, role-cast work | `sophistication-grading/`, `network-trades/` |

### Network-trades family (lumber → television)

A 12-example family, spread across all four levels, that models **network
negotiation across a socio-economic ladder of goods** — lumber, cement, copper,
steel, coffee, cotton, solar panels, lithium cells, semiconductors, server
racks, smartphones, and televisions. Each example scouts a network of venue
quotes, finds the best buy/sell spread, negotiates a fair clearing price
(closed-form, so a fan of counterparties clears in `O(parties)`), and emits a
cash-flow **receipt**: money in → a trade → time → cash out. Each is paired with
an equally-named `.md` giving per-tier **class consensus for safe arrival /
ethical use** and the receipt math. The reusable classes live in
[`lib/advantage/network/`](../network/) (`SLMarketScout`, `SLNegotiationRound`,
`SLCashflow`). The "internet search" is modelled by a deterministic quote source
whose public shape matches a live transport fetch — no live network call is made
from inside the SLeeLa VM.

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
