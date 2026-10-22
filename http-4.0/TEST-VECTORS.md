# HTTP 4.0 Golden Test Vectors

These vectors are normative for the current draft. Integers are big-endian. Whitespace between hexadecimal octets is only for readability.

## Vector 1 — DATA frame

Logical fields:

- VERSION = 4
- TYPE = DATA (2)
- FLAGS = INCREMENTAL | RESUMABLE = 0x0018
- STREAM-ID = 7
- REQUEST-ID = 42
- SEQUENCE = 3
- PAYLOAD-LENGTH = 19
- PAYLOAD = UTF-8/ASCII bytes for "incremental payload"

Header bytes (32 bytes):

```text
04 02 00 18
00 00 00 00 00 00 00 07
00 00 00 00 00 00 00 2a
00 00 00 00 00 00 00 03
00 00 00 13
```

Payload bytes (19 bytes):

```text
69 6e 63 72 65 6d 65 6e 74 61 6c 20 70 61 79 6c 6f 61 64
```

Complete frame length is 51 bytes (32-byte header + 19-byte payload).

## Vector 2 — Empty OPEN frame

Logical fields: VERSION=4, TYPE=OPEN (1), FLAGS=0, STREAM-ID=1, REQUEST-ID=42, SEQUENCE=0, PAYLOAD-LENGTH=0.

```text
04 01 00 00
00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 2a
00 00 00 00 00 00 00 00
00 00 00 00
```

Complete frame length is 32 bytes. Implementations MUST reject a frame with the same header but a truncated or over-limit payload. These vectors validate encoding and framing only; they do not validate authentication, authorization, session negotiation, or resume-token security.
