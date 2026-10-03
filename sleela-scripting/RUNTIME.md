# Sleela Script Runtime

The scripting runtime is an interpreter-first runtime designed to be embedded
in the existing SLeeLa C/C++ execution core.

## Pipeline

**Source → Lexer → Parser → AST/IR → Evaluator → Host API**

A bytecode backend may be added later without changing language semantics.

## Runtime responsibilities

The runtime owns:

1. lexical analysis;
2. parsing;
3. scope creation;
4. value representation;
5. function calls;
6. control-flow execution;
7. runtime error reporting;
8. host-capability dispatch.

The runtime does not independently resolve project configuration. It consumes
the canonical configuration context established by SLeeLa.

## VM relationship

Sleela Script is VM-aware but not VM-dependent.

`vm.select(1)` through `vm.select(11)` selects a SLeeLa VM target through
the common configuration layer. It does not create a second scripting VM
configuration hierarchy.

## Memory

Script values are owned by the scripting runtime and must participate in the
standard SLeeLa garbage-collection boundary when embedded. Native host handles
use explicit lifetime hooks.

## Errors

Every execution context carries:

- source name;
- line;
- column;
- error category;
- human-readable message.

A host may convert a script error into a process exit status.

## Embedding

The intended C interface is:

```text
create context
→ attach host API
→ load source
→ execute
→ inspect result/error
→ destroy context
```

No script may bypass the host capability table by constructing arbitrary native
function pointers or OS handles.
