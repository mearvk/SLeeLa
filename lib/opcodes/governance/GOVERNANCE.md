<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa Opcode Governance — Registrar · Listener · Event Observer

**Family:** `lib/opcodes/governance`
**Revision:** 0.1
**Builds on:** `lib/opcodes` (one class per VM opcode)
**Native bridge:** `native/include/sleela_gov.h`, `native/src/sleela_gov.cpp`

A developer *may* run opcodes raw into the VM with `SLOpcodeStream.run()`.
Usually, though, we want **discretion** before a series of opcodes runs upon a
program. This sub-family gives SLeeLa herself that procedural determination: a
program is considered from **A to B** before it runs, watched **as it comes**,
and judged as a **musical, ordered process** once complete. SLeeLa and the VM
both know to listen for ordering at three phases — **BEFORE**, **DURING**, and
**AFTER** — modeled by `SLGovPhase`.

## The three principals

### Registrar — `SLOpcodeRegistrar` (BEFORE)
Added to a `.sleela` source, the Registrar *considers the whole program A → B
before any opcode runs* and checks that the opcodes are **in order unto
themselves**:

1. every opcode is a valid, armed, canonical opcode (codes 0–102);
2. the sequence **terminates** — exactly one `OP_HALT`, and it is last (or the
   source is explicitly marked a composable fragment);
3. **nothing runs past the terminal**;
4. the program is non-empty.

A clean pass **ADMITs**. A terminal-placement problem **PAUSEs as unrest** (it
may be a fragment awaiting composition). A structural violation **REJECTs**.

### Listener — `SLOpcodeListener` (DURING)
The Listener *listens to the live VM code* and confirms the admitted sequence
fits **relatively and respectively** into the program as it arrives:

- **relatively** — each opcode respects its neighbours and partners: `OP_RET`
  only with an open `OP_CALL` frame, `OP_UNLOCK` only while a `OP_LOCK` is held,
  socket reads/writes/closes only on a tracked open socket;
- **respectively** — the live instruction pointer the VM reports advances
  monotonically through the admitted sequence and does not diverge (a backward
  ip outside a sanctioned branch is **unrest**).

At the end of DURING, `settle()` turns unresolved pairings (open frames, held
locks, open sockets) into **unrest** or a **warning**.

### Event Observer — `SLOpcodeEventObserver` (AFTER)
The Event Observer decides whether the operations form a **musical and ordered
process** about program **architecture, breadth, height, and purpose**, and
whether the inputs, commands, outputs, and resulting stimulus all make sense
against the base concepts (`SLGovConcept`):

| Concept | What is checked |
|---|---|
| **straightness** | control flow is straight — `OP_CALL`/`OP_RET` balance over the whole |
| **linear reals** | division/modulo stay on the real line (guard non-zero divisor) |
| **outright goals** | the program declares an outright goal (`purpose`) |
| **ethics and norms** | no operation violates ethics or norms — a hard **REJECT** |
| **finalization over fields** | opened files and sockets are all closed |
| **times upon counts** | time reads are proportionate to program counts |
| **final goals** | the program reaches a definite final goal (it halts) |

It also measures **breadth** (how many capability groups are touched) and
**height** (deepest call/lock nesting). From these it returns a graded verdict:
**ADMIT**, **WARN** (concern recorded), **PATCH** (a known symbol map was
applied), **PAUSE** (unrest), or **REJECT** (ethics/norms). Where it finds a
patchable fault, it consults `SLGovSymbolMap` — a registry of **known symbol
maps that just work** — and substitutes the known-good opcode rather than
rejecting the program. An unknown fault is never silently "fixed".

## Discretion as a graded verdict — `SLGovVerdict`

Rather than a raw pass/fail, every principal returns a graded disposition so the
program can be admitted, warned about, patched, **paused as unrest**, or
rejected:

```
ADMIT  <  WARN  <  PATCH  <  PAUSE  <  REJECT
```

The more restrictive disposition always wins when verdicts combine, so a single
`REJECT` or `PAUSE` anywhere governs the sequence.

## Orchestration — `SLGovernedExecution`

`SLGovernedExecution` ties the three principals around an `SLOpcodeStream`:

```
BEFORE : registrar.consider(stream)         -> block (REJECT/PAUSE) or proceed
DURING : for each opcode as the VM reaches it:
             listener.observe(op, liveIp, i) -> block or continue (WARN counts)
             op.step(frame)                   -> fire exactly one opcode
         listener.settle()
AFTER  : observer.assess(stream, flaw)       -> ADMIT / WARN / PATCH / PAUSE / REJECT
```

Each phase is `announce()`d across the native bridge so **both SLeeLa and the
VM** register their interest and listen for ordering. `admitted()` is true only
when the program passed all three phases without being blocked; `reasonHalted()`
explains any block.

## Classes

| Class | Phase / role |
|---|---|
| `SLGovPhase` | the BEFORE / DURING / AFTER vocabulary |
| `SLGovVerdict` | the graded discretionary outcome |
| `SLGovConcept` | the seven base concepts the observer weighs |
| `SLGovSymbolMap` | known symbol maps that just work (safe patches) |
| `SLOpcodeRegistrar` | BEFORE — considers A → B; admit / reject / pause |
| `SLOpcodeListener` | DURING — live fit, relatively and respectively |
| `SLOpcodeEventObserver` | AFTER — the musical, ordered process judgement |
| `SLGovernedExecution` | the orchestrator over an `SLOpcodeStream` |

## Native bridge

- `sleela_gov_announce_phase(phase)` — notify SLeeLa and the VM of BEFORE/DURING/AFTER.
- `sleela_gov_live_ip(frame, index)` — the live instruction pointer the Listener hears.
- `sleela_gov_current_phase()` / `sleela_gov_phase_count(phase)` — phase tracking.

The governance *policy* lives in SLeeLa source; this bridge only announces
phases and reports the live ip. The authoritative execution stays in `/impl/core`.

```sh
cd native
make test   # builds + self-tests audio, opcode, and governance bridges
```

**Max Rupplin — MEARVK LLC — 2026**
