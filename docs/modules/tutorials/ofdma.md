<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# ofdma: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help ofdma`: verbs `negotiate grade simulate validate info`) |
| Registry summary | OFDMA channelization models |
| Lives in | `ofdma/` (`include/ofdma.h`, `src/`, `cpp/`, `tests/`, `examples/ofdma.conf`, `Makefile`) |
| Writeup type | Tutorial, because you build a small C/C++17 library and program against its C API, and every step below was executed successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `make clean && make && make test` pass, `build/ofdma-demo` runs, `sleelactl info/validate ofdma` work; `negotiate`, `grade`, `simulate` do nothing useful (see Status). |

## What it is

A small, generic OFDMA resource-grid calculator plus a round-robin resource-block scheduler. The README calls it a "simulation/reference model" and says plainly that it is **not** a modem, **not** a conformance-tested 3GPP implementation, and does not do RF, channel coding, synchronization, MIMO, HARQ or standards-complete scheduling (`ofdma/README.md`). The numbers it uses (2048-point FFT, 15 kHz spacing, 12-subcarrier RB, 14 symbols) look like LTE-style values, but `examples/ofdma.conf` says "Generic OFDMA starter profile; not a standards profile". The design is this project's own generic model, not an implementation of a particular standard. The README roadmap lists LTE/NR profiles as future work.

## Before you start

You need `cc` and `c++` (C11 and C++17). No other dependencies. All commands run from the repository root unless stated.

## Step 1 - Build from clean

```sh
cd ofdma
timeout 120 make clean && timeout 120 make
```

Real output:

```
rm -rf build
mkdir -p build
cc -Iinclude -std=c11 -O2 -Wall -Wextra -Wpedantic -c src/ofdma.c -o build/ofdma.o
cc -Iinclude -std=c11 -O2 -Wall -Wextra -Wpedantic -c src/scheduler.c -o build/scheduler.o
g++ -Iinclude -std=c++17 -O2 -Wall -Wextra -Wpedantic -Icpp cpp/ofdma_demo.cpp build/ofdma.o build/scheduler.o -o build/ofdma-demo
cc -Iinclude -std=c11 -O2 -Wall -Wextra -Wpedantic tests/test_ofdma.c build/ofdma.o build/scheduler.o -o build/test-ofdma
```

No warnings. Products go to `ofdma/build/`, which is git-ignored (`**/build/` in `.gitignore`).

## Step 2 - Run the tests

```sh
timeout 60 make test
```

```
./build/test-ofdma
OFDMA tests: PASS
```

`tests/test_ofdma.c` is 16 lines of `assert`s. It checks that the numerology `{2048, 1200, 15000, 14, 144, 12}` validates, that it gives 100 resource blocks and 16800 grid resource elements for 14 symbols, and that `ofdma_schedule_round_robin` splits 6 RBs among requests of 3, 2 and 4 as 2/2/2. Nothing else is tested (no error paths, no capacity limit).

## Step 3 - Run the C++ demo

```sh
timeout 20 ./build/ofdma-demo
```

```
SLeeLa OFDMA reference demo
Resource blocks: 100
Useful symbol: 66.667 us
Nominal sample rate: 30.720 MS/s
Grid resource elements (before pilots/overhead): 16800
```

These follow from `src/ofdma.c`, `ofdma_calculate_metrics`:

| Output | Formula in the code |
|---|---|
| Useful symbol | `1 / subcarrier_spacing_hz` = 1/15000 s |
| Nominal sample rate | `fft_size * subcarrier_spacing_hz` = 2048 x 15000 |
| Resource blocks | `active_subcarriers / resource_block_subcarriers` = 1200/12 |
| Grid resource elements | `active_subcarriers * grid_symbols` = 1200 x 14 |

The `cyclic_prefix_samples` and `symbols_per_slot` fields are only used in validation (`cyclic_prefix_samples < fft_size`, `symbols_per_slot` non-zero). They do not enter any computed metric.

## Step 4 - Understand the C API

`include/ofdma.h` has three functions and four status codes (`OFDMA_OK`, `OFDMA_ERR_ARGUMENT`, `OFDMA_ERR_RANGE`, `OFDMA_ERR_CAPACITY`):

- `ofdma_validate_numerology(n)`: any of FFT size, active subcarriers, spacing, symbols per slot or RB size equal to 0 gives `ARGUMENT`; active subcarriers above the FFT size, or not a multiple of the RB size, or CP samples >= FFT size gives `RANGE`.
- `ofdma_calculate_metrics(n, grid_symbols, &out)`: the table above.
- `ofdma_schedule_round_robin(users, count, available_rbs, &allocated)`: hands out one RB at a time to each user in order until all requests are met or RBs run out. More than `OFDMA_MAX_USERS` (64) users returns `CAPACITY`. It does not weight, prioritize or consider channel quality.

The C++ wrapper `cpp/ofdma.hpp` (`sleela::ofdma::Numerology`) only wraps the first two and throws `std::invalid_argument` on failure; it does not wrap the scheduler.

## Step 5 - Drive it with sleelactl

```sh
bin/sleelactl info ofdma
bin/sleelactl validate ofdma
```

```
  ✓ ofdma — OFDMA channelization models
  location: ofdma
  see: ofdma/README.md
    doc: README.md
...
  make -C ofdma test
./build/test-ofdma
OFDMA tests: PASS
  ✓ ofdma validated (test)
```

`validate` simply runs `make -C ofdma test`. (Run it from the repository root; it needs no `SLEELA_*` variables.)

## What you did

You built and tested a generic OFDMA numerology calculator and a round-robin RB allocator, ran its demo, and saw which parts of the numerology feed the metrics.

## Status and limits (from commands run)

- `bin/sleelactl negotiate ofdma` and `grade ofdma` print "no standalone ... CLI is built" and exit 3. They point to "the negotiate/grade section in ofdma/README.md", but `ofdma/README.md` has no such section (`grep -i "negotiate\|grade"` finds nothing).
- `bin/sleelactl simulate ofdma` prints "ofdma: no simulator registered (sim= unset)" and exits 3.
- `examples/ofdma.conf` is not read by any code: the only reference to the file name in `ofdma/` is the README layout list. The demo and tests hard-code the same numbers.
- `src/scheduler.c` is a comment-only extension point; the round-robin scheduler lives in `src/ofdma.c`.
- Not modelled, as the README states: pilots, guard bands, control channels, coding, channel, MIMO, HARQ.

## Next steps

- [`ofdma/README.md`](../../../ofdma/README.md): scope and the six-step expansion roadmap (LTE/NR profiles first).
- [`wi-fi-6`](../explanations/wi-fi-6.md): the Wi-Fi 6 profile lists OFDMA as a feature of 802.11ax but contains no OFDMA code; this module is the only OFDMA calculation in these five modules.

## Sources

Read: `ofdma/README.md`, `ofdma/Makefile`, `ofdma/examples/ofdma.conf`, `ofdma/include/ofdma.h`, `ofdma/src/ofdma.c`, `ofdma/src/scheduler.c`, `ofdma/cpp/ofdma.hpp`, `ofdma/cpp/ofdma_demo.cpp`, `ofdma/tests/test_ofdma.c`, `bin/sleelactl.d/registry.conf` (line 57), `bin/sleelactl` (protocol verbs, around line 495).
Run: `make clean && make`, `make test`, `./build/ofdma-demo`, `bin/sleelactl info|validate|negotiate|grade|simulate ofdma`.
