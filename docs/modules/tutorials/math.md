<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# math: Tutorial

| | |
|---|---|
| Nature | subject (`bin/sleelactl help math`): native C/C++ library compiled into the `sleela` runtime |
| Registry summary | Native math subject (__NativeMath: pi/e/sqrt/exp/log/sin/pow/clamp...); import math; |
| Lives in | `impl/subjects/math/math_native.cpp`, dispatcher in `impl/subjects/native/native_api.cpp` |
| Writeup type | Tutorial, because you write a SLeeLa program that calls `math.*` and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl run math` exits 0 and its output matches `lib/ui/example/science/math.sleela.expected.txt`; `impl/tests/subjects/math_values.sleela` gives 27 PASS, 0 FAIL; a new program with 15 calls was compared with python3. |

## What it is

`math` is the base numeric subject. `import math;` makes qualified calls such as `math.sqrt(2.0)` available. The compiler does not call the C library. `addMath` in [`math_native.cpp`](../../../impl/subjects/math/math_native.cpp) builds a class `__NativeMath` out of ordinary SLeeLa Core methods (loops, series, range reduction), and `lowerProgram` in [`native_api.cpp`](../../../impl/subjects/native/native_api.cpp) rewrites `math.sqrt(x)` into a call to `__native_math_sqrt`. All other quantitative subjects (physics, economics, inference, astrophysics, sociology, chemistry, financial) require `import math;`.

The vocabulary (from the file header and the demo): constants `pi() e() tau()`; unary `abs sign sqrt exp log log10 trunc floor ceil sin cos tan`; binary `pow hypot min max fmod`; ternary `clamp`. Constants are calls, so they need parentheses.

## Before you start

You need the checkout and the prebuilt `impl/build/sleela` (do not rebuild it). Set the environment once:

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
impl/build/sleela version
```

```
SLeeLa™ Language/Runtime 1.3.2-pro (supported source syntax 1.0 – 3.1; native C/C++ Core VM)
```

Use `sleela run` for subject programs. `sleela check` does not load subjects: checking a program that uses `math.sqrt` reports `Semantic error: use of undeclared variable 'math'`.

Keep your own programs outside the repository, for example in `~/sleela-scratch/`. In SLeeLa source, comments are only `<`/`<<` blocks at line start, comments must not contain apostrophes, and `close`, `record`, `native` are reserved identifiers. The programs below need no comments.

## Step 1 - Inspect the module and run the reference demo

```sh
timeout 120 bin/sleelactl info math
timeout 120 bin/sleelactl run math
```

`info` prints the nature and the demo path; `run` executes `lib/ui/example/science/math.sleela`:

```
  ✓ math — Native math subject (...); import math;
  nature: subject — native C/C++ library compiled into the sleela runtime
  reached from SLeeLa as:  import math;   then qualified calls, e.g.  math.<fn>(...)
  reference demo: lib/ui/example/science/math.sleela
...
[constants]
  pi  = 3.14159
  e   = 2.71828
  tau = 6.28319   (2*pi)
[unary]
  abs(-12.5)   = 12.5
  sqrt(144)    = 12
  ...
  sign(-9)     = -1
[binary]
  pow(2, 8)    = 256
  hypot(3, 4)  = 5
  ...
[ternary]
  clamp(400, 50, 300) = 300
...
  ✓ math: ran the native subject demo (its __Native* methods executed)
```

Running the demo directly and diffing with the stored fixture (the first two lines of stdout are the security check):

```sh
timeout 120 impl/build/sleela run lib/ui/example/science/math.sleela 2>/dev/null | tail -n +3 \
  | diff - lib/ui/example/science/math.sleela.expected.txt && echo same
```

```
same
```

## Step 2 - Write your own program

Create `math_tour.sleela`:

```
#sleela 2.5
import math;

class MathTour {
    void main() {
        print("sqrt(2)   = " + math.sqrt(2.0));
        print("pow(2,10) = " + math.pow(2.0, 10.0));
        print("hypot     = " + math.hypot(5.0, 12.0));
        print("clamp     = " + math.clamp(7.5, 0.0, 5.0));
        print("sin(1)    = " + math.sin(1.0));
        print("log(10)   = " + math.log(10.0));
        print("exp(-2.5) = " + math.exp(0.0 - 2.5));
    }
}
```

Negative numbers are written `0.0 - 2.5`, the style the reference demo uses. Run it:

```sh
timeout 120 impl/build/sleela run ~/sleela-scratch/math_tour.sleela 2>/dev/null
```

```
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
sqrt(2)   = 1.41421
pow(2,10) = 1024
hypot     = 13
clamp     = 5
sin(1)    = 0.841471
log(10)   = 2.30259
exp(-2.5) = 0.082085
```

`print` shows six significant digits.

## Step 3 - Check accuracy against python3

To see more digits, subtract a high-precision reference inside the program. `math_residuals.sleela` prints `math.f(x) - <python3 value>` for 15 calls (for example `math.sqrt(2.0) - 1.4142135623730951`):

```
sqrt(2.0) residual = -2.22045e-16
sqrt(1e10) residual = 0
exp(1.5) residual = -8.88178e-16
exp(10) residual = -7.27596e-12
log(10) residual = 0
log(0.01) residual = -8.88178e-16
log10(5000) residual = 0
sin(1) residual = 0
sin(10) residual = -4.44089e-16
cos(2) residual = 1.11022e-16
tan(0.5) residual = 0
pow(2,0.5) residual = 4.44089e-16
pow(7,3) residual = 1.7053e-13
hypot(5,12) residual = 0
fmod(10.5,3) residual = 0
```

The reference values came from python3 (`math.sqrt`, `math.exp`, `math.log`, `math.sin`, ...). The worst relative error is about 5e-16 (`exp(10)` is 2.2e4, residual 7e-12). `sin(1e6)` differs from python3 by 3.7e-11 (absolute).

## Step 4 - Know the edge behaviour

`math_edges.sleela` prints:

```
pow(0,2)     = 1
pow(-2,3)    = 1
sqrt(-4)     = 0
log(0)       = 0
log(-1)      = 0
sin(1e6)-ref = 3.65161e-11
exp(700)     = 1.01423e+304
floor(-3.2)  = -4
ceil(-3.2)   = -3
```

`sqrt` and `log` return 0 for non-positive input (guards in `math_native.cpp`). `pow(x, y)` is built as `exp(y * log(x))`, so for `x <= 0` it computes `exp(0)` and returns 1. python3 gives `0**2 = 0` and `(-2)**3 = -8`. See "Limits" below.

## What you did

You ran the math subject through `sleelactl`, diffed its demo with a fixture, wrote a program that calls `math.*`, and measured the results against python3.

## Limits found

- `math.pow(0.0, 2.0)` returns 1 and `math.pow(0.0 - 2.0, 3.0)` returns 1 (run above). Correct values are 0 and -8. The cause is visible in `math_native.cpp`: `pow` is `exp(y*log(x))` and `log(x <= 0)` returns 0. Every subject that calls `math.pow` inherits this (economics, financial, inference `cagr`).
- Domain errors (`sqrt` of a negative, `log` of 0) return 0 silently instead of failing.
- `sleela check` does not resolve `math.*`; use `run`.

## Next steps

- Reference demo source: [`lib/ui/example/science/math.sleela`](../../../lib/ui/example/science/math.sleela), README: [`lib/ui/example/science/README.md`](../../../lib/ui/example/science/README.md).
- Numeric assertions: [`impl/tests/subjects/math_values.sleela`](../../../impl/tests/subjects/math_values.sleela) (27 checks).
- Subject layout: [`impl/subjects/README.md`](../../../impl/subjects/README.md), [`impl/subjects/native/README.md`](../../../impl/subjects/native/README.md).
- Other subjects that build on math: [physics](physics.md), [economics](economics.md), [inference](inference.md), [astrophysics](astrophysics.md), [sociology](sociology.md), [chemistry](chemistry.md), [financial](financial.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 79), `impl/subjects/math/math_native.cpp`, `impl/subjects/native/native_api.cpp`, `impl/subjects/native/README.md`, `impl/subjects/README.md`, `lib/ui/example/science/math.sleela`, `.../math.sleela.expected.txt`, `impl/tests/subjects/math_values.sleela`, `impl/tests/subjects/run_subject_tests.sh`.

Commands run: `sleelactl info math`, `sleelactl run math`, `sleela run` on the demo (diff against fixture: identical), `run_subject_tests.sh` (math: 27 checks passed), `sleela run` on `math_tour.sleela`, `math_residuals.sleela`, `math_edges.sleela`, `math_ternary.sleela` (`flag ? math.sqrt(16.0) : 0.0` printed 4), `sleela check` on `math_tour.sleela` (undeclared variable 'math'), python3 for the reference values. Scratch programs are in `~/sleela-scratch/`.
