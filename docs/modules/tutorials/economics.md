<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# economics: Tutorial

| | |
|---|---|
| Nature | subject (`bin/sleelactl help economics`): native C/C++ library compiled into the `sleela` runtime |
| Registry summary | Native economics subject (economic and time-value functions); import economics; |
| Lives in | `impl/subjects/economics/economics_native.cpp` (dispatcher: `impl/subjects/native/native_api.cpp`) |
| Writeup type | Tutorial, because you write a SLeeLa program that calls `economics.*` and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl run economics` prints 12 PASS lines (first line of the program output included); `run_subject_tests.sh` reports 12 checks passed; a new program was checked with python3. |

## What it is

`economics` builds `__NativeEconomics` from SLeeLa Core methods ([`economics_native.cpp`](../../../impl/subjects/economics/economics_native.cpp)). Its header says results are models, not empirical forecasts. It has 12 functions and depends on `math` (`pow`, `log`, `exp`), so `import economics;` needs `import math;`.

| Call | Formula in the code |
|---|---|
| `future_value(principal, rate, periods)` | `P * (1+r)^n` |
| `present_value(future, rate, periods)` | `F / (1+r)^n` |
| `annuity_present(payment, rate, periods)` | `P * (1 - (1+r)^-n) / r` |
| `annuity_future(payment, rate, periods)` | `P * ((1+r)^n - 1) / r` |
| `elasticity(pct_quantity, pct_price)` | `pct_quantity / pct_price` |
| `real_rate(nominal, inflation)` | `(1+nominal)/(1+inflation) - 1` |
| `fisher_nominal(real, inflation)` | `(1+real)*(1+inflation) - 1` |
| `doubling_time(rate)` | `ln 2 / ln(1+rate)` |
| `continuous_value(principal, rate, time)` | `P * e^(r*t)` |
| `profit(revenue, cost)` | `revenue - cost` |
| `margin(profit, revenue)` | `profit / revenue` |
| `gdp_identity(C, I, G)` | `C + I + G` |

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use `sleela run`, not `check`. Keep your own programs outside the repo, for example in `~/sleela-scratch/`. SLeeLa comment rules: only `<`/`<<` blocks at line start, no apostrophes in comments; `close`, `record`, `native` are reserved.

## Step 1 - Run the registered demo

The economics demo is a test program, [`impl/tests/subjects/economics_values.sleela`](../../../impl/tests/subjects/economics_values.sleela). It computes known values and prints `PASS <name>` or `FAIL <name> got=... expected=...`.

```sh
timeout 120 bin/sleelactl run economics
```

```
  ✓ economics — Native economics subject (economic and time-value functions); import economics;
  ...
PASS future_value
PASS present_value
PASS annuity_present
PASS annuity_future
PASS elasticity
PASS real_rate
PASS fisher_nominal
PASS doubling_time
PASS continuous_value
PASS profit
PASS margin
PASS gdp_identity
  ✓ economics: ran the native subject demo (its __Native* methods executed)
```

Twelve PASS, no FAIL. (If you cut the output with `sed`, remember the first PASS line follows the security lines.) The test's own comments give the expected numbers, e.g. `future_value(1000, 0.05, 10)` = 1628.894626777442 within 0.01.

## Step 2 - Write your own program

Create `economics_tour.sleela`:

```
#sleela 2.5
import math;
import economics;

class EconomicsTour {
    void main() {
        print("future_value residual   = " + (economics.future_value(2500.0, 0.04, 8.0) - 3421.4226260131845));
        print("present_value           = " + economics.present_value(10000.0, 0.06, 5.0));
        print("annuity_present         = " + economics.annuity_present(500.0, 0.03, 12.0));
        print("real_rate               = " + economics.real_rate(0.08, 0.03));
        print("doubling_time(0.07)     = " + economics.doubling_time(0.07));
        print("margin(200, 500)        = " + economics.margin(200.0, 500.0));
        print("gdp_identity(700,200,100) = " + economics.gdp_identity(700.0, 200.0, 100.0));
    }
}
```

```sh
timeout 120 impl/build/sleela run ~/sleela-scratch/economics_tour.sleela 2>/dev/null
```

```
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
future_value residual   = 9.09495e-13
present_value           = 7472.58
annuity_present         = 4977
real_rate               = 0.0485437
doubling_time(0.07)     = 10.2448
margin(200, 500)        = 0.4
gdp_identity(700,200,100) = 1000
```

python3 reference values: `2500*1.04**8` = 3421.4226260131845 (residual 9e-13, relative 3e-16), `10000/1.06**5` = 7472.58172866057, `500*(1-1/1.03**12)/0.03` = 4977.001996783785, `1.08/1.03-1` = 0.04854368932038833, `log(2)/log(1.07)` = 10.244768351058712. All agree.

## What you did

You ran the economics assertions through `sleelactl`, then used time-value, rate and accounting functions from your own program and compared them with python3.

## Limits found

- No zero-rate guard. `economics.annuity_present(100.0, 0.0, 10.0)` and `annuity_future(100.0, 0.0, 10.0)` both print `-nan`. The `financial` subject has the same two functions with an `r == 0` branch (returns `payment*periods`), see [financial](financial.md).
- `economics.margin(1.0, 0.0)` prints `inf` (plain division, no guard).
- The functions use `math.pow`, which is wrong for a zero or negative base ([math](math.md)). That does not matter for `1+rate > 0`.
- The registered "demo" prints PASS lines, not a worked example. There is no `economics.sleela` under `lib/ui/example/science/`.

## Next steps

- Assertions source: [`economics_values.sleela`](../../../impl/tests/subjects/economics_values.sleela); subject layout: [`impl/subjects/README.md`](../../../impl/subjects/README.md).
- Related: [financial](financial.md) (overlapping time-value functions), [inference](inference.md), [math](math.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 84), `impl/subjects/economics/economics_native.cpp`, `impl/subjects/native/native_api.cpp`, `impl/subjects/native/README.md`, `impl/tests/subjects/economics_values.sleela`, `impl/tests/subjects/run_subject_tests.sh`.

Commands run: `sleelactl info economics`, `sleelactl run economics`, `run_subject_tests.sh` (economics: 12 checks passed), `sleela run` on `economics_tour.sleela` and `economics_edges.sleela`, python3 for reference values. Scratch programs: `~/sleela-scratch/`.
