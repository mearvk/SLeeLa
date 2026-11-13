<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# sociology: Tutorial

| | |
|---|---|
| Nature | subject (`bin/sleelactl help sociology`): native C/C++ library compiled into the `sleela` runtime |
| Registry summary | Native sociology subject (population and social-statistical measures); import sociology; |
| Lives in | `impl/subjects/sociology/` (`sociology_native.cpp` = SLeeLa side; `sociology_math.c`, `sociology.cpp` = C ABI and C++ facade); docs in `api/sleela/2.1/subjects/sociology/` |
| Writeup type | Tutorial, because you write a SLeeLa program that calls `sociology.*` and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl run sociology` exits 0 and matches `sociology.sleela.expected.txt`; demo numbers and a new program checked with python3; the C smoke test was compiled in a scratch dir and passes. |

## What it is

Sociology is a descriptive-statistics subject: population measures, not causal claims (the demo prints "ASSOCIATION != CAUSATION"). From SLeeLa it offers four functions ([`sociology_native.cpp`](../../../impl/subjects/sociology/sociology_native.cpp)):

| Call | Formula |
|---|---|
| `proportion(part, total)` | `part / total` |
| `rate(events, population, scale)` | `events / population * scale` |
| `growth_rate(oldValue, newValue)` | `(new - old) / old` |
| `transition_probability(transitions, origin)` | `transitions / origin` |

The C ABI in `sociology_math.c` has 11 functions (adds `mean`, `variance`, `weighted_mean`, `risk_ratio`, `odds_ratio`, `gini`, `dissimilarity`). The README says the native frontend "contains a bounded first vocabulary", which matches the four above. `import sociology;` requires `import math;`.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use `sleela run`, not `check`. Keep your own programs outside the repo, for example in `~/sleela-scratch/`. SLeeLa comment rules: only `<`/`<<` blocks at line start, no apostrophes in comments; `close`, `record`, `native` are reserved.

## Step 1 - Run the reference demo

```sh
timeout 120 bin/sleelactl run sociology
```

```
  ✓ sociology — Native sociology subject (population and social-statistical measures); import sociology;
  ...
All figures are OBSERVED (BLS / Census). ASSOCIATION != CAUSATION.
[proportion]
  labor force as a proportion of population = 0.491537
[rate]  per 100,000
  unemployed per 100,000 of the labor force = 4200
[growth_rate]
  unemployment rate 3.8 -> 4.2 percent      = 0.105263
  median household income 80610 -> 83730    = 0.0387049
[transition_probability]
  transition_probability(30 of 100)         = 0.3
[density]  population / land area (persons per km^2)
  US population density                     = 37.3641
...
  ✓ sociology: ran the native subject demo (its __Native* methods executed)
```

A direct run of `lib/ui/example/science/sociology.sleela` matches `sociology.sleela.expected.txt` (diff empty). python3: `168e6/341784857` = 0.49153728, `(4.2-3.8)/3.8` = 0.10526316, `(83730-80610)/80610` = 0.03870488, `341784857/9147420` = 37.364072. The demo's "density" line is just `proportion(population, land_area)`; there is no separate density function.

## Step 2 - Write your own program

Create `sociology_tour.sleela`:

```
#sleela 2.5
import math;
import sociology;

class SociologyTour {
    void main() {
        print("proportion(25, 100)         = " + sociology.proportion(25.0, 100.0));
        print("rate(5 events, 2000, per 1000) = " + sociology.rate(5.0, 2000.0, 1000.0));
        print("growth_rate(100 -> 110)     = " + sociology.growth_rate(100.0, 110.0));
        print("transition_probability(30, 100) = " + sociology.transition_probability(30.0, 100.0));
        print("proportion(1, 0)            = " + sociology.proportion(1.0, 0.0));
    }
}
```

```sh
timeout 120 impl/build/sleela run ~/sleela-scratch/sociology_tour.sleela 2>/dev/null
```

```
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
proportion(25, 100)         = 0.25
rate(5 events, 2000, per 1000) = 2.5
growth_rate(100 -> 110)     = 0.1
transition_probability(30, 100) = 0.3
proportion(1, 0)            = inf
```

python3: `25/100` = 0.25, `5/2000*1000` = 2.5, `(110-100)/100` = 0.1, `30/100` = 0.3. The last line shows what a zero total does (see Limits).

## Step 3 - Run the C kernels (optional)

`make test-sociology` would build into `impl/build/`, which must stay untouched, so compile the same sources into a scratch directory:

```sh
cd ~/sleela-scratch && mkdir -p build
S=$SLEELA_HOME/impl
gcc -O1 -I$S/subjects/sociology $S/tests/subjects/sociology_smoke.c \
    $S/subjects/sociology/sociology_math.c -o build/soc_smoke -lm
./build/soc_smoke
```

```
sociology: PASS
```

## What you did

You ran the sociology demo, confirmed the figures with python3, used all four SLeeLa functions with your own numbers, and ran the C smoke test.

## Limits found

- Only 4 of the 11 C functions are reachable from `.sleela`. `sociology.gini(1.0, 2.0)` fails with `sleelvac: Semantic error: call to unknown method '__native_sociology_gini'` (the C `gini` also takes an array and a length, which the SLeeLa side has no binding for).
- Zero denominators are handled differently on the two sides. The C kernels return 0 (`sleela_soc_proportion(p, 0)` is 0, per `sociology_math.c`); the SLeeLa version divides directly and gives `inf` (run above).
- `api/sleela/2.1/subjects/sociology/README.md` states that the library is descriptive and that a statistic does not establish causation; the functions are plain arithmetic and I found no code that enforces this; it is documentation.
- The demo's figures are hard-coded in the source (for example `laborForce = 168000000.0`, marked approximate in a comment). I did not re-verify them against BLS or Census.

## Next steps

- Docs: [`api/sleela/2.1/subjects/sociology/README.md`](../../../api/sleela/2.1/subjects/sociology/README.md) plus `model.xml`, `observation.xml`, `procedure.xml`, `EXCELLENT.md` in that folder (not parsed by any step here).
- Demo source: [`lib/ui/example/science/sociology.sleela`](../../../lib/ui/example/science/sociology.sleela).
- Related: [math](math.md), [inference](inference.md) for statistics on series.

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 83), everything under `impl/subjects/sociology/`, `impl/subjects/native/native_api.cpp`, `impl/Makefile` (sociology targets), `impl/tests/subjects/sociology_smoke.c`, `api/sleela/2.1/subjects/sociology/README.md`, `lib/ui/example/science/sociology.sleela` and `.sleela.expected.txt`.

Commands run: `sleelactl info sociology`, `sleelactl run sociology`, direct `sleela run` plus fixture diff (identical), `sleela run` on `sociology_tour.sleela` and `sociology_gini.sleela` (error above), the gcc build and run in Step 3, python3 for reference values.
