# HTTP 5.0 Frame Test Vectors

The HTTP 5.0 envelope uses a 32-byte header:

`VERSION(1) TYPE(1) FLAGS(2) STREAM-ID(8) REQUEST-ID(8) SEQUENCE(8) PAYLOAD-LENGTH(4) PAYLOAD(N)`

All multibyte integers are unsigned big-endian.

## Minimal empty DATA frame

Header fields:

- VERSION = 5
- TYPE = 2 (DATA)
- FLAGS = 0
- STREAM-ID = 1
- REQUEST-ID = 2
- SEQUENCE = 3
- PAYLOAD-LENGTH = 0

Expected 32-byte header (hex):

```text
05 02 00 00
00 00 00 00 00 00 00 01
00 00 00 00 00 00 00 02
00 00 00 00 00 00 00 03
00 00 00 00
```

## Invalid vectors

An implementation must reject each of these before dispatch:

1. VERSION is not 5.
2. TYPE is 0 or greater than 13.
3. FLAGS contains any bit outside `0x003F`.
4. PAYLOAD-LENGTH exceeds `HTTP5_MAX_PAYLOAD`.
5. The input ends before all 32 header bytes are present.
6. The input contains fewer payload bytes than PAYLOAD-LENGTH.
7. An HTTP `/__sleela/h5` request has trailing bytes after the one declared frame.

## Current scope

These are envelope vectors, not complete semantic vectors. Type-specific payload grammars, negotiation transcript vectors, and session-state transition vectors remain release gates. Do not claim complete HTTP 5.0 conformance from envelope validation alone.
