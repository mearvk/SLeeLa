# advantage

A small module that connects **advantage math** to a **store of knowledge**,
so it can answer: *which advantage should I draw on, and what must I check
before I do?*

```
evidence ──▶ C core: mean, stddev, confidence ──┐
                                                ├─▶ score = mean × confidence × trust ─▶ ranked counsel
assumptions (held / doubted) ──▶ trust ─────────┘
```

## Parts

| File | Role |
|------|------|
| `advantage.{h,c}`, `advantage.{hpp,cpp}` | C math core + C++ wrapper: `advantage_value`, `advantage_mean`, `advantage_stddev`, `advantage_confidence`; `advantage::mean_checked` rejects mismatched series |
| `advantages.xml`, `advantage_store.*` | Knowledge store: sources, assumptions, knowledge strings, evidence |
| `counsel.*` | Joins the two: ranks sources with scores, things to verify, and cautions |
| `enrich.*` | Opt-in, allowlisted fetch of extra (untrusted) knowledge lines |
| `analyze.cpp` | CLI over all of the above |
| `transport/` | Secure acquisition layer used by enrichment (see its README) |
| `examples/` | Worked trade examples (illustrative data; see its README) |

## Build and run

```sh
make                 # builds ./advantage (demo) and ./analyze
make run             # math demo
make run-analyze     # load + summarize advantages.xml
make run-counsel     # ranked counsel
```

## XML additions

```xml
<sourceAdvantage id="..." kind="...">
  <evidence weight="1.0">                      <!-- weight >= 0, optional -->
    <observation reward="3.0" baseline="2.0"/>
  </evidence>
  <assumptions>
    <assumption>held by default</assumption>
    <assumption status="doubted">discounts trust until verified</assumption>
  </assumptions>
</sourceAdvantage>
```

The loader **fails closed** (exit 1, with the source named) on: a
non-numeric or non-finite `reward`/`baseline`/`weight`, a negative weight, an
assumption `status` other than `held`/`doubted`, a duplicate source `id`, or an
`<observation>` outside `<evidence>`, or a document that is cut short (a tag
with no closing `>`, an unclosed element, or a missing `</advantages>`) --
a partial store never passes for a complete one. Attribute names match whole
(`xweight=` is not `weight=`; `uuid=` is not `id=`).

The loader is also **bounded**: at most 16 MiB per document, 10,000 sources,
and 100,000 observations per source. Beyond that it errors instead of
consuming unbounded memory (see the `kMax*` constants in
`advantage_store.hpp`). Attribute values must use double quotes.

The observations shipped in `advantages.xml` are **illustrative seed data**,
not measurements. Replace them with your own outcomes.

## How the score stays honest

- `confidence = n / (n + prior)` (default prior 3): few observations earn
  little trust.
- `trust` = held assumptions / total: doubted assumptions discount the score.
- No evidence is **unknown, not zero**: scored 0 and flagged.
- Spread larger than the average gain is flagged as inconsistent.
- Overflowing evidence values withhold the score (0, flagged) rather than
  producing NaN.
- Fetched (remote) lines are never used in scoring or advice; they are only
  counted, as untrusted.

## Enrichment safety (`analyze --enrich`)

Off unless `--enrich` is given. Default schemes: `https` only. Hosts must be
named with `--allow-host`. `file://` reads local files and is **not** covered
by the host allowlist, so it needs the explicit `--allow-file`. Plaintext http
needs `--allow-http` (discouraged).
