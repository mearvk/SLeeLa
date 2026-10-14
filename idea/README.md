# Idea System(TM) — native core (C / C++)

*From Author; distributive.*

Special native code the Standard SLeeLa VM may need to implement that is not
originally Sleela. The Sleela "Sleela runnable" lives in
[`../lib/idea/`](../lib/idea/); this directory is its C/C++ engine.

## Files

| File | Role |
|------|------|
| `idea_system.h`   | Public **C ABI** (`extern "C"`): System/State model, signal input, Series engine, organization scheme. |
| `idea_system.c`   | C implementation of the core. No network I/O (the fetcher is pluggable). |
| `idea_system.hpp` | **C++ facade**: RAII `idea::System` + `idea::SeriesRunner` driver. |
| `idea_main.cpp`   | Reference driver demonstrating the full narrative end to end. |
| `Makefile`        | Build the static C core and the demo driver. |

## Build & run

```sh
make        # build the demo driver -> build/idea-system
make lib    # build just the static C core -> build/libideasystem.a
make run    # build and run the demo
make clean
```

Builds clean under `-std=c11` / `-std=c++17` with `-Wall -Wextra -Wpedantic`.

## Model

- **System / State.** A `idea_system` presents exactly one `idea_state_kind` at
  a time (`LOADING → RECEIVING → SOLVING/DECIDING/LOOPING → FINAL`). The State
  *is* the System; callers observe only what `idea_present()` reports and what
  `idea_is_capable()` admits — not all is present/capable in every State.
- **Signal input.** `idea_receive_signal()` pulls an Author-controlled hex code
  from the known internet resource through a registered `idea_fetch_fn`;
  `idea_feed_hex()` feeds one directly. The decoder tolerates whitespace and
  `0x` prefixes.
- **Series.** `idea_build_series()` sizes a forward, linear Series from the
  signal seed, bounded by design goals (`IDEA_MAX_HOURS` = 30,
  `IDEA_MAX_DECISIONS` = 9, depth ≤ `IDEA_MAX_SERIES_DEPTH` = 3024). Law is
  careful: bounds are clamped, not rejected.
- **Time.** `idea_tick(delta_seconds)` is the single forward maintenance step;
  Time never reverses. A zero-delta tick *seems* to loop (`IDEA_STATE_LOOPING`)
  but stays consistent. `IDEA_EXHAUSTED` marks Finality.
- **Organization.** A `idea_scheme` (Concepts / Finality / Signal / Society) and
  up to 9 AI objectives organize the System.

The C core is the single source of truth; the C++ facade and the Sleela native
bridge both link against it.
