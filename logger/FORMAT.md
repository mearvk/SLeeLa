# SLeeLa Packet Log Format 1

## File Format

- Encoding: UTF-8
- Container: JSON Lines
- Extension: .jsonl
- One packet event per line
- Record identifier: SLeeLa-PacketLog-1
- Segment limit: 240 MiB (251,658,240 bytes)

## Required Fields

| Field | Type | Meaning |
|---|---|---|
| format | string | Format identifier |
| timestamp | string | UTC ISO-8601 timestamp |
| direction | string | sent or received |
| severity | string | trace through critical |
| admission | string | accepted, rejected, or error |
| protocol | string | Protocol/generation |
| packet_bytes | integer | Network packet/event byte count |
| heuristic_score | number | 0.0 through 1.0 |
| sequence | integer | Transport/application sequence when known |

## Optional Fields

service, operation, source, destination, stream_id, request_id, reason, and payload_base64.

The payload field is emitted only when explicitly enabled.

## Rotation

The logger never intentionally writes a segment beyond 240 MiB. Rotation occurs before the next complete JSON line would exceed the boundary. A record whose serialized representation alone exceeds 240 MiB is not written.

## Ordering

Write order is preserved within a segment. Consumers may sort records by timestamp, direction, severity, protocol, size, or sequence.
