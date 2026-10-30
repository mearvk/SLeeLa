# Networking examples — what to learn here

These examples teach SLeeLa's **socket programming** with the built-in
networking intrinsics (`listen`, `accept`, `connect`, `sockread`, `sockwrite`,
`sockclose`) together with threads (`spawn`/`join`). Two of the three open real
sockets — but **only on `127.0.0.1` (loopback)**, so they never touch an
external network and are safe to run anywhere.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-endpoint-validation.sleela`](BASIC-endpoint-validation.sleela) | The cheap checks that belong *before* a connect: host present, port range, privileged-port policy. No I/O. |
| Moderate | [`USED-loopback-tcp-echo.sleela`](USED-loopback-tcp-echo.sleela) | One real round trip: a server thread accepts, reads, and replies; the client sends and reads back. |
| Advanced | [`ADVANCED-request-response-protocol.sleela`](ADVANCED-request-response-protocol.sleela) | A tiny command protocol (`PING`/`ECHO`/unknown) with the server parsing requests over several round trips. |

## What we want you to take away

1. **Validate before you connect.** The basic example rejects an empty host, an
   out-of-range port, and (by policy) privileged ports below 1024 — all without
   opening a socket. Failing early and cheaply is good networking hygiene.
2. **A server is: listen → accept → read → write → close.** The moderate example
   is the smallest complete version of that loop. The server runs on its own
   thread (`spawn(server)`) while the client drives it from `main`.
3. **A protocol is just an agreed set of request/response strings.** The advanced
   server inspects each request line and chooses a reply, including a clear error
   for unknown commands. Real protocols (HTTP, Redis, SMTP) are this same shape
   with more rules.
4. **Always clean up.** Every example closes its client sockets, and `main`
   closes the listener after `join()`. Leaked sockets are a classic source of
   "address already in use" on the next run.

## Practical notes for SLeeLa 1.7

- **Loopback only.** The examples hard-code `127.0.0.1` and a fixed high port.
  Reaching a physical interface or a remote host is a separate concern the
  networking subsystem README covers.
- **String matching is `==`/`!=` only.** At this language level there is no
  substring or split intrinsic, so the advanced server matches whole request
  lines against the exact commands the client sends. A production parser would
  use the text library for prefix/argument splitting.
- **Threads carry the server.** `spawn(server)` plus `join()` is the same
  pattern as the runtime examples — see `lib/examples/runtime/` for a deeper
  look at concurrency.

## Run them

```sh
sleela run lib/examples/network/USED-loopback-tcp-echo.sleela
sleela run lib/examples/network/ADVANCED-request-response-protocol.sleela
```

Output is deterministic: the echo prints `pong`; the protocol server answers
`PING -> PONG`, `ECHO hello -> hello`, and `BOGUS -> ERR unknown-command`.
