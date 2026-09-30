# Java Flow 0.3.21 Implementation Gate

This document defines the next executable source-semantic improvement for SLeeLa Java authorship qualification. The target is Java source/API congruence; JVM, bytecode, SLVM, and runtime equivalence are outside this gate.

## Required implementation

1. Boolean branch facts: definitely-assigned and definitely-unassigned state when an expression is true or false.
2. Boolean flow: constants, logical complement, conditional-and, conditional-or, and conditional expressions.
3. Control transfer: unlabeled and labeled break/continue, target validation, and normal-completion joins.
4. Loops: while, do, and for sequencing, break exits, continue paths, and constant-true completion.
5. Switch: traditional groups and fall-through first; modern rule completion as parser/AST support advances.
6. try/catch/finally: normal and abrupt paths with correct joins.
7. Constructors and blank-final fields: definite-unassignment across constructor and initializer boundaries.
8. Lambda/capture boundaries: assignment-state checks at capture boundaries.

## Qualification evidence

Each rule requires a Java fixture, normalized SLeeLa representation, named semantic rule, positive case, negative case, and unified-manifest evidence. Parser acceptance alone is not qualification.

## Test gate

The dedicated flow suite must cover these rules and the unified Java qualification manifest must include the flow result. Platform records describe declared targets and observed hosts; they are not remote execution claims.

## Scope

This gate qualifies Java source-level authorship and semantic structure. It does not require a JVM or SLVM.

Oracle's Java SE specifications identify Java SE 27 as the current released specification baseline. Chapter 16 defines definite assignment around possible execution paths and gives special treatment to boolean expressions and abrupt completion.

**SLeeLa — MEARVK LLC — 2026**
