# Nordshrift SST — Munction Reach Block (NS-SST-MUN-0001)

Status: implementation model
Revision: 1.0.0
Depends on: Munction 1.3 (`MUNCTION.md`, `impl/core/sleela_munction.*`)

## Purpose

A `.sst` sheet may declare a first-class **`reach:`** block describing the
Munction reach-composition series it uses. Like the `network:` and `finance:`
blocks, the reach block carries a **closed** verb and channel series, so a sheet
cannot silently request an unsupported Munction verb or system-method channel.
Unknown members are reported with structured `NSS-E-MUN-*` diagnostics.

The block is metadata for the compilation model; executable reach behavior is
expressed by the Munction source surface (`Munction.start(...).connect(...)...`)
and lowered into the core VM opcodes.

## Grammar

```sst
reach:
  verbs     [ start, connect, enable, send, thatch, consume, observe,
              propagate, latch, contain, closeWithReceipt, abort ]
  channels  [ pipe, file, tcp, http, sdps, crypto ]
  min-verbs 4
  max-verbs 16
  receivable true
  coherent   true
```

### Closed series

| Key | Members |
|---|---|
| `verbs` | `start`, `connect`, `open`, `enable`, `send`, `thatch`, `consume`, `observe`, `propagate`, `latch`, `contain`, `close`, `closeWithReceipt`, `abort` |
| `channels` | `pipe`, `file`, `tcp`, `http`, `sdps`, `crypto` |

### Scalars

| Key | Meaning | Default |
|---|---|---|
| `min-verbs` | sentence floor; **may not go below 4** | 4 |
| `max-verbs` | sentence ceiling; **may not exceed 16** | 16 |
| `receivable` | every reach yields a receipt | true |
| `coherent` | every send accounts offered vs. acknowledged bytes | true |

## Diagnostics

| Code | Condition |
|---|---|
| `NSS-E-MUN-001` | unknown Munction verb in `verbs` |
| `NSS-E-MUN-002` | unknown Munction channel in `channels` |
| `NSS-E-MUN-003` | `min-verbs` below the Munction floor of 4 (clamped to 4) |
| `NSS-E-MUN-004` | `max-verbs` above the Munction ceiling of 16 (clamped to 16) |

The 4..16 sanity bound is fixed by the language; a sheet may **narrow** it but
never **widen** it.

## Manifest emission

When a sheet declares `reach:`, `nordshrift build` emits it into the component
manifest for every target:

- **Java** — `REACH_VERBS`, `REACH_CHANNELS`, `REACH_MIN_VERBS`,
  `REACH_MAX_VERBS`, `REACH_RECEIVABLE`, `REACH_COHERENT`.
- **C** — `ns_reach_verbs[]`, `ns_reach_channels[]`, `ns_reach_min_verbs`,
  `ns_reach_max_verbs`, `ns_reach_receivable`, `ns_reach_coherent`.
- **Sleela** — the same series as `String`/`int`/`boolean` fields.

`nordshrift check` prints a one-line summary:

```
reach: 12 verb(s) [start, connect, ...], 6 channel(s) [pipe, file, tcp, http, sdps, crypto], bound=4..16, receivable=true, coherent=true [Munction]
```

## Example

See [`examples/reach-measure.sst`](examples/reach-measure.sst).
