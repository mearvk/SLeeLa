# SLeeLa — NETWORK.md

## Sleela 1.1 Network Layer

The Sleela network layer is a deliberately small, system-centric TCP interface built directly into the Sleela virtual machine. It follows the same architectural rule used by the existing threading operations: **source-level calls are ordinary calls, the compiler lowers recognized names to VM opcodes, and the C core performs the runtime operation against bounded VM-owned state.**

The network layer is therefore not a separate runtime library bolted onto the language. It is part of the language execution model.

---

## 1. Design Ordinance

The network layer is governed by six principles:

1. **Small surface** — expose only the primitives required to establish, accept, read, write, and close TCP connections.
2. **VM ownership** — socket handles belong to the Sleela VM rather than exposing process-global operating-system descriptors directly to Sleela programs.
3. **Bounded resources** — socket resources are kept inside a fixed-size table, consistent with the existing bounded thread, lock, and mailbox facilities.
4. **Thread compatibility** — blocking network operations are compatible with Sleela's existing pthread-based execution model and can be used from spawned workers.
5. **Explicit lifecycle** — sockets are allocated, used, closed, and drained as VM resources; shutdown does not depend on a program remembering every descriptor.
6. **Execution-model separation** — blocking socket primitives and non-blocking execution are distinct concepts; asynchronous execution through workers is not represented as kernel-level `O_NONBLOCK`.

The layer intentionally does **not** attempt to become a complete HTTP, TLS, UDP, asynchronous-event, or connection-pooling framework.

---

## 2. Language Surface

Sleela 1.1 provides six network built-ins:

| Built-in | Result | Purpose |
|---|---|---|
| `listen(port)` | `int` | Create a TCP listener and return a VM-local listener handle. |
| `accept(listener)` | `int` | Block until a connection arrives and return a client handle. |
| `connect(host, port)` | `int` | Resolve and connect to a TCP endpoint. |
| `sockread(socket)` | `String` | Block for a read of up to 4096 bytes. |
| `sockwrite(socket, data)` | `int` | Write the bytes represented by a Sleela String. |
| `sockclose(socket)` | `null` | Close and release a socket handle. |

Failure conventions are intentionally simple:

- `listen()` / `accept()` / `connect()` return `-1` when the operation cannot produce a socket handle.
- `sockwrite()` returns the number of bytes written or `-1` on failure.
- `sockread()` returns a String; EOF/error is represented by the empty String.
- `sockclose()` returns `null` after releasing the handle.

### Example

```sleela
#sleela 1.1
class EchoServer {
    int listener;

    void server() {
        int client = accept(listener);
        String request = sockread(client);
        sockwrite(client, request);
        sockclose(client);
    }

    void main() {
        listener = listen(8080);
        spawn(server);
        join();
        sockclose(listener);
    }
}
```

For a complete loopback example, see [`examples/network_echo.sleela`](examples/network_echo.sleela).

---

## 3. Blocking Network Model

The foundational SLeeLa network model is **blocking**. A network opcode executes the corresponding POSIX socket operation directly on the SLeeLa execution line performing the call.

```text
SLeeLa instruction
      │
      ▼
VM network opcode
      │
      ▼
POSIX socket call
      │
      ├── wait for connection/data/space
      │
      ▼
result returned to the same execution line
```

### Blocking behavior

| Call | Blocking behavior |
|---|---|
| `listen(port)` | Creates the listener; does not wait for a client. |
| `accept(listener)` | Waits until a client connection is available. |
| `connect(host, port)` | Waits for connection establishment/address attempts. |
| `sockread(socket)` | Waits for data or EOF/error. |
| `sockwrite(socket, data)` | May wait according to the operating system's send behavior. |
| `sockclose(socket)` | Releases the socket. |

The blocking model is intentionally direct. It gives the language a small, predictable TCP ABI without requiring an event loop, readiness API, or additional scheduler.

A blocking call occupies the **SLeeLa worker executing that operation**, not necessarily the entire VM. Because SLeeLa already supports pthread-backed `spawn()`, other workers may continue independently.

---

## 4. Non-Blocking Execution Model

SLeeLa also supports a distinct **non-blocking execution model** using its existing bounded worker-thread and mailbox facilities. This model does not make the socket descriptor itself non-blocking. Instead, the potentially blocking operation is moved to a worker so the coordinating execution line remains free to perform other work.

```text
                 +--------------------+
                 | coordinator/main   |
                 +---------+----------+
                           │
                       spawn(worker)
                           │
                           ▼
                 +--------------------+
                 | network worker      |
                 | accept/read/write   |
                 +---------+----------+
                           │
                    mailbox/send
                           │
                           ▼
                 +--------------------+
                 | coordinator resumes |
                 +--------------------+
```

The coordinator therefore does not execute `accept()` or `sockread()` itself. A worker owns the network operation and communicates its result through the existing mailbox primitives.

### Non-blocking execution rules

- The coordinator does not wait on network I/O directly.
- A network worker owns the socket operation it performs.
- Results can be returned through `send(slot, value)` and `recv(slot)`.
- `join()` remains available for worker lifecycle completion.
- The VM socket table remains the sole owner of socket handles.
- No second socket ownership system is introduced.
- No lexer or parser changes are required.

### Non-blocking execution example

```sleela
#sleela 1.1
class AsyncEcho {
    int listener;

    void network_worker() {
        int client = accept(listener);
        if (client >= 0) {
            String data = sockread(client);
            sockwrite(client, data);
            sockclose(client);
            send(0, 1);
        } else {
            send(0, -1);
        }
    }

    void main() {
        listener = listen(8080);
        if (listener < 0) {
            print(-1);
            return;
        }

        spawn(network_worker);

        // The coordinator is not performing socket I/O. The worker owns
        // accept/read/write while the coordinator waits on a mailbox result.
        int result = recv(0);
        print(result);

        sockclose(listener);
        join();
    }
}
```

The worker may block in `accept()` and `sockread()` while the coordinator remains available for other computation. This is **non-blocking execution**, not kernel-level non-blocking socket I/O.

A corresponding example is maintained at [`impl/examples/network_nonblocking.sleela`](impl/examples/network_nonblocking.sleela).

---

## 5. Blocking vs. Non-Blocking Execution

The two models use the same TCP primitives and VM socket ownership, but place the wait in different execution contexts.

| Property | Blocking model | Non-blocking execution model |
|---|---|---|
| Socket API | Six TCP built-ins | Same six built-ins |
| Where I/O runs | Current execution line | Spawned worker |
| `accept()` / `sockread()` | May block caller | May block worker |
| Coordinator availability | Occupied by direct I/O | Remains available |
| Concurrency mechanism | Direct VM operation | `spawn()` + mailbox coordination |
| Kernel socket mode | Blocking | Blocking unless separately changed |
| Event loop required | No | No |
| Additional socket ownership | No | No |

This separation is important. Calling a worker-thread model “non-blocking” means **the coordinator is not blocked by the network operation**. It does not mean that the underlying descriptor has `O_NONBLOCK` set.

---

## 6. True Kernel-Level Non-Blocking I/O

A future SLeeLa runtime model may expose actual POSIX non-blocking sockets:

```text
fcntl(fd, F_SETFL, O_NONBLOCK)
        │
        ├── accept()  → EAGAIN / EWOULDBLOCK
        ├── connect() → EINPROGRESS
        ├── recv()    → EAGAIN / EWOULDBLOCK
        └── send()    → EAGAIN / EWOULDBLOCK
```

That model is intentionally separate from the worker abstraction. True kernel-level non-blocking I/O requires explicit readiness and status semantics and, for scalable multiplexing, a polling facility such as `poll`, `select`, `epoll`, `kqueue`, or an equivalent host mechanism.

The architecture therefore distinguishes three layers:

```text
Layer 1 — Blocking TCP primitives
    listen / accept / connect / sockread / sockwrite / sockclose

Layer 2 — Non-blocking execution
    spawn workers + mailbox coordination

Layer 3 — Future kernel-level non-blocking I/O
    O_NONBLOCK + readiness/polling/event integration
```

Layer 2 is available without changing the six-operation TCP ABI. Layer 3 should be introduced only with explicit readiness/error semantics rather than ambiguous reuse of the existing `-1` and empty-string conventions.

---

## 7. Source-to-VM Pipeline

Network calls deliberately use the normal Sleela call machinery.

```text
.sleela source
    │
    ▼
lexer
    │
    ▼
parser → Call AST
    │
    ▼
Compiler::emitCall()
    │
    ▼
Compiler::tryEmitBuiltin()
    │
    ▼
OP_LISTEN / OP_ACCEPT / OP_CONNECT /
OP_SOCKREAD / OP_SOCKWRITE / OP_SOCKCLOSE
    │
    ▼
C VM dispatch loop
    │
    ▼
VM-owned socket table
    │
    ▼
POSIX TCP socket
```

There are **no network-specific lexer or parser keywords**. A call such as `connect("127.0.0.1", 8080)` is parsed as an ordinary `Call` expression. Recognition happens at the compiler's hardcoded built-in dispatch point.

This keeps the grammar stable while allowing the runtime vocabulary to grow through compiler/core coordination.

---

## 8. Core Opcode Model

The network operations are represented by new `SLOp` values in `core/sleela_core.h`:

- `OP_LISTEN`
- `OP_ACCEPT`
- `OP_CONNECT`
- `OP_SOCKREAD`
- `OP_SOCKWRITE`
- `OP_SOCKCLOSE`

They use the existing `SLInstr { op, a }` instruction representation and the existing bytecode builder / `SLX_EMIT` exchange path. **No new exchange ABI operation is required.**

The important distinction is that socket handles are runtime values. Unlike the lock table, whose lock number is a compile-time literal operand, network handles are produced by `listen()`, `accept()`, and `connect()` and therefore travel through the VM operand stack as `SL_INT` values.

---

## 9. VM Socket State

The C core maintains a bounded socket table in the VM, parallel to the existing shared runtime state for threads, locks, and mailboxes.

A socket slot contains the operating-system descriptor plus lifecycle information and synchronization. Socket handles exposed to Sleela are **VM-local table indexes**, not raw OS descriptors.

The current bound is:

```text
SL_MAX_SOCKETS = 128
```

This has two advantages:

- Sleela code does not depend on Linux/Unix descriptor numbering.
- The VM has a clear resource ceiling that can be reasoned about and tested.

The socket allocation table has its own synchronization. Individual socket operations use the socket slot's mutex so the VM-wide allocation lock is not held across a blocking network operation.

---

## 10. Socket Lifecycle

### `listen(port)`

1. Validate the runtime port value.
2. Create a TCP socket.
3. Bind it to the requested port.
4. Mark it listening.
5. Allocate a free VM socket-table slot.
6. Return the VM-local handle.

Port `0` may be supplied for operating-system-selected ephemeral binding, although the current six-function API does not expose the selected port back to the program.

### `accept(listener)`

`accept()` validates the VM-local listener handle, performs a blocking POSIX `accept()`, allocates a new VM socket-table slot for the resulting client descriptor, and returns that new handle.

### `connect(host, port)`

`connect()` uses normal address resolution (`getaddrinfo`) and establishes a TCP connection. On success the descriptor is placed into the VM socket table and a VM-local handle is returned.

### `sockread(socket)`

A read operation uses a bounded 4096-byte buffer. The resulting bytes become a Sleela String. EOF/error produces the empty String rather than leaking a raw operating-system error representation into the language surface.

### `sockwrite(socket, data)`

The String is converted to its byte representation and sent through the socket. The operation returns the number of bytes reported by the underlying send operation. The implementation also protects against `SIGPIPE` where `MSG_NOSIGNAL` is available, so a broken peer does not terminate the entire VM process merely because a write was attempted.

### `sockclose(socket)`

The descriptor is closed and the table slot is released. The operation is idempotence-conscious at the VM boundary: invalid handles are rejected rather than being treated as arbitrary process descriptors.

### VM shutdown

When the VM is freed, remaining active socket descriptors are drained/closed and socket mutexes are destroyed. This mirrors the lifecycle discipline used for other VM-owned concurrent resources.

---

## 11. Threading Interaction

Sleela already uses pthreads for `spawn()` and a bounded thread table of 128 threads. The network layer intentionally works within that model rather than introducing a second concurrency architecture.

A blocking call such as:

```sleela
int client = accept(listener);
```

blocks the **Sleela worker executing that operation**, just as `recv()` blocks an existing thread/mailbox operation. Other spawned Sleela workers remain independently schedulable by the host pthread implementation.

The non-blocking execution model simply makes this worker boundary explicit:

```text
main/coordinator
 ├── listen()
 ├── spawn(network-worker)
 │     └── accept()
 │          ├── sockread()
 │          ├── sockwrite()
 │          └── sockclose()
 ├── other coordinator work
 └── recv()/join() when coordination is required
```

This makes a natural server pattern possible without requiring a second scheduler.

---

## 12. Why Networking Is a Runtime Built-In

Sleela has another category of built-in operation: the conducted methods (`conduct`, `role`, `insight`, `congruent`, `route`, `sysdepth`, `degreemax`). Those operations resolve against the static `SHEET.sheet` catalog during compilation and generally become constants.

Networking is fundamentally different.

| Property | Conducted method | Network operation |
|---|---|---|
| Primary state | Static catalog | Live OS/VM resource |
| Resolution | Compile time | Runtime |
| Result | Usually constant | Depends on current I/O |
| Lifecycle | None | Allocate/use/close |
| VM opcode | Usually `OP_CONST` | Dedicated `OP_*` |
| Concurrency | Not inherently blocking | May block |

Consequently, network operations belong beside the threading opcodes in the runtime core, **not** inside the catalog-backed conducted-method mechanism.

---

## 13. Compiler Integration

The principal compiler insertion point is `Compiler::tryEmitBuiltin()` in `frontend/compiler.cpp`.

The compiler recognizes these names:

```text
listen
accept
connect
sockread
sockwrite
sockclose
```

Each validates its argument count, emits its runtime expressions, and emits the corresponding network opcode.

The argument strategy is intentionally runtime-oriented:

- `listen(port)` — runtime port value
- `accept(listener)` — runtime listener handle
- `connect(host, port)` — runtime host and port
- `sockread(socket)` — runtime socket handle
- `sockwrite(socket, data)` — runtime socket and data
- `sockclose(socket)` — runtime socket handle

This differs from `lock(n)` and similar operations whose resource identifiers are compile-time literal operands.

There is currently no built-in registration table. The built-in vocabulary is hardcoded in the compiler and mirrored by the VM's opcode switch.

---

## 14. Version Awareness

Network operations are a **Sleela 1.1 feature**.

A source file using the network layer should declare:

```text
#sleela 1.1
```

The supported syntax range now extends through 1.1. Network calls under an explicitly declared `#sleela 1.0` program are rejected by the compiler.

The driver resolves the source syntax version before compilation and passes that version into the compiler so semantic feature gates can be enforced without changing the lexer grammar.

This is an important distinction: the network syntax itself does not require new tokens, but the **meaning of the recognized built-ins is version-gated**.

---

## 15. Testing

The network layer includes both low-level and language-level coverage.

### C-level smoke test

[`core/socket_smoke.c`](core/socket_smoke.c) exercises the core directly through the bytecode-builder interface. It is the low-level analogue of the existing thread smoke test and covers the complete TCP path:

```text
listen → spawn → accept → connect → sockread → sockwrite → sockclose
```

Run it with:

```sh
cd impl
make test-network
```

### Version tests

`tests/version/` contains fixtures for both sides of the feature gate:

- `network_too_early.sleela` — network built-ins under `#sleela 1.0`, expected to fail.
- `network_1_1.sleela` — network built-ins under `#sleela 1.1`, expected to compile/run.

### End-to-end example

[`examples/network_echo.sleela`](examples/network_echo.sleela) demonstrates a real loopback exchange using the Sleela threading model.

### Non-blocking execution example

[`examples/network_nonblocking.sleela`](examples/network_nonblocking.sleela) demonstrates the worker-thread model: a spawned worker performs `accept()` / `sockread()` / `sockwrite()` while the coordinator receives a mailbox completion result.

The intended server/client sequence is:

```text
listener = listen()
        │
        ├── blocking worker → accept() → sockread() → sockwrite() → sockclose()
        │
        └── client → connect() → sockwrite() → sockread() → sockclose()
```

The important test distinction is that the network primitives remain blocking at the socket level while the second example demonstrates non-blocking **execution** at the coordinator level.

---

## 16. Resource and Safety Boundaries

The network layer is deliberately bounded and explicit.

| Resource | Current boundary |
|---|---:|
| Sleela threads | 128 |
| VM socket handles | 128 |
| Socket read buffer | 4096 bytes |
| Network protocol | TCP / IPv4-oriented implementation |
| Socket ownership | VM-local |
| Blocking operations | Allowed |
| Non-blocking execution | `spawn()` + mailbox coordination |
| Kernel `O_NONBLOCK` | Not currently exposed |
| Automatic HTTP parsing | No |
| TLS | No |
| UDP | No |
| Event loop | No |
| Connection pool | No |

These boundaries are implementation constraints, not claims that the underlying operating system has the same limits.

---

## 17. Future Extensions

The six primitives intentionally leave room for higher-level facilities without making the VM overly complicated.

Potential future layers include:

- HTTP request/response helpers implemented above TCP.
- TLS-backed socket variants.
- UDP primitives.
- Explicit socket-address inspection.
- Ephemeral-port discovery for `listen(0)`.
- Configurable read/write limits.
- Timeouts.
- True kernel-level nonblocking mode.
- `poll` / `select` / `epoll` / `kqueue` readiness integration.
- A first-class event loop.
- Higher-level connection objects.
- Connection pooling.

Such extensions should preserve the central rule: **language-level networking remains bounded, explicit, VM-owned, and compatible with the existing concurrency model.**

---

## 18. Implementation Map

For maintainers, the principal source locations are:

| Area | File | Responsibility |
|---|---|---|
| Opcode/value ABI | `core/sleela_core.h` | `SLOp`, `SLValue`, socket bounds, builder/exchange declarations |
| VM runtime | `core/sleela_core.c` | socket table, synchronization, POSIX operations, opcode dispatch, cleanup |
| Compiler | `frontend/compiler.cpp` | network built-in recognition and opcode emission |
| Compiler interface | `frontend/compiler.h` | syntax-version-aware compile interface |
| Driver | `frontend/driver.cpp` | resolve and pass `#sleela` version |
| Version policy | `frontend/version.h` | supported syntax range |
| Core test | `core/socket_smoke.c` | direct runtime/network smoke test |
| Version tests | `tests/version/` | 1.0 rejection / 1.1 acceptance |
| Example | `examples/network_echo.sleela` | end-to-end threaded TCP example |
| Async example | `examples/network_nonblocking.sleela` | non-blocking execution through a spawned worker |
| Build/test | `Makefile` | `test-network` target |

The architecture intentionally requires no changes to `lexer.cpp`, `parser.cpp`, or `ast.h` for the six current network calls.

---

## 19. Status

**Sleela 1.1 TCP Network Layer: implemented.**

The implementation provides a foundational blocking TCP ABI and a non-blocking execution pattern based on the existing pthread/mailbox runtime. The repository includes the compiler integration, VM socket lifecycle, version gating, low-level and language-level tests, blocking and non-blocking examples, and this document.

The distinction is deliberate:

- **Blocking network operations** are the primitive runtime ABI.
- **Non-blocking execution** is achieved by moving those operations into SLeeLa workers.
- **Kernel-level non-blocking sockets** remain a future extension requiring explicit readiness semantics.

Protocol-specific behavior belongs above the TCP primitive layer; resource ownership and primitive socket operations belong inside the VM.
