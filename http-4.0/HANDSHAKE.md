# SLeeLa HTTP 4.0 Capability Handshake

HTTP 4.0 is experimental in this repository; this handshake is a SLeeLa protocol mechanism, not an IETF HTTP/4 negotiation.

Initial capability bits:

| Bit | Capability |
|---|---|
| 0x00000001 | FRAME_BASE |
| 0x00000002 | INCREMENTAL |
| 0x00000004 | RESUMABLE |
| 0x00000008 | FLOW_CONTROL |
| 0x00000010 | MIGRATION |
| 0x00000020 | CAPSULE |
| 0x00000040 | MULTI_PATH |

The common capability set is the bitwise intersection of local and remote capabilities.

A feature is enabled only when its corresponding bit exists in that common set.

The handshake is independent of the HTTP/3 carrier. An HTTP/3 adapter can transport the capability exchange as an opaque SLeeLa capsule/body, while a future native carrier can map it directly to control frames.

## Downgrade rule

A peer MUST NOT claim support for a feature merely because the carrier supports a related HTTP feature. SLeeLa HTTP 4.0 capability support is negotiated explicitly.

A peer that cannot understand HTTP 4.0 SHOULD remain on the existing HTTP/3 carrier path rather than interpreting HTTP 4.0 frames as ordinary HTTP semantics.
