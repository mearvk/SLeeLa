# lib/math

The SLeeLa standard-library mathematics package: **243 classes** with real,
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
