# SLeeLa Stock Market Terminal

`stock-market.sh` is a clean Bash/ANSI terminal dashboard for the SLeeLa stock-market examples.

## Run

From the repository root:

```bash
chmod +x tools/stock-market/stock-market.sh
./tools/stock-market/stock-market.sh
```

Continuous display:

```bash
./tools/stock-market/stock-market.sh --watch
```

Help:

```bash
./tools/stock-market/stock-market.sh --help
```

## Display

The dashboard provides:

- AAPL, MSFT, NVDA, and TSLA watchlist
- price and percentage movement
- compact relative-movement bars
- session date and operating mode
- a clean ANSI terminal layout

The renderer intentionally uses ordinary Bash plus standard terminal escape sequences. It does not require a graphical desktop.

## Market-data boundary

The checked-in display values are a snapshot, not a promise of live quotes. A future market-data adapter can replace the values while preserving the terminal renderer. Any live adapter should validate ticker, timestamp, price, volume, and market status before passing data to the display or paper portfolio.

The program is observational/paper-trading software. It does not submit brokerage orders.

## SLeeLa market program

The SLeeLa-side market checker is at [`impl/examples/stock_market_live.sleela`](../../impl/examples/stock_market_live.sleela). The original deterministic example remains at [`impl/examples/stock_market.sleela`](../../impl/examples/stock_market.sleela).
