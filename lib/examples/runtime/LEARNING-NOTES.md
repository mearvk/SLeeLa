# Runtime examples — what to learn here

These examples teach SLeeLa's **execution model**: a single thread first, then
the real concurrency intrinsics the runtime provides — `spawn`, a mailbox
(`send`/`recv`), lock slots (`lock`/`unlock`), and `join`. Unlike most other
example folders, these run *actual* threads, yet their results are deterministic
because the arithmetic does not depend on interleaving.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-sequential-accumulate.sleela`](BASIC-sequential-accumulate.sleela) | The baseline: one thread, one loop, one result (the number parallel code must still match). |
| Moderate (`USED`) | [`USED-thread-workers.sleela`](USED-thread-workers.sleela) | Spawn workers, guard a shared counter with a lock, report via a mailbox, and `join`. This folder's moderate step is an established workflow, so it carries the `USED` prefix rather than `MODERATE`. |
| Advanced | [`ADVANCED-producer-consumer.sleela`](ADVANCED-producer-consumer.sleela) | A producer/consumer pipeline: one producer feeds a mailbox, several consumers fold results into a lock-guarded total. |

## What we want you to take away

1. **Know the sequential answer first.** The basic example computes
   `sum 0..999 = 499500`. Any concurrent version that claims to do the same work
   must produce the same number — that is how you catch a race.
2. **Shared state needs a lock.** Both threaded examples update a `static`
   counter from many threads and wrap every update in `lock(0) ... unlock(0)`.
   Without the lock the increments could be lost. Lock *slots* are small integer
   ids (0..31); pick one per independent piece of shared state.
3. **Mailboxes decouple threads.** `send(slot, value)` and `recv(slot)` pass work
   between threads without sharing a variable. The advanced pipeline uses slot 1
   as a work queue: the producer sends items, consumers receive them. `join()`
   in `main` waits for everyone to finish before the totals are printed.
4. **Determinism by design.** The advanced total is the sum of squares 1..12 =
   650 no matter how the four consumers interleave, because addition is
   associative and the lock makes each update atomic. Writing concurrency whose
   *result* is order-independent is a goal worth copying.

## Practical notes for SLeeLa 1.7

- **`spawn(f)` takes a bare, declared, zero-argument method name** — not a call.
  The runtime starts that method on a new thread.
- **`join()` waits for all spawned threads**; call it in `main` before reading
  shared results.
- **Lock slots and mailbox slots are separate namespaces** of small integers.
  The examples use lock slot 0 for the shared counter and mailbox slot 1 for
  messages.
- These are the only examples in this catalogue that use real threads, so run
  them to see the runtime actually schedule work:

```sh
sleela run lib/examples/runtime/ADVANCED-producer-consumer.sleela
```

Output is deterministic: 12 items processed by 4 consumers, total 650.
