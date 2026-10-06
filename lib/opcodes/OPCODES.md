<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

# SLeeLa `/lib/opcodes` — One Class Per Opcode

**Family:** `lib/opcodes`
**Revision:** 0.1
**Canonical ISA:** `lib/vm/InstructionSet.sleela` / `lib/vm/OPCODE-MAP.md`
**Native bridge:** `native/include/sleela_opcode.h`, `native/src/sleela_opcode.cpp`

The `/lib/opcodes` family expresses the SLeeLa virtual-machine instruction set
as **one SLeeLa source class per opcode**. Each class carries exactly one opcode
and nothing else, so the object inventory of the ISA is directly measurable and
every instruction has a first-class, nameable identity in SLeeLa source.

## What is here

| Count | Content |
|------:|---------|
| 1 | `SLOpcodeBase.sleela` — the shared single-opcode contract |
| 1 | `SLOpcodeStream.sleela` — the verbatim, ordered opcode sequence driver |
| 103 | `SLOp*.sleela` — one class per opcode, codes **0–102** |
| 1 | `OPCODES.md` — this document |
| 8 | `governance/` — the Registrar · Listener · Event Observer series (see `governance/GOVERNANCE.md`) |
| 6 | `running/` — grouping, conditional-reactive, and warming helpers (see `running/RUNNING.md`) |

Codes **0–97** are the **base 98** opcodes (`OP_NOP` … `OP_AUDIO_PLATFORM`).
Codes **98–102** are the five array-extension opcodes added for syntax 1.4
(`OP_NEWARRAY`, `OP_ARRGET`, `OP_ARRSET`, `OP_ARRLEN`, `OP_ARRPUSH`). The package
therefore ships **the 98 opcodes, and more** — the full canonical set.

## The single-opcode contract

Every opcode class extends `SLOpcodeBase` and carries one `CODE` and one
`MNEMONIC`. The base defines the ordered contract the user asked for — *order
the opcodes toward the VM as SLeeLa source that immediately, carefully calls the
VM to the next instruction, then executes the single opcode*:

```
step(frame):
    ip = advance(frame)   // 1. carefully call the VM to the NEXT instruction (fetch)
    return fire(frame, ip)// 2. execute THIS one opcode, return the next ip
```

This mirrors the authoritative native dispatch loop in `impl/core/sleela_core.c`:

```c
SLInstr in = vm->code[ip++];   /* fetch: advance to the next instruction */
switch (in.op) { ... }         /* dispatch exactly one decoded opcode    */
```

`advance()` is the `ip++` fetch; `fire()` is the single `switch` case. Because
each object does one fetch and one dispatch and then returns the next
instruction pointer, a program is simply a stream of these objects handed to the
VM in order.

### Ordering the stream toward the VM

`SLOpcodeStream` is the verbatim, ordered container. `run()` walks the stream in
program order and, for each opcode object, performs the contract — fetch the
next instruction, execute the one opcode — flowing control `1,2,3,…,N` exactly
as emitted. The VM owns the instruction pointer; the stream only hands it the
next single opcode. A negative return (from `OP_HALT` or a rejected opcode) ends
the stream, matching the runtime rule below.

### Governance — considering a program before it runs

A developer *may* run opcodes raw via `SLOpcodeStream.run()`. Usually, though, a
program should be **considered before, watched during, and judged after** it
runs. The `governance/` sub-family adds that discretion in SLeeLa herself: a
**Registrar** considers the program A → B before any opcode runs, a **Listener**
confirms the admitted sequence fits the live VM program as it comes, and an
**Event Observer** decides whether the operations form a musical, ordered process
about architecture, breadth, height, and purpose — issuing warnings or patching
faults with known symbol maps. SLeeLa and the VM both listen for ordering at the
`BEFORE` / `DURING` / `AFTER` phases. See `governance/GOVERNANCE.md`.

### Running helpers — grouping, conditional-reactive, warming

Beyond a flat stream, the `running/` sub-family offers richer ways to run
opcodes: **grouping** (`SLOpcodeGroup`, `SLOpcodeGroupSet`) runs cohesive
clusters as units; a **conditional-reactive** layer (`SLOpcodeCondition`,
`SLOpcodeConditionalReactive`, `SLOpcodeReactorBank`) reacts to program state by
warming, gating, running, or skipping a group; and **warming**
(`SLOpcodeWarmer`) readies hot paths before they run. These compose with the
governance series. See `running/RUNNING.md`.

## Native bridge and the runtime rule

The two primitives below the SLeeLa layer are:

- `sleela_opcode_vm_next(frame)` — the fetch (advance the instruction pointer).
- `sleela_opcode_execute_one(frame, code, ip)` — dispatch exactly one opcode.

Consistent with `lib/vm/OPCODE-MAP.md`, an **unknown opcode is rejected, never
silently treated as a no-op**: `execute_one` returns `-1` and records
`"unsupported opcode"`. `OP_HALT` also returns `-1` to stop the stream. The
authoritative execution semantics remain in `/impl/core`; this bridge models the
fetch/dispatch discipline and the mnemonic table (103 entries, codes 0–102).

Build and self-test:

```sh
cd native
make test   # builds + self-tests audio, crypto, and opcode bridges
```

---

## Why 98 opcodes is complete for a modern program and developer

A deliberate claim of this ISA is that a compact, well-chosen set of 98 opcodes
(codes 0–97) is **functionally complete** for building modern software — not
merely Turing-complete in theory, but practically sufficient for the programs a
working developer actually writes. The set is small because it is organised by
*capability*, and each capability is covered without redundant encodings:

**1. Computation and data flow (0–22).**
Constants, stack discipline (`OP_POP`, `OP_DUP`), global and local load/store,
the full arithmetic set (`ADD` … `NEG`), the full comparison set (`EQ` … `GE`),
and boolean logic (`AND`, `OR`, `NOT`). Any pure expression a program evaluates
reduces to these. `OP_ADD` is overloaded for string concatenation, so text
assembly needs no separate opcode.

**2. Control flow and procedures (23–27).**
`OP_JMP`, `OP_JMPF`, `OP_CALL`, `OP_RET`. Conditional and unconditional branching
plus call/return are the complete basis for every higher-level control
structure — `if`, `while`, `for`, `switch`, recursion, and early return are all
lowered onto exactly these four. No structured-control opcodes are needed because
structure is a *compiler* concern, not an *ISA* concern.

**3. Observable output (27).**
`OP_PRINT` gives programs a direct, honest observable effect.

**4. Concurrency and synchronization (28–33).**
`OP_SPAWN`, `OP_JOINALL`, `OP_LOCK`, `OP_UNLOCK`, `OP_SEND`, `OP_RECV`. Threads,
joins, mutual exclusion, and channel message-passing. This is enough to express
both shared-memory and message-passing concurrency — the two models modern
software actually uses — without baking in a specific scheduler.

**5. Networking (34–42).**
Full socket life-cycle (`LISTEN`, `ACCEPT`, `CONNECT`, `SOCKREAD`, `SOCKWRITE`,
`SOCKCLOSE`) plus pipes and FIFOs. A server or client program needs no more than
these to speak to the outside world; higher protocols (HTTP, RMI) are libraries
built *on* them.

**6. Files and the filesystem (43–47).**
Open, read, write, close, unlink. The complete minimal set for durable storage.

**7. Lifecycle (48).**
`OP_HALT` cleanly stops the machine.

**8. Time (49–57).**
UTC and monotonic clocks, precision, location, and interchange formats (HTTP
date, JSON, NTP). Modern programs are deeply time-aware — logging, scheduling,
tokens, caching — so time is a first-class ISA capability rather than an
afterthought.

**9. Structured data (58–62).**
Struct construction, field get/set, and pack/unpack to JSON. This gives
programs composite records and a serialization boundary in five opcodes.

**10. Honest measurement and reach — Synchro, Munction, Best-of (63–90).**
These three capability groups cover network probing with honest measurement,
reach composition, and configurable route/accuracy selection — the operations a
modern distributed program performs when it must *choose* among peers and report
truthfully about what it did.

**11. Native media (91–97).**
The audio job life-cycle and host-platform query, delegating heavy media work to
the native bridge rather than pretending the VM is a mixer.

### Why not fewer, and why not many more

- **Not fewer:** every group above is a distinct *capability class*. Removing any
  one removes a kind of program a developer must write (you cannot drop sockets
  and still write a server). Within each group the opcodes are orthogonal — no
  opcode is expressible as a short, obvious sequence of the others at the same
  layer.
- **Not many more:** richer behaviour is layered as *libraries and compiler
  lowering*, not new opcodes. `if`/`while`/`for`, exceptions, iterators,
  pattern matching, HTTP, TLS, and the rest of the standard library all compile
  down to this set. Keeping the ISA at ~100 opcodes keeps the VM small enough to
  audit, port, and verify — the native dispatch is a single reviewable `switch`.

The array extension (98–102) demonstrates the growth discipline: dynamic arrays
were a genuinely new *capability* (a growable indexed sequence) that the base 98
could not express cheaply, so five opcodes were appended **at the end** so no
existing serialized-artifact opcode number shifted. New opcodes are added only
when a true new capability appears, following the maintenance rule in
`lib/vm/OPCODE-MAP.md`.

**The result:** a developer writing a modern program — concurrent, networked,
time-aware, persisting structured data, producing media, and choosing among
peers honestly — is fully served by the 98 base opcodes, with the whole ISA
remaining small enough to hold in your head and trust.

**Max Rupplin — MEARVK LLC — 2026**
