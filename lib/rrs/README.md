# lib/rrs

SLeeLa standard-library mirror of the `artificial-intelligence/rrs` C subsystem:
**RRS — Reasonable Responsibility Solidity**, a three-stage congruent
feed-forward model over a ternary signal.

The signal is `1 = AFFIRM`, `0 = ELIMINATE`, `2 = POWER_ADD`. Three stages are
chained feed-forward:

1. **Conjunctive** — surface assumptions, strict AND; a single elimination
   breaks the stage.
2. **Responsibility** — the reasonable-responsibility prelude; a premise whose
   responsibility is unmet is forced to ELIMINATE.
3. **Solidity** — the culmination (`potestas`, a warranted capacity-to-continue).

RRS *subtracts faith*: any faith or trial premise is forced to ELIMINATE. It
ships no weights — the ternary net is a tiny conjunctive accumulator.

| SLeeLa class | Mirrors (native prefix) | Purpose |
|---|---|---|
| `SLPackage` | — | Package facade for the compiler, Loader, and Nordshrift. |
| `SLRrs` | `sllmrrs_` | The fold kernel, the three-stage feed-forward, and the `potestas` culmination (NONE / PROVISIONAL / WARRANTED). |
| `SLRrsSignalMonitor` | `sllmrm_` | Read-only observability: per-signal and per-reason tallies over the same run, never changing the outcome. |

A runnable demo lives in `examples/RrsDemo.sleela`.

**Culmination rule** (`SLRrs.feedForward`): a broken conjunction fails closed to
an honest `NONE`; otherwise `WARRANTED` needs a solid chain, a responsibility
stage that affirmed at least one premise, and total power ≥ 4; `PROVISIONAL`
needs a solid chain and total power ≥ 2; else `NONE`.

Every class carries the standard Metadata Class record (see
`.kiro/steering/metadata-class-standard.md` and `SLEELA.syntax` §15).
