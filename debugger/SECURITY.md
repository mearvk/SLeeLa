# Debugger Security Boundary

Version: 0.8.0

Debugger operations are privileged diagnostic actions.

Controls include:
- explicit attach authorization;
- explicit memory-write authorization and byte limits;
- bounded expression evaluation;
- no shell execution through expressions or commands;
- plugin trust boundaries;
- hostile symbol/crash-artifact handling;
- path and resource limits;
- auditable security-sensitive actions.

Read, inspect and report operations are baseline diagnostic operations. Native backends remain responsible for operating-system permission failures.
