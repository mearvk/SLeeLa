# CPU Clock and I/O Timing Specification

Clock speed is not treated as equivalent to instruction latency.

Each model may define:

- base frequency
- maximum/boost frequency
- clock domains
- phase/edge behavior
- pipeline cycles
- bus cycles
- memory wait states
- cache-hit latency
- cache-miss latency
- I/O request latency
- DMA arbitration latency
- interrupt entry latency

The implementation records exact published timings where available. Unknown values remain `unspecified` rather than being fabricated.

For modern out-of-order cores, throughput, latency and clock frequency are separate model properties. AMD's Zen 2 documentation, for example, describes changes to execution pipelines, front-end bandwidth, floating-point width and load/store bandwidth independently of clock frequency. citeturn0search14