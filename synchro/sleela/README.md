# SLeeLa Synchro Contract

The Java, C, C++, and Python ports share one packet contract.

Wire packet: 16 bytes: offset 0 = ASCII SYNC magic; offset 4 = uint32
sequence in network byte order; offset 8 = uint64 monotonic send timestamp
in network byte order.

Implementations ignore packets with invalid magic or length below 16 bytes.
RTT is measured locally from a monotonic clock. A timeout is recorded as loss.

Percentiles use nearest-rank semantics. SLA figures are observations:
measured, not guaranteed.

## Language integration layers

Each implementation now exposes a host/VM integration boundary with the same lifecycle: initialize, prepare a probe, validate/record an acknowledgement, record timeout, and read statistics.

- Java: `../java/.../SynchroIntegration.java` delegates transport and SLA work to the Java implementation.
- C: `../c/synchro_integration.h/.c` is the stable C ABI suitable for a native VM boundary.
- C++: `../cpp/synchro_integration.hpp/.cpp` provides an RAII wrapper over the C ABI.

The C ABI is the native interoperability boundary; C++ does not duplicate the wire or statistics implementation. Java keeps its native-independent integration path.

Directories:
- ../java — Java 21 implementation.
- ../c — C11 packet/statistics core.
- ../cpp — C++17 wrapper.
- ../synchro — existing Python implementation.
