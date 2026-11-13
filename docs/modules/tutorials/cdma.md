<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# cdma: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help cdma`: verbs `negotiate grade simulate validate info`) |
| Registry summary | CDMA reference models + simulator (cdma-sim) |
| Lives in | `cdma/` (`common/`, `is-95-cdmaone/`, `cdma2000-1xrtt/`, `cdma2000-evdo/`, `wcdma-umts/`, `tools/`); SLeeLa-language counterpart in `lib/cdma/` |
| Writeup type | Tutorial, because you build a small C simulator and run it, and every step below was executed successfully in this checkout (one documented step, `make -C cdma/build test`, fails and is therefore not a tutorial step; see Status below). |
| Verified | 2026-11-13 against `master` e048707fd: `make -C cdma/build all` builds `cdma/tools/cdma-sim`; four per-standard `make test` targets pass; `bin/sleelactl simulate cdma ...` runs the simulator. |

## What it is

`cdma/` is a set of compact, deterministic C11/C++17 reference models for direct-sequence CDMA:
Walsh and OVSF channelization codes, PN sequences, spreading and correlation, a shared multi-user
channel, plus a link layer with AWGN, a simple multipath model, a RAKE combiner, a power-control
step and Monte-Carlo bit-error-rate (BER) measurement. A command-line driver, `cdma-sim`, ties
these together ([`cdma/README.md`](../../../cdma/README.md), [`cdma/STACK.md`](../../../cdma/STACK.md)).

**What is real standard and what is this project's own design.** The module's own files say the
named standards (IS-95/cdmaOne, CDMA2000 1xRTT, CDMA2000 1x EV-DO, W-CDMA/UMTS) are "design
references, not a claim of interoperability or full conformance" (`cdma/README.md`). The code
borrows published parameters (chip rates 1.2288 and 3.84 Mcps, Walsh-64, OVSF spreading factors
4 to 512) but everything else (the integer chip-domain model, the simulator CLI, the
exponentially-decaying multipath profile, the open-loop equal-power users) is this project's own
construction. There is no channel coding, no MAC/RLC/RRC/NAS, no RF or fixed-point DSP
(`cdma/LEGAL-AND-SCOPE.md` section 3, `cdma/STACK.md` section 5).

**Legal scope (from `cdma/LEGAL-AND-SCOPE.md`).** The code is MIT-licensed. The MIT grant covers
source copyright only. It does not grant CDMA patent rights (SEP/FRAND), FCC equipment
authorization or carrier certification. It is education / golden-model software and must not be
represented as a conforming or interoperable air-interface implementation or as modem firmware.
This tutorial only runs a simulation on your computer; it transmits nothing.

## Before you start

- Linux shell, `cc` (C11) and `make`. The Makefiles use `-Werror`, so a very different compiler
  version may reject warnings that gcc here accepted.
- Run everything from the repository root (`SLeeLa/`).
- Optional, only for step 6: the environment used by `sleelactl`:

```sh
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

## Step 1 - Build everything

```sh
timeout 300 make -C cdma/build all
```

This runs each sub-module's `syntax` check (compile with `-fsyntax-only`) and then builds the
simulator. Abridged real output:

```
make[1]: Entering directory '.../cdma/common/build'
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -I../include -fsyntax-only ../src/cdma_common.c
...
make[1]: Entering directory '.../cdma/tools'
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -I../common/include cdma_sim.c ../common/src/cdma_common.c ../common/src/cdma_link.c -o cdma-sim -lm
```

Build product: `cdma/tools/cdma-sim`, produced by `cdma/tools/Makefile` (`cc -std=c11 ... cdma_sim.c ../common/src/cdma_common.c ../common/src/cdma_link.c -o cdma-sim -lm`), which `make -C cdma/build all` calls; `make -C cdma/tools` builds it alone. A second pass rebuilt it from a clean copy of `cdma/` with the same result. It is not covered by `.gitignore`, so `git status` shows it as untracked.

## Step 2 - Ask the simulator what it accepts

```sh
./cdma/tools/cdma-sim --help
```

```
Usage: ./cdma/tools/cdma-sim --std <name> [options]

  --std <name>     is95 | 1xrtt | evdo | wcdma
  --sf <n>         spreading factor (power of two; default: std max)
  --users <n>      active users 1..sf (default 2)
  --ebn0 <dB>      Eb/N0 in dB (default 8.0)
  --paths <k>      multipath resolvable paths 1..8 (default 1)
  --trials <n>     Monte-Carlo trials (default 20000)
  --seed <n>       PRNG seed (default 1)
  --help           show this help

Standards:
  is95    IS-95 cdmaOne, Walsh-64 (max SF 64)
  1xrtt   CDMA2000 1xRTT, Walsh spreading (max SF 128)
  evdo    CDMA2000 1x EV-DO, Walsh spreading (max SF 512)
  wcdma   W-CDMA / UMTS, OVSF SF 4-512 (max SF 512)
```

## Step 3 - A clean link: no errors

```sh
./cdma/tools/cdma-sim --std is95 --users 1 --ebn0 20 --trials 20000
```

```
cdma-sim  standard=is95  SF=64  users=1  Eb/N0=20.0 dB  paths=1  trials=20000
          per-chip noise sigma=0.5657   reference-user BER=0.00000  (no errors in this run)
```

At 20 dB the noise per chip is small relative to the 64-chip correlation gain, so user 0's
symbol is recovered every time.

## Step 4 - Orthogonal codes: more users, almost the same BER

Same noise, W-CDMA, spreading factor 64, Eb/N0 = 2 dB, flat channel:

```sh
for u in 1 32 64; do ./cdma/tools/cdma-sim --std wcdma --sf 64 --users $u --ebn0 2 --trials 20000; done
```

```
... users=1   ... reference-user BER=0.03745  (Monte-Carlo estimate)
... users=32  ... reference-user BER=0.03705  (Monte-Carlo estimate)
... users=64  ... reference-user BER=0.03645  (Monte-Carlo estimate)
```

All three BERs agree within Monte-Carlo scatter. This is the property `cdma/STACK.md` section 3
describes: on a synchronous flat channel the Walsh/OVSF codes are mutually orthogonal, so
interferers cancel and noise alone sets the BER.

## Step 5 - Reproducibility, then multipath and the RAKE

The model uses its own xorshift128+ PRNG, so equal seeds give equal results:

```sh
./cdma/tools/cdma-sim --std wcdma --sf 64 --users 32 --ebn0 2 --trials 20000 --seed 2   # BER=0.03575
./cdma/tools/cdma-sim --std wcdma --sf 64 --users 32 --ebn0 2 --trials 20000             # BER=0.03705 (seed 1, run twice: identical)
./cdma/tools/cdma-sim --std wcdma --sf 64 --users 32 --ebn0 2 --trials 20000 --paths 3   # BER=0.00035
```

With `--paths 3` the BER fell from 0.03705 to 0.00035. Read `cdma/tools/cdma_sim.c` before you
interpret that number: the tool builds the profile as one unit-gain path plus extra taps of gain
`0.6/k` at delay `k`, and `cdma/common/src/cdma_link.c` contains no step that normalizes the total
path energy (a `grep` for "norm"/"energy" finds only comments). So part of the improvement is simply
more received energy, not only RAKE diversity. This is an observation from reading the code, not a
documented statement.

Bad input is rejected with exit code 2, for example `--std is95 --sf 128`:

```
error: --sf must be a power of two in [2, 64] for is95
```

## Step 6 - Run each standard's own test

```sh
for d in is-95-cdmaone cdma2000-1xrtt cdma2000-evdo wcdma-umts; do make -C cdma/$d/build test; done
```

Last lines of each (real output):

```
IS-95 cdmaOne: shared-channel recovery PASS (+1, -1)
CDMA2000 1xRTT: shared-channel recovery PASS (+1, -1)
CDMA2000 1x EV-DO: shared-channel recovery PASS (+1, -1)\nmake: Leaving directory ...
W-CDMA / UMTS: shared-channel recovery PASS (+1, -1)
```

(The literal `\n` in the EV-DO line is real: its `test.c` prints a backslash-n instead of a newline.)
Each test spreads `+1` and `-1` for two users on different codes, sums them and recovers both.
Build products land in `cdma/*/build/` (`cdma-test`, `cdma-common-test`), which `.gitignore` covers.

## Step 7 - Run it through `sleelactl`

```sh
bin/sleelactl simulate cdma --std wcdma --sf 64 --users 16 --ebn0 6 --paths 3 --trials 20000
```

```
  cdma/tools/cdma-sim --std wcdma --sf 64 --users 16 --ebn0 6 --paths 3 --trials 20000
cdma-sim  standard=wcdma  SF=64  users=16  Eb/N0=6.0 dB  paths=3  trials=20000
          per-chip noise sigma=2.8351   reference-user BER=0.00000  (no errors in this run)
```

`sleelactl simulate cdma` only forwards its arguments to the registered `sim=cdma/tools/cdma-sim`.
With no arguments the simulator itself reports `error: --std is required` and the verb exits 2.
Before step 1, the verb prints `simulator cdma/tools/cdma-sim not built; build it with make -C cdma/tools`
and exits 3.

## What you did

You built the CDMA reference models and the `cdma-sim` driver, measured reference-user BER for
IS-95 and W-CDMA settings, saw orthogonal-code behavior and deterministic seeding, ran the four
per-standard tests, and reached the simulator through `sleelactl`.

## Status and limits

- **Broken documented command.** `make -C cdma/build test` (given in `cdma/README.md` and
  `cdma/STACK.md`) fails: `cdma/common/build/Makefile` compiles `test_link.c`, which does not exist.

  ```
  cc ... ../src/cdma_common.c ../src/cdma_link.c test_link.c -o cdma-link-test -lm
  cc1: fatal error: test_link.c: No such file or directory
  make[1]: *** [Makefile:17: test] Error 1
  ```

  The aggregate runs `common` first with `set -e`, so the four per-standard tests never run through
  it; run them directly as in step 6. The link layer (PRNG, AWGN, RAKE, power control, BER) has no
  unit test in this checkout; step 3 to 5 only exercise it through `cdma-sim`.
- **README claims that the code does not show.** `cdma2000-evdo/README.md` says "slot and DRC tier
  model", but `evdo.h` only defines the constants `EVDO_SLOT_CHIPS`, `EVDO_DRC_MIN`, `EVDO_DRC_MAX`
  and three spread/recover functions; no function uses a slot or DRC tier. `wcdma-umts/README.md`
  mentions "PN-based scrambling concepts" and `is-95-cdmaone/README.md` "PN concepts", but
  `wcdma.c` and `is95.c` call no PN function; `cdma_pn_sequence` exists only in `common/` and is
  tested for tap validation only. `CDMA2000_1X_MIN_SF` (4) is defined but `cdma2000_1x_make_code`
  accepts spreading factors from 2.
- **Verbs.** `bin/sleelactl validate cdma` prints "no validate/syntax/test target" (exit 3) because
  there is no `cdma/Makefile`. `negotiate` and `grade` print a generic message pointing at a README
  section that does not exist in `cdma/README.md`, and exit 3.

## Next steps

- [`cdma/README.md`](../../../cdma/README.md), [`cdma/STACK.md`](../../../cdma/STACK.md),
  [`cdma/LEGAL-AND-SCOPE.md`](../../../cdma/LEGAL-AND-SCOPE.md).
- Per-standard notes: `cdma/is-95-cdmaone/README.md`, `cdma/cdma2000-1xrtt/README.md`,
  `cdma/cdma2000-evdo/README.md`, `cdma/wcdma-umts/README.md`, `cdma/common/README.md`.
- The SLeeLa-language counterpart: [`lib/cdma/README.md`](../../../lib/cdma/README.md). It was not run
  for this writeup.

## Sources

Files read: `cdma/README.md`, `cdma/STACK.md`, `cdma/LEGAL-AND-SCOPE.md`, `cdma/*/README.md`,
`cdma/build/Makefile`, `cdma/common/build/Makefile`, `cdma/*/build/Makefile`, `cdma/*/build/test.c`,
`cdma/common/include/cdma_common.h`, `cdma/common/include/cdma_link.h`, `cdma/common/src/cdma_common.c`,
`cdma/common/src/cdma_link.c`, `cdma/tools/cdma_sim.c`, `cdma/tools/Makefile`, `cdma/*/src/*.c`,
`cdma/*/include/*.h`, `lib/cdma/README.md`, `bin/sleelactl.d/registry.conf`, `bin/sleelactl`
(protocol verb section).

Commands run: `make -C cdma/build all test`; `make -C cdma/<std>/build test` for the four standards;
`cdma/tools/cdma-sim` with the arguments shown above plus invalid arguments;
`bin/sleelactl info|validate|negotiate|grade|simulate cdma`.
