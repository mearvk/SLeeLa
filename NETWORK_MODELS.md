# SLeeLa — Network Execution Models

## 1. Purpose

SLeeLa now defines two complementary TCP execution models:

1. **Blocking network model** — direct socket operations wait for the requested I/O event. This is the existing `listen`, `accept`, `connect`, `sockread`, `sockwrite`, and `sockclose` model.
2. **Non-blocking execution model** — network work is isolated behind SLeeLa's existing `spawn`/`join` concurrency boundary so the coordinating execution line does not wait on socket I/O. The socket operation itself may remain a blocking POSIX call inside its worker.

This distinction is intentional. It provides an immediately usable asynchronous programming model without pretending that a worker-thread abstraction is the same thing as kernel-level `O_NONBLOCK` or an event loop.

---

## 2. Blocking Model

The blocking model is the primitive TCP model:

```text
SLeeLa instruction
      |
      v
VM network opcode
      |
      v
POSIX socket call
      |
      +--> wait for connection/data/space
      |
      v
result returned to the same execution line
```

### Operations

| Call | Blocking behavior |
|---|---|
| `listen(port)` | Creates the listener; does not wait for a client. |
| `accept(listener)` | Waits until a client connection is available. |
| `connect(host, port)` | Waits for connection establishment/address attempts. |
| `sockread(socket)` | Waits for data or EOF/error. |
| `sockwrite(socket, data)` | Waits according to the operating system's send behavior. |
| `sockclose(socket)` | Releases the socket. |

The principal advantage is simplicity: one source call corresponds directly to one runtime operation. The principal cost is that an execution line can be occupied by I/O.

---

## 3. Non-Blocking Execution Model

The non-blocking model uses SLeeLa's existing bounded pthread runtime as an asynchronous boundary:

```text
                 +--------------------+
                 | coordinator/main   |
                 +---------+----------+
                           |
                       spawn(worker)
                           |
                           v
                 +--------------------+
                 | network worker      |
                 | accept/read/write   |
                 +---------+----------+
                           |
                    mailbox/send
                           |
                           v
                 +--------------------+
                 | coordinator resumes |
                 +--------------------+
```

A coordinator does not execute `accept()` or `sockread()` itself. Instead, a worker owns the blocking network operation and communicates the result through the existing mailbox primitives.

This gives SLeeLa an asynchronous **execution model** while preserving the current VM's small and bounded runtime architecture.

### Model rules

- The coordinator never waits on network I/O directly.
- Each network worker owns the socket operation it performs.
- Results are returned through `send(slot, value)` / `recv(slot)`.
- `join()` remains available for lifecycle completion.
- The VM socket table remains the owner of socket handles.
- The model does not require parser or lexer changes.

---

## 4. Blocking Example

```sleela
#sleela 1.1
class BlockingEcho {
    void main() {
        int listener = listen(8080);
        int client = accept(listener);
        String data = sockread(client);
        sockwrite(client, data);
        sockclose(client);
        sockclose(listener);
    }
}
```

The main execution line owns the complete request lifecycle and therefore waits at `accept()` and `sockread()`.

---

## 5. Non-Blocking Worker Example

```sleela
#sleela 1.1
class AsyncEcho {
    int listener;

    void accept_worker() {
        int client = accept(listener);
        String data = sockread(client);
        sockwrite(client, data);
        sockclose(client);
        send(0, 1);
    }

    void main() {
        listener = listen(8080);
        spawn(accept_worker);

        // The coordinator is free to perform other work here.
        int completed = recv(0);
        print(completed);

        sockclose(listener);
        join();
    }
}
```

The coordinator's network-independent work is not blocked by the worker's `accept()` or `sockread()` operation. The worker carries the blocking system call.

---

## 6. True Kernel-Level Non-Blocking Mode

A separate future model may expose actual POSIX non-blocking sockets:

```text
fcntl(fd, F_SETFL, O_NONBLOCK)
        |
        +--> accept()       -> EAGAIN/EWOULDBLOCK
        +--> connect()      -> EINPROGRESS
        +--> recv()         -> EAGAIN/EWOULDBLOCK
        +--> send()         -> EAGAIN/EWOULDBLOCK
```

That is **not** the same model as the worker abstraction above. It requires explicit readiness/status semantics and, for scalable use, a polling facility such as `poll`, `select`, `epoll`, `kqueue`, or an equivalent host mechanism.

SLeeLa should keep these two concepts separate:

- **Non-blocking execution**: already expressible with `spawn` + mailbox coordination.
- **Non-blocking socket I/O**: a future runtime extension with explicit readiness and error states.

This separation prevents an asynchronous API from silently conflating thread scheduling with kernel socket state.

---

## 7. Resource and Ownership Rules

Both models use the same VM-owned socket table:

- maximum socket handles: `SL_MAX_SOCKETS = 128`
- handles are VM-local indexes
- raw OS descriptors never become language values
- socket operations are synchronized by the socket slot mutex
- VM shutdown closes remaining active descriptors

The non-blocking execution model adds no second socket ownership system.

---

## 8. Design Decision

For SLeeLa 1.1, the recommended separation is:

**Blocking primitives** remain the foundational TCP ABI.

**Non-blocking execution** is expressed through worker threads and mailbox coordination.

**Kernel-level non-blocking sockets** remain a distinct future ABI rather than being simulated through ambiguous return values.

This keeps the runtime deterministic, bounded, and system-centric while leaving a clean path toward a future event-driven network layer.
