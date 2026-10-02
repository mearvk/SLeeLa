# Improvements from SLVM/1 to SLVM/2

SLVM/2 uses /1 as its execution and security specification rather than replacing it.

| Area | /1 foundation | /2 improvement |
|---|---|---|
| Runtime | Runtime/OS boundary | User/deployment-selectable runtime target and adapter |
| Security | Memory, runtime, I/O, capability, broker security | Composed fail-closed policy for sensitive operations |
| Cryptography | Security/broker hooks | Explicit provider abstraction; no custom crypto |
| Linking | Core execution | Hashes, signatures, ABI and capability declarations |
| Observability | Function/object/memory/I/O observer | Security, crypto, link, certificate and runtime events |
| Certificates | Credential hooks | Trust-store policy and attestation |
| Government | General security architecture | Declarative technical compliance profiles |
| Configuration | VM defaults | Versioned runtime-selection configuration |

The central invariant remains: if the authoritative compiler can legally produce an Output Symbol, the VM generation must define its execution semantics or explicitly reject an incompatible artifact rather than silently reinterpret it.
