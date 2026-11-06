# RRS signal monitor

Observability over the RRS ternary **1 / 0 / 2** signal stream. The monitor
watches an RRS feed-forward and produces a **signal analysis**: for every
premise in every stage it records the *effective* signal RRS actually applied
(after faith is subtracted and after the responsibility prelude), why any
elimination happened, the running power, and the potestas culmination.

It is **read-only** — it never changes an RRS outcome. It runs the real model
(`sllmrrs_feed_forward`) and re-derives each effective signal exactly as
`sllmrrs_fold_stage` does, so the analysis matches what RRS computed.

## What it reports

| Field | Meaning |
|---|---|
| `affirm` / `eliminate` / `power-add` | counts of effective **1 / 0 / 2** signals |
| eliminations by reason | `signalled-no`, `faith-subtracted`, `responsibility-unmet` |
| stage power | accumulated power per stage (conjunctive / responsibility / solidity) |
| culmination | `total_power`, `solid`, `potestas` (none / provisional / warranted) |
| stream | per-premise events: stage, effective signal, reason, running power, text |

## Usage

```c
sllmrm_event ev[32];
sllmrm_monitor mon;
sllmrm_monitor_init(&mon, ev, 32);         /* or (NULL, 0) for tally-only */
sllmrm_observe(&mon, s1,n1, s2,n2, s3,n3, NULL);

char report[2048];
sllmrm_format_report(&mon, report, sizeof report);
puts(report);
```

A sample run (solid + responsible): `affirm=1 eliminate=0 power-add=2`,
`total_power=5 solid=1 potestas=warranted`. A faith premise shows up as
`eliminate` with reason `faith-subtracted`; an unmet responsibility as
`responsibility-unmet`.

## Files

| File | Role |
|---|---|
| `rrs_signal_monitor.h/.c` | C11 monitor (`sllmrm_*`, ABI v1): observe a feed-forward, tally the 1/0/2 signals + reasons + stage power, format a report |
| `rrs_signal_monitor_demo.c` | Warranted / faith-subtracted / responsibility-unmet runs + tally-only mode; self-checks |

## Build and run

```sh
cc -std=c11 -O2 -Wall -Wextra -pedantic \
   ../rrs.c ../../large-language-models/large_language_models.c \
   ../../large-language-models/large_language_models_reasoning.c \
   rrs_signal_monitor.c rrs_signal_monitor_demo.c -o rrs_signal_monitor_demo
./rrs_signal_monitor_demo
```

## What is NOT bundled

No weights. The monitor is pure observability over the RRS core (`../rrs.h`).
See `../RRS.md` for the model it watches.
