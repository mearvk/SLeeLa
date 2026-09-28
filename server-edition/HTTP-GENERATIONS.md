# SLeeLa Server Edition — HTTP Generations 1.0–9.0

The Server Edition recognizes the complete SLeeLa HTTP generation family as an explicit dispatch domain.

**Holding Document → Forwarding Annotation → Nexter Colony → HTTP Generation → Transport Adapter → Execution**

## Generation policy
- HTTP 1.0 / 1.1: Internet HTTP compatibility and ordinary HTTP/1.x request semantics.
- HTTP 2.0 / 2.1: SLeeLa's documented HTTP/2 generation and stream-oriented server path.
- HTTP 3.0: SLeeLa's HTTP/3/QUIC generation.
- HTTP 4.0–9.0: SLeeLa project-specific protocol generations; they must not be represented as official IETF HTTP standards.

The Server Edition must never silently reinterpret one generation as another.

## Annotation forwarding
A Holding Document can declare: @next system/network/transport
The Server Edition interprets this as a Forwarding Annotation and resolves its declared Nexter Colony before handing data to the selected HTTP-generation adapter.

The annotation does not itself choose privileges, open a native port, execute code, or authorize deployment.

## Safety boundaries
Every generation adapter must preserve capability checks, resource limits, destination validation, trace identifiers, loop/hop limits, structured logging, production counters, and release/stage controls.

## Internet boundary
HTTP 1.0/1.1 interoperability is treated separately from SLeeLa-specific generations. HTTP 2.0–9.0 names identify SLeeLa project generations where applicable; they are not claims that the Internet has standardized official HTTP/4 through HTTP/9 protocols.

## Production trace
HTTP Generation → Holding Document → Forwarding Annotation → Nexter Colony → Source → Group → Counter → Runbook → Evidence

Max Rupplin — MEARVK LLC — 2026
