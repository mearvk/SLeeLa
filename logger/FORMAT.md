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

## Completed-Segment Copy / Remote Destination

When `move_completed_segments` is enabled and `archive_directory` is set, each completed 240 MiB log segment is copied to the configured destination as part of rotation. The complete file is flushed, copied, and size-verified before rotation advances to the next local segment.

Example configuration:

```text
move_completed_segments = true
archive_directory = "/mnt/remote/sleela-logs"
```

The destination is a normal filesystem path. A remote drive or network share should be mounted by the operating system first. Existing destination names are preserved by selecting a `.copy-N` suffix rather than overwriting an earlier archive.

The local segment is retained by this safety-oriented copy operation. This means a remote destination can receive a full historical segment without making the local logging path dependent on the remote drive remaining available. If the destination is unavailable, the local log continues to be retained and the archive operation reports failure.
