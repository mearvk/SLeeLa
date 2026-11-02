# Mathematics Canon

> 44 equations, sourced from `SMMathCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

The foundational mathematics every graduate curriculum rests on -- identities, theorems, and inequalities from geometry through analysis, number theory, probability, and set theory. Each row is a faithful record: the result under the name history gives it, with the rightful discoverer and year. Where attribution is genuinely diffuse (e.g. the quadratic formula, the 2x2 determinant) the discoverer field says so, rather than crediting a single name.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `integral`,
`sum`, `product`, `laplacian`, Greek names spelled out) so they stay greppable and
diff-friendly.

## The 44 equations

| # | Equation | Form | Field | Discoverer(s) | Year |
|---|----------|------|-------|---------------|------|
| 1 | Pythagorean theorem | `a^2 + b^2 = c^2` | geometry | Pythagoras (attrib.) | c. 530 BC |
| 2 | Euler's identity | `e^(i*pi) + 1 = 0` | analysis | Leonhard Euler | 1748 |
| 3 | Euler's formula | `e^(i*x) = cos(x) + i*sin(x)` | analysis | Leonhard Euler | 1748 |
| 4 | Fundamental theorem of calculus | `integral_a^b f'(x) dx = f(b) - f(a)` | calculus | Newton & Leibniz | c. 1670 |
| 5 | Binomial theorem | `(x+y)^n = sum_{k=0}^n C(n,k) x^(n-k) y^k` | algebra | generalized by Newton | 1665 |
| 6 | Quadratic formula | `x = (-b +/- sqrt(b^2 - 4ac)) / (2a)` | algebra | ancient; modern form diffuse | unknown |
| 7 | Taylor series | `f(x) = sum_{n>=0} f^(n)(a)/n! * (x-a)^n` | analysis | Brook Taylor | 1715 |
| 8 | Fourier series | `f(x) = a0/2 + sum (a_n cos(n x) + b_n sin(n x))` | analysis | Joseph Fourier | 1807 |
| 9 | Cauchy-Schwarz inequality | `\|<u,v>\| <= \|\|u\|\| * \|\|v\|\|` | analysis | Cauchy; Bunyakovsky; Schwarz | 1821 |
| 10 | Triangle inequality | `\|\|u + v\|\| <= \|\|u\|\| + \|\|v\|\|` | analysis | classical | unknown |
| 11 | Gauss (divergence) theorem | `integral_V (div F) dV = surface_integral F . dS` | vector calculus | Gauss; Ostrogradsky | 1813 |
| 12 | Green's theorem | `closed_integral (P dx + Q dy) = integral_D (dQ/dx - dP/dy) dA` | vector calculus | George Green | 1828 |
| 13 | Stokes' theorem | `integral_S (curl F) . dS = closed_integral_C F . dr` | vector calculus | Kelvin; Stokes | 1854 |
| 14 | De Moivre's theorem | `(cos x + i sin x)^n = cos(n x) + i sin(n x)` | complex analysis | Abraham de Moivre | 1707 |
| 15 | Cauchy integral formula | `f(a) = (1/(2*pi*i)) closed_integral f(z)/(z-a) dz` | complex analysis | Augustin-Louis Cauchy | 1831 |
| 16 | Residue theorem | `closed_integral f(z) dz = 2*pi*i * sum Res(f, a_k)` | complex analysis | Augustin-Louis Cauchy | 1831 |
| 17 | Riemann zeta functional equation | `zeta(s) = 2^s pi^(s-1) sin(pi s/2) Gamma(1-s) zeta(1-s)` | number theory | Bernhard Riemann | 1859 |
| 18 | Euler product | `zeta(s) = product_p 1/(1 - p^(-s))` | number theory | Leonhard Euler | 1737 |
| 19 | Prime number theorem | `pi(x) ~ x / ln(x)` | number theory | Hadamard; de la Vallee Poussin | 1896 |
| 20 | Bayes' theorem | `P(A\|B) = P(B\|A) P(A) / P(B)` | probability | Thomas Bayes | 1763 |
| 21 | Gaussian (normal) density | `f(x) = (1/(sigma sqrt(2 pi))) e^(-(x-mu)^2/(2 sigma^2))` | probability | Gauss; de Moivre | 1809 |
| 22 | Central limit theorem | `sqrt(n)(Xbar - mu) -> N(0, sigma^2) in distribution` | probability | Laplace; Lyapunov; Lindeberg | 1810 |
| 23 | Stirling's approximation | `n! ~ sqrt(2 pi n) (n/e)^n` | analysis | James Stirling; de Moivre | 1730 |
| 24 | Gamma function | `Gamma(n) = integral_0^inf t^(n-1) e^(-t) dt` | analysis | Leonhard Euler | 1729 |
| 25 | Basel problem | `sum_{n>=1} 1/n^2 = pi^2 / 6` | analysis | Leonhard Euler | 1734 |
| 26 | Harmonic/log limit (Euler-Mascheroni) | `lim (sum_{k=1}^n 1/k - ln n) = gamma` | analysis | Euler; Mascheroni | 1734 |
| 27 | Fibonacci / golden ratio | `phi = (1 + sqrt(5)) / 2` | number theory | classical; Fibonacci | 1202 |
| 28 | Law of cosines | `c^2 = a^2 + b^2 - 2ab cos(C)` | geometry | classical (Euclid II) | c. 300 BC |
| 29 | Heron's formula | `Area = sqrt(s(s-a)(s-b)(s-c)), s=(a+b+c)/2` | geometry | Heron of Alexandria | c. 60 AD |
| 30 | Euler's polyhedron formula | `V - E + F = 2` | topology | Leonhard Euler | 1752 |
| 31 | Determinant of 2x2 | `det([[a,b],[c,d]]) = ad - bc` | linear algebra | classical | unknown |
| 32 | Eigenvalue equation | `A v = lambda v` | linear algebra | Cauchy; Hilbert (spectral) | 1829 |
| 33 | Green-Tao theorem | `primes contain arbitrarily long arithmetic progressions` | number theory | Ben Green & Terence Tao | 2004 |
| 34 | Bayes-rule odds form | `posterior odds = likelihood ratio * prior odds` | probability | Thomas Bayes | 1763 |
| 35 | L'Hopital's rule | `lim f/g = lim f'/g'  (0/0 or inf/inf)` | calculus | l'Hopital; Bernoulli | 1696 |
| 36 | Integration by parts | `integral u dv = u v - integral v du` | calculus | Brook Taylor (attrib.) | 1715 |
| 37 | Chain rule | `d/dx f(g(x)) = f'(g(x)) g'(x)` | calculus | Leibniz (notation) | 1676 |
| 38 | Jensen's inequality | `f(E[X]) <= E[f(X)]  (f convex)` | analysis | Johan Jensen | 1906 |
| 39 | AM-GM inequality | `(x1+...+xn)/n >= (x1...xn)^(1/n)` | analysis | Cauchy (proof) | 1821 |
| 40 | Sum of first n integers | `sum_{k=1}^n k = n(n+1)/2` | algebra | classical; Gauss (attrib.) | unknown |
| 41 | Geometric series | `sum_{k=0}^inf r^k = 1/(1-r),  \|r\| < 1` | analysis | classical | unknown |
| 42 | Euler's totient product | `phi(n) = n product_{p\|n} (1 - 1/p)` | number theory | Leonhard Euler | 1763 |
| 43 | Fermat's little theorem | `a^p = a (mod p),  p prime` | number theory | Pierre de Fermat | 1640 |
| 44 | Cantor's diagonal cardinality | `\|P(S)\| > \|S\|  (2^aleph0 > aleph0)` | set theory | Georg Cantor | 1891 |

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
see `SMProvenance.sleela` for the tiered source registry (primary papers, secondary
references such as NIST DLMF / Abramowitz & Stegun, and tertiary locators including
Wikipedia, cited as CC BY-SA and never treated as a primary authority) and the repo
`README.md` for per-class integrity goals. This archive records and attributes
published results; it does not originate, own, or certify them.
