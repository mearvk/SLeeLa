artifact;
pure;
gasoline;

# network-trades (advanced)

Manufactured goods, seller-leaning bargaining power (alpha > 0.5), larger
positions, and concentration risk. Same `SLMarketScout` ->
`SLNegotiationRound` -> `SLCashflow` flow.

| Example | Good | alpha | Receipt (in -> out, profit) |
|---------|------|------:|-----------------------------|
| `solar-panel-trade.sleela` | solar panels | 0.6 | 76000 -> 88736, +12736 |
| `lithium-cell-trade.sleela` | lithium cells | 0.6 | 47500 -> 55163, +7663 |
| `semiconductor-trade.sleela` | semiconductors | 0.65 | 110000 -> 136470, +26470 |

Each `.sleela` has an equally-named `.md` including the per-tier consensus:
diversify counterparties, cap single-venue exposure, and stress the receipt
against an adverse quote before clearing.

> Illustrative Advantage(TM) examples with best-guess numbers. Not financial
> advice.

```sh
./working-binaries/sleela run lib/advantage/examples/advanced/network-trades/semiconductor-trade.sleela
```
