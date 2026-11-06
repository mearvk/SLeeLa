# Finance & Economics Canon

> 50 equations, sourced from `SMFinanceCanon.sleela` -- the single
> source of truth. This document is a human-readable mirror of that record;
> if the two ever disagree, the `.sleela` canon wins and this file is stale.

The foundational equations behind money, capitalism, and credit -- the equation of exchange and the quantity theory of money, the time value of money (compounding, discounting, annuities), asset pricing (CAPM, beta, Sharpe, Markowitz), corporate finance (Modigliani-Miller, WACC, Gordon growth), fixed income (bond pricing, duration), macroeconomics (Phillips curve, Taylor rule, Solow growth), and operations economics (Little's Law). Each row credits the economist and year as the record gives them, with a one-line note on why it matters.

The one hard rule governs every row: **values are faithful to the subject's
real origin where publicly known**; diffuse or unknown attributions are marked
honestly (`unknown`, `classical`, `attrib.`), never invented to look legitimate.
Symbolic forms use plain-ASCII notation (`^` power, `*` product, `sqrt`, `sum`, Greek
names spelled out) so they stay greppable and diff-friendly. The **Why it matters**
column states each result's significance in one faithful line.

## The 50 equations

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
| 26 | Black-Scholes delta | `delta_call = N(d1)` | option Greeks | Black; Scholes; Merton | 1973 | The option price's sensitivity to the underlying; the primary hedge ratio. |
| 27 | Black-Scholes gamma | `gamma = N'(d1) / (S sigma sqrt(T))` | option Greeks | Black; Scholes; Merton | 1973 | The rate of change of delta; measures hedging instability near the strike. |
| 28 | Black-Scholes vega | `vega = S N'(d1) sqrt(T)` | option Greeks | Black; Scholes; Merton | 1973 | Sensitivity of option value to volatility; the key risk in volatility trading. |
| 29 | Black-Scholes theta | `theta = -(S N'(d1) sigma)/(2 sqrt(T)) - r K e^(-r T) N(d2)` | option Greeks | Black; Scholes; Merton | 1973 | The option's time decay; how value erodes as expiry approaches. |
| 30 | Binomial option pricing (CRR) | `C = e^(-r dt)[p Cu + (1-p) Cd]` | derivatives pricing | Cox; Ross; Rubinstein | 1979 | Prices options on a discrete price tree; an intuitive route to Black-Scholes. |
| 31 | Black-76 (futures option) | `C = e^(-r T)[F N(d1) - K N(d2)]` | derivatives pricing | Fischer Black | 1976 | Adapts Black-Scholes to options on futures and forwards; standard in commodities and rates. |
| 32 | Forward price (cost of carry) | `F = S e^((r - q) T)` | derivatives pricing | classical (no-arbitrage) | unknown | Sets the no-arbitrage forward price from spot and carry costs; a basic pricing identity. |
| 33 | Covered interest rate parity | `F/S = (1 + r_d)/(1 + r_f)` | international finance | John Maynard Keynes | 1923 | Links forward and spot FX rates to interest differentials; a core no-arbitrage condition. |
| 34 | Vasicek short-rate model | `dr = a(b - r) dt + sigma dW` | fixed income | Oldrich Vasicek | 1977 | A mean-reverting model of interest rates; an early tractable term-structure model. |
| 35 | Cox-Ingersoll-Ross model | `dr = a(b - r) dt + sigma sqrt(r) dW` | fixed income | Cox; Ingersoll; Ross | 1985 | A short-rate model keeping rates non-negative; widely used in fixed income. |
| 36 | Merton structural credit model | `equity = call option on firm assets` | credit risk | Robert C. Merton | 1974 | Treats equity as a call on firm assets; the foundation of structural credit risk. |
| 37 | Modified duration | `ModDur = MacaulayDur / (1 + y/k)` | fixed income | Frederick Macaulay (basis) | 1938 | A bond's price sensitivity to yield; the primary interest-rate risk measure. |
| 38 | Bond convexity | `C = (1/P) d2P/dy2` | fixed income | classical | unknown | The curvature of the price-yield relation; refines duration for large yield moves. |
| 39 | Yield to maturity (IRR of bond) | `P = sum CF_t / (1 + y)^t` | fixed income | classical | unknown | The single discount rate equating price to cash flows; a bond's internal rate of return. |
| 40 | Fama-French three-factor model | `R - Rf = a + b(Rm-Rf) + s SMB + h HML` | asset pricing | Fama; French | 1993 | Explains returns by market, size, and value factors; a benchmark of empirical asset pricing. |
| 41 | Arbitrage pricing theory (APT) | `E[R] = Rf + sum beta_k lambda_k` | asset pricing | Stephen Ross | 1976 | Prices assets by exposure to multiple risk factors; a flexible alternative to CAPM. |
| 42 | Jensen's alpha | `alpha = R_p - [Rf + beta(Rm - Rf)]` | performance measurement | Michael Jensen | 1968 | Return beyond what beta predicts; the classic measure of manager skill. |
| 43 | Treynor ratio | `T = (R_p - Rf) / beta_p` | performance measurement | Jack Treynor | 1965 | Excess return per unit of systematic risk; a risk-adjusted performance measure. |
| 44 | Sortino ratio | `S = (R_p - R_target) / downside deviation` | performance measurement | Frank Sortino | 1980 | Excess return per unit of downside risk; penalizes only harmful volatility. |
| 45 | Value at Risk (parametric) | `VaR = mu - z_alpha sigma` | risk management | J.P. Morgan (RiskMetrics) | 1994 | A loss threshold at a confidence level; a standard regulatory risk metric. |
| 46 | GARCH(1,1) variance | `sigma_t^2 = omega + alpha e_{t-1}^2 + beta sigma_{t-1}^2` | financial econometrics | Tim Bollerslev | 1986 | Models time-varying, clustering volatility; the workhorse of financial econometrics. |
| 47 | Rule of 72 (doubling time) | `t_double = 72 / r%` | personal finance | Luca Pacioli (attrib.) | 1494 | Estimates doubling time by dividing 72 by the rate; a quick compounding heuristic. |
| 48 | Dividend payout / retention | `g = ROE * retention ratio` | corporate finance | classical | unknown | Sustainable growth equals return on equity times retention; a corporate-finance identity. |
| 49 | Economic order quantity (EOQ) | `Q* = sqrt(2 D S / H)` | operations economics | Ford W. Harris | 1913 | The order size minimizing inventory cost; the founding formula of inventory theory. |
| 50 | Herfindahl-Hirschman index | `HHI = sum s_i^2` | industrial organization | Herfindahl; Hirschman | 1945 | Market concentration from summed squared shares; a standard antitrust measure. |

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

## Definitions & relevance (second-layer additions)

The entries below are the second-layer additions to this canon, organized by
type. Each gives a short **definition** (what the quantity is and what its
symbols mean) and its **relevance** (why it is foundational to quantitative
finance and economics).

### Option Greeks (Black-Scholes sensitivities)

**Black-Scholes delta** — `delta_call = N(d1)`. The sensitivity of an option's
price to a small move in the underlying asset; it equals the number of shares
needed to hedge one option. It is the primary hedge ratio and the first Greek any
options desk monitors.

**Black-Scholes gamma** — `gamma = N'(d1) / (S sigma sqrt(T))`. The rate of change
of delta as the underlying moves, largest near the strike and near expiry. It
measures how unstable a hedge is and therefore how often a position must be
rebalanced.

**Black-Scholes vega** — `vega = S N'(d1) sqrt(T)`. The sensitivity of option
value to a change in volatility. It is the key risk for volatility trading and
the link between option prices and the market's view of future uncertainty.

**Black-Scholes theta** — `theta = -(S N'(d1) sigma)/(2 sqrt(T)) - r K e^(-r T) N(d2)`.
The option's time decay — how value erodes as expiry approaches with all else
fixed. It is central to the economics of holding options, the cost paid for
optionality over time.

### Derivatives pricing

**Binomial option pricing (CRR)** — `C = e^(-r dt)[p Cu + (1-p) Cd]`. Prices an
option by stepping back through a discrete tree of up/down moves using
risk-neutral probabilities. It is an intuitive, flexible route to Black-Scholes
that also handles early exercise, making it a teaching and pricing staple.

**Black-76 (futures option)** — `C = e^(-r T)[F N(d1) - K N(d2)]`. Adapts
Black-Scholes to options written on a forward or futures price. It is the market
standard for interest-rate caps/floors, swaptions, and commodity options.

**Forward price (cost of carry)** — `F = S e^((r - q) T)`. The no-arbitrage
forward price equals the spot grown at the financing rate net of any yield `q`. It
is a foundational replication identity: it must hold or a riskless arbitrage
exists.

### Fixed income

**Covered interest rate parity** — `F/S = (1 + r_d)/(1 + r_f)`. The forward
exchange rate is pinned by the interest-rate differential between two currencies.
It is one of the most reliable no-arbitrage conditions in international finance.

**Vasicek short-rate model** — `dr = a(b - r) dt + sigma dW`. Models the short
interest rate as mean-reverting with constant volatility. It was an early
tractable term-structure model and remains a reference point for interest-rate
modeling.

**Cox-Ingersoll-Ross model** — `dr = a(b - r) dt + sigma sqrt(r) dW`. A
mean-reverting short-rate model whose square-root diffusion keeps rates
non-negative. It is widely used in fixed income and (in the same form) in
stochastic-volatility models.

**Modified duration** — `ModDur = MacaulayDur / (1 + y/k)`. The percentage change
in a bond's price for a small change in yield. It is the primary first-order
measure of interest-rate risk.

**Bond convexity** — `C = (1/P) d2P/dy2`. The curvature of the price–yield
relationship, correcting duration for larger yield moves. It matters because it
captures the asymmetry that benefits bondholders when rates swing.

**Yield to maturity (IRR of bond)** — `P = sum CF_t / (1 + y)^t`. The single
discount rate that equates a bond's price to the present value of its cash flows.
It is the standard summary of a bond's return if held to maturity.

### Credit & asset pricing

**Merton structural credit model** — equity is a call option on the firm's assets
struck at its debt. Default occurs when assets fall below liabilities at maturity.
It is the foundation of structural credit risk and of distance-to-default
measures.

**Fama-French three-factor model** — `R - Rf = a + b(Rm-Rf) + s SMB + h HML`.
Explains asset returns by exposures to the market, a size factor (SMB), and a
value factor (HML). It is the empirical benchmark that reshaped asset pricing
after CAPM.

**Arbitrage pricing theory (APT)** — `E[R] = Rf + sum beta_k lambda_k`. Prices
assets by their exposures to several systematic risk factors, derived from
no-arbitrage rather than from a single market portfolio. It is the flexible,
multi-factor alternative to CAPM.

### Performance measurement

**Jensen's alpha** — `alpha = R_p - [Rf + beta(Rm - Rf)]`. The return earned
beyond what CAPM predicts for a portfolio's risk. It is the classic measure of
manager skill (or luck).

**Treynor ratio** — `T = (R_p - Rf) / beta_p`. Excess return per unit of
*systematic* risk (beta). It is relevant for ranking well-diversified portfolios
whose only remaining risk is market risk.

**Sortino ratio** — `S = (R_p - R_target) / downside deviation`. Like the Sharpe
ratio but penalizing only downside volatility relative to a target. It matters
when investors care about losses, not symmetric variation.

### Risk, econometrics & operations

**Value at Risk (parametric)** — `VaR = mu - z_alpha sigma`. The loss threshold
that will not be exceeded at a stated confidence level over a horizon. It became
the dominant regulatory and reporting risk metric, for all its known limitations.

**GARCH(1,1) variance** — `sigma_t^2 = omega + alpha e_{t-1}^2 + beta sigma_{t-1}^2`.
Models volatility that clusters and mean-reverts, with today's variance built
from yesterday's shock and variance. It is the workhorse of financial
econometrics and volatility forecasting.

**Rule of 72 (doubling time)** — `t_double = 72 / r%`. A mental-math estimate of
how long money takes to double at a given compound rate. Its relevance is
pedagogical and practical: it makes the power of compounding tangible.

**Dividend payout / retention** — `g = ROE * retention ratio`. A firm's
sustainable growth rate equals its return on equity times the fraction of
earnings retained. It is a core identity linking payout policy to growth.

**Economic order quantity (EOQ)** — `Q* = sqrt(2 D S / H)`. The order size that
minimizes the sum of ordering and holding costs. It is the founding result of
inventory theory and a staple of operations economics.

**Herfindahl-Hirschman index** — `HHI = sum s_i^2`. Market concentration measured
as the sum of squared market shares. It is a standard antitrust screen for market
power and merger review.
