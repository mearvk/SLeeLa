# Sleela Script — Turing 5

Turing 5 is the SLeeLa Script execution profile for general-purpose temporary computation. It is Turing-complete with five integration boundaries:

1. Compute — arbitrary procedures, recursion, iteration and data transformation.
2. Scientific — mathematics, units, symbolic expressions, calculus, numerical methods and scientific lookups.
3. Observe — inspect approved SLeeLa objects, VM state, known variables, queues and sequence identifiers.
4. Act — update explicitly writable object properties, queues and runtime conditions.
5. Control — pause, resume, yield, schedule, timeout and terminate temporary work.

The name describes the execution profile; it does not claim that a finite machine exceeds Turing-machine computational power.

## Authority

**The SLeeLa document remains authoritative. The script is a temporary worker.**

A script may calculate, inspect, propose, and perform explicitly permitted runtime changes. It may not silently redefine the SLeeLa source document or bypass its contracts.

## Finite lifetime

Every script context has a deadline. Supported units are seconds, minutes, hours and days.

    timeout 45 minutes

A host may impose a shorter maximum. The timeout covers evaluation and registered host operations. CPU, memory, output, queue, recursion and handle limits may also apply.

## Temporary work model

    SLeeLa Document
        -> Script Context
        -> Observe
        -> Calculate
        -> Act
        -> Report
        -> Expire

Persistent changes require an explicit writable host operation.
