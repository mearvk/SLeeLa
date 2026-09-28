# Sleela ↔ Java 28 SecureJDK Memory Integration

This directory is the **integration link** that lets a Sleela program run
against the **memory model of the Java 28 SecureJDK** — Model A: Sleela drives
the computation, the Java 28 SecureJDK owns the objects. Sleela holds only
opaque **handles** and drives Java-resident objects over one of two
interchangeable channels:

- **Port (socket):** a Sleela-side native client speaks a small framed protocol
  to a Java 28 SecureJDK **memory server** over TCP loopback.
- **JNI (local feedback):** the same protocol in-process — a native bridge calls
  back into the Java 28 memory model with no socket.

Both channels funnel through one dispatcher
(`SleelaMemoryServer.handleLine`), so they behave identically. The normative
contract is [`spec/JAVA28-MEMORY-INTEGRATION.md`](spec/JAVA28-MEMORY-INTEGRATION.md)
(series **J28-MEM-0001**).

## Layout

```
java28/
  spec/JAVA28-MEMORY-INTEGRATION.md   normative contract (memory model, value + wire, JNI, security)
  src/java/com/mearvk/sleela/java28/
    SleelaMemoryServer.java   the Java 28 SecureJDK memory model + shared dispatcher
    SleelaValue.java          typed value <-> wire encoding (SLValue + object handle)
    Java28PortServer.java     TCP loopback server (port channel)
    Java28JniRuntime.java     JNI facade (local feedback)
    Counter.java, Ledger.java allow-listed demo objects
  src/native/
    sleela_java28.h           Sleela-side C ABI (both channels)
    sleela_java28_port.c      socket client
    sleela_java28_demo.c      driver demo over the port channel
    sleela_java28_bridge.c    JNI bridge (local feedback)
  examples/run_demo.sh        build + run both channels
  Makefile
```

## Build & run

```sh
cd java28
make all          # Java classes, native port driver, JNI shared library
make demo         # runs BOTH channels (port + JNI) and prints results
make demo-port    # port channel only
make demo-jni     # JNI channel only
```

`make` auto-detects the JDK from `javac` on `PATH`. To build against a specific
JDK (e.g. the Java 28 SecureJDK), set `JAVA_HOME`:

```sh
make all JAVA_HOME=/path/to/java28-securejdk
```

See [`examples/EXPECTED_OUTPUT.txt`](examples/EXPECTED_OUTPUT.txt) for the
reference run.

## What the demo shows

Sleela creates `Counter` and `Ledger` objects **on the Java heap**, receives
only handles, and drives them (`add`, `get`, `credit`, `label`). A method that
returns another object (`Ledger.cents()`) hands back a fresh handle. The Java 28
SecureJDK posture is enforced: a non-allow-listed class and a released handle
both return `ERR` rather than an escape hatch, and the live-handle count is
bounded.

## Note on the JDK

"Java 28 SecureJDK" is the target memory host. The reference implementation
builds and runs on the available JDK; the value contract, wire framing, and JNI
signatures are the normative, forward-compatible parts.
