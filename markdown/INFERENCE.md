# SLeeLa Inference

The **inference** subject library is a native (C/C++-synthesized) statistics layer
for evaluating national / economic **data series** from SLeeLa. A program feeds a
series through it and gets back an evaluation in two forms:

- **Mathematical** — mean, standard deviation, an ordinary-least-squares trend
  (slope / intercept), correlation and R², compound growth (CAGR), a z-score,
  and a next-period forecast.
- **Paragraph** — a plain-language reading of those numbers (direction, strength,
  confidence), produced by mapping the library's classification codes to words in
  the SLeeLa layer, with an explicit assumptions caveat.

It is the inferencing counterpart to the `economics` library: `economics` states
identities and relationships, `inference` evaluates observed series against them.

> **Discipline (Part B).** Every output is a *descriptive statistic over the
> supplied data and stated assumptions* — not a causal claim and not an official
> forecast (`ASSOCIATION ≠ CAUSATION`). Interpretation is kept separate from the
> arithmetic: the numbers come from the library, the wording from a thin mapping
> layer you can read.

---

## 1. How it is built

`inference` is a native module like `math`, `physics`, and `economics`: the
compiler synthesizes the `__NativeInference` class from ordinary SLeeLa Core
methods (see [`impl/subjects/inference/inference_native.cpp`](impl/subjects/inference/inference_native.cpp)),
and qualified calls `inference.fn(...)` lower to `__native_inference_fn`. The
library **depends on `math`** (it uses `sqrt`, `pow`, `abs`), so a program must
`import math;` alongside `import inference;`.

The functions are **array-free**: SLeeLa has no arrays, so a series is summarised
by running sums that a program accumulates in one pass. This is the standard
sufficient-statistics form for regression and correlation, and it composes
cleanly with SLeeLa `struct`s (see [`STRUCTS.md`](STRUCTS.md)).

## 2. The running-sum contract

For a series of points `(X_i, Y_i)` — typically `X` = period index, `Y` = the
observed quantity — accumulate:

| Sum | Meaning |
|---|---|
| `n`     | number of observations |
| `sumX`  | Σ X |
| `sumY`  | Σ Y |
| `sumXX` | Σ X² |
| `sumYY` | Σ Y² |
| `sumXY` | Σ X·Y |

## 3. API

All arguments and results are `double` unless noted; classification helpers
return `int` codes.

| Call | Result |
|---|---|
| `inference.mean(sum, n)` | arithmetic mean `sum / n` |
| `inference.variance(sumSq, sum, n)` | population variance `sumSq/n − mean²` |
| `inference.stdev(sumSq, sum, n)` | population standard deviation |
| `inference.slope(n, sumX, sumY, sumXX, sumXY)` | OLS trend slope |
| `inference.intercept(n, sumX, sumY, slope)` | OLS trend intercept |
| `inference.correlation(n, sumX, sumY, sumXX, sumYY, sumXY)` | Pearson `r` |
| `inference.r_squared(r)` | coefficient of determination `r²` |
| `inference.zscore(x, mean, stdev)` | standard score of `x` |
| `inference.forecast(intercept, slope, x)` | trend value at `x` |
| `inference.cagr(first, last, periods)` | compound growth per period |
| `inference.trend_code(slope, meanY, eps)` | `−1` declining · `0` flat · `1` rising |
| `inference.strength_code(absR)` | `0` none · `1` weak · `2` moderate · `3` strong |
| `inference.significance_code(rSquared)` | `0` low · `1` modest · `2` substantial · `3` high |

`trend_code` treats a slope within `eps · |meanY|` per period as flat.
Degenerate inputs (zero denominators, non-positive CAGR base) return `0` rather
than trapping.

## 4. Usage pattern

Model each observation and the series as structs, fold the observations into the
running sums in one pass, then evaluate:

```sleela
#sleela 1.2
import math;
import inference;

struct Obs    { int period; double value; }
struct Series { String name; double n; double sumX; double sumY;
                double sumXX; double sumYY; double sumXY; double first; double last; }

class Demo {
    void ingest(Series s, Obs o) {
        double x = o.period; double y = o.value;
        s.n = s.n + 1.0; s.sumX = s.sumX + x; s.sumY = s.sumY + y;
        s.sumXX = s.sumXX + x*x; s.sumYY = s.sumYY + y*y; s.sumXY = s.sumXY + x*y;
        if (s.n == 1.0) { s.first = y; }
        s.last = y;
    }
    void main() {
        Series g = new Series(); g.name = "Real GDP index";
        Obs a = new Obs(); a.period = 1; a.value = 100.0; ingest(g, a);
        Obs b = new Obs(); b.period = 2; b.value = 102.1; ingest(g, b);
        // ... more observations ...
        double slope = inference.slope(g.n, g.sumX, g.sumY, g.sumXX, g.sumXY);
        double r     = inference.correlation(g.n, g.sumX, g.sumY, g.sumXX, g.sumYY, g.sumXY);
        print("slope=" + slope + " r=" + r);
    }
}
```

(`ingest` mutates the caller's `Series` because structs are reference types.)

## 5. The runnable model

[`inference-model/national_series.sleela`](inference-model/national_series.sleela)
is a complete national/economic inference model. It ingests two illustrative
series (a rising real-GDP index and a declining unemployment rate), then prints,
for each, a `[mathematical]` block and an `[evaluation]` paragraph. Run it from
the repository root with the verified-build manifest configured:

```sh
export SLEELA_SHA256_MANIFEST=security/sha256-manifest.json
export SLEELA_SHEET=SHEET.sheet
impl/build/sleela run inference-model/national_series.sleela
```

Example (GDP series, abridged):

```
=== Inference over: Real GDP index (index) ===
[mathematical]
  trend slope:      2.10857 per period
  correlation r:    0.99816
  r^2:              0.996324
  forecast(t=7):    112.647
[evaluation]
  Over 6 periods, Real GDP index shows a strong rising trend (slope 2.10857
  index per period, r^2 0.996324), with high statistical fit to a straight line.
  ...
```

These figures are reproducible and were checked against an independent
computation; the input data is illustrative.

## 6. Implementation notes

| Layer | What |
|---|---|
| Native library | [`impl/subjects/inference/inference_native.cpp`](impl/subjects/inference/inference_native.cpp) synthesizes `__NativeInference`. |
| Dispatcher | [`impl/subjects/native/native_api.cpp`](impl/subjects/native/native_api.cpp) registers the `inference` module, enforces the `math` dependency, and lowers `inference.fn` → `__native_inference_fn`. |
| Build | `impl/Makefile` compiles `build/inference_native.o` and links it into `sleela`/`nordshrift`; `make test-inference` builds it standalone. |
| Requires | `#sleela 1.2` when combined with structs (the recommended ingest pattern); the `inference` calls themselves need only `import math; import inference;`. |

*Inference is additive and version-independent at the call level; the struct-based
ingest pattern requires syntax 1.2. A computed statistic is not a measurement of
the world — cite the data and state the assumptions.*
