# Server Edition — First-Class Language Annotations

The Server Edition consumes the same annotation metadata produced by the SLeeLa
language front end. It does not parse a second annotation dialect.

## End-to-end contract

**Wrapper → Lexer → Parser → AST → Semantic Analysis → Compiler → Runtime → Server Edition**

The Server Edition receives:

- Holding Document identity;
- all parsed annotations;
- Forwarding Annotation(s);
- resolved Nexter Colony when a single safe @next is present;
- source/trace metadata;
- capability and operational metadata;
- HTTP-generation context when an HTTP adapter is selected.

## Forwarding boundary

The Server Edition must treat @next as a request to enter the explicit forwarding
policy, not as arbitrary source execution.

Required order:

1. Receive the already-parsed annotation metadata.
2. Validate the annotation/runtime contract.
3. Resolve the Nexter Colony through the configured routing/provider layer.
4. Check capability, authorization, resource, and policy controls.
5. Apply hop/cycle/resource limits.
6. Preserve the Holding Document and forwarding trace.
7. Select the requested HTTP-generation adapter without silently translating
   SLeeLa HTTP 4.0–9.0 into an official Internet HTTP version.
8. Forward only when the destination is admitted by policy.
9. Log the decision and counters.

## HTTP boundary

The existing HTTP bridge remains the protocol boundary:

**Holding Document → Forwarding Annotation → Nexter Colony → HTTP Generation → Transport Adapter**

The language front end is upstream of that boundary. The Server Edition therefore
does not need to reinterpret source syntax.

## Runtime invariant

A program containing multiple @next annotations is valid metadata but the current
runtime rejects forwarding until a multi-destination policy is explicitly selected.
This prevents accidental fan-out.

Max Rupplin — MEARVK LLC — 2026
