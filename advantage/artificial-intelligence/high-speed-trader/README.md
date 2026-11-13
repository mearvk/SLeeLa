# High speed trader (courseware model)

One file, four classes, run from this folder:

```sh
export SLEELA_HOME=<repo root>
$SLEELA_HOME/working-binaries/sleela run high-speed-trader.sleela
```

(One file on purpose: outside `/lib`, `sleela run A.sleela B.sleela` duplicates
library units, so the classes live together.)

| Class | Role |
|---|---|
| `SLHstPragmatics` | Registry of the 24 pragmatics: name, phase, hard minimum, priority order |
| `SLHighSpeedTrader` | The AI: 24 features, the first **P** pragmatics, weighted score, rate governor, learning, kill switch |
| `SLHstMarket` | Deterministic synthetic candidate stream with hidden truth (same seed, same stream) |
| `HstDemo` | Runs P = 8, 16, 24 and 31 checks |

## The pragmatics (8 to 24)

A pragmatic is one *evolution* of judgement. There are 24, in three phases:

| Phase | Ids | Question |
|---|---|---|
| **Logic** (clear logic) | L1..L8 | Is the candidate coherent: well-formed, positive net edge, evidence, plausibility, costs real? |
| **Use** (logic of use) | U1..U8 | Does it fit the purpose: size, freshness, headroom, liquidity, rate headroom, learned prior? |
| **Ground** (secure and grounded trade) | G1..G8 | Is it safe: venue trust, kill switch, price collar, position limit, drawdown, evidence, staleness? |

`configure(P, tradesPerMinute)` activates the first **P** by priority order and
**refuses** anything outside 8..24 (the trader then stays not ready and rejects
everything). The order interleaves the phases, so every prefix keeps all three,
and the first 8 are the minimal safe core with every hard gate.

## Decision

1. 24 features in 0..1 are computed (1 = best).
2. For each active pragmatic in order: a feature below its **hard minimum** is a
   veto, so the candidate is rejected and the reason is recorded.
3. Otherwise `score = sum(w*f) / sum(w)` must reach the threshold.
4. A token-bucket **rate governor** must have a token (target trades per minute).
5. A once-a-second controller moves the threshold: pickier when the cap binds,
   looser when under target.

## Evolution

After an accepted trade, `learn(realizedBps)` applies multiplicative weights:
features that were high on a win gain weight, on a loss lose it (bounded
0.25..4). A regime prior learns the win rate per net-edge bucket. If drawdown
comes within 5% of the limit the trader **halts**; only a person can `rearm()`.

## Some people are brilliant

`insight(id, factor)` lets a person nudge one pragmatic weight by 0.5..2.0.
It can never change a hard minimum, so insight **cannot unlock a veto or the
kill switch**; an out-of-range request is refused, not clamped.

## Measured here (one run of the demo, 8-core sandbox)

| P | evals/s (wall) | trades, last full sim minute | max trades in a sim minute |
|--:|--:|--:|--:|
| 8 | ~120,000 | 1,765 | 1,898 |
| 16 | ~108,000 | 1,773 | 1,885 |
| 24 | ~99,000 | 1,894 | 1,917 |

Throughput is the whole loop (generate, evaluate, learn) and varies by machine.
The requirement of at least 5,000 evaluations per second is met about 20x over.
Trades per minute sit just under the 2,000 cap because the cap is a hard
ceiling (token bucket, burst of 8). Learning ON averaged about 16.8 bps per
trade versus about 9.8 with learning off (guard disabled) on the same
synthetic stream. That is a property of the generator, **not evidence of profit**.

## Honest limits

- Synthetic data with a hidden truth built to be learnable; real markets are not.
- No order placement, venue connection, latency model, or fee schedule.
- Compliance obligations for real automated trading (market-access risk controls,
  supervision, record keeping) are outside this model; it only teaches the shape
  of fail-closed gates.
