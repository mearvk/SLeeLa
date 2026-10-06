# SLeeLa Time API

The SLeeLa Time API exposes one portable timing interface over the native C/C++ timepiece.

## Raw international time request

### SLeeLa

```
sleela ping $(timezone 1) 1
```

The command represents a standard NTP request followed by one separate byte, ASCII `1` (`0x31`). The marker is deliberately separate from the NTP packet so the time request remains standard.

### C/C++

```c
#include "sleela_time.h"

SLTimeSample sample;
int rc = sltime_send_raw_time(
    "pool.ntp.org",
    123,
    (uint8_t)'1',
    1500,
    &sample
);
```

The function sends the normal NTP client request to UDP port 123 and then sends the one-byte marker as a separate UDP datagram. It returns a local observation record; use `sltime_query_ntp()` when a four-timestamp NTP offset/delay measurement is required.

### API methods

| Method | Purpose |
|---|---|
| `timeUtcMillis()` | Absolute UTC milliseconds |
| `timeUtcNanos()` | Absolute UTC nanoseconds at host clock resolution |
| `timeMonotonicNanos()` | High-resolution monotonic elapsed-time clock |
| `timePrecisionMillis()` | Local timing uncertainty estimate |
| `timeLocation()` | Current country/timezone selection |
| `timeSetLocation(country, timezone)` | Explicit location selection |
| `timeHttpDate()` | HTTP Date representation |
| `timeJson()` | JSON timing record |
| `timeNtp(host)` | NTP measurement |
| `sltime_send_raw_time()` | Raw NTP request + one-byte marker |

## Internationalization rule

`timezone 1` is an endpoint-selection convention, not an assertion that a timezone name itself is a network address. A deployment may map country/timezone identifiers to an agreed NTP endpoint registry.

The one-byte `1` marker is a SLeeLa interoperability convention, not part of the NTP standard. Systems that do not implement the convention may ignore or reject the additional datagram.

## Security

Do not use the marker or an unauthenticated remote NTP response as an authentication credential. Preserve the source, offset, stratum, and uncertainty when timing is security-sensitive.
