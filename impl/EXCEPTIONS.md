# VM Exception and Stack-Unwind Design

Status: runtime foundation and first source-level catch-all lowering implemented on `work/vm-exception-compiler-stage1`; typed catches and finally remain incomplete.

## Why this work is required

The current compiler accepts Java-shaped `try`, `catch`, `finally`, and
`throw` syntax, but the execution path is not exception-safe:

- `emitTry` emits the try body and then the finally body. It does not
  register catch targets or arrange exceptional cleanup.
- `ThrowStmt` evaluates its operand and emits `OP_POP`, discarding the
  thrown value.
- The VM's current runtime error path writes a diagnostic and returns
  `SLR_ERROR`; it has no per-thread handler stack or language-level unwind.

This is a semantic gap, not merely a missing Java library method. Java String
null/bounds failures and many other JDK-compatible operations cannot be
implemented faithfully until the VM can transfer control to a handler.

## Required runtime model

### 1. Per-thread handler records

Exception state belongs to `SLThread`, never to the shared `SLVM`. Each
handler record must capture at least:

- handler instruction pointer;
- operand-stack height at registration;
- call-frame depth at registration;
- owning frame identity / handler-stack base;
- catch matching metadata (initially catch-all only, later runtime type).

The handler stack must be bounded consistently with the VM's fixed execution
limits. Registration overflow is a normal VM diagnostic; it must not corrupt
the operand or frame stacks.

### 2. Per-frame handler ownership

Each `SLFrame` records the handler-stack depth that existed when the frame
was entered. Returning from a frame discards handlers installed by that frame.
Unwinding across a frame restores the saved operand-stack height and removes
that frame's handlers before continuing in its caller.

This prevents a handler in a returned method from remaining active and
intercepting an unrelated later throw.

### 3. Throw and dispatch

A throw operation consumes one `SLValue` and searches the current thread's
handler stack from newest to oldest. On a match it:

1. preserves the thrown value as a GC-visible root;
2. discards handlers newer than the selected handler;
3. unwinds call frames to the handler's saved depth;
4. restores the operand stack to the saved height;
5. pushes the thrown value for the compiler-generated catch prologue; and
6. transfers the instruction pointer to the catch entry.

If no handler matches, the value becomes the thread's uncaught-exception
payload and execution terminates with a distinct uncaught-exception result
or diagnostic. It must not be silently converted to a normal return.

A throw from a catch body must search only handlers still active outside that
catch. A rethrow must preserve the original value and identity.

### 4. GC and lifetime rules

The active exception payload, any pending exception during cleanup, and
exception values stored in handler records are VM roots. Root enumeration must
include every live thread, including suspended worker threads, and the
native-callback caller stack. A handler must never retain a raw object pointer;
it stores the normal tagged `SLValue` handle.

The thrown value must remain rooted across stack truncation, frame unwinding,
and any cleanup code that may allocate or trigger a GC safepoint.

### 5. Finally and abrupt completion

Finally is not equivalent to code appended after a try block. It must execute
on normal completion, return, break/continue leaving the protected region,
and exception propagation. If finally itself throws, returns, or otherwise
completes abruptly, that completion replaces the pending completion according
to Java-like rules.

Implement this as explicit pending-completion state (kind plus payload/target),
not by duplicating unstructured finally bytecode at every exit. The initial
exception milestone may deliberately ship catch/throw without finally only if
the compiler rejects or clearly diagnoses finally in that mode; it must never
silently skip a finally block on exceptional exit.

### 6. Catch typing and source compatibility

The first runtime milestone may support a single catch-all clause whose
variable receives an arbitrary thrown `SLValue`. It must not pretend that
different declared catch types are distinguished. Typed catches require
runtime type identity / assignability metadata and ordered matching, including
multi-catch rules.

The compiler must reject unsupported multi-catch or typed-catch constructs
rather than routing every exception to the first clause. Null thrown values
must remain distinguishable from "no pending exception" through an explicit
pending-state flag.

## Bytecode and artifact compatibility

- Existing opcode values are serialized in artifacts and are append-only.
- Any handler/throw opcodes must be appended after the current last opcode;
  never insert them into the historical enum.
- Update the native enum, source opcode registry, opcode map, artifact
  validator, ABI minor version, and compile-time opcode assertions together.
- Validate handler targets as in-range instruction addresses.
- Older artifacts must retain their existing opcode interpretation.
- Do not serialize process-local handler records or exception handles.

A minimal opcode family is expected to provide handler registration,
handler removal, and throw dispatch. Exact names and operands should be fixed
only alongside the compiler lowering and artifact validator changes.

## Runtime error boundary

Language exceptions and VM integrity failures are different:

- Explicit source `throw` and selected language-level faults (for example
  null dereference and bounds errors) are candidates for catchable values.
- Stack overflow, malformed bytecode, invalid opcode, corrupt handles,
  allocator corruption, and host authorization failures remain fatal VM
  errors unless a narrowly defined recoverable contract is added.

Never convert every `TERR` into a catchable exception. That would allow source
code to catch and continue after VM invariant violations.

## Implementation sequence

1. Add per-thread handler/pending-exception state and root enumeration.
2. Add append-only handler push/pop/throw opcodes and artifact validation.
3. Add compiler lowering for one catch-all clause and a catch variable.
4. Add deterministic tests for nested handlers, cross-frame unwind, return
   cleanup, rethrow, uncaught values, stack restoration, and GC during unwind.
5. Add explicit finally/pending-completion lowering for all abrupt exits.
6. Add runtime type identity and typed catch matching.
7. Convert selected Java-compatible language faults into catchable exception
   objects, then connect Java String and JDK facades to those types.

## Acceptance criteria

No Java exception-compatibility claim is valid until tests prove all of the
following: nested catch selection, cross-method propagation, correct operand
and local restoration, rethrow identity, uncaught propagation, GC survival
during unwind, finally execution on every abrupt exit, and typed catch
ordering. A source parser accepting `try/catch/throw` is not evidence of
runtime support.

## Implemented runtime milestone

The current runtime branch now includes:

- bounded per-thread handler records and pending/uncaught exception slots;
- per-frame handler-stack ownership depth;
- GC marking for active exception payloads in the executing and suspended
  native-callback thread roots;
- append-only `OP_TRY_PUSH` (176), `OP_TRY_POP` (177), and `OP_THROW` (178);
- catch-all dispatch with saved stack/frame restoration and cross-frame unwind;
- an accessor for the last uncaught exception payload;
- artifact validation for handler targets and zero-operand exception opcodes;
- VM ABI minor version 6, with the artifact serialization layout unchanged.

An opt-in C smoke test is available as `make -C impl test-exceptions`. It covers
cross-frame catch dispatch and preservation of an uncaught false payload. It
has been added but has not been run in this milestone.

## Current limitation

The source compiler now lowers a single `catch (Any name)` clause to
`OP_TRY_PUSH`, `OP_TRY_POP`, and a handler prologue that stores the thrown
value in the catch local. Explicit `throw expression;` now emits `OP_THROW`.
The compiler rejects multiple catch clauses, narrower declared catch types,
catch-variable collisions with existing method locals, and any `finally`
block. This is deliberately narrower than Java: matching is catch-all only,
and the catch variable holds the original arbitrary tagged `SLValue`.

Typed exception classes and ordered matching, rethrow syntax/identity
semantics, selected language-fault conversion, and `finally`/abrupt-completion
lowering remain unimplemented. The compiler change and opt-in runtime smoke
test have not been executed in this milestone; source-level Java exception
compatibility is not claimed.


## Implemented compiler milestone

On `work/vm-exception-compiler-stage1`, `try { ... } catch (Any error) { ... }`
registers a catch-all handler, removes it on normal completion, and transfers
the thrown value into `error` on exceptional completion. `throw expression;`
now dispatches the evaluated value rather than discarding it. `finally` and
all unsupported catch shapes fail compilation explicitly. This is a first
source-to-bytecode integration step, not a claim of full Java exception
semantics.
