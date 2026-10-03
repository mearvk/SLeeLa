# Sleela Script Security

Sleela Script follows a capability-based host model.

## Rules

1. Core language evaluation is side-effect limited.
2. Filesystem access requires a host capability.
3. Process execution requires a host capability.
4. Network access requires an explicitly registered capability.
5. Native pointers are never script values.
6. Host handles are opaque.
7. Script modules resolve through approved search roots.
8. Resource limits are host-controlled.
9. Configuration is resolved through the canonical SLeeLa configuration root.
10. Restricted CI mode should disable process, network, and arbitrary-write
   capabilities unless explicitly enabled.

A script being syntactically valid does not grant it native authority.

## Resource limits

Hosts should be able to bound:

- execution time;
- instruction/evaluation steps;
- memory;
- recursion depth;
- output bytes;
- open handles;
- module count.

The exact mechanism is host-specific but the policy boundary is part of the
language contract.
