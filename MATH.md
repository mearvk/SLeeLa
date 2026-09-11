# MATH.md

## Purpose

`MATH.md` is the primary mathematical reference for the executable Math domain in SLeeLa. It describes the current scalar kernel, its algebraic role, approximation policy, and extension path toward richer mathematical objects.

## Executable Math API

```text
math.pi()
math.e()
math.tau()
math.abs(x)
math.sign(x)
math.min(a,b)
math.max(a,b)
math.clamp(x,lo,hi)
math.sqrt(x)
math.pow(x,y)
math.exp(x)
math.log(x)
math.sin(x)
math.cos(x)
math.tan(x)
math.hypot(a,b)
math.fmod(a,b)
```

These functions are lowered into Sleela Core operations through the native API rather than being documentation-only aliases.

## Approximation Model

- `sqrt` uses a fixed-iteration Newton-style approximation.
- `exp` uses a Taylor-style series.
- `log` uses an atanh-style series.
- `sin` uses a Taylor series with range handling.
- `cos` is represented through the sine relation with a phase shift.
- `pow` uses exponential/logarithmic transformation for its supported positive-base regime.

Results therefore belong to the implemented numerical model and should not be assumed to be unrestricted host `libm` equivalents.

## Mathematical Structure

The mathematical architecture preserves the distinction between subject, quantity, domain, equation, transformation, result, and interpretation. The longer-term target includes linear algebra, complex numbers, calculus, ODEs, numerical approximation, Fourier and Laplace transforms, probability, statistics, optimization, and richer algebraic structures.

Groups, closure, associativity, identity, and inverse remain important semantic concepts for mathematical objects.

## Fourier and Laplace

M-Fouriers and Laplace L's remain first-class transform families in the project design. Future implementations should preserve transform definitions, numerical representations, convergence conditions, and interpretation separately.

## Numerical Contract

A mathematical result is not automatically a physical observation, economic forecast, or empirical fact. The intended bridge is:

**quantity → unit/domain → assumption → equation → transformation → numerical approximation → result → interpretation**.

## Cross-Domain Role

Math is the executable mathematical dependency for the current Physics, Economics, Chemistry, and Financial integrations. Domain adapters use the same native math kernels so that formulas remain inspectable and executable within Sleela Core.

## Design Markers

**7th Order Educated on Planet** and **17th Grade of Education** are project design markers for desired breadth and rigor, not literal academic credentials or psychometric claims. **Permanent enlightenment** means that definitions, invariants, transformations, assumptions, and semantic relationships remain inspectable in the system.

## Example

```sleela
import math;

class Analysis {
    static double magnitude(double x) {
        return math.abs(x);
    }
}
```

## Status

Scalar mathematical execution is implemented. Physics and Economics build directly upon it, while Chemistry and Financial use it through their domain lowerers. Rich transforms, algebraic objects, dimensional typing, and additional numerical kernels remain planned extensions.
