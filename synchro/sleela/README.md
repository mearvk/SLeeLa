# SLeeLa Synchro Contract

The Java, C, C++, and Python ports share one packet contract.

Wire packet: 16 bytes: offset 0 = ASCII SYNC magic; offset 4 = uint32
sequence in network byte order; offset 8 = uint64 monotonic send timestamp
in network byte order.

Implementations ignore packets with invalid magic or length below 16 bytes.
RTT is measured locally from a monotonic clock. A timeout is recorded as loss.

Percentiles use nearest-rank semantics. SLA figures are observations:
measured, not guaranteed.

Directories: ../java, ../c, ../cpp, and ../synchro.
