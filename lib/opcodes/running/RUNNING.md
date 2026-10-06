<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa Opcode Running Helpers — Grouping · Conditional-Reactive · Warming

**Family:** `lib/opcodes/running`
**Revision:** 0.1
**Builds on:** `lib/opcodes` (one class per VM opcode) and `lib/opcodes/governance`
**Native bridge:** `native/include/sleela_opcode.h`, `native/src/sleela_opcode.cpp`

These classes are additional, suggested ways of **running** opcodes beyond a flat
`SLOpcodeStream`: cohesive **grouping**, a **conditional-reactive** layer that
reacts to program state, and **warming** that readies hot paths before they run.
They are composable with the governance series (Registrar / Listener / Event
Observer) — governance decides *whether* a program may run; these helpers shape
*how* its opcodes are organised and triggered.

## Grouping

A program is rarely one flat run; it is clusters of opcodes that belong together.

| Class | Role |
|---|---|
| `SLOpcodeGroup` | a named, ordered group of opcodes run as one unit — a loop body, critical section, handshake. Can `run()` directly, `warm()`, or `emitInto()` a stream. |
| `SLOpcodeGroupSet` | an ordered collection of groups (setup → work → teardown): `warmAll()`, `byName()`, flatten `emitInto()` a stream, or `run()` group-by-group. |

Grouping lets a developer reason about and gate opcodes in meaningful clusters
instead of a single long sequence, while order is still preserved inside each
group and across the set.

## Conditional-reactive

The reactive layer watches the running program and reacts to conditions by
grouping, warming, gating, or skipping opcodes.

| Class | Role |
|---|---|
| `SLOpcodeCondition` | a named predicate over a VM signal (ip, lock depth, call depth, open sockets, warning count, stream index) compared with `EQ`/`NE`/`LT`/`LE`/`GT`/`GE`. |
| `SLOpcodeConditionalReactive` | binds a condition (trigger) to a group (subject) and a reaction; edge-sensitive so it fires on the condition's rising edge. |
| `SLOpcodeReactorBank` | a bank of reactives ticked at each stream index; `driveStream()` runs a stream while ticking reactives around every opcode. |

**Reactions** a conditional-reactive can perform when its condition first holds:

- **WARM** — pre-warm the subject group so a hot path is ready before it is reached.
- **RUN** — run the subject group now, inline, as a reactive sub-sequence.
- **GATE** — admit the subject group only now that the condition holds (gated inclusion).
- **SKIP** — hold the subject group back (do not run it) while the condition holds.

Because reactives are edge-sensitive, they react once per rising edge and can be
`rearm()`ed for reuse. The bank's `admits()`/reaction logic lets a driver decide,
per group, whether it should run given the current conditions.

## Warming

`SLOpcodeWarmer` readies opcodes ahead of execution so the first fire of a hot
path has no cold-start cost: it `arm()`s each opcode in SLeeLa and asks the VM to
pre-stage the fetch via the native bridge. It can warm one opcode (`warmOne`), a
whole group (`warmGroup`), or an entire set (`warmSet`), and keeps a tally so the
reactive layer can tell whether a path is warm before gating it in. Warming never
executes an opcode.

## Native bridge additions

Two primitives were added to the opcode bridge for this sub-family:

- `sleela_opcode_signal(frame, signal, index)` — observe a well-known VM signal
  (`SIG_IP`, `SIG_LOCK_DEPTH`, `SIG_CALL_DEPTH`, `SIG_OPEN_SOCKETS`,
  `SIG_WARN_COUNT`, `SIG_INDEX`) for `SLOpcodeCondition`.
- `sleela_opcode_prestage(frame, code)` — warm (ready) an opcode without
  executing it, for `SLOpcodeWarmer`.

The authoritative counters and fetch/decode live in `/impl/core`; this bridge
exposes deterministic reference values so the grouping and reactive layers can be
exercised in isolation.

```sh
cd native
make test   # builds + self-tests audio, opcode (incl. signal/prestage), and governance bridges
```

## Classes

| Class | Kind |
|---|---|
| `SLOpcodeGroup` | grouping |
| `SLOpcodeGroupSet` | grouping |
| `SLOpcodeCondition` | conditional-reactive trigger |
| `SLOpcodeConditionalReactive` | conditional-reactive |
| `SLOpcodeWarmer` | warming |
| `SLOpcodeReactorBank` | conditional-reactive dispatch |

**Max Rupplin — MEARVK LLC — 2026**
