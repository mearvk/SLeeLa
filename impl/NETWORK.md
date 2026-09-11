# Sleela 1.1 Network Layer

Sleela 1.1 adds a deliberately small TCP networking surface implemented directly in the C VM core. Network calls are ordinary Sleela calls at the parser level and lower to runtime opcodes, just like the existing threading built-ins.

## Built-ins

| Built-in | Result | Behavior |
|---|---|---|
| `listen(port)` | `int` | Bind/listen on all IPv4 interfaces; returns a VM-local socket handle or `-1`. Port `0` is allowed for OS-selected ephemeral binding, although the current API does not expose the selected port. |
| `accept(listener)` | `int` | Blocking TCP accept; returns a new VM-local socket handle or `-1`. |
| `connect(host, port)` | `int` | Blocking TCP connection using `getaddrinfo`; returns a VM-local socket handle or `-1`. |
| `sockread(socket)` | `String` | Blocking read of up to 4096 bytes. Returns an empty String on EOF/error. |
| `sockwrite(socket, data)` | `int` | Sends the bytes represented by the Sleela String; returns bytes written or `-1`. |
| `sockclose(socket)` | `null` | Closes and releases the VM-local handle. |

A socket handle is not the operating-system descriptor. It is an index into the VM's bounded socket table (`SL_MAX_SOCKETS`, currently 128). This prevents Sleela programs from depending on process-global descriptor values.

## Concurrency and lifecycle

Each socket slot has its own mutex. Blocking `accept`, `read`, and `write` operations therefore do not hold the VM-wide socket-allocation mutex. The VM joins spawned workers before shutdown, then closes any remaining active sockets and destroys the socket mutexes.

This matches the existing VM design: program bytecode is immutable while running, thread execution state is private, and mutable shared resources have explicit synchronization.

## Versioning

Network built-ins require:

    #sleela 1.1

The supported syntax range is now `1.0 .. 1.1`. A program declaring `1.0` that calls a network built-in is rejected by the compiler, while a `1.1` program may use the network operations.

## Example

    #sleela 1.1
    class EchoServer {
        void main() {
            int listener = listen(8080);
            int client = accept(listener);
            String data = sockread(client);
            sockwrite(client, data);
            sockclose(client);
            sockclose(listener);
        }
    }

## Tests

`make test-network` builds and runs `core/socket_smoke.c`. The smoke test uses a loopback TCP connection and exercises the full opcode path: `listen`, `spawn`, `accept`, `connect`, `sockread`, `sockwrite`, and `sockclose`.

The version test suite also checks that network built-ins are rejected under `#sleela 1.0` and accepted under `#sleela 1.1`.

## Scope

This is intentionally a minimal TCP layer. It does not provide HTTP parsing, TLS, UDP, DNS-specific APIs, nonblocking I/O, event loops, or automatic connection pooling. Higher-level protocol libraries can be built above the six primitive operations.
