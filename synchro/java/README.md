# Java Synchro

Java 21 implementation of the SLeeLa Synchro packet format, UDP dispatcher,
latency statistics, and measured SLA reporting.

Compile:

    javac -d build $(find src -name '*.java')

Wire format: SYNC + uint32 sequence + uint64 monotonic send timestamp,
all in network byte order. The implementation reports observations only;
it does not claim a delivery guarantee.
