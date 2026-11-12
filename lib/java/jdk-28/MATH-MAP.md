# Mapping Java static `Math` / `StrictMath` references to SLeeLa

Rule (PURPOSE.md, row 5): a static `Math` reference is **mapped, not rewritten**.
The translator keeps the call as written and the SLeeLa function of the same name
answers it.

| Java source | SLeeLa source | Implemented by |
|---|---|---|
| `Math.f(args)` | `Math.f(args)` | `jdk-28/java.base/java/lang/Math.sleela` (real statics) |
| `StrictMath.f(args)` | `StrictMath.f(args)` | `StrictMath.sleela`, each static returns `Math.f(args)` |
| `Math.PI`, `Math.E`, `Math.TAU` | same | `static final double` in `Math.sleela` |

A program selects this tier with `import java.jdk-28;` (see
`examples/java/jdk-28/MathRun.sleela`, `StrictMathRun.sleela`,
`MathExactRun.sleela`).

## Member groups

| Group | Members |
|---|---|
| Sign and extremes | `abs`, `absExact`, `max`, `min`, `signum`, `copySign`, `clamp` |
| Rounding | `floor`, `ceil`, `rint`, `round` |
| Roots and powers | `sqrt`, `cbrt`, `hypot`, `pow`, `exp`, `expm1`, `log`, `log10`, `log1p` |
| Trigonometry | `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `atan2`, `sinh`, `cosh`, `tanh`, `toRadians`, `toDegrees` |
| Integer division | `floorDiv`, `floorMod`, `ceilDiv`, `ceilMod`, `divideExact`, `floorDivExact`, `ceilDivExact` |
| Exact arithmetic (64-bit range, `ArithmeticException`) | `addExact`, `subtractExact`, `multiplyExact`, `negateExact`, `incrementExact`, `decrementExact`, `toIntExact`, `powExact` |
| Wide products | `multiplyFull`, `multiplyHigh`, `unsignedMultiplyHigh` |
| Unsigned exact | `unsignedMultiplyExact`, `unsignedPowExact` |
| Floating-point structure | `fma`, `scalb`, `getExponent`, `ulp`, `nextUp`, `nextDown`, `nextAfter`, `IEEEremainder` |
| Random | `random` |

Evidence: `examples/java/differential/MathStatics.{java,sleela}` runs these on a
real JDK and on the SLeeLa VM and the output must be identical
(`examples/java/differential/run-differential.sh`).

## Overload collapse

The VM has one integer kind (64-bit) and one floating kind, so `int`/`long` and
`float`/`double` overloads cannot be told apart at a call site. Overload ties go
to source order, and the **long forms are declared first**, so every `*Exact`
call checks the 64-bit range. A Java program that depends on the 32-bit
`Math.addExact(int, int)` overflow, or on `float` rounding, needs the explicit
wrap (`Math.toIntExact(...)`, PURPOSE.md row 16).

## Why not the native `math` facade

`import math;` offers `math.sqrt`, `math.sin`, `math.pow` and so on
(`impl/subjects/math/math_native.cpp`). It was compared with a real JDK and is
**not** a drop-in for `java.lang.Math`:

| Call | Java | native `math.*` |
|---|---|---|
| `log(0.0)` | `-Infinity` | `0` |
| `pow(0.0, -1.0)` | `Infinity` | `1` |
| `pow(-8.0, 0.3333333333)` | `NaN` | `1` |
| `sqrt(-1.0)` | `NaN` | `0` |
| `trunc`, `sign`, `fmod` | no `Math` member | extra names only |

There is also no `asin`, `acos`, `atan`, `atan2`, `sinh`, `cosh`, `tanh`, `cbrt`,
`rint`, `round` or any `*Exact` member in the facade. The translator therefore
maps to `Math.*` (which keeps the Java special cases) and leaves `math.*` for
code written directly in SLeeLa.
