<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# financial: Tutorial

| | |
|---|---|
| Nature | subject (`bin/sleelactl help financial`): native C/C++ library compiled into the `sleela` runtime |
| Registry summary | Native financial subject (valuation and algebra helpers); import financial; |
| Lives in | `impl/subjects/finance/` (`financial_api.cpp` = SLeeLa lowering, `financial.cpp` = C++ reference layer, `FINANCIAL.model`, `README.md`); demo `impl/examples/financial.sleela` |
| Writeup type | Tutorial, because you write a SLeeLa program that calls `financial.*` and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl run financial` exits 0 and prints 8 values (1628.89, 1000, 772.173, 0.75, 10, 0.112, 0.0698, 2); `financial_values.sleela` gives 15 PASS, 0 FAIL; a new program was checked with python3. |

## What it is

`financial` gives SLeeLa programs time-value-of-money, valuation, capital-cost and small algebra functions. [`financial_api.cpp`](../../../impl/subjects/finance/financial_api.cpp) lowers `financial.<fn>(...)` into methods of a synthesized class `__NativeFinancial`; the lowering is separate from the shared math/physics/economics dispatcher. It requires `import math;` and fails otherwise with `financial requires 'import math;' for its numerical kernels`. The README says it is "informed by" QuantLib but contains original SLeeLa source ([`README.md`](../../../impl/subjects/finance/README.md)).

The allowed calls (the `financialAllowed` set):

| Call | Meaning |
|---|---|
| `future_value(p, r, n)`, `present_value(f, r, n)` | `p*(1+r)^n`, `f/(1+r)^n` |
| `annuity_present(p, r, n)`, `annuity_future(p, r, n)` | ordinary annuity; `p*n` when `r == 0` |
| `npv4(r, c1, c2, c3, c4)` | NPV of four cash flows |
| `bond_price(face, coupon, yield, n)` | coupon annuity plus discounted face |
| `capm(rf, beta, rm)` | `rf + beta*(rm - rf)` |
| `wacc(ew, dw, ce, cd, tax)` | `ew*ce + dw*cd*(1-tax)` |
| `determinant2(a, b, c, d)` | `a*d - b*c` |
| `solve2x2_x(a,b,c,d,e,f)`, `solve2x2_y(...)` | Cramer's rule for `ax+by=e`, `cx+dy=f` |
| `quadratic_discriminant`, `quadratic_root_plus`, `quadratic_root_minus` `(a, b, c)` | `b^2-4ac` and the two roots |
| `ratio(n, d)` | `n/d`, 0 if `d == 0` |

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use `sleela run`, not `check`. Keep your own programs outside the repo, for example in `~/sleela-scratch/`. SLeeLa comment rules: only `<`/`<<` blocks at line start, no apostrophes in comments; `close`, `record`, `native` are reserved.

## Step 1 - Run the reference demo

```sh
timeout 120 bin/sleelactl run financial
```

```
  $SLEELA_HOME/impl/build/sleela run impl/examples/financial.sleela
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
1628.89
1000
772.173
0.75
10
0.112
0.0698
2
  ✓ financial: ran the native subject demo (its __Native* methods executed)
```

The eight lines are, in order, `future_value(1000, 0.05, 10)`, `present_value(1628.8946, 0.05, 10)`, `annuity_present(100, 0.05, 10)`, `ratio(75, 100)`, `determinant2(4, 7, 2, 6)`, `capm(0.04, 1.2, 0.10)`, `wacc(0.6, 0.4, 0.09, 0.05, 0.21)`, `quadratic_root_plus(1, -3, 2)`, per [`impl/examples/financial.sleela`](../../../impl/examples/financial.sleela). python3: `1000*1.05**10` = 1628.894626777442, `100*(1-1.05**-10)/0.05` = 772.1734929184818, `4*6-7*2` = 10, `0.04+1.2*0.06` = 0.112, `0.6*0.09+0.4*0.05*0.79` = 0.0698.

The assertion test [`financial_values.sleela`](../../../impl/tests/subjects/financial_values.sleela) has 15 checks; `run_subject_tests.sh` reports `15 checks passed`.

## Step 2 - Write your own program

Create `financial_tour.sleela`:

```
#sleela 2.5
import math;
import financial;

class FinancialTour {
    void main() {
        print("future_value residual = " + (financial.future_value(1000.0, 0.05, 10.0) - 1628.894626777442));
        print("npv4                  = " + financial.npv4(0.10, 100.0, 100.0, 100.0, 100.0));
        print("bond_price (par)      = " + financial.bond_price(1000.0, 0.05, 0.05, 3.0));
        print("bond_price (y=6%)     = " + financial.bond_price(1000.0, 0.05, 0.06, 3.0));
        print("capm                  = " + financial.capm(0.04, 1.2, 0.10));
        print("wacc                  = " + financial.wacc(0.60, 0.40, 0.09, 0.05, 0.21));
        print("solve x               = " + financial.solve2x2_x(2.0, 3.0, 1.0, 2.0, 8.0, 5.0));
        print("solve y               = " + financial.solve2x2_y(2.0, 3.0, 1.0, 2.0, 8.0, 5.0));
        print("roots of x^2-3x+2     = " + financial.quadratic_root_minus(1.0, -3.0, 2.0) + " and " + financial.quadratic_root_plus(1.0, -3.0, 2.0));
    }
}
```

```sh
timeout 120 impl/build/sleela run ~/sleela-scratch/financial_tour.sleela 2>/dev/null
```

```
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
future_value residual = 0
npv4                  = 316.987
bond_price (par)      = 1000
bond_price (y=6%)     = 973.27
capm                  = 0.112
wacc                  = 0.0698
solve x               = 1
solve y               = 2
roots of x^2-3x+2     = 1 and 2
```

python3 reference values: NPV `sum(100/1.1**k, k=1..4)` = 316.98654463492926; bond at 6%: `50*(1-1.06**-3)/0.06 + 1000/1.06**3` = 973.27 (computed term by term: `sum(50/1.06**k) + 1000/1.06**3`); the 2x2 system `2x+3y=8, x+2y=5` has solution x=1, y=2; roots of `x^2-3x+2` are 1 and 2. All agree.

## Step 3 - Try the edge cases

```
#sleela 2.5
import math;
import financial;

class FinancialEdges {
    void main() {
        print("annuity_present r=0   = " + financial.annuity_present(100.0, 0.0, 10.0));
        print("complex roots x^2+1   = " + financial.quadratic_root_plus(1.0, 0.0, 1.0));
        print("singular system x     = " + financial.solve2x2_x(1.0, 2.0, 2.0, 4.0, 3.0, 6.0));
    }
}
```

```
annuity_present r=0   = 1000
complex roots x^2+1   = 0
singular system x     = 0
```

The zero-rate annuity returns `payment*periods` (1000). A negative discriminant gives 0 and a singular system gives 0 instead of an error.

## What you did

You ran the financial demo, wrote a program using valuation, cost-of-capital and algebra functions, and compared the results with python3 and with the repository's own assertions.

## Limits found

- Silent wrong answers: `quadratic_root_plus(1, 0, 1)` returns 0 (the roots of `x^2+1` are complex; `math.sqrt` of a negative returns 0 so the result is `-b/2a`). A singular 2x2 system returns 0.
- Documented functions missing from the SLeeLa side: the C++ layer `financial.cpp` has `bondDuration`, which `financialAllowed` does not list, so `financial.bond_duration(...)` is rejected as unsupported. `financial.cpp` itself is linked into `sleela` but I found no caller; the lowering builds its own methods.
- Calls inside a ternary expression are not lowered: `double t = flag ? financial.ratio(1.0, 4.0) : 0.0;` gives `Semantic error: use of undeclared variable 'financial'`. The lowering handles variable declarations, assignments, expression and print and return statements, if, while, for, blocks and labeled statements, and nested calls, binary and unary expressions; I did not test other statement kinds.
- `bond_price` uses the closed annuity form, so it accepts fractional periods; `financial.cpp` `bondPrice` loops over whole periods only. The closed form divides by `yield`, so `yield = 0` is not handled in the SLeeLa version.
- `FINANCIAL.model` and `impl/nordshrift/IQ_CONSERVATORS.FINANCIAL.model` describe an explanation chain and "IQ Conservator" records (the README says IQ is not a psychometric score). No SLeeLa call exposes them; they are Nordshrift-side data.
- Economics repeats four of these functions ([economics](economics.md)) without the zero-rate guard.

## Next steps

- [`impl/subjects/finance/README.md`](../../../impl/subjects/finance/README.md), [`markdown/FINANCIAL.md`](../../../markdown/FINANCIAL.md) for the integration contract and formulas.
- [`impl/examples/financial.sleela`](../../../impl/examples/financial.sleela) and [`financial_values.sleela`](../../../impl/tests/subjects/financial_values.sleela) as templates.
- Related: [math](math.md), [economics](economics.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 86), everything under `impl/subjects/finance/`, `impl/examples/financial.sleela`, `impl/tests/subjects/financial_values.sleela`, `markdown/FINANCIAL.md`, `impl/subjects/native/native_api.cpp`.

Commands run: `sleelactl info financial`, `sleelactl run financial`, `run_subject_tests.sh` (financial: 15 checks passed), `sleela run` on `financial_tour.sleela`, `financial_edges.sleela`, and a variant with the ternary line (error above), python3 for reference values. Scratch programs: `~/sleela-scratch/`.
