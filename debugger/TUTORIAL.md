# SLeeLa Debugger — User Tutorial

Version: 0.8.0

## Purpose

This tutorial takes a new SLeeLa user through ten progressively deeper steps for understanding and using the debugger and its decompiler-oriented inspection workflow.

> **Important:** The debugger's current portable/model capabilities are not the same thing as a completed native decompiler. Native execution, symbol recovery, DWARF/PDB processing, and advanced reverse-engineering features must be verified through their corresponding backend and conformance tests before being treated as production capabilities.

## Step 1 — Understand the Debugger

Start with the basic mental model:

**Target → Native Backend → DebugEngine → DebugSession/Event Bus → Actions → Diagnostics/Evidence → Tests/UI**

The debugger observes and controls a program through explicit contracts. It should never silently claim that an operation is supported when only the data model exists.

Learn these concepts first:

- DebugSession
- DebugEvent
- Breakpoint
- Watchpoint
- Stack frame
- Thread
- Register
- Memory
- Symbol
- Evidence
- Capability

Read:

- `DEBUGGER.ARCHITECTURE.md`
- `GLOSSARY.md`
- `1-2-3-4.md`

## Step 2 — Build the Debugger

From the `debugger/` directory:

```sh
make
```

Run the complete test path:

```sh
make test
```

Run only the conformance suite:

```sh
make conformance
```

Clean generated executables:

```sh
make clean
```

A successful build proves that the selected source set compiles. A successful portable/conformance test does **not** by itself prove native Linux, macOS, or Windows debugging.

## Step 3 — Learn Sessions and Stops

A debugging session has a lifecycle:

1. Launch or attach to a target.
2. Establish the session identity.
3. Set or discover debugging controls.
4. Resume execution.
5. Receive an event.
6. Record why execution stopped.
7. Inspect program state.
8. Continue, step, or terminate.
9. Preserve evidence.

The important question is not merely “did it stop?” but:

**Why did it stop, what evidence proves that, and what program state was observed?**

Use the Stop Record and Evidence Bundle concepts when reading debugger output.

## Step 4 — Use Breakpoints and Watchpoints

A breakpoint asks the debugger to stop execution at a defined location.

A watchpoint asks it to stop when a memory location is accessed according to a specified access policy.

Understand the difference between:

- source/location breakpoints;
- temporary breakpoints;
- conditional breakpoints;
- watchpoints;
- hardware-assisted breakpoints/watchpoints;
- modelled versus native breakpoints.

Example conceptual workflow:

```text
launch target
    ↓
set breakpoint
    ↓
continue
    ↓
breakpoint event
    ↓
inspect frame/registers/memory
    ↓
record evidence
```

Do not interpret a portable breakpoint record as proof that the operating system inserted a native breakpoint.

## Step 5 — Read Program State

Once stopped, work outward from the current execution point.

Inspect:

1. Current thread.
2. Current stack frame.
3. Calling frames.
4. Registers.
5. Relevant memory.
6. Symbols and source location.
7. Loaded modules.
8. Exception or signal information.

The goal is to reconstruct a defensible picture of the program state rather than relying on a single line of output.

Native register, memory, stack, and symbol availability is backend-specific.

## Step 6 — Understand the Decompiler Boundary

The debugger and a decompiler answer related but different questions.

A debugger asks:

> **What is the program doing right now?**

A decompiler asks:

> **What higher-level structure can reasonably be reconstructed from compiled code?**

A decompiler workflow can involve:

- executable/module identification;
- architecture identification;
- section and segment inspection;
- symbol discovery;
- relocation information;
- strings and constants;
- control-flow reconstruction;
- function-boundary analysis;
- call-graph construction;
- type inference;
- source correlation when debug information exists;
- reconstructed pseudocode.

SLeeLa should preserve the distinction between **observed facts**, **recovered symbols**, and **inferred structure**.

## Step 7 — Trace Decompiled Functions Back to Evidence

When examining a recovered function, associate the decompiler result with evidence wherever possible.

A useful chain is:

```text
binary
  ↓
module identity
  ↓
address/range
  ↓
function
  ↓
instruction/control-flow evidence
  ↓
symbol/source information
  ↓
debugger runtime observation
  ↓
diagnostic evidence
```

When source information is unavailable, label the result accordingly.

Do not present inferred names, types, or control flow as original source unless the evidence actually supports that conclusion.

## Step 8 — Use Diagnostics and Reproduction

When something fails, preserve the evidence.

The DiagnosticsEngine supports the conceptual record of:

- replay checkpoints;
- replay records;
- crash information;
- sanitizer findings;
- memory findings;
- profiling samples;
- coverage records;
- synchronization/lock records;
- session artifacts;
- security policy decisions.

Use the `.sleela-debug` artifact model to keep a session reproducible and inspectable.

A useful debugging report should answer:

- What target was examined?
- Which binary/build was used?
- Which debugger/backend was active?
- What happened?
- Where did it happen?
- Why did execution stop?
- What evidence was collected?
- What capabilities were actually available?

## Step 9 — Apply the Conformance and Security Rules

Before calling a debugger feature complete, move it through:

**Model → Implement → Integrate → Verify**

The Conformance Suite exists to prevent a documented interface from being mistaken for a finished native capability.

Security is equally important.

Treat:

- attach;
- memory writes;
- expression evaluation;
- plugins;
- paths;
- session artifacts;
- crash dumps;
- symbols;
- external input

as potentially sensitive operations.

Never use debugger expressions as an accidental shell.

Never treat an untrusted debugging artifact as trustworthy merely because it has the expected file extension.

Read:

- `CONFORMANCE.md`
- `SECURITY.md`
- `SESSION_FORMAT.md`
- `COMMAND_LANGUAGE.md`

## Step 10 — Grow From User to Debugger Developer

After mastering the first nine steps, begin contributing new capabilities.

For every proposed feature, ask:

### 1. Model
What data, states, errors, and contracts are required?

### 2. Implement
Does the code actually perform the operation?

### 3. Integrate
Does it connect correctly to the backend, DebugEngine, session, diagnostics, and user interface?

### 4. Verify
Are there executable positive, negative, regression, security, and platform-aware tests?

For decompiler-oriented work, add evidence at every stage:

```text
Input Binary
    ↓
Identity / Architecture
    ↓
Sections / Symbols
    ↓
Instructions
    ↓
Control Flow
    ↓
Functions
    ↓
Types / Variables
    ↓
Recovered Representation
    ↓
Runtime Validation
    ↓
Evidence / Report
```

The mature SLeeLa workflow is therefore not simply:

**“Decompiler → output.”**

It is:

**“Input → analysis → evidence → reconstruction → debugger correlation → verification → report.”**

## Next Reading

After completing this tutorial, continue with:

- `DEBUGGER.ARCHITECTURE.md`
- `1-2-3-4.md`
- `DEBUG_ENGINE.ARCHITECTURE.md`
- `DEBUG_DIAGNOSTICS.md`
- `CONFORMANCE.md`
- `NATIVE_EXECUTION.md`
- `SESSION_FORMAT.md`
- `SECURITY.md`
- `COMMAND_LANGUAGE.md`
- `DEBUGGER.QUALITY.md`

## Completion Check

A user has completed the tutorial when they can explain, without relying on undocumented behavior:

- what the debugger knows;
- what the backend actually implements;
- what the decompiler infers;
- why execution stopped;
- where evidence came from;
- which capabilities are native;
- which capabilities are only modelled;
- how a session is preserved;
- how security boundaries are enforced; and
- how a new feature moves through Model → Implement → Integrate → Verify.
