# SLeeLa HTTP Packet Specification

**Scope:** SLeeLa HTTP 1.0 through HTTP 9.0.  
**Purpose:** Exact application-level packet fields, inheritance, and generation-specific additions.

This is a SLeeLa application protocol specification. It does not redefine IETF HTTP, HTTP/2, HTTP/3, QUIC, TCP, UDP, TLS, or native socket addressing.

## Common packet rules

A packet MUST identify its SLeeLa generation and MUST carry the application information required by its operation.

| Field | Meaning |
|---|---|
| PROTOCOL-GRADE | SLeeLa generation identifier |
| LOGICAL-PORT | SLeeLa application routing identifier; not inherently a TCP/UDP port |
| SERVICE-ID | Target logical service |
| OP-ID | Requested application operation |
| REQUEST-ID | Request/response correlation where defined |
| SEQUENCE | Packet/frame order where defined |
| PAYLOAD-LENGTH | Exact application payload length for framed generations |
| PAYLOAD | Application data unless the frame type explicitly has no payload |

A packet MUST NOT assume LOGICAL-PORT equals a native TCP/UDP port.

### Common DOWNLOAD mode

For files larger than 50 MB:

SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE

All seven fields are required for resume metadata. The threshold selects resume-oriented handling; it is not a maximum file size.

---

# HTTP 1.0

HTTP 1.0 is the baseline application packet.

### Required request

PROTOCOL-GRADE | LOGICAL-PORT | SERVICE-ID | OP-ID | PAYLOAD

Requirements:

1. PROTOCOL-GRADE = HTTP/1.0.
2. LOGICAL-PORT identifies the SLeeLa application route.
3. SERVICE-ID identifies the service.
4. OP-ID identifies the operation.
5. PAYLOAD contains operation data.

A response MUST retain enough request context to associate it with the originating operation.

DOWNLOAD adds the common seven-field resume record.

---

# HTTP 2.0 / 2.1

HTTP 2.x retains HTTP 1.0 routing and adds request correlation, retry classification, and stream-aware application handling.

### Required request

PROTOCOL-GRADE | LOGICAL-PORT | SERVICE-ID | OP-ID | REQUEST-ID | PAYLOAD

REQUEST-ID MUST correlate the request and response within the applicable session scope.

Retry-capable packets SHOULD distinguish first attempt, retry, non-retryable failure, and application-declined retry. Retry metadata MUST NOT cause an unsafe duplicate of a non-idempotent operation.

When a carrier stream is available, the application relationship is:

STREAM -> REQUEST-ID -> SERVICE-ID -> OP-ID

Native stream IDs remain carrier metadata.

DOWNLOAD uses the common seven-field record.

---

# HTTP 3.0

HTTP 3.0 extends HTTP 2.x with an application integrity/security boundary. HTTP/3 and QUIC may be carriers.

### Required application fields

PROTOCOL-GRADE | LOGICAL-PORT | SERVICE-ID | OP-ID | REQUEST-ID | PAYLOAD

Where application integrity is enabled, the configured integrity metadata MUST also be carried.

Carrier security and application integrity are separate:

- TLS/QUIC security is carrier security.
- SLeeLa integrity is application-packet security.
- One MUST NOT silently substitute for the other.

Processing-stage and retry information MAY be carried when required.

DOWNLOAD uses the common seven-field record.

---

# HTTP 4.0

HTTP 4.0 formalizes the message/frame lifecycle.

### Base frame

VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD

Every HTTP 4.0 frame MUST contain all eight fields.

- VERSION: SLeeLa HTTP 4.0 framing identifier.
- TYPE: frame operation.
- FLAGS: frame-control flags; unknown mandatory flags MUST be rejected.
- STREAM-ID: SLeeLa logical stream.
- REQUEST-ID: application request correlation.
- SEQUENCE: frame order in the stream/request scope.
- PAYLOAD-LENGTH: exact payload byte count.
- PAYLOAD: frame-specific data.

### Frame types

| TYPE | Required content |
|---|---|
| OPEN | Logical request/stream and operation context |
| DATA | Application data |
| END | Completion context |
| RESET | Termination/reset context |
| WINDOW | Receive-capacity information |
| PING | Liveness request and optional correlation |
| PONG | Corresponding PING response |
| RESUME | Interrupted-transfer continuation context |
| CAPSULE | Negotiated/session metadata |

SEQUENCE MUST be checked for ordering and replay conditions.

---

# HTTP 5.0

HTTP 5.0 retains the HTTP 4.0 frame/session model and adds Friends' Packs.

### Base frame

VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD

### FRIENDS_PACK

A FRIENDS_PACK payload MAY contain:

PACK-ID | RELATIONSHIP-ID | FP-BALANCE | FRIEND-LIST | BONUS-OFFER-REFERENCES | EXPIRATION | APPLICATION-POLICY

A friend entry is:

FRIEND-NAME | POINTS | DOCUMENT-REFERENCE

### Extensions

- FRIENDS_PACK: pack and friend records.
- BONUS_OFFER: optional offer reference.
- FP_UPDATE: application-level balance update.
- AUDIT: defensive conformance/audit event.

If FP reaches zero, the packet MAY report FP-BALANCE = 0. FP exhaustion MUST NOT be interpreted as network authority or as permission to affect unrelated traffic.

---

# HTTP 6.0

HTTP 6.0 retains HTTP 5.0 and adds the Consolidated Friends' Bet and Teamster Debate records.

### Base frame

VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD

### CONSOLIDATED_FRIENDS_BET

The record MAY contain:

FRIEND-LIST | POINTS | DOCUMENT-REFERENCES | TEAM-AREA | CONSOLIDATE-IQ | DEBATE-TOPIC | DEBATE-POSITION | RECIPIENT-LABEL

An implementation MUST distinguish an omitted optional field from a deliberately supplied empty field.

### TEAMSTER_DEBATE

TEAM-AREA | CONSOLIDATE-IQ | DEBATE-TOPIC | DEBATE-POSITION | RECIPIENT-LABEL

RECIPIENT-LABEL is application data identifying an intended audience; it does not force delivery or acceptance.

HTTP 6.0 retains Friends' Packs, points, document references, bonus-offer references, FP accounting, AUDIT data, and HTTP 4.0 frame/session fields.

---

# HTTP 7.0

HTTP 7.0 adds the Reality Assertion record.

### Base frame

VERSION | TYPE | FLAGS | STREAM-ID | REQUEST-ID | SEQUENCE | PAYLOAD-LENGTH | PAYLOAD

### ASSERTION

Required:

ASSERTION-TYPE | STATEMENT | STATUS | SOURCE | SOURCE-DATE | AUTHOR

Optional:

DOCUMENT-REFERENCE

Defined STATUS values:

- FACTUAL_DOCUMENTED
- USER_AUTHORED
- FICTIONAL
- COUNTERFACTUAL
- DISPUTED
- UNVERIFIED

The status is an application classification. The packet itself is not proof that the statement is true.

Forwarding SHOULD preserve AUTHOR, SOURCE, SOURCE-DATE, and DOCUMENT-REFERENCE.

---

# HTTP 8.0

HTTP 8.0 adds the cryptographic/session-security boundary and National Emblems, Signals, and Frequency metadata.

### Packet metadata

PROTOCOL-GRADE | PRIOR-PACKET-METADATA | IDENTITY | MONITORING/FREQUENCY | NATIONAL-SIGNAL-FREQUENCY | SEQUENCE | APPLICATION-PAYLOAD

PRIOR-PACKET-METADATA is present when chaining/history is enabled.

### NationalSignalFrequency

Required record fields:

EMBLEM | SIGNAL | FREQUENCY-UNIT | FREQUENCY-RANGE | JURISDICTION | SOURCE | TIMESTAMP

FREQUENCY-RANGE MUST be interpreted together with FREQUENCY-UNIT. Implementations MUST NOT silently change the unit.

Supported descriptive examples include Hz, kHz, MHz, and GHz.

SOURCE, TIMESTAMP, and JURISDICTION SHOULD be preserved when available.

These records are metadata only. They do not establish authenticity, ownership, legal authority, clearance, or operational control.

---

# HTTP 9.0

HTTP 9.0 MUST preserve the HTTP 8.0 packet metadata and adds DARK-BAND and INTERNATIONAL-DATA.

### Complete semantic packet

PROTOCOL-GRADE | PRIOR-PACKET-METADATA | IDENTITY | MONITORING | NATIONAL-SIGNAL-FREQUENCY | INTERNATIONAL-DATA | DARK-BAND | SEQUENCE | APPLICATION-PAYLOAD

### DARK-BAND

DARK-BAND-NAME | ORIGINAL-CONTENT-HEX

Default:

- DARK-BAND-NAME = Dark Band
- ORIGINAL-CONTENT-HEX = 0x4441524B42414E44

The default hexadecimal value is the application content/identifier representation of DARKBAND.

It is data only. It is NOT a cryptographic key, password, credential, authorization token, radio-control instruction, or hidden operational channel.

### INTERNATIONAL-DATA

Required fields:

SECURITY | SAFETY | POLICE | JURISDICTION | ORGANIZATION | IDENTIFIER | CLASSIFICATION | SOURCE | TIMESTAMP

Meanings:

- SECURITY: descriptive international security metadata.
- SAFETY: descriptive international safety metadata.
- POLICE: descriptive police/law-enforcement metadata.
- JURISDICTION: jurisdiction associated with the record.
- ORGANIZATION: associated organization.
- IDENTIFIER: application record identifier.
- CLASSIFICATION: application classification label.
- SOURCE: provenance/source.
- TIMESTAMP: documented time associated with the record's defined event.

POLICE is a data category, not a command channel. CLASSIFICATION does not itself grant access or clearance.

### HTTP 9.0 inheritance

An HTTP 9.0 upgrade MUST preserve the representability of:

- PROTOCOL-GRADE
- PRIOR-PACKET-METADATA
- IDENTITY
- MONITORING/FREQUENCY
- NATIONAL-SIGNAL-FREQUENCY
- SEQUENCE

---

# Generation summary

| Generation | Defining packet fields |
|---|---|
| HTTP 1.0 | PROTOCOL-GRADE, LOGICAL-PORT, SERVICE-ID, OP-ID, PAYLOAD |
| HTTP 2.0 / 2.1 | HTTP 1.0 + REQUEST-ID + retry/stream correlation |
| HTTP 3.0 | HTTP 2.x + application integrity/security metadata where enabled |
| HTTP 4.0 | VERSION, TYPE, FLAGS, STREAM-ID, REQUEST-ID, SEQUENCE, PAYLOAD-LENGTH, PAYLOAD |
| HTTP 5.0 | HTTP 4.0 + FRIENDS_PACK, BONUS_OFFER, FP_UPDATE, AUDIT |
| HTTP 6.0 | HTTP 5.0 + CONSOLIDATED_FRIENDS_BET and TEAMSTER_DEBATE |
| HTTP 7.0 | HTTP 4.x frame/session basis + ASSERTION |
| HTTP 8.0 | HTTP 8.0 security/session metadata + NationalSignalFrequency |
| HTTP 9.0 | HTTP 8.0 + DARK-BAND + INTERNATIONAL-DATA |

# Upgrade and downgrade

### Upgrade

1. Preserve fields that remain defined.
2. Add the new generation fields.
3. Do not reinterpret old values solely because the generation changed.
4. Preserve provenance and timestamps.
5. Validate new mandatory fields before transmission.

### Downgrade

1. The peer MUST explicitly accept the earlier generation.
2. Unsupported fields MUST NOT be falsely represented as transmitted.
3. Required security properties MUST NOT be silently weakened.
4. The application SHOULD report metadata that cannot be represented.
5. The downgrade MUST NOT imply unsupported feature support.

# Packet validation order

1. Validate PROTOCOL-GRADE.
2. Validate frame type and boundaries where applicable.
3. Validate LOGICAL-PORT, SERVICE-ID, and OP-ID.
4. Validate REQUEST-ID where required.
5. Validate SEQUENCE ordering.
6. Verify PAYLOAD-LENGTH against the actual payload.
7. Validate generation-specific mandatory fields.
8. Preserve/validate SOURCE and TIMESTAMP where required.
9. Enforce configured security/session policy.
10. Apply receiving application policy.
11. Reject malformed, truncated, overlong, or inconsistent packets.

# Authority boundary

A SLeeLa packet is application data. A packet field does not by itself create legal authority, police authority, security clearance, ownership, identity verification, system-access permission, network-control permission, frequency-transmission permission, or permission to intercept communications.

Those properties belong to the applicable authentication, authorization, legal, transport, deployment, and organizational systems.

# Conformance target

An implementation claiming support for a generation SHOULD be able to:

1. Construct required fields.
2. Serialize deterministically.
3. Parse without ambiguity.
4. Validate required fields.
5. Preserve provenance.
6. Reject malformed input.
7. Preserve supported predecessor metadata.
8. Negotiate the generation explicitly.
9. Handle DOWNLOAD resume metadata.
10. Document intentionally omitted optional fields.

This document is the packet-level companion to the SLeeLa http-1.0 through http-9.0 implementation directories.
