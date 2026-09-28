# SLeeLa Proposed HTTP Packet Specification

This is the wire contract used by the native HTTP 4.0–9.0 server family.

## Fixed packet header

Every HTTP 4–9 packet is a 32-byte network-byte-order header followed by PAYLOAD-LENGTH bytes of payload.

| Offset | Size | Field |
|---:|---:|---|
| 0 | 1 | VERSION |
| 1 | 1 | TYPE |
| 2 | 2 | FLAGS |
| 4 | 8 | STREAM-ID |
| 12 | 8 | REQUEST-ID |
| 20 | 8 | SEQUENCE |
| 28 | 4 | PAYLOAD-LENGTH |

PAYLOAD-LENGTH is limited to 16 MiB.

## Common lifecycle

1. The first packet on a connection is OPEN with SEQUENCE=0.
2. VERSION must equal the server's generation.
3. SEQUENCE must never regress for a connection.
4. STREAM-ID identifies the logical stream.
5. REQUEST-ID identifies the request within that stream.
6. FLAGS are transport/application flags and are logged as metadata.
7. Payload bytes are opaque to the common framing layer and interpreted by the target generation.
8. END terminates the current connection/session in the native listeners.

## Packet types

Types 1–9 are common transport/session controls: OPEN, DATA, END, RESET, WINDOW, PING, PONG, RESUME, CAPSULE.

HTTP 5 adds 10–13: FRIENDS_PACK, BONUS_OFFER, FP_UPDATE, AUDIT.

HTTP 6 adds 14–20: CONSOLIDATED_FRIENDS_BET, TEAMSTER_DEBATE, CONSOLIDATE_IQ, TEAM_AREA, DEBATE_TOPIC, DEBATE_POSITION, RECIPIENT_LABEL.

HTTP 7–9 retain the same envelope. Their payload is generation-specific:
- HTTP 7: semantic/assertion request and response data.
- HTTP 8: handshake, subscription/radio/session and checker data.
- HTTP 9: packet metadata, identity, monitoring configuration, international data and Dark Band metadata.

The protocol names are SLeeLa application generations; they are not claims that public Internet standards currently define HTTP/4 through HTTP/9.

## Server requirement

Each native server must parse the common envelope before interpreting generation-specific payload data, reject an invalid version/type/length, enforce the first-packet handshake rule, enforce sequence monotonicity, and log packet identity without logging opaque payload contents.
