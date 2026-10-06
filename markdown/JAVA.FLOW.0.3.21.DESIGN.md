# Java Flow 0.3.21 Design

Purpose: complete SLeeLa's Java source-level definite-assignment, definite-unassignment, reachability, and abrupt-completion analysis. This is source/API congruence, not JVM, bytecode, SLVM, JNI, or runtime equivalence.

## Core model

Use an internal JavaFlowState containing definitelyAssigned, definitelyUnassigned, reachable, abruptExits, and diagnostics. Add JavaFlowBooleanState with whenTrue, whenFalse, and abruptExits. Add JavaFlowAbruptExit with kind, label, assigned, unassigned, and sourceDepth. Add JavaFlowControlTarget with label, acceptsBreak, acceptsContinue, and sourceDepth. Preserve the existing public result API while enriching internal analysis.

## Rules

1. Track assigned and unassigned independently. Absence from one set does not imply membership in the other.
2. Boolean expressions require separate true and false normal states.
3. Joins intersect facts across every normal path. Abrupt paths are never treated as normal fall-through.
4. Assignment evaluates the right side before changing the target state.
5. Control targets are resolved with an explicit stack. A labeled non-loop accepts break but not continue.

## Boolean flow

- true constant: true path reachable, false path unreachable.
- false constant: false path reachable, true path unreachable.
- !E: swap E's true and false states.
- A && B: evaluate B only from A-true; true comes from B-true; false joins A-false and B-false.
- A || B: evaluate B only from A-false; false comes from B-false; true joins A-true and B-true.
- C ? A : B: analyze C directionally, then analyze A and B from their respective states and join their true/false results independently.

## Statement flow

If: then gets true facts; else gets false facts; missing else contributes incoming facts.

While: maintain condition-false exits, matching breaks, continues back to the condition, and escaping abrupt exits. A constant-true loop has no condition-false exit and can complete normally only through a matching break.

Do: body first, then condition; continue targets the condition; normal completion joins condition-false and matching-break exits.

For: initialization, condition, body, update, then condition again. Break skips update; continue reaches update before the next condition.

Switch: distinguish selector evaluation, case reachability, traditional fall-through, break exits, no-match completion, and modern rule completion.

Try/catch/finally: preserve normal and abrupt paths separately. Finally runs on every outgoing path. An abrupt finally supersedes the prior path; a normal finally allows the prior path to continue outward.

## Constructor/final model

Use a dedicated constructor context. Track constructor entry, superclass-constructor invocation, instance-initializer boundaries, blank-final fields, definite-unassignment, and assignment sites. A blank final must be definitely unassigned before its permitted assignment.

## Lambda/capture model

Analyze lambda bodies as nested flow regions. Validate captures against the enclosing state. State changes inside a lambda do not mutate enclosing flow facts.

## Exception integration

Checked-exception analysis should consume the same structural traversal. Each construct contributes normal flow, abrupt throw flow, and checked exception types; the exception layer then applies catch coverage and declaration rules.

## Qualification corpus

Add positive and negative fixtures for: boolean paths; &&, ||, !, ?:; boolean constants; while/break; non-terminating loops; do-while; for update; labeled break; invalid labeled continue; switch fall-through and break; try/catch; normal and abrupt finally; blank-final constructor assignment; double assignment; lambda capture; and checked-exception propagation.

Every rule requires a Java fixture, normalized SLeeLa representation, named semantic rule, positive result, negative result, and unified-manifest evidence.

## Implementation order

A State model → B Boolean flow → C control targets → D loops → E switch → F try/finally → G constructor/final → H lambda/capture → I exception composition → J unified qualification closure.

Oracle's Java SE 27 specification is the current released baseline; JLS Chapter 16 defines flow in terms of possible execution paths, including directional boolean analysis and abrupt completion.

**SLeeLa — MEARVK LLC — 2026**
