# SLeeLa Server Edition Wire Protocol

This is the transport envelope used by sleelas for the SLeeLa HTTP 1.0 through 9.0 application packet family described by PACKET.md. It is an SLeeLa application carrier, not a redefinition of IETF HTTP.

## Fixed 40-byte header

All integers are unsigned big-endian.

| Offset | Size | Field |
|---:|---:|---|
| 0 | 4 | MAGIC = ASCII SLPK |
| 4 | 1 | WIRE-VERSION = 1 |
| 5 | 1 | PROTOCOL-GRADE = 1 through 9 |
| 6 | 2 | FLAGS |
| 8 | 4 | HEADER-LENGTH |
| 12 | 4 | PAYLOAD-LENGTH |
| 16 | 8 | STREAM-ID |
| 24 | 8 | REQUEST-ID |
| 32 | 8 | SEQUENCE |

The variable section is HEADER-LENGTH bytes followed by PAYLOAD-LENGTH bytes.

Header records are one per line as KEY=VALUE. Required routing keys are PROTOCOL-GRADE, SERVICE-ID, and OP-ID. The fixed-header grade is authoritative and a mismatch is rejected.

HTTP 1 through 3 use the routing fields directly. HTTP 4 through 9 retain stream, request, sequence, and payload framing while carrying their generation-specific fields from PACKET.md.

The server treats complex metadata as application data. It never executes a field as a command.

## Hard limits

The server rejects bad magic, bad wire version, truncated frames, duplicate headers, missing routing fields, non-printable routing values, unknown grades in strict mode, denied services or operations, packet-rate violations, and configured size-limit violations.

Responses are application admission responses in the form:

SLPK/1 STATUS REASON

They are not native HTTP status lines.
