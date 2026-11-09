# lib/math

The SLeeLa standard-library mathematics package: **244 classes** with real,
compiler-verified implementations spanning numeric types through advanced
numerical methods. Every class carries the Metadata Class record (see
`.kiro/steering/metadata-class-standard.md` and `SLEELA.syntax` §15).

The package facade is `SLPackage.sleela`; a runnable tour is in
`examples/MathDemo.sleela`.

## Domains

| Area | Representative classes |
|---|---|
| Arithmetic & number theory | `SLGcd`, `SLLcm`, `SLFactorial`, `SLFibonacci`, `SLPrime`, `SLPrimeFactorization`, `SLDivisor`, `SLModulo`, `SLPower`, `SLBinomialCoefficient` |
| Elementary functions | `SLExponential`, `SLLogarithm`, `SLRoot`, `SLSine`, `SLCosine`, `SLTangent`, `SLTrigonometry` |
| Statistics & probability | `SLMean`, `SLVariance`, `SLStandardDeviation`, `SLMedian`, `SLPercentile`, `SLCovariance`, `SLCorrelation`, `SLSkewness`, `SLKurtosis`, `SLZScore`, `SLBayes`, `SLHistogram` |
| Distributions | `SLNormalDistribution`, `SLUniformDistribution`, `SLBinomialDistribution`, `SLPoissonDistribution` |
| Regression & tests | `SLLinearRegression`, `SLPolynomialRegression`, `SLLeastSquares`, `SLHypothesisTest`, `SLTTest`, `SLChiSquareTest`, `SLANOVA`, `SLConfidenceInterval` |
| Linear algebra | `SLVector2/3/4`, `SLVector`, `SLMatrix`, `SLMatrixOperations`, `SLDeterminant`, `SLInverse`, `SLTranspose`, `SLEigenvalue`, `SLEigenvector`, `SLConditionNumber` |
| Calculus & numerics | `SLDerivative`, `SLIntegral`, `SLNumericalIntegration`, `SLSimpsonRule`, `SLTrapezoidRule`, `SLNewtonMethod`, `SLSecantMethod`, `SLBisectionSolver`, `SLInterpolation`, `SLLagrangeInterpolation` |
| ODE / PDE | `SLODESolver`, `SLRungeKutta`, `SLOrdinaryDifferentialEquation`, `SLPDE` |
| Transforms & signal | `SLDiscreteFourier`, `SLFastFourier`, `SLFourier`, `SLFourierSeries`, `SLLaplace`, `SLZTransform`, `SLSignal` |
| Optimization | `SLGradientDescent`, `SLCoordinateDescent`, `SLNewtonOptimization`, `SLLinearProgram`, `SLQuadraticProgram`, `SLObjective`, `SLConstraint`, `SLRegularization` |
| Sequences, series, combinatorics | `SLSequence`, `SLProgression`, `SLSeries`, `SLRecurrence`, `SLCombinatorics`, `SLCombination`, `SLPermutation`, `SLCombinationGenerator` |
| Set theory & relations | `SLSetTheory`, `SLSetOperations`, `SLUnion`, `SLIntersection`, `SLFiniteSet`, `SLCardinality`, `SLRelation` |
| Geometry & transforms | `SLGeometry`, `SLPoint`, `SLLine`, `SLDistance`, `SLPolarCoordinate`, `SLRotation`, `SLTransform`, `SLQuaternion`, `SLBezier`, `SLCubicSpline` |
| Number types | `SLComplex`, `SLRational`, `SLBigInteger`, `SLBigDecimal`, `SLFixedPoint`, `SLNumber`, `SLNumeric` |
| Expressions & symbolic | `SLExpression`, `SLExpressionTree`, `SLExpressionEvaluator`, `SLFormula`, `SLFormulaParser`, `SLSymbolicMath`, `SLLambda`, `SLPredicate` |
| Fields | `SLScalarField`, `SLVectorField`, `SLGradientField`, `SLGradient` |
| Numeric infrastructure | `SLPrecision`, `SLMachineEpsilon`, `SLOverflow`, `SLUnderflow`, `SLNumericFormatter`, `SLNumericParser`, `SLMathContext`, `SLMathEngine` |
| Units & constants | `SLUnit`, `SLUnitConverter`, `SLQuantity`, `SLMetric`, `SLConstant`, `SLPhysicalConstant` |
| Graphs & tensors | `SLGraph`, `SLGraphNode`, `SLGraphEdge`, `SLAdjacencyMatrix`, `SLTensor`, `SLMarkovChain` |

## Implementation notes

- Pure SLeeLa (`#sleela 2.5`). No native builtins are assumed: `sqrt`, `exp`,
  `ln`, `sin`/`cos`, and `nth-root` are implemented inline (Newton iteration /
  range-reduced Taylor series).
- Matrices and tensors are flat row-major `double[]` arrays with an explicit
  shape, which keeps them within the language's array support.
- Functions that are inherently "bring your own f(x)" take sampled data
  (paired `xs[]`/`ys[]`) or caller-supplied evaluated slopes, since the SLeeLa
  front end has no first-class function values.
- Three class names (`SLNumber`, `SLFunction`, `SLOptimizationProblem`) also
  exist in other `/lib` packages (`core`, `deliberation`); refer to the math
  versions through a versioned import when a program pulls in several packages.

All implementations were validated with the source-built compiler
(`impl/build/sleela run`) against known values.


## Native kernel boundary

The source-level `SLVector`, `SLMatrix`, and `SLComplex` classes remain the language-facing mathematical objects and preserve pure-SLeeLa fallback implementations. The native numerical-structure kernel lives under `impl/subjects/math/linear_algebra.{h,cpp}` behind a stable C ABI. `OP_MATH_CALL` (append-only opcode 179) and the typed Core array bridge expose native vector, matrix, complex, factorization, spectral, and Fourier operations to `#sleela 3.0` programs. Native built-ins use VM arrays and compile-time literal dimensions; output arrays must be preallocated to the documented shape. Programs that do not use these built-ins continue to use the pure SLeeLa methods.


## Native numerical coverage

The language-facing types are extended with vector subtraction/normalization/cross product, matrix multiplication/transpose/determinant, and complex subtraction/division. The native C ABI and compiler built-ins now cover vector add/subtract/scale/dot/norm/normalize/cross3; matrix multiply/transpose/inverse; complex add/subtract/multiply/divide; thin QR; symmetric eigendecomposition; and radix-2 FFT. Additional low-level array and scalar kernels remain available through the C ABI.

Native entry points are in impl/subjects/math/linear_algebra.h. The Core exposes slvm_math_dispatch_arrays to marshal SL_ARRAY handles to temporary numeric buffers while holding the array mutex, invoke the C ABI, and write results back. The helper does not retain VM memory. The current integration links the kernel into runtime executables and the reusable Core archive. The direct VM bytecode smoke test also serializes and reloads an artifact containing `OP_MATH_CALL`, so the append-only opcode is checked through the artifact validator and loader. Run `make -C impl test-linear-algebra test-linear-algebra-vm test-linear-algebra-bytecode` after building to exercise the C ABI, source-to-runtime path, and bytecode/artifact path. These targets have not yet been executed in this environment.


## Unified native facade

`SLNativeMathEngine.sleela` is the SLeeLa-facing facade for the native kernels. It groups vector, matrix, complex, QR, symmetric-eigen, and FFT operations behind one class while preserving the existing `SLVector`, `SLMatrix`, `SLComplex`, and `SLMathEngine` implementations as pure-SLeeLa/Core fallbacks. It requires `#sleela 3.0`. Arrays remain caller-owned; output arrays may be freshly allocated and are written only over the result shape, leaving any extra capacity unchanged.

The native facade is an explicit acceleration API, not an implicit change to the semantics of existing math classes. This keeps older source programs and artifacts on their established Core implementation unless they opt into the native interface.
