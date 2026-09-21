# Nordshrift SST — Synchro Measure Block (NS-SST-SYN-0001)

Status: implementation model
Revision: 1.0.0
Depends on: Synchro 1.3 (`SYNCHRO.md`, `impl/core/sleela_synchro.*`)

## Purpose

A `.sst` sheet may declare a first-class **`measure:`** block describing the
Synchro honest-measurement series it uses. Like `network:`/`finance:`/`reach:`,
the measure block carries a **closed** metric series; unknown members are
reported with structured `NSS-E-SYN-*` diagnostics.

Synchro is **honest by construction**: it measures what actually happened and
makes no delivery-time guarantee. A sheet therefore cannot set `honest false`.

## Grammar

```sst
measure:
  metrics    [ sent, received, loss, mean, min, max, p95, report ]
  timeout-ms 1000
  honest     true
```

### Closed series

| Key | Members |
|---|---|
| `metrics` | `sent`, `received`, `loss`, `mean`, `min`, `max`, `p95`, `report` |

### Scalars

| Key | Meaning | Default |
|---|---|---|
| `timeout-ms` | default per-dispatch measurement budget | 1000 |
| `honest` | measured, never promised; **must be true** | true |

## Diagnostics

| Code | Condition |
|---|---|
| `NSS-E-SYN-001` | unknown Synchro metric in `metrics` |
| `NSS-E-SYN-002` | `honest false` (rejected; Synchro is honest by construction) |

## Manifest emission

When a sheet declares `measure:`, `nordshrift build` emits it into the component
manifest for every target:

- **Java** — `MEASURE_METRICS`, `MEASURE_TIMEOUT_MS`, `MEASURE_HONEST`.
- **C** — `ns_measure_metrics[]`, `ns_measure_timeout_ms`, `ns_measure_honest`.
- **Sleela** — the same series as `String`/`int`/`boolean` fields.

`nordshrift check` prints a one-line summary:

```
measure: 8 metric(s) [sent, received, loss, mean, min, max, p95, report], timeout-ms=1000, honest=true [Synchro]
```

## Example

See [`examples/reach-measure.sst`](examples/reach-measure.sst).
