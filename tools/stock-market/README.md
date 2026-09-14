# SLeeLa Stock Market — C++ Engine + SLeeLa Layer

This directory contains a two-layer stock-market analysis design.

```text
SLeeLa market session
        |
        v
C++ analysis / graphing engine
        |
        v
ANSI terminal dashboard
```

## C++ Engine

The native C++ layer provides:

- quote structures;
- mean, minimum, maximum, change percentage, and volatility calculations;
- terminal sparkline generation;
- ANSI dashboard rendering.

Build the demonstration from this directory:

```bash
g++ -std=c++17 -O2 -Wall -Wextra \
    market_engine.cpp market_engine_demo.cpp \
    -o stock-market-cpp

./stock-market-cpp
```

It is terminal-native and requires no graphical desktop library.

## SLeeLa Layer

The SLeeLa source is:

```text
../../impl/examples/stock_market_analysis.sleela
```

Build SLeeLa from the `impl` directory:

```bash
cd ../../impl
make
./build/sleela check examples/stock_market_analysis.sleela
./build/sleela run examples/stock_market_analysis.sleela
```

SLeeLa provides the higher-level market session, portfolio calculations, and interpretation while the C++ layer handles numerical analysis and terminal graphing.

## Existing Bash Dashboard

The original ANSI dashboard remains available:

```bash
chmod +x stock-market.sh
./stock-market.sh
./stock-market.sh --watch
```

## Data Boundary

The checked-in demonstration values are observations/sample data. The C++ engine deliberately does not contain credentials, brokerage access, or real order execution. A future market-data adapter can feed fresh quotes into the same `Quote` and `Point` structures without changing the analysis model.

The architecture separates:

1. market-data acquisition;
2. numerical analysis;
3. terminal graphing;
4. SLeeLa market/session logic;
5. any future trading integration.

No real trade is submitted by these examples.
