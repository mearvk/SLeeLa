<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Logger

The SLeeLa Logger is the common packet-storage layer for SLeeLa server editions and HTTP generations. It accepts sent and received packet records and applies deterministic logger logic before storage.

## Six Logger Logic Items

1. Direction — select sent, received, or both.
2. Severity — retain records at or above the configured severity.
3. Protocol — select HTTP generation, transport, or service prefixes.
4. Admission — distinguish accepted, rejected, and error records.
5. Endpoint — select source and destination prefixes.
6. Size / heuristic — select packet-size ranges and heuristic-score thresholds.

All six selectors are evaluated before a record is written. A filtered record consumes no log storage.

## 240 MiB Storage Boundary

Each JSON Lines segment has a hard 240 MiB boundary (251,658,240 bytes). When the next complete record would cross the boundary, the logger closes the current segment and starts the next numbered segment.

This is a log-file rotation limit, not a network MTU and not a transport packet-size limit. A single serialized record larger than 240 MiB is rejected rather than producing an oversized segment.

## Privacy

Payload bytes are not written by default. Packet byte counts and routing metadata are sufficient for normal operational logging. Payload logging must be explicitly enabled.

## Integration

Use one Logger at a server-edition or shared runtime boundary. HTTP 1 through HTTP 9 can submit the same PacketRecord structure, including resolver-selected source or destination information. The logger does not make routing or admission decisions; it records the result of those decisions.

Build the smoke test with:

    make

Run:

    ./logger-smoke

## Corrections

Fixed build-blocking defects: a broken string literal (`"\\""` →
`"\\\""`, which left an unterminated string) and two member functions
(`archive_segment`, `archive_path_for`) that were defined and used but never
declared in the `Logger` class. The logger (and the server edition that links
it) now builds. See the 2026-10-03 entry in [`../REVISIONS.md`](../REVISIONS.md).