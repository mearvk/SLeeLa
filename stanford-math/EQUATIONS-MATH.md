# Mathematics Canon

> 126 equations, sourced from `SMMathCanon.sleela` -- the single
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

## The 126 equations

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
| 76 | Lagrange multipliers | `grad f = lambda grad g` | optimization | Joseph-Louis Lagrange | 1788 | Finds extrema subject to constraints by matching gradients; the backbone of constrained optimization. |
| 77 | Euler-Lagrange equation | `d/dt (dL/dq') - dL/dq = 0` | calculus of variations | Euler; Lagrange | 1755 | The condition a path must satisfy to extremize an action; the foundation of variational mechanics. |
| 78 | Divergence of a curl | `div(curl F) = 0` | vector calculus | classical | unknown | A curl field has zero divergence; a core identity of vector calculus and magnetostatics. |
| 79 | Gradient theorem (line integrals) | `integral_C grad f . dr = f(b) - f(a)` | vector calculus | classical | unknown | A conservative field's line integral depends only on endpoints; the FTC for gradients. |
| 80 | Leibniz integral rule | `d/dx integral_{a(x)}^{b(x)} f dt = f(b) b' - f(a) a' + integral f_x dt` | calculus | Gottfried Leibniz | 1697 | Differentiates an integral with variable limits; essential across physics and probability. |
| 81 | Taylor's theorem with remainder | `f(x) = sum_{k=0}^{n} f^(k)(a)/k! (x-a)^k + R_n` | analysis | Brook Taylor; Lagrange (remainder) | 1715 | Bounds the error of a Taylor approximation; the rigorous basis of numerical estimates. |
| 82 | Mean value theorem for integrals | `integral_a^b f dx = f(c)(b - a)` | calculus | classical (Cauchy) | 1823 | Guarantees a point whose value equals the integral's average; a staple existence result. |
| 83 | Cauchy's integral theorem | `closed_integral f(z) dz = 0  (f holomorphic)` | complex analysis | Augustin-Louis Cauchy | 1825 | A holomorphic function integrates to zero around a closed loop; the start of complex analysis. |
| 84 | Liouville's theorem (bounded entire) | `a bounded entire function is constant` | complex analysis | Joseph Liouville | 1847 | A bounded entire function must be constant; yields a one-line proof of the fundamental theorem of algebra. |
| 85 | Maximum modulus principle | `\|f\| attains its max on the boundary` | complex analysis | Riemann; Schwarz | 1851 | A holomorphic function reaches its maximum size only on the boundary; central to complex analysis. |
| 86 | Schwarz lemma | `\|f(z)\| <= \|z\| and \|f'(0)\| <= 1 for f: disk->disk, f(0)=0` | complex analysis | Hermann Schwarz | 1869 | Constrains holomorphic self-maps of the disk; the seed of hyperbolic geometry and Pick interpolation. |
| 87 | Morera's theorem | `closed_integral f = 0 for all triangles => f holomorphic` | complex analysis | Giacinto Morera | 1886 | The converse of Cauchy's theorem: zero loop integrals imply holomorphy. |
| 88 | Rouche's theorem | `\|g\| < \|f\| on C => f and f+g have equally many zeros inside` | complex analysis | Eugene Rouche | 1862 | Counts zeros by comparing magnitudes on a contour; a practical zero-counting tool. |
| 89 | Jordan curve theorem | `a simple closed curve splits the plane into inside and outside` | topology | Camille Jordan | 1887 | A simple closed curve separates the plane into inside and outside; a deep foundational fact of topology. |
| 90 | Brouwer fixed-point theorem | `a continuous map of a closed ball to itself has a fixed point` | topology | L. E. J. Brouwer | 1911 | Every continuous self-map of a ball has a fixed point; a cornerstone of topology and game theory. |
| 91 | Gauss-Bonnet theorem | `integral_M K dA + integral_{dM} k_g ds = 2 pi chi(M)` | differential geometry | Gauss; Bonnet | 1848 | Ties total curvature to the Euler characteristic; the bridge between geometry and topology. |
| 92 | Noether's theorem | `every differentiable symmetry yields a conservation law` | mathematical physics | Emmy Noether | 1918 | Every continuous symmetry yields a conservation law; the organizing principle of modern physics. |
| 93 | Implicit function theorem | `F(x,y)=0, F_y != 0 => y = g(x) locally` | analysis | Dini; Cauchy | 1877 | Guarantees a relation defines a function locally; essential to manifolds and optimization. |
| 94 | Inverse function theorem | `f' invertible at a => f invertible near a` | analysis | classical | unknown | A map with invertible derivative is locally invertible; a foundation of differential calculus. |
| 95 | Picard-Lindelof existence | `y' = f(x,y), f Lipschitz => unique local solution` | differential equations | Picard; Lindelof | 1894 | Lipschitz continuity guarantees a unique ODE solution; the existence theorem of differential equations. |
| 96 | Wronskian (linear independence) | `W(f,g) = f g' - f' g` | differential equations | Jozef Wronski | 1812 | A determinant test for independence of solutions; standard in linear ODE theory. |
| 97 | Bessel's equation | `x^2 y'' + x y' + (x^2 - n^2) y = 0` | special functions | Friedrich Bessel | 1824 | Governs cylindrical waves and vibrations; its solutions appear throughout physics and engineering. |
| 98 | Legendre's equation | `(1 - x^2) y'' - 2 x y' + l(l+1) y = 0` | special functions | Adrien-Marie Legendre | 1782 | Yields the Legendre polynomials central to potential theory and spherical harmonics. |
| 99 | Hermite's equation | `y'' - 2 x y' + 2 n y = 0` | special functions | Charles Hermite | 1864 | Yields Hermite polynomials; the eigenfunctions of the quantum harmonic oscillator. |
| 100 | Laguerre's equation | `x y'' + (1 - x) y' + n y = 0` | special functions | Edmond Laguerre | 1879 | Yields Laguerre polynomials; the radial part of the hydrogen-atom wavefunction. |
| 101 | Beta function | `B(x,y) = integral_0^1 t^(x-1)(1-t)^(y-1) dt = Gamma(x)Gamma(y)/Gamma(x+y)` | special functions | Euler; Legendre | 1730 | Euler's integral linking factorials and probabilities; the basis of the Beta distribution. |
| 102 | Riemann sum (definite integral) | `integral_a^b f dx = lim sum f(x_i*) dx_i` | calculus | Bernhard Riemann | 1854 | Defines the integral as a limit of partition sums; the rigorous meaning of area under a curve. |
| 103 | Weierstrass M-test | `sum M_n converges, \|f_n\| <= M_n => sum f_n converges uniformly` | analysis | Karl Weierstrass | 1880 | A simple bound guaranteeing uniform convergence of a series; a workhorse of analysis. |
| 104 | Abel's summation / test | `convergence of sum a_n b_n via partial sums` | analysis | Niels Henrik Abel | 1826 | Summation by parts giving convergence of products of sequences; foundational to series analysis. |
| 105 | Dirichlet's test for series | `a_n monotone ->0, partial sums of b_n bounded => sum a_n b_n converges` | analysis | Peter Gustav Lejeune Dirichlet | 1862 | Convergence from a monotone factor and bounded partial sums; a standard series test. |
| 106 | Holder continuity | `\|f(x) - f(y)\| <= C \|x - y\|^alpha` | analysis | Otto Holder | 1882 | Quantifies smoothness between continuity and differentiability; central to PDE regularity. |
| 107 | Poisson summation formula | `sum f(n) = sum F-hat(k)` | harmonic analysis | Simeon Denis Poisson | 1827 | Equates a sum of a function to a sum of its Fourier transform; links time and frequency sampling. |
| 108 | Parseval-Plancherel theorem | `integral \|f\|^2 dx = integral \|F-hat\|^2 dw` | harmonic analysis | Michel Plancherel | 1910 | Energy is preserved by the Fourier transform; the isometry of harmonic analysis. |
| 109 | Convolution theorem | `F{f * g} = F{f} . F{g}` | harmonic analysis | classical (Fourier) | unknown | Convolution in time is multiplication in frequency; the reason transforms simplify filtering. |
| 110 | Spectral theorem (symmetric matrix) | `A = Q Lambda Q^T,  A symmetric` | linear algebra | Cauchy; Hilbert | 1829 | A symmetric matrix diagonalizes in an orthonormal basis; the foundation of PCA and quantum observables. |
| 111 | LU decomposition | `A = L U` | numerical linear algebra | Alan Turing (modern); Gauss | 1948 | Factors a matrix for fast linear solves; the standard method behind Gaussian elimination. |
| 112 | QR decomposition | `A = Q R,  Q orthogonal, R upper-triangular` | numerical linear algebra | classical (Gram-Schmidt) | 1907 | Orthogonal-times-triangular factorization; the stable workhorse of least squares and eigenvalues. |
| 113 | Cholesky decomposition | `A = L L^T,  A symmetric positive-definite` | numerical linear algebra | Andre-Louis Cholesky | 1910 | An efficient factorization for positive-definite matrices; standard in optimization and statistics. |
| 114 | Rayleigh quotient | `R(A,x) = (x^T A x) / (x^T x)` | linear algebra | Lord Rayleigh | 1877 | Estimates eigenvalues by a normalized quadratic form; the basis of iterative eigenvalue methods. |
| 115 | Perron-Frobenius theorem | `a positive matrix has a unique largest positive eigenvalue` | linear algebra | Perron; Frobenius | 1907 | A positive matrix has a dominant positive eigenvalue; underpins PageRank and Markov chains. |
| 116 | Sylvester's law of inertia | `congruent forms share the signature (p, q, z)` | linear algebra | James Joseph Sylvester | 1852 | The signature of a quadratic form is invariant under congruence; classifies conics and metrics. |
| 117 | Hadamard's inequality | `\|det A\| <= product of column norms` | linear algebra | Jacques Hadamard | 1893 | Bounds a determinant by its column norms; a geometric bound on volume. |
| 118 | Cramer-Rao bound | `Var(theta-hat) >= 1 / I(theta)` | mathematical statistics | Cramer; Rao | 1945 | Sets the minimum variance of an unbiased estimator; the gold standard of estimation theory. |
| 119 | Maximum likelihood estimator | `theta-hat = argmax L(theta; x)` | mathematical statistics | Ronald A. Fisher | 1922 | Chooses parameters that make the data most probable; the dominant estimation principle in statistics. |
| 120 | Poisson distribution | `P(k) = lambda^k e^(-lambda) / k!` | probability | Simeon Denis Poisson | 1837 | Models counts of rare independent events; ubiquitous in queues, physics, and reliability. |
| 121 | Binomial distribution | `P(k) = C(n,k) p^k (1-p)^(n-k)` | probability | Jacob Bernoulli | 1713 | Counts successes in independent trials; the discrete foundation of probability. |
| 122 | Markov chain stationary distribution | `pi P = pi,  sum pi_i = 1` | probability | Andrey Markov | 1906 | The long-run state frequencies of a chain; central to MCMC and equilibrium analysis. |
| 123 | Borel-Cantelli lemma | `sum P(A_n) < inf => P(A_n i.o.) = 0` | probability | Borel; Cantelli | 1909 | Links summable probabilities to almost-sure non-recurrence; a key tool of probability theory. |
| 124 | Jensen's gap / convex duality | `f convex => f(E[X]) <= E[f(X)] with Legendre dual` | convex analysis | Legendre; Fenchel | 1949 | Convex-conjugate duality underlying bounds and optimization; the Legendre-Fenchel transform. |
| 125 | Simpson's rule (quadrature) | `integral_a^b f dx = (h/3)[f0 + 4 f1 + f2 + ...]` | numerical analysis | Thomas Simpson | 1743 | Approximates integrals with parabolic arcs; a staple of numerical integration. |
| 126 | Runge-Kutta (RK4) step | `y_{n+1} = y_n + (h/6)(k1 + 2 k2 + 2 k3 + k4)` | numerical analysis | Runge; Kutta | 1901 | A fourth-order one-step ODE integrator; the default method for numerical simulation. |

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

## Definitions & relevance (second-layer additions)

The entries below are the second-layer additions to this canon, organized by
type. Each gives a short **definition** (what the result states and what its
symbols mean) and its **relevance** (why it is foundational to mathematics).

### Optimization & the calculus of variations

**Lagrange multipliers** — `grad f = lambda grad g`. At a constrained extremum the
gradient of the objective is parallel to the gradient of the constraint. It is the
backbone of constrained optimization, from economics to physics to machine
learning.

**Euler-Lagrange equation** — `d/dt (dL/dq') - dL/dq = 0`. The condition a path
must satisfy to make an integral (an "action") stationary. It is the foundation of
variational mechanics and of every principle-of-least-action formulation of
physics.

### Vector & single-variable calculus

**Divergence of a curl** — `div(curl F) = 0`. The divergence of any curl field
vanishes identically. It is a core vector-calculus identity and the reason
magnetic fields have no sources.

**Gradient theorem (line integrals)** — `integral_C grad f . dr = f(b) - f(a)`. A
line integral of a gradient depends only on the endpoints. It is the fundamental
theorem of calculus for gradient fields and defines what "conservative" means.

**Leibniz integral rule** — `d/dx integral f dt = f(b)b' - f(a)a' + integral f_x dt`.
Differentiates an integral whose integrand and limits depend on the parameter. It
is a workhorse across physics, probability, and applied analysis.

**Mean value theorem for integrals** — `integral_a^b f dx = f(c)(b - a)`. A
continuous function attains its average value somewhere on the interval. It is a
basic existence result underpinning error estimates and the FTC.

**Riemann sum (definite integral)** — `integral_a^b f dx = lim sum f(x_i*) dx_i`.
Defines the integral as the limit of partition sums. It is the rigorous meaning of
"area under a curve" and the starting point of integration theory.

### Real analysis

**Taylor's theorem with remainder** — `f(x) = sum f^(k)(a)/k!(x-a)^k + R_n`. Adds a
quantified error term to the Taylor expansion. It is what makes polynomial
approximation rigorous and bounds the accuracy of numerical methods.

**Implicit function theorem** — `F(x,y)=0, F_y != 0 => y = g(x)` locally. A
relation defines one variable as a function of the others wherever a derivative is
nonzero. It is essential to the theory of manifolds, constraints, and
optimization.

**Inverse function theorem** — a map whose derivative is invertible at a point is
locally invertible. It is a foundation of differential calculus on `R^n` and the
companion to the implicit function theorem.

**Weierstrass M-test** — bounding each term by a convergent series `sum M_n`
guarantees uniform convergence. It is the simplest, most-used test for the uniform
convergence that lets one interchange limits, sums, and integrals.

**Abel's summation / test** — summation by parts yields convergence for products
of sequences. It is a foundational technique for series whose terms are products of
a monotone and an oscillating factor.

**Dirichlet's test for series** — a monotone factor tending to zero times a factor
with bounded partial sums converges. It is a standard, powerful convergence test,
e.g. for conditionally convergent trigonometric series.

**Holder continuity** — `|f(x) - f(y)| <= C |x - y|^alpha`. Quantifies smoothness
on a scale between mere continuity (`alpha -> 0`) and Lipschitz (`alpha = 1`). It is
central to the regularity theory of PDEs.

### Complex analysis

**Cauchy's integral theorem** — `closed_integral f(z) dz = 0` for a holomorphic
function. The loop integral of an analytic function vanishes. It is the
cornerstone from which the whole edifice of complex analysis is built.

**Liouville's theorem (bounded entire)** — a bounded entire function must be
constant. Its relevance includes a one-line proof of the fundamental theorem of
algebra and deep rigidity results for analytic functions.

**Maximum modulus principle** — a non-constant holomorphic function attains its
maximum size only on the boundary. It is a defining rigidity property of analytic
functions, used throughout complex analysis and PDE.

**Schwarz lemma** — a holomorphic self-map of the unit disk fixing the origin
satisfies `|f(z)| <= |z|`. It seeds hyperbolic geometry on the disk and the theory
of conformal self-maps.

**Morera's theorem** — vanishing integrals over all triangles imply holomorphy. It
is the converse of Cauchy's theorem and a key tool for proving functions are
analytic (e.g. limits of analytic functions).

**Rouche's theorem** — if `|g| < |f|` on a contour, then `f` and `f+g` have the same
number of zeros inside. It is the practical instrument for counting and locating
zeros, including a slick proof of the fundamental theorem of algebra.

### Topology & geometry

**Jordan curve theorem** — a simple closed curve divides the plane into an inside
and an outside. Obvious to the eye yet subtle to prove, it is a foundational fact
of plane topology.

**Brouwer fixed-point theorem** — any continuous map of a closed ball to itself has
a fixed point. It is a cornerstone of topology with profound consequences in
economics (equilibrium existence) and game theory (Nash).

**Gauss-Bonnet theorem** — `integral_M K dA + integral_{dM} k_g ds = 2 pi chi(M)`.
Total curvature equals `2 pi` times the Euler characteristic. It is the prototypical
bridge between local geometry (curvature) and global topology.

**Noether's theorem** — every continuous symmetry of an action yields a conservation
law. It is the organizing principle of modern physics: time symmetry gives energy
conservation, space symmetry gives momentum, and so on.

### Differential equations & special functions

**Picard-Lindelof existence** — a Lipschitz right-hand side guarantees a unique
local solution of `y' = f(x,y)`. It is the fundamental existence-and-uniqueness
theorem for ordinary differential equations.

**Wronskian (linear independence)** — `W(f,g) = f g' - f' g`. A determinant that
tests whether solutions of a linear ODE are independent. It is a standard tool in
the structure theory of linear differential equations.

**Bessel's equation** — `x^2 y'' + x y' + (x^2 - n^2) y = 0`. Its solutions,
Bessel functions, describe cylindrical waves and vibrations. They appear across
physics and engineering wherever cylindrical symmetry arises.

**Legendre's equation** — `(1 - x^2) y'' - 2 x y' + l(l+1) y = 0`. Yields the
Legendre polynomials, central to potential theory and the spherical harmonics.

**Hermite's equation** — `y'' - 2 x y' + 2 n y = 0`. Yields the Hermite
polynomials, the eigenfunctions of the quantum harmonic oscillator and a basis for
Gaussian-weighted analysis.

**Laguerre's equation** — `x y'' + (1 - x) y' + n y = 0`. Yields the Laguerre
polynomials, which form the radial part of the hydrogen-atom wavefunction.

**Beta function** — `B(x,y) = integral_0^1 t^(x-1)(1-t)^(y-1) dt`. Euler's integral
linking factorials via the Gamma function. It underlies the Beta distribution and
many integral evaluations.

### Harmonic analysis

**Poisson summation formula** — `sum f(n) = sum F-hat(k)`. Equates a sum of a
function over a lattice to a sum of its Fourier transform over the dual lattice. It
links sampling in time to periodicity in frequency and is central to analytic
number theory.

**Parseval-Plancherel theorem** — `integral |f|^2 dx = integral |F-hat|^2 dw`. The
Fourier transform preserves energy (the `L^2` norm). It is the isometry at the heart
of harmonic analysis and signal processing.

**Convolution theorem** — `F{f * g} = F{f} . F{g}`. Convolution in one domain is
pointwise multiplication in the transform domain. It is the reason transforms make
filtering and differential equations tractable.

### Linear algebra

**Spectral theorem (symmetric matrix)** — `A = Q Lambda Q^T` for symmetric `A`. A
symmetric matrix is diagonalized by an orthonormal basis of eigenvectors. It is the
foundation of principal component analysis and of quantum observables.

**Rayleigh quotient** — `R(A,x) = (x^T A x)/(x^T x)`. A normalized quadratic form
whose extremes are eigenvalues. It is the basis of variational eigenvalue estimates
and iterative methods.

**Perron-Frobenius theorem** — a positive matrix has a unique largest positive
eigenvalue with a positive eigenvector. It underpins PageRank, Markov-chain
equilibria, and population-growth models.

**Sylvester's law of inertia** — the numbers of positive, negative, and zero
eigenvalues of a quadratic form are invariant under congruence. It classifies
quadratic forms and the signature of metrics.

**Hadamard's inequality** — `|det A| <= product of column norms`. The determinant is
bounded by the product of its column lengths, with equality for orthogonal columns.
It is a sharp geometric bound on volume.

### Numerical linear algebra

**LU decomposition** — `A = L U`. Factors a matrix into lower- and upper-triangular
parts for efficient repeated solves. It is the matrix form of Gaussian elimination
and the default direct solver.

**QR decomposition** — `A = Q R`, with `Q` orthogonal and `R` upper-triangular. A
numerically stable factorization. It is the workhorse behind least squares and the
QR eigenvalue algorithm.

**Cholesky decomposition** — `A = L L^T` for symmetric positive-definite `A`. An
efficient, stable factorization using half the work of LU. It is standard in
optimization, statistics (covariance), and simulation.

### Mathematical statistics, probability & convexity

**Cramer-Rao bound** — `Var(theta-hat) >= 1 / I(theta)`. Sets the minimum variance
any unbiased estimator can achieve, via the Fisher information `I`. It is the gold
standard for judging estimator efficiency.

**Maximum likelihood estimator** — `theta-hat = argmax L(theta; x)`. Chooses the
parameters that make the observed data most probable. It is the dominant estimation
principle across statistics, with strong large-sample optimality.

**Poisson distribution** — `P(k) = lambda^k e^(-lambda)/k!`. The law of counts of
rare, independent events at constant average rate. It is ubiquitous in queues,
physics, and reliability.

**Binomial distribution** — `P(k) = C(n,k) p^k (1-p)^(n-k)`. The law of the number
of successes in independent trials. It is the discrete foundation of probability
and the seed of the normal approximation.

**Markov chain stationary distribution** — `pi P = pi`, `sum pi_i = 1`. The
long-run frequencies a chain settles into. It is central to Markov chain Monte
Carlo, PageRank, and equilibrium analysis.

**Borel-Cantelli lemma** — if `sum P(A_n) < inf` then `A_n` happens only finitely
often, almost surely. It is a basic but powerful tool for almost-sure convergence
and the strong law of large numbers.

**Jensen's gap / convex duality** — convex-conjugate (Legendre-Fenchel) duality
underlying many bounds and optimization problems. It formalizes the gap in Jensen's
inequality and is foundational to convex optimization.

### Numerical analysis

**Simpson's rule (quadrature)** — `integral_a^b f dx = (h/3)[f0 + 4 f1 + f2 + ...]`.
Approximates an integral with parabolic arcs over pairs of subintervals. It is a
staple of numerical integration, markedly more accurate than the trapezoidal rule.

**Runge-Kutta (RK4) step** — `y_{n+1} = y_n + (h/6)(k1 + 2 k2 + 2 k3 + k4)`. A
fourth-order one-step method for integrating ODEs. It is the default general-purpose
integrator in scientific computing.
