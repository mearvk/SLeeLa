# Annotation Forwarding

## Uniform model
**Holding Document → Forwarding Annotation → Nexter Colony**

A forwarding annotation declares where held data continues. It is not an executable command, capability grant, or deployment authorization.

## Required behavior
1. Parse the annotation.
2. Validate destination syntax.
3. Resolve the declared Nexter Colony.
4. Check policy and capability.
5. Check resource and forwarding limits.
6. Detect cycles and forwarding loops.
7. Preserve trace and source metadata.
8. Forward data unchanged unless the destination contract explicitly permits a documented transformation.
9. Record success or rejection.
10. Emit an observable diagnostic when forwarding cannot complete.

## Failure modes
Malformed destination: reject.
Unauthorized capability: reject.
Unknown Nexter Colony: unresolved.
Loop or hop-limit violation: reject.
Resource limit: reject.
Accepted destination: forward and record success.

Example: @next system/network/transport. The document is the Holding Document; system/network/transport is its Nexter Colony.