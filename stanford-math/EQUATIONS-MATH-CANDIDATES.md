# Mathematics Canon -- Candidate Additions

> **Status: proposed, not yet entered into the canon.** These 16 foundational
> mathematics formulas were drafted for review as the next batch of additions to
> `SMMathCanon.sleela`. They are **not** part of the compiled canon yet, so they
> are kept here -- deliberately separate from [`EQUATIONS-MATH.md`](EQUATIONS-MATH.md),
> which mirrors the committed 59-entry record exactly. When (and only when) these
> rows are added to `SMMathCanon.sleela`, they move into `EQUATIONS-MATH.md` and
> this file is retired or trimmed. Until then, treat this as a staging list.

The same one hard rule that governs the archive governs every row here: **values
are faithful to the subject's real origin where publicly known**; diffuse or
unknown attributions are marked honestly (`diffuse`, `attrib.`, named-pair), never
invented to look legitimate. Symbolic forms use plain-ASCII notation (`^` power,
`*` product, `sqrt`, `integral`, `sum`, `product`, Greek names spelled out) so they
stay greppable and diff-friendly. The **Why it matters** column states each
result's established significance in one faithful line.

These 16 span algebra, calculus, complex analysis, numerical analysis, linear
algebra, real analysis, probability, number theory, and combinatorics -- all
genuinely foundational and individually citable. None duplicates an entry already
in the 59-entry canon (checked against `SMMathCanon.sleela`).

## The 16 candidate equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Fundamental theorem of algebra | `every degree-n polynomial has exactly n complex roots (with multiplicity)` | algebra | Carl Friedrich Gauss | 1799 | Guarantees the complex numbers are algebraically closed; the reason every polynomial factors completely. |
| 2 | Rolle's theorem | `f(a) = f(b) => exists c in (a,b): f'(c) = 0` | calculus | Michel Rolle | 1691 | The special case that powers the mean value theorem; the first rigorous link between equal values and a flat tangent. |
| 3 | Cauchy-Riemann equations | `u_x = v_y,  u_y = -v_x` | complex analysis | Cauchy; Riemann | 1851 | The exact condition for complex differentiability; the gateway from calculus to analytic-function theory. |
| 4 | Argument principle | `(1/(2*pi*i)) closed_integral f'(z)/f(z) dz = Z - P` | complex analysis | Augustin-Louis Cauchy | 1831 | Counts zeros minus poles inside a contour; the tool behind Rouche's theorem and root-locating. |
| 5 | Lagrange interpolation | `p(x) = sum_j y_j product_{m!=j} (x - x_m)/(x_j - x_m)` | numerical analysis | Lagrange; Waring | 1795 | Builds the unique polynomial through given points; the foundation of polynomial interpolation and quadrature. |
| 6 | Newton's method | `x_{n+1} = x_n - f(x_n)/f'(x_n)` | numerical analysis | Newton; Raphson | 1690 | Finds roots by successive linearization, converging quadratically; the workhorse of numerical root-finding and optimization. |
| 7 | Singular value decomposition | `A = U Sigma V^T` | linear algebra | Beltrami; Jordan | 1873 | Factors any matrix into rotations and scalings; the backbone of least squares, PCA, and low-rank approximation. |
| 8 | Bolzano-Weierstrass theorem | `every bounded sequence in R^n has a convergent subsequence` | analysis | Bolzano; Weierstrass | 1817 | Guarantees limit points of bounded sets; a foundational compactness result underpinning existence proofs. |
| 9 | Banach fixed-point theorem | `a contraction on a complete metric space has a unique fixed point` | analysis | Stefan Banach | 1922 | Turns iteration into existence-and-uniqueness; the engine behind ODE solutions and numerical convergence. |
| 10 | Holder's inequality | `sum \|a_k b_k\| <= (sum \|a_k\|^p)^(1/p) (sum \|b_k\|^q)^(1/q),  1/p + 1/q = 1` | analysis | Otto Holder | 1889 | Generalizes Cauchy-Schwarz to L^p spaces; indispensable across functional analysis and probability bounds. |
| 11 | Minkowski's inequality | `\|\|a + b\|\|_p <= \|\|a\|\|_p + \|\|b\|\|_p` | analysis | Hermann Minkowski | 1896 | The triangle inequality for L^p norms; what makes L^p a normed space. |
| 12 | Weak law of large numbers | `Xbar_n -> mu in probability as n -> inf` | probability | Jacob Bernoulli | 1713 | Sample averages converge to the true mean; the formal reason large samples are reliable. |
| 13 | Markov's inequality | `P(X >= a) <= E[X]/a,  X >= 0, a > 0` | probability | Andrey Markov | 1884 | Bounds a nonnegative variable's tail by its mean alone; the seed of Chebyshev and concentration inequalities. |
| 14 | Mobius inversion | `g(n) = sum_{d\|n} f(d)  <=>  f(n) = sum_{d\|n} mu(n/d) g(d)` | number theory | August Ferdinand Mobius | 1832 | Inverts divisor sums via the Mobius function; a central identity of multiplicative number theory. |
| 15 | Dirichlet's theorem (primes in AP) | `gcd(a,d)=1 => infinitely many primes of the form a + n*d` | number theory | Peter Gustav Lejeune Dirichlet | 1837 | Primes are spread through every valid arithmetic progression; a landmark that launched analytic number theory. |
| 16 | Catalan number formula | `C_n = (1/(n+1)) C(2n, n)` | combinatorics | Euler; Catalan | 1838 | Counts balanced structures (parenthesizations, trees, paths); one of combinatorics' most ubiquitous sequences. |

## How these would be recorded (if accepted)

Each row maps one-to-one to an `add(name, form, field, who, year)` call in
`SMMathCanon.sleela`, exactly like the existing 59 entries. Adding all 16 would
take the mathematics canon to **75 entries** and the whole equation canon from
**210** to **226** (the running total `SMArchive` prints). On acceptance:

1. append the 16 `add(...)` calls to `SMMathCanon.build()` and grow the backing
   array to fit;
2. move these rows into [`EQUATIONS-MATH.md`](EQUATIONS-MATH.md) (renumbered 60-75)
   and bump its header counts;
3. update [`README.md`](README.md) canon totals (210 -> 226; Mathematics 59 -> 75);
4. validate by running `SMArchive` and confirming the printed total.

## Sources & attribution

Names, forms, discoverers, and years follow the standard history-of-science
record; the **Why it matters** notes summarize each result's established
significance. Where a result is credited to a named pair (e.g. Cauchy-Riemann,
Beltrami/Jordan for the SVD, Newton/Raphson), both names are given as the
literature records them. See `SMProvenance.sleela` for the archive's tiered source
registry and `README.md` for per-class integrity goals. This archive records and
attributes published results; it does not originate, own, or certify them.

*Content was rephrased from standard references for compliance with licensing
restrictions.*
