# Mathematics Canon

> 75 equations, sourced from `SMMathCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

The foundational mathematics every graduate curriculum rests on -- identities, theorems, and inequalities from geometry through analysis, number theory, probability, and set theory. Each row is a faithful record: the result under the name history gives it, with the rightful discoverer and year, plus a one-line note on why it matters. Where attribution is genuinely diffuse (e.g. the quadratic formula, the 2x2 determinant) the discoverer field says so, rather than crediting a single name.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `integral`,
`sum`, `product`, `laplacian`, Greek names spelled out) so they stay greppable and
diff-friendly. The **Why it matters** column states each result's significance in one
faithful line.

## The 75 equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Pythagorean theorem | `a^2 + b^2 = c^2` | geometry | Pythagoras (attrib.) | c. 530 BC | The defining relation of Euclidean distance; the seed of trigonometry and metric geometry. |
| 2 | Euler's identity | `e^(i*pi) + 1 = 0` | analysis | Leonhard Euler | 1748 | Ties the five fundamental constants in one equation; the canonical example of mathematical beauty. |
| 3 | Euler's formula | `e^(i*x) = cos(x) + i*sin(x)` | analysis | Leonhard Euler | 1748 | Unifies exponential and trigonometric functions; the backbone of Fourier and signal analysis. |
| 4 | Fundamental theorem of calculus | `integral_a^b f'(x) dx = f(b) - f(a)` | calculus | Newton & Leibniz | c. 1670 | Links differentiation and integration as inverse operations -- the pivot of all of calculus. |
| 5 | Binomial theorem | `(x+y)^n = sum_{k=0}^n C(n,k) x^(n-k) y^k` | algebra | generalized by Newton | 1665 | Expands any power of a sum; foundational to combinatorics, series, and probability. |
| 6 | Quadratic formula | `x = (-b +/- sqrt(b^2 - 4ac)) / (2a)` | algebra | ancient; modern form diffuse | unknown | Solves every quadratic in closed form; the first general equation-solver students meet. |
| 7 | Taylor series | `f(x) = sum_{n>=0} f^(n)(a)/n! * (x-a)^n` | analysis | Brook Taylor | 1715 | Approximates smooth functions by polynomials; the engine behind numerical analysis. |
| 8 | Fourier series | `f(x) = a0/2 + sum (a_n cos(n x) + b_n sin(n x))` | analysis | Joseph Fourier | 1807 | Decomposes periodic signals into sinusoids; the basis of harmonic analysis and DSP. |
| 9 | Cauchy-Schwarz inequality | `\|<u,v>\| <= \|\|u\|\| * \|\|v\|\|` | analysis | Cauchy; Bunyakovsky; Schwarz | 1821 | The most-used inequality in analysis; underpins inner-product geometry and probability bounds. |
| 10 | Triangle inequality | `\|\|u + v\|\| <= \|\|u\|\| + \|\|v\|\|` | analysis | classical | unknown | Defines what it means to be a distance (a metric); foundational to all of analysis and topology. |
| 11 | Gauss (divergence) theorem | `integral_V (div F) dV = surface_integral F . dS` | vector calculus | Gauss; Ostrogradsky | 1813 | Converts volume integrals of divergence to surface flux; central to electromagnetism and fluids. |
| 12 | Green's theorem | `closed_integral (P dx + Q dy) = integral_D (dQ/dx - dP/dy) dA` | vector calculus | George Green | 1828 | Relates a plane region integral to its boundary; the 2D ancestor of Stokes and divergence. |
| 13 | Stokes' theorem | `integral_S (curl F) . dS = closed_integral_C F . dr` | vector calculus | Kelvin; Stokes | 1854 | The general 'boundary' theorem unifying Green, divergence, and FTC on manifolds. |
| 14 | De Moivre's theorem | `(cos x + i sin x)^n = cos(n x) + i sin(n x)` | complex analysis | Abraham de Moivre | 1707 | Powers of complex numbers via angles; the route to roots of unity and trig identities. |
| 15 | Cauchy integral formula | `f(a) = (1/(2*pi*i)) closed_integral f(z)/(z-a) dz` | complex analysis | Augustin-Louis Cauchy | 1831 | A holomorphic function is fixed by its boundary values; the heart of complex analysis. |
| 16 | Residue theorem | `closed_integral f(z) dz = 2*pi*i * sum Res(f, a_k)` | complex analysis | Augustin-Louis Cauchy | 1831 | Evaluates contour integrals from isolated singularities; a powerful tool for real integrals too. |
| 17 | Riemann zeta functional equation | `zeta(s) = 2^s pi^(s-1) sin(pi s/2) Gamma(1-s) zeta(1-s)` | number theory | Bernhard Riemann | 1859 | The symmetry s <-> 1-s of zeta; the stage on which the Riemann Hypothesis plays out. |
| 18 | Euler product | `zeta(s) = product_p 1/(1 - p^(-s))` | number theory | Leonhard Euler | 1737 | Rewrites zeta as a product over primes; the analytic bridge into number theory. |
| 19 | Prime number theorem | `pi(x) ~ x / ln(x)` | number theory | Hadamard; de la Vallee Poussin | 1896 | Pins the density of primes to x/ln x; the crowning result of analytic number theory. |
| 20 | Bayes' theorem | `P(A\|B) = P(B\|A) P(A) / P(B)` | probability | Thomas Bayes | 1763 | How evidence updates belief; the foundation of Bayesian inference and modern machine learning. |
| 21 | Gaussian (normal) density | `f(x) = (1/(sigma sqrt(2 pi))) e^(-(x-mu)^2/(2 sigma^2))` | probability | Gauss; de Moivre | 1809 | The bell curve that sums of many small effects converge to; ubiquitous in statistics. |
| 22 | Central limit theorem | `sqrt(n)(Xbar - mu) -> N(0, sigma^2) in distribution` | probability | Laplace; Lyapunov; Lindeberg | 1810 | Explains why the normal distribution is everywhere; the reason averages are predictable. |
| 23 | Stirling's approximation | `n! ~ sqrt(2 pi n) (n/e)^n` | analysis | James Stirling; de Moivre | 1730 | A fast, accurate estimate of n!; essential in statistical mechanics and combinatorics. |
| 24 | Gamma function | `Gamma(n) = integral_0^inf t^(n-1) e^(-t) dt` | analysis | Leonhard Euler | 1729 | Extends the factorial to all complex numbers; appears throughout analysis and probability. |
| 25 | Basel problem | `sum_{n>=1} 1/n^2 = pi^2 / 6` | analysis | Leonhard Euler | 1734 | Euler's stunning sum 1/n^2 = pi^2/6; the opening move of zeta-function theory. |
| 26 | Harmonic/log limit (Euler-Mascheroni) | `lim (sum_{k=1}^n 1/k - ln n) = gamma` | analysis | Euler; Mascheroni | 1734 | Defines gamma, the constant gap between the harmonic series and ln n -- still deeply mysterious. |
| 27 | Fibonacci / golden ratio | `phi = (1 + sqrt(5)) / 2` | number theory | classical; Fibonacci | 1202 | The limiting ratio of Fibonacci numbers; recurs across growth, art, and number theory. |
| 28 | Law of cosines | `c^2 = a^2 + b^2 - 2ab cos(C)` | geometry | classical (Euclid II) | c. 300 BC | Generalizes Pythagoras to any triangle; the workhorse of trigonometric computation. |
| 29 | Heron's formula | `Area = sqrt(s(s-a)(s-b)(s-c)), s=(a+b+c)/2` | geometry | Heron of Alexandria | c. 60 AD | Area of a triangle from its three sides alone; a classic of computational geometry. |
| 30 | Euler's polyhedron formula | `V - E + F = 2` | topology | Leonhard Euler | 1752 | V - E + F = 2 launched topology; the first glimpse of a shape-invariant (the Euler characteristic). |
| 31 | Determinant of 2x2 | `det([[a,b],[c,d]]) = ad - bc` | linear algebra | classical | unknown | Measures signed area/volume scaling; detects invertibility of a linear map. |
| 32 | Eigenvalue equation | `A v = lambda v` | linear algebra | Cauchy; Hilbert (spectral) | 1829 | Finds the directions a transformation merely stretches; the core of spectral theory and quantum mechanics. |
| 33 | Green-Tao theorem | `primes contain arbitrarily long arithmetic progressions` | number theory | Ben Green & Terence Tao | 2004 | Primes contain arbitrarily long arithmetic progressions; a landmark of modern number theory. |
| 34 | Bayes-rule odds form | `posterior odds = likelihood ratio * prior odds` | probability | Thomas Bayes | 1763 | Recasts Bayes as prior odds times a likelihood ratio -- the most practical way to update beliefs. |
| 35 | L'Hopital's rule | `lim f/g = lim f'/g'  (0/0 or inf/inf)` | calculus | l'Hopital; Bernoulli | 1696 | Resolves indeterminate limits via derivatives; a staple technique of first-year calculus. |
| 36 | Integration by parts | `integral u dv = u v - integral v du` | calculus | Brook Taylor (attrib.) | 1715 | The integral mirror of the product rule; indispensable across analysis and physics. |
| 37 | Chain rule | `d/dx f(g(x)) = f'(g(x)) g'(x)` | calculus | Leibniz (notation) | 1676 | Differentiates compositions; the rule that makes backpropagation and all of deep learning possible. |
| 38 | Jensen's inequality | `f(E[X]) <= E[f(X)]  (f convex)` | analysis | Johan Jensen | 1906 | Convexity controls averages; the engine behind countless bounds in probability and information theory. |
| 39 | AM-GM inequality | `(x1+...+xn)/n >= (x1...xn)^(1/n)` | analysis | Cauchy (proof) | 1821 | The arithmetic mean never undershoots the geometric mean; a foundational optimization inequality. |
| 40 | Sum of first n integers | `sum_{k=1}^n k = n(n+1)/2` | algebra | classical; Gauss (attrib.) | unknown | The triangular-number formula; the archetypal closed form for a finite sum. |
| 41 | Geometric series | `sum_{k=0}^inf r^k = 1/(1-r),  \|r\| < 1` | analysis | classical | unknown | A convergent sum with a clean closed form; the basis of compound interest, decay, and generating functions. |
| 42 | Euler's totient product | `phi(n) = n product_{p\|n} (1 - 1/p)` | number theory | Leonhard Euler | 1763 | Counts integers coprime to n; central to modular arithmetic and RSA cryptography. |
| 43 | Fermat's little theorem | `a^p = a (mod p),  p prime` | number theory | Pierre de Fermat | 1640 | A prime's fingerprint in modular arithmetic; the basis of primality tests and public-key crypto. |
| 44 | Cantor's diagonal cardinality | `\|P(S)\| > \|S\|  (2^aleph0 > aleph0)` | set theory | Georg Cantor | 1891 | Proves there are strictly larger infinities; the birth of set theory and the uncountable. |
| 45 | Laplace's equation | `laplacian(f) = 0` | partial differential equations | Pierre-Simon Laplace | 1784 | The defining equation of harmonic functions and potential theory; governs steady-state heat, gravity, and electrostatics. |
| 46 | Euler's theorem (totient) | `a^phi(n) = 1 (mod n),  gcd(a,n)=1` | number theory | Leonhard Euler | 1763 | Generalizes Fermat's little theorem to any modulus; the arithmetic that makes RSA decryption work. |
| 47 | Quadratic reciprocity | `(p/q)(q/p) = (-1)^(((p-1)/2)((q-1)/2))` | number theory | Carl Friedrich Gauss | 1801 | Gauss's 'golden theorem' linking when two primes are squares mod each other; the seed of class field theory. |
| 48 | Cramer's rule | `x_i = det(A_i) / det(A)` | linear algebra | Gabriel Cramer | 1750 | Solves a linear system by determinant ratios; the closed-form expression of a unique solution. |
| 49 | Wallis product | `pi/2 = product_{n>=1} (2n)(2n)/((2n-1)(2n+1))` | analysis | John Wallis | 1656 | An infinite product for pi predating calculus; an early landmark in the analysis of limits and products. |
| 50 | Lagrange mean value theorem | `f'(c) = (f(b) - f(a)) / (b - a)` | calculus | Joseph-Louis Lagrange | 1797 | Guarantees a point where the instantaneous rate equals the average rate; the backbone of error bounds in calculus. |
| 51 | Curl of a gradient | `curl(grad f) = 0` | vector calculus | classical | unknown | A gradient field is always irrotational; the identity behind conservative fields and scalar potentials. |
| 52 | Intermediate value theorem | `f(a) < y < f(b) => exists c in (a,b): f(c) = y` | analysis | Bernard Bolzano | 1817 | A continuous function hits every value between its endpoints; the rigorous root of existence proofs. |
| 53 | Wilson's theorem | `(p-1)! = -1 (mod p),  p prime` | number theory | Lagrange (proof); Wilson | 1771 | A factorial test that characterizes primes exactly; a classic link between combinatorics and primality. |
| 54 | Chinese remainder theorem | `x = a_i (mod n_i) has unique soln mod product n_i` | number theory | Sunzi (attrib.) | c. 400 AD | Reconstructs a number from its remainders modulo coprime bases; foundational to modular arithmetic and RSA. |
| 55 | Vandermonde determinant | `det = product_{i<j} (x_j - x_i)` | linear algebra | Alexandre-Theophile Vandermonde | 1772 | A clean closed form for a key determinant; central to polynomial interpolation and coding theory. |
| 56 | Cayley-Hamilton theorem | `p(A) = 0  (A satisfies its characteristic polynomial)` | linear algebra | Cayley; Hamilton | 1858 | Every square matrix satisfies its own characteristic equation; the basis for matrix functions and minimal polynomials. |
| 57 | Gram-Schmidt orthogonalization | `u_k = v_k - sum_{j<k} proj_{u_j}(v_k)` | linear algebra | Gram; Schmidt | 1907 | Turns any basis into an orthonormal one; the engine behind QR decomposition and least squares. |
| 58 | Parseval's identity | `sum \|c_n\|^2 = (1/(2 pi)) integral_{-pi}^{pi} \|f(x)\|^2 dx` | analysis | Marc-Antoine Parseval | 1799 | Energy is conserved between a signal and its Fourier coefficients; the cornerstone of Hilbert-space harmonic analysis. |
| 59 | Chebyshev's inequality | `P(\|X - mu\| >= k sigma) <= 1/k^2` | probability | Pafnuty Chebyshev | 1867 | Bounds how far a random variable strays from its mean using only variance; the first general concentration inequality. |
| 60 | Fundamental theorem of algebra | `every degree-n polynomial has exactly n complex roots (with multiplicity)` | algebra | Carl Friedrich Gauss | 1799 | Guarantees the complex numbers are algebraically closed; the reason every polynomial factors completely. |
| 61 | Rolle's theorem | `f(a) = f(b) => exists c in (a,b): f'(c) = 0` | calculus | Michel Rolle | 1691 | The special case that powers the mean value theorem; the first rigorous link between equal values and a flat tangent. |
| 62 | Cauchy-Riemann equations | `u_x = v_y,  u_y = -v_x` | complex analysis | Cauchy; Riemann | 1851 | The exact condition for complex differentiability; the gateway from calculus to analytic-function theory. |
| 63 | Argument principle | `(1/(2*pi*i)) closed_integral f'(z)/f(z) dz = Z - P` | complex analysis | Augustin-Louis Cauchy | 1831 | Counts zeros minus poles inside a contour; the tool behind Rouche's theorem and root-locating. |
| 64 | Lagrange interpolation | `p(x) = sum_j y_j product_{m!=j} (x - x_m)/(x_j - x_m)` | numerical analysis | Lagrange; Waring | 1795 | Builds the unique polynomial through given points; the foundation of polynomial interpolation and quadrature. |
| 65 | Newton's method | `x_{n+1} = x_n - f(x_n)/f'(x_n)` | numerical analysis | Newton; Raphson | 1690 | Finds roots by successive linearization, converging quadratically; the workhorse of numerical root-finding and optimization. |
| 66 | Singular value decomposition | `A = U Sigma V^T` | linear algebra | Beltrami; Jordan | 1873 | Factors any matrix into rotations and scalings; the backbone of least squares, PCA, and low-rank approximation. |
| 67 | Bolzano-Weierstrass theorem | `every bounded sequence in R^n has a convergent subsequence` | analysis | Bolzano; Weierstrass | 1817 | Guarantees limit points of bounded sets; a foundational compactness result underpinning existence proofs. |
| 68 | Banach fixed-point theorem | `a contraction on a complete metric space has a unique fixed point` | analysis | Stefan Banach | 1922 | Turns iteration into existence-and-uniqueness; the engine behind ODE solutions and numerical convergence. |
| 69 | Holder's inequality | `sum \|a_k b_k\| <= (sum \|a_k\|^p)^(1/p) (sum \|b_k\|^q)^(1/q),  1/p + 1/q = 1` | analysis | Otto Holder | 1889 | Generalizes Cauchy-Schwarz to L^p spaces; indispensable across functional analysis and probability bounds. |
| 70 | Minkowski's inequality | `\|\|a + b\|\|_p <= \|\|a\|\|_p + \|\|b\|\|_p` | analysis | Hermann Minkowski | 1896 | The triangle inequality for L^p norms; what makes L^p a normed space. |
| 71 | Weak law of large numbers | `Xbar_n -> mu in probability as n -> inf` | probability | Jacob Bernoulli | 1713 | Sample averages converge to the true mean; the formal reason large samples are reliable. |
| 72 | Markov's inequality | `P(X >= a) <= E[X]/a,  X >= 0, a > 0` | probability | Andrey Markov | 1884 | Bounds a nonnegative variable's tail by its mean alone; the seed of Chebyshev and concentration inequalities. |
| 73 | Mobius inversion | `g(n) = sum_{d\|n} f(d)  <=>  f(n) = sum_{d\|n} mu(n/d) g(d)` | number theory | August Ferdinand Mobius | 1832 | Inverts divisor sums via the Mobius function; a central identity of multiplicative number theory. |
| 74 | Dirichlet's theorem (primes in AP) | `gcd(a,d)=1 => infinitely many primes of the form a + n*d` | number theory | Peter Gustav Lejeune Dirichlet | 1837 | Primes are spread through every valid arithmetic progression; a landmark that launched analytic number theory. |
| 75 | Catalan number formula | `C_n = (1/(n+1)) C(2n, n)` | combinatorics | Euler; Catalan | 1838 | Counts balanced structures (parenthesizations, trees, paths); one of combinatorics' most ubiquitous sequences. |

## Validate

These equations are compiled, not just documented. Build the source compiler, then
check and run the sibling demo (which prints every entry and the running total across
all four canons):

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

## Sources & attribution

Names, forms, discoverers, and years follow the standard history-of-science record;
the **Why it matters** notes summarize the result's established significance. See
`SMProvenance.sleela` for the tiered source registry (primary papers, secondary
references such as NIST DLMF / Abramowitz & Stegun, and tertiary locators including
Wikipedia, cited as CC BY-SA and never treated as a primary authority) and the repo
`README.md` for per-class integrity goals. This archive records and attributes
published results; it does not originate, own, or certify them.
