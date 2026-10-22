# SLeeLa HTTP 6.0 — Status

HTTP 6.0 is an experimental SLeeLa application protocol, not an IETF HTTP/6 standard and not production-certified. The reference server uses cleartext TCP; TLS, peer authentication, application authorization, request deadlines, rate limits, and a complete multiplexed stream state machine are not implemented.

## Implemented/documented

- Consolidated Friends' Bet application record;
- Friends' Pack inheritance;
- Teamster Debate fields;
- explicit recipient/team-area labels;
- negotiation C/C++ sources;
- C/C++ syntax-check build;
- bounded C++ Consolidated Friends' Bet model with UTF-8 validation and deterministic JSON serialization;
- strict HTTP request framing and response-header validation in the hardening branch;
- regression tests for malformed request framing, reserved frame flags, download-state fields, and bounded JSON serialization;
- common logical-port and DOWNLOAD contracts.

## Boundary

All packet contents are ordinary user/application data. Recipient labels do not imply delivery, endorsement, authority, or acceptance.

## Verification targets

Run the exact branch/commit through the cross-platform hardening workflow. Remaining production gates include socket read deadlines, rate/concurrency limits, authenticated TLS, fuzzing, full stream/replay/window semantics, and independent security review. A successful parser test is not authorization or proof of safety.
