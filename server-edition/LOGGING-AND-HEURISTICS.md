# Logging, Filtering, and Heuristics

The server uses deterministic filtering first and anomaly heuristics second.

## Filtering

Configured policy can limit:

- minimum log severity;
- allowed services;
- denied services;
- denied operations;
- packet, header, and payload sizes;
- supported protocol grades;
- packets per second;
- idle time.

Rejected packets are never dispatched.

## Heuristics

The heuristic score is an anomaly indicator from 0.0 to 1.0. It is not a finding of malicious behavior.

Signals currently include:

- excessive packet rate;
- repeated parse failures;
- missing provenance fields on HTTP 8 and 9 metadata;
- missing Dark Band metadata on HTTP 9;
- sequence zero on a non-zero stream;
- non-printable routing data;
- pressure near configured packet limits.

High scores generate alert-level logs. The score is evidence about server processing only; it is not a statement about a sender's identity, intent, authorization, or legal status.

## Structured logs

Logs are JSON Lines and include UTC time, severity, event, generation, stream/request/sequence, service, operation, admission status, heuristic score, and reason where available.

Payload contents are not logged by default. The log-payload switch records payload byte counts only.

## Example policy

A deployment can permit only known service names, deny selected operations, cap packets and payloads, and retain all rejected events for audit review without treating the heuristic system as an automatic accusation engine.


## Packet Logger Integration

The common SLeeLa Logger in /logger is the storage implementation for this policy. It accepts both sent and received packet events and applies six selectors before storage: direction, severity, protocol, admission, endpoint, and size/heuristic threshold.

Packet logs use SLeeLa-PacketLog-1 JSON Lines files. Each segment is capped at 240 MiB (251,658,240 bytes); rotation occurs before a complete record would cross the boundary. Payload storage is disabled by default.

HTTP generations and server editions should submit records at packet admission/egress boundaries rather than implementing separate log formats.
