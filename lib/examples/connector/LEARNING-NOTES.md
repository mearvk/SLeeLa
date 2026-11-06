# Connector examples — what to learn here

A *connector* is the component that manages a relationship with an external
service: configure it, open it, exchange data, close it. These examples teach
how to make that relationship **correct and resilient** — without ever touching a
real network. Every program is deterministic and has no side effects.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-connector-lifecycle.sleela`](BASIC-connector-lifecycle.sleela) | The lifecycle as an ordered checklist: configure → validate → open → exchange → close. |
| Moderate | [`MODERATE-state-machine.sleela`](MODERATE-state-machine.sleela) | The lifecycle as a guarded state machine: transitions only fire from a legal state, and illegal calls fail closed. |
| Advanced | [`ADVANCED-retry-backoff.sleela`](ADVANCED-retry-backoff.sleela) | Resilience: retry transient failures with exponential backoff and a hard cap. |

The original [`CLASSIC-connector-lifecycle.sleela`](CLASSIC-connector-lifecycle.sleela)
is the shortest starter.

## What we want you to take away

1. **A lifecycle is a contract.** Step order matters. The basic example names the
   steps; the moderate example *enforces* them. If you try to open before
   configuring, or reopen after closing, the connector moves to `FAILED` instead
   of pretending everything is fine.
2. **Model state explicitly.** Integer state constants plus small transition
   methods that each check their precondition is a pattern you will reuse for any
   lifecycle (sockets, files, sessions, hardware). The method names say what they
   do, and each returns the new state so callers can react.
3. **Transient failure is normal; hammering is a bug.** The advanced example
   retries, but each retry waits twice as long as the last (100 → 200 → 400 ms),
   clamped to a ceiling. This protects the remote service and your own process.
   Here the delays are only *simulated and summed* so the run is reproducible.

## Practical notes for SLeeLa 1.7

- **`close` is reserved.** A no-argument method named `close()` collides with a
  built-in runtime handle, so the state machine names its teardown transition
  `requestClose()`. Keep this in mind whenever you model a lifecycle.
- **"Break" without `break`.** The advanced loop ends early by setting the loop
  counter to its limit (`attempt = maxAttempts;`) on success — a clean way to
  stop a `while` in these teaching files.
- **No real I/O.** For an example that *does* open a socket (loopback only), see
  `lib/examples/network/USED-loopback-tcp-echo.sleela`. The connector folder
  stays conceptual so it is safe and deterministic everywhere.

## Run them

```sh
sleela check lib/examples/connector/MODERATE-state-machine.sleela
sleela run   lib/examples/connector/ADVANCED-retry-backoff.sleela
```

Output is deterministic: the state machine ends in `FAILED` after an illegal
re-open, and the retry example connects on attempt 4 after 700 ms of simulated
backoff.
