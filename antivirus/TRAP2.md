# Sleelavirin™ Trap-2 Operator Heuristic

Trap-2 is a SLeeLa/Sleelavirin convention, not an industry-standard antivirus algorithm.

The profile makes heuristic reasoning visible to the operator and bounded by an explicitly declared operator basis and license/profile context.

## Operator basis

A Trap-2 profile declares operator identity, license/profile identifier, basis identifier, normal threshold, Trap-2 threshold, whether automated action is permitted, and explicit operator acknowledgement.

## Two-stage interpretation

Stage 1 records observed indicators. Each indicator has an identifier, weight, observation state, and human-readable reason.

Stage 2 evaluates the declared thresholds. Trap-2 is a stronger operator-defined evidence state; it is not an automatic claim that a file is malicious.

Automated action is permitted only when the profile explicitly allows it and the operator, basis, and license identifiers are present and acknowledged.

## Operator-friendly behavior

- no hidden indicator weights;
- no undeclared external action;
- no automatic deletion or quarantine;
- no privilege escalation;
- no assumption that a semi-match is malicious;
- evidence and basis remain available to the caller.

The implementation is designed to complement provider integrations such as Windows AMSI and rule systems such as YARA rather than silently replacing them.
