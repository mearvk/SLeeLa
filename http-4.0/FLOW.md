# SLeeLa HTTP 4.0 Flow

The HTTP 4.0 processing model is deliberately more explicit than the HTTP 3.0 application pipeline.

1. Carrier accepts bytes.
2. Frame decoder validates the fixed header and payload bound.
3. Session validates capability and stream state.
4. Sequence state checks ordering/replay policy.
5. OPEN establishes logical request context.
6. DATA carries incremental application content.
7. WINDOW communicates receive capacity.
8. RESUME identifies an accepted continuation point.
9. END closes a successful exchange.
10. RESET closes an exchange with an explicit error condition.
11. Observability records frame/session events without modifying application payloads.

## Incremental forwarding

HTTP 4.0 treats incremental DATA as a first-class protocol state. A carrier adapter may forward validated frames before an entire logical response has arrived. This is compatible with current HTTP ecosystem work on incremental message forwarding, but it is not a claim that SLeeLa HTTP 4.0 itself is an IETF standard.

## Resumability

A RESUME frame identifies stream ID, request ID, last accepted sequence, and an application transfer identifier. Persistent transfer state is a separate subsystem and is not hidden inside the frame decoder.

## Backpressure

WINDOW is an explicit receiver advertisement. The application layer MUST NOT infer unlimited buffering from a successful connection.

## Migration

A session may survive carrier/path changes if the negotiated MIGRATION capability is present and the implementation has authenticated session state. Stream IDs remain logical identifiers; native sockets are carrier details.
