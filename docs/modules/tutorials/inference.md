<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# inference: Tutorial

| | |
|---|---|
| Nature | subject (`bin/sleelactl help inference`): native C/C++ library compiled into the `sleela` runtime |
| Registry summary | Native inference subject (descriptive statistics and trend analysis); import inference; |
| Lives in | `impl/subjects/inference/inference_native.cpp` (dispatcher: `impl/subjects/native/native_api.cpp`) |
| Writeup type | Tutorial, because you write a SLeeLa program that calls `inference.*` on a data series and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl run inference` prints 15 PASS lines and no FAIL; a new program over a 6-point series was checked with python3. |

## What it is

`inference` computes descriptive statistics and a least-squares trend without arrays in the library: your program loops once over its series, accumulates the running sums `n, sumX, sumY, sumXX, sumYY, sumXY`, and passes them to the functions ([`inference_native.cpp`](../../../impl/subjects/inference/inference_native.cpp), header comment). It depends on `math` (`sqrt`, `pow`, `abs`), so `import inference;` needs `import math;`. Results are descriptive; the header says they are not causal claims or official forecasts.

| Call | Meaning |
|---|---|
| `mean(sum, n)` | `sum / n` |
| `variance(sumSq, sum, n)` | population variance `sumSq/n - mean^2` |
| `stdev(sumSq, sum, n)` | `sqrt(variance)` |
| `slope(n, sumX, sumY, sumXX, sumXY)` | OLS slope; 0 if the denominator is 0 |
| `intercept(n, sumX, sumY, slope)` | `(sumY - slope*sumX)/n` |
| `correlation(n, sumX, sumY, sumXX, sumYY, sumXY)` | Pearson r; 0 if the denominator is 0 |
| `r_squared(r)` | `r*r` |
| `zscore(x, mean, stdev)` | `(x-mean)/stdev`; 0 if `stdev == 0` |
| `forecast(intercept, slope, x)` | `intercept + slope*x` |
| `cagr(first, last, periods)` | `(last/first)^(1/periods) - 1`; 0 if `first <= 0` or `periods <= 0` |
| `trend_code(slope, meanY, eps)` | -1 declining, 0 flat, +1 rising (threshold `eps*|meanY|`) |
| `strength_code(absR)` | 0 none (<0.2), 1 weak (<0.4), 2 moderate (<0.7), 3 strong |
| `significance_code(rSquared)` | 0 low (<0.25), 1 modest (<0.5), 2 substantial (<0.8), 3 high |

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use `sleela run`, not `check`. Keep your own programs outside the repo, for example in `~/sleela-scratch/`. SLeeLa comment rules: only `<`/`<<` blocks at line start, no apostrophes in comments; `close`, `record`, `native` are reserved.

## Step 1 - Run the registered demo

The demo [`impl/tests/subjects/inference_values.sleela`](../../../impl/tests/subjects/inference_values.sleela) feeds the reference series X = 1..5, Y = 2,4,..,10 (a perfect line) and prints one PASS or FAIL per function.

```sh
timeout 120 bin/sleelactl run inference
```

```
  ✓ inference — Native inference subject (descriptive statistics and trend analysis); import inference;
  ...
PASS mean
PASS variance
PASS stdev
PASS slope
PASS intercept
PASS correlation
PASS r_squared
PASS zscore
PASS forecast
PASS cagr
PASS trend_rising
PASS trend_flat
PASS strength_strong
PASS strength_none
PASS significance_high
  ✓ inference: ran the native subject demo (its __Native* methods executed)
```

15 PASS lines, matching `EXPECTED_PASS=15` in the file.

## Step 2 - Run statistics on your own series

Create `inference_tour.sleela`. It uses the series Y = 3, 5, 4, 8, 9, 12 at X = 0..5. Arrays are declared with `new double[6]`.

```
#sleela 2.5
import math;
import inference;

class InferenceTour {
    void main() {
        double n = 0.0;
        double sx = 0.0;
        double sy = 0.0;
        double sxx = 0.0;
        double syy = 0.0;
        double sxy = 0.0;
        double[] ys = new double[6];
        ys[0] = 3.0;
        ys[1] = 5.0;
        ys[2] = 4.0;
        ys[3] = 8.0;
        ys[4] = 9.0;
        ys[5] = 12.0;
        int i = 0;
        while (i < 6) {
            double x = i;
            double y = ys[i];
            n = n + 1.0;
            sx = sx + x;
            sy = sy + y;
            sxx = sxx + x * x;
            syy = syy + y * y;
            sxy = sxy + x * y;
            i = i + 1;
        }
        double slope = inference.slope(n, sx, sy, sxx, sxy);
        double icpt = inference.intercept(n, sx, sy, slope);
        double r = inference.correlation(n, sx, sy, sxx, syy, sxy);
        print("mean            = " + inference.mean(sy, n));
        print("stdev (pop)     = " + inference.stdev(syy, sy, n));
        print("slope           = " + slope);
        print("intercept       = " + icpt);
        print("correlation r   = " + r);
        print("r squared       = " + inference.r_squared(r));
        print("forecast(x=6)   = " + inference.forecast(icpt, slope, 6.0));
        print("cagr(3 -> 12,5) = " + inference.cagr(3.0, 12.0, 5.0));
        print("trend_code      = " + inference.trend_code(slope, inference.mean(sy, n), 0.01));
        print("strength_code   = " + inference.strength_code(r));
    }
}
```

```sh
timeout 120 impl/build/sleela run ~/sleela-scratch/inference_tour.sleela 2>/dev/null
```

```
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
mean            = 6.83333
stdev (pop)     = 3.13138
slope           = 1.74286
intercept       = 2.47619
correlation r   = 0.950537
r squared       = 0.903521
forecast(x=6)   = 12.9333
cagr(3 -> 12,5) = 0.319508
trend_code      = 1
strength_code   = 3
```

## Step 3 - Compare with python3

Using python3 with the same series (`statistics.mean`, `statistics.pstdev`, and the sums formulas written out; numpy is not installed, so the closed forms were used):

```
mean 6.833333333333333   pstdev 3.131382371342656
slope 1.7428571428571429  intercept 2.4761904761904763
r 0.950537133290682  r^2 0.9035208417644678
forecast(6) 12.933333333333334   cagr (12/3)**(1/5)-1 = 0.3195079107728942
```

Every printed value agrees with the SLeeLa output to six significant digits. Codes: slope 1.74 is far above `0.01*6.83`, so trend 1 (rising); |r| = 0.95 is at least 0.7, so strength 3.

## What you did

You ran the inference assertions, accumulated sums over a series in SLeeLa, and obtained mean, deviation, regression line, correlation, forecast, growth rate and classification codes, all matched to python3.

## Limits found

- Population variance and standard deviation (divide by `n`), not sample statistics. `sociology_math.c` in the C ABI uses `n-1`; the two subjects differ.
- `trend_code`, `strength_code`, `significance_code` return `int`; in SLeeLa output they print as 1, 3, and so on. The words ("rising", "strong") are left to your program.
- `cagr` uses `math.pow`; see [math](math.md) for the accuracy of that function.
- Fewer than two points or `n == 0` are not guarded in `mean`, `variance`, `stdev` (plain division); I did not run those cases.
- The registered demo prints PASS lines only. The header comment says a SLeeLa layer maps the integer codes to words for a paragraph-form evaluation; that layer is not part of this library and I did not look for it.

## Next steps

- [`inference_values.sleela`](../../../impl/tests/subjects/inference_values.sleela) as a template for your own checks (`check(name, got, expected, tolerance)`).
- Related: [math](math.md), [economics](economics.md), [sociology](sociology.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 85), `impl/subjects/inference/inference_native.cpp`, `impl/subjects/native/native_api.cpp`, `impl/subjects/sociology/sociology_math.c`, `impl/tests/subjects/inference_values.sleela`, `impl/tests/subjects/run_subject_tests.sh`.

Commands run: `sleelactl info inference`, `sleelactl run inference` (15 PASS), `run_subject_tests.sh` (inference: 15 checks passed), `sleela run` on `inference_tour.sleela`, python3 for the reference values. Scratch programs: `~/sleela-scratch/`.
