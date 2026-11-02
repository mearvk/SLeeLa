# Finance & Economics Canon

> 25 equations, sourced from `SMFinanceCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

The foundational equations behind money, capitalism, and credit -- the equation of exchange and the quantity theory of money, the time value of money (compounding, discounting, annuities), asset pricing (CAPM, beta, Sharpe, Markowitz), corporate finance (Modigliani-Miller, WACC, Gordon growth), fixed income (bond pricing, duration), macroeconomics (Phillips curve, Taylor rule, Solow growth), and operations economics (Little's Law). Each row credits the economist and year as the record gives them, with a one-line note on why it matters.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `sum`, Greek
names spelled out) so they stay greppable and diff-friendly. The **Why it matters**
column states each result's significance in one faithful line.

## The 25 equations

| # | Equation | Form | Field | Discoverer(s) | Year | Why it matters |
|---|----------|------|-------|---------------|------|----------------|
| 1 | Equation of exchange (quantity theory of money) | `M V = P T` | monetary economics | Irving Fisher | 1911 | Ties money and velocity to prices and output; the core identity of monetary economics and inflation. |
| 2 | Compound interest | `A = P (1 + r/n)^(n t)` | finance | classical; mercantile practice | unknown | Interest earning interest -- the exponential engine of saving, debt, and the whole credit system. |
| 3 | Continuous compounding | `A = P e^(r t)` | finance | classical (Euler's e) | unknown | The limit of ever-more-frequent compounding; the natural-exponential form used throughout finance. |
| 4 | Present value (discounting) | `PV = sum_{t} CF_t / (1 + r)^t` | finance | classical; Fisher (formalized) | 1930 | A future dollar is worth less today; the single most important idea in valuation. |
| 5 | Future value of an annuity | `FV = C ((1 + r)^n - 1) / r` | finance | classical actuarial practice | unknown | Values a stream of equal payments; the basis of loans, mortgages, and pensions. |
| 6 | Perpetuity value | `PV = C / r` | finance | classical | unknown | Prices a cash flow that never ends; the simplest valuation formula and a building block for others. |
| 7 | Gordon growth (dividend discount) | `P = D1 / (r - g)` | corporate finance | Myron J. Gordon | 1959 | Prices a stock from its growing dividends; a cornerstone of equity valuation. |
| 8 | Fisher equation (nominal vs real rate) | `1 + i = (1 + r)(1 + pi) ~ i = r + pi` | monetary economics | Irving Fisher | 1930 | Separates real return from inflation; how interest rates and inflation relate. |
| 9 | Money multiplier | `m = 1 / rr  (reserve ratio rr)` | monetary economics | classical; Phillips (formalized) | 1920 | How reserves expand into the broader money supply; the mechanics of bank-created credit. |
| 10 | Capital Asset Pricing Model (CAPM) | `E[R_i] = R_f + beta_i (E[R_m] - R_f)` | asset pricing | William Sharpe; Lintner; Mossin | 1964 | Prices risk: expected return rises with market risk (beta); the foundation of modern asset pricing (Sharpe's Nobel). |
| 11 | Beta (systematic risk) | `beta_i = Cov(R_i, R_m) / Var(R_m)` | asset pricing | William Sharpe | 1964 | Measures how much an asset moves with the market; the risk that cannot be diversified away. |
| 12 | Modigliani-Miller (irrelevance, no tax) | `V_levered = V_unlevered` | corporate finance | Modigliani & Miller | 1958 | In a frictionless market, capital structure does not change firm value; the starting point of corporate finance (two Nobels). |
| 13 | Weighted average cost of capital | `WACC = (E/V) r_E + (D/V) r_D (1 - Tc)` | corporate finance | standard corporate finance | unknown | The blended cost of a firm's debt and equity; the discount rate for corporate investment decisions. |
| 14 | Bond price (discounted cash flows) | `P = sum_{t} C / (1 + y)^t + F / (1 + y)^n` | fixed income | classical valuation | unknown | A bond is the present value of its coupons and face value; the basis of all fixed-income pricing. |
| 15 | Macaulay duration | `D = (1/P) sum_{t} t * CF_t / (1 + y)^t` | fixed income | Frederick Macaulay | 1938 | The weighted-average time to a bond's cash flows; the standard measure of interest-rate sensitivity. |
| 16 | Sharpe ratio | `S = (R_p - R_f) / sigma_p` | portfolio theory | William Sharpe | 1966 | Return earned per unit of risk; the most widely used measure of investment performance. |
| 17 | Markowitz portfolio variance | `sigma_p^2 = sum_i sum_j w_i w_j Cov(R_i, R_j)` | portfolio theory | Harry Markowitz | 1952 | Diversification quantified: portfolio risk depends on covariances, not just individual risks (Markowitz's Nobel). |
| 18 | Put-call parity | `C - P = S - K e^(-r T)` | mathematical finance | classical; Stoll (formalized) | 1969 | A no-arbitrage link between calls, puts, and the underlying; a bedrock relation in options markets. |
| 19 | Okun's law | `dU ~ -c (g_Y - g_bar)` | macroeconomics | Arthur Okun | 1962 | Links the output gap to unemployment; an empirical rule of thumb in macroeconomics. |
| 20 | Phillips curve | `pi = pi_e - a (u - u_n)` | macroeconomics | A. W. Phillips | 1958 | The short-run trade-off between inflation and unemployment; central to monetary-policy debate. |
| 21 | Taylor rule | `i = r* + pi + a_pi (pi - pi*) + a_y (y - y*)` | monetary policy | John B. Taylor | 1993 | A formula for setting interest rates from inflation and output gaps; a benchmark for central banks. |
| 22 | Cobb-Douglas production function | `Y = A K^alpha L^(1 - alpha)` | economic growth | Cobb & Douglas | 1928 | Output from capital and labor with constant returns; the workhorse production function in economics. |
| 23 | Solow growth (steady state) | `s f(k) = (n + delta) k` | economic growth | Robert Solow | 1956 | Explains long-run growth and convergence from saving and capital; the founding model of growth theory (Solow's Nobel). |
| 24 | Marginal cost = marginal revenue | `MC = MR  (profit-maximizing output)` | microeconomics | classical marginalism (Cournot) | 1838 | The profit-maximizing rule for any firm; the heart of microeconomic decision-making. |
| 25 | Little's Law (throughput-inventory) | `L = lambda W  (avg inventory = throughput * avg flow-time)` | operations economics | John D. C. Little | 1961 | Inventory equals throughput times flow-time; the exact, proven sense in which the pace of a system is set by how much is in it -- the real backbone of the 'speed of the economy' idea. |

## Validate

These equations are compiled, not just documented. Build the source compiler, then
check and run the sibling demo (which prints every entry and the running total across
all canons):

```sh
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela check stanford-math/SMArchive.sleela
SLEELA_LIB="$PWD/stanford-math" impl/build/sleela run   stanford-math/SMArchive.sleela
```

## Sources & attribution

Names, forms, discoverers, and years follow the standard history-of-science / history-
of-economics record; the **Why it matters** notes summarize each result's established
significance. See `SMProvenance.sleela` for the tiered source registry and the repo
`README.md` for per-class integrity goals. This archive records and attributes
published results; it does not originate, own, or certify them.
