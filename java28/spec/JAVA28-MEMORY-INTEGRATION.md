# Sleela ↔ Java 28 SecureJDK Memory Integration

**Series:** J28-MEM-0001
**Status:** Normative specification, rev 1.0.0
**Model:** A — *Sleela core drives; the Java 28 SecureJDK owns the object memory.*

## 0. Purpose

This specification defines the **integration link** by which a Sleela program
executes against the **memory model of the Java 28 SecureJDK**. Sleela remains
the driver of control flow and business logic; the durable object state — the
"memory model" — lives on the Java 28 SecureJDK heap. Sleela reaches that memory
over one of two interchangeable channels:

- **Local feedback (JNI):** an in-process shared library bound to the running
  Java 28 SecureJDK via the Java Native Interface. Lowest latency; single
  address space.
- **Port (socket):** a TCP loopback (or remote) connection to a Java 28
  SecureJDK **memory server** that owns the objects. Process-isolated; the same
  logical contract as JNI.

Both channels speak the **same logical operation set** and the **same value
contract** defined below, so a Sleela program is agnostic to which one is in
use.

> **Note on "Java 28 SecureJDK".** Java 28 SecureJDK is the *target* memory
> host. The reference implementation in this directory runs on the available
> JDK toolchain; the contract, framing, and value mapping are the normative
> parts and are forward-compatible with the Java 28 SecureJDK.

## 1. Roles

| Role | Owner | Responsibility |
|---|---|---|
| **Driver** | Sleela (C/C++ core) | Executes logic; issues operations; holds *handles*, never objects. |
| **Memory host** | Java 28 SecureJDK | Owns every object on its heap; assigns handles; enforces the security posture. |
| **Link** | JNI or port | Carries operations and values between driver and memory host. |

The asymmetry is deliberate: Java is **not** a second business-logic
implementation. It is the authoritative object memory that Sleela computes
against.

## 2. Value contract (SLValue ↔ Java 28)

Sleela's core value model is the tagged union `SLValue`
(`SL_NULL, SL_INT, SL_DOUBLE, SL_BOOL, SL_STR`; see
`impl/core/sleela_core.h`). Objects on the Java heap cannot be copied into
Sleela, so they are represented Sleela-side as **opaque handles**.

| Sleela `SLValue` | Wire tag | Java 28 SecureJDK value |
|---|---|---|
| `SL_NULL` | `n` | `null` |
| `SL_INT` (value) | `i` | `long` |
| `SL_DOUBLE` | `d` | `double` |
| `SL_BOOL` | `b` | `boolean` |
| `SL_STR` | `s` | `String` (UTF-8) |
| **object handle** | `h` | a live object in the memory host's handle table |

**Handle rules**

1. A handle is a positive `int64`. `0` is the null handle.
2. Handles are **opaque** to Sleela: it may store, pass, and return them, but it
   may not interpret their bits.
3. A handle is valid only for the session that produced it. Handles are **not**
   globally stable and **must not** be persisted across sessions.
4. The memory host owns the object's lifetime. Sleela releases interest with
   `free`; the host may reclaim only released or unreachable objects.

## 3. Operation set

Every operation is a request from the driver to the memory host and yields one
typed reply. The logical operations are identical on both channels:

| Op | Request | Reply |
|---|---|---|
| `hello` | protocol/version handshake | server identity + protocol version |
| `new` | class name, ctor args (values/handles) | object **handle** |
| `call` | target handle, method name, args | value or handle |
| `get` | target handle, field name | value or handle |
| `set` | target handle, field name, value | `ok` |
| `free` | handle | `ok` |
| `describe` | handle | class name + field/method summary |
| `stats` | — | live handle count, high-water mark |
| `bye` | — | closes the session |

A reply is either **OK** (carrying one value per §2) or **ERR** (carrying a
message). A transport-level success never implies business success: the reply
status is authoritative.

## 4. Port channel — wire protocol

Framed, UTF-8, newline-delimited request/response over a stream socket
(TCP loopback by default). Each **request** is one line:

```
OP arg1 arg2 ... argN
```

Tokens are space-separated. Any token that is a value uses the **typed value
encoding**:

```
n                     null
i:<int64>             integer
d:<double>            double
b:0 | b:1             boolean
s:<len>:<utf8-bytes>  string (length-prefixed; may contain spaces)
h:<int64>             object handle
```

Bare identifiers (class names, method names, field names) are sent as plain
tokens. Each **response** is one line:

```
OK <value>            success, one typed value (n if void)
ERR <s:len:msg>       failure, message as a typed string
```

Example session (loopback):

```
> hello i:1
< OK s:20:Java28 SecureJDK mem
> new Counter i:10
< OK h:1
> call h:1 add i:5
< OK i:15
> get h:1 value
< OK i:15
> free h:1
< OK n
> bye
< OK n
```

The default endpoint is `127.0.0.1:<port>`; the port is chosen by the host and
reported to the driver. Length-prefixed strings make the framing robust to
spaces and to binary-safe UTF-8.

## 5. JNI channel — local feedback

The JNI channel exposes the **same operation set** through a single native
entry point bound to the memory host in-process:

```c
/* One request line in, one response line out — identical grammar to §4. */
const char *sl_java28_call(void *session, const char *request_line);
```

- The C side (`java28/src/native/sleela_java28_bridge.c`) marshals the request
  line into a JNI call on the Java `SleelaMemoryServer`, which returns the
  response line. This reuses the §4 grammar verbatim, so the two channels are
  behaviorally identical.
- The Java facade `Java28JniRuntime` loads `libsleela_java28` and calls the
  native `dispatch(String)` method; the native `Java_..._dispatch` implements
  it by invoking the shared `SleelaMemoryServer.handleLine`.
- Because both channels funnel through `SleelaMemoryServer.handleLine`, the
  object memory model and semantics are guaranteed to match.

## 6. Security posture (Java 28 SecureJDK)

The Java 28 SecureJDK memory host enforces:

1. **No arbitrary classes.** `new`/`call` are restricted to an explicit
   **allow-list** registered by the host (`registerClass`). An unknown class or
   method is an `ERR`, never a reflective escape hatch.
2. **Handle isolation.** A driver can only touch objects via handles it was
   given in the same session; forged or stale handles yield `ERR`.
3. **No DTD / no code loading over the wire.** The port protocol carries values
   and names only — never bytecode, class files, or serialized objects.
4. **Bounded memory.** The host enforces a maximum live-handle count; exceeding
   it fails `new` rather than exhausting the heap.

These mirror the SecureJDK posture already used by the `.xclass` loader
(reject-DTD, structure-not-bytecode, advisory→enforced grades).

## 7. Reference layout

```
java28/
  spec/JAVA28-MEMORY-INTEGRATION.md   this document (J28-MEM-0001)
  src/native/
    sleela_java28.h                   C ABI: session + call for both channels
    sleela_java28_port.c              port (socket) client used by Sleela
    sleela_java28_bridge.c            JNI bridge (local feedback)
    sleela_java28_demo.c              driver demo exercising the port channel
  src/java/com/mearvk/sleela/java28/
    SleelaMemoryServer.java           the Java 28 SecureJDK memory model + dispatch
    SleelaValue.java                  typed value + wire encode/decode (§2, §4)
    Java28PortServer.java             TCP loopback server (port channel)
    Java28JniRuntime.java             JNI facade (local feedback)
    Counter.java, Ledger.java         allow-listed demo objects
  examples/                           runnable demo scripts + expected output
```

## 8. Governing principle

> Sleela owns the computation; the Java 28 SecureJDK owns the memory. The link
> is a value-and-handle contract — identical over JNI and port — that lets a
> Sleela program run against Java-resident objects without either side
> becoming a copy of the other.
