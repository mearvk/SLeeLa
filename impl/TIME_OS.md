# SLeeLa Precision Time

SLeeLa now exposes a cross-platform precision-time facility for absolute UTC time, monotonic elapsed time, location metadata, NTP calibration, and transport adapters.

## Clock model

1. **UTC milliseconds** — the best available host realtime clock, represented as Unix epoch milliseconds.
2. **Monotonic nanoseconds** — a non-wall-clock counter for precise elapsed-time measurement. Linux uses CLOCK_MONOTONIC; Windows uses QueryPerformanceCounter.
3. **Precision estimate** — returned as a conservative uncertainty estimate for the local clock sample.
4. **Location** — ISO-3166 country metadata plus an IANA timezone name may be supplied. A CPU does not inherently know its physical country, so SLeeLa does not invent a country from hardware alone.

The IANA Time Zone Database publishes country and zone mapping data, and the current release is 2026d. IANA also publishes country mapping files such as iso3166.tab and zone.tab. citeturn0search2turn2view0

## Network time

The timeNtp(host) method and sltime_query_ntp() can query a hostname or literal IP on UDP/123. This works with a router only when that router actually exposes an NTP service; an arbitrary router IP does not inherently expose a clock.

The NTP exchange records the local transmit/receive times and the server's NTP timestamps, allowing an offset and delay estimate rather than treating packet arrival time as the remote clock. RFC 5905 defines the four-timestamp offset and delay calculations and distinguishes precision, resolution, delay, dispersion, and jitter. citeturn1search1

## RMI, BODI, and HTTP

The C API exposes:

- sltime_rmi_record() — stable UTF-8 timing record for a Java/RMI bridge. It is intentionally not represented as Java Object Serialization or as the RMI wire protocol.
- sltime_bodi_record() — fixed 32-byte network-order binary timing envelope.
- sltime_http_date() — standards-compatible HTTP Date value.
- sltime_json() — machine-readable timing record suitable for HTTP APIs and other RPC layers.

HTTP-date is UTC and RFC 7231 specifies the IMF-fixdate form for generated HTTP dates; it also recommends NTP or a similar synchronization source for a server's clock. citeturn1search0

## SLeeLa methods

- timeUtcMillis()
- timeMonotonicNanos()
- timePrecisionMillis()
- timeLocation()
- timeHttpDate()
- timeJson()
- timeNtp(host)

For maximum integrity, consumers should retain the source, offset, uncertainty, and stratum fields rather than treating every timestamp as equally authoritative.

## Security

Remote NTP is an advisory measurement. It must not silently replace the trusted host clock for authentication, signatures, audit integrity, or other security-critical decisions. High-assurance deployments should compare independent time sources and preserve uncertainty.

## Raw interoperability ping

SLeeLa also provides a deliberately small raw-time interoperability primitive. The conceptual command is:

    sleela ping $(timezone 1) 1

where `$(timezone 1)` resolves to the selected standard time endpoint/timezone target and the final `1` is exactly one byte (`0x31`) used as an interoperability marker. The standard NTP request remains a normal 48-byte NTP client request; the marker is sent separately as a one-byte UDP datagram so an NTP server is not handed a malformed NTP packet.

The native entry point is `sltime_send_raw_time(host, port, marker, timeout_ms, sample)`. It is intended for small, internationally interoperable probes and diagnostics. A remote host should only interpret the marker if an application-level convention explicitly says it should; the marker is not part of the NTP standard.

This facility does not claim that every timezone name maps to a network endpoint. Timezone selection and network endpoint selection are separate concerns and should be resolved by the caller or an agreed registry.
