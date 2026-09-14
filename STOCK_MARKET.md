# SLeeLa Stock Market Program

The repository now includes a deterministic **stock-market paper-trading program** at:

```text
impl/examples/stock_market.sleela
```

## Purpose

The program demonstrates how SLeeLa can model a small equity market and portfolio without requiring a network connection or a live market-data provider. The prices are intentionally fixed sample values so the example is repeatable and suitable for compiler and runtime testing.

## Market model

The sample market tracks four equities:

| Symbol | Sample Price | Opening Price | Position |
|---|---:|---:|---:|
| AAPL | $225.00 | $220.00 | 100 shares |
| MSFT | $510.00 | $500.00 | 50 shares |
| NVDA | $178.00 | $170.00 | 25 shares |
| TSLA | $330.00 | $320.00 | 40 shares |

The portfolio begins with **$100,000.00 cash** and the positions listed above.

## Program operations

The example implements:

- market price display;
- dollar price change calculation;
- percentage change calculation;
- position valuation;
- cash accounting;
- a guarded NVDA purchase;
- a guarded TSLA sale;
- before/after portfolio valuation;
- session profit/loss calculation.

The `buyNvidia()` and `sellTesla()` methods demonstrate basic trade validation: a purchase is rejected when available cash is insufficient, and a sale is rejected when the portfolio lacks the requested shares.

## Run

From the repository's implementation directory:

```sh
cd impl
make
./build/sleela run examples/stock_market.sleela
```

Validation without execution:

```sh
./build/sleela check examples/stock_market.sleela
```

## Design boundary

This is a **paper-trading example**, not a brokerage or live-market execution system. It does not connect to an exchange, submit real orders, hold credentials, or represent live prices. A future live-data adapter can be added separately without changing the basic portfolio accounting demonstrated here.

## SLeeLa features demonstrated

The program uses the versioned Wrapper™ format (`#sleela 1.0`), class fields, typed locals, methods, arithmetic, comparisons, conditionals, returns, and `print()`. It is intended to remain a small, readable example that exercises the same C/C++ execution core used by the other SLeeLa examples.
