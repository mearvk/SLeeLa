# Improvements from SLVM/10 to SLVM/11

SLVM/11 (**Sovereign**) establishes a filesystem-module **host** above the
verified storage contracts of SLVM/9 and SLVM/10. It provides a stable slot for
custom filesystem definitions while keeping filesystem semantics versioned and
capability-driven; a module does not receive authority merely by being present.

| Area | /10 | /11 |
|---|---|---|
| Storage | Verified storage lifecycle | Filesystem-module host above /9 + /10 |
| Extension | TAC3 as a profile | `file-system/modules/<module-id>/` standard slot |
| TAC3 | Advanced profile | First formulaically standardized module (byte-identical with /9, /10) |
| Admission | Operation verification | Module discovered, version-checked, identity-verified, capability-qualified |
| Schema | Fixed contracts | Schema-driven extension for custom modules |
| Authority | Authority is not permanence | A present module is still not authority |

SLVM/11 is a **real, runnable virtual machine**: it links the shared native
Core engine (`impl/build/libslcore.a`) and runs compiled `.sleela` artifacts.
Its generation contribution — the filesystem-module host
(`src/slvm11_filesystem_module.c`) — is applied as a pre-boot admission step:
the baseline TAC3 module must validate before a program boots, and a malformed
module is rejected under `--selfcheck`.

SLVM/11 remains an execution evolution, not a second language implementation.
