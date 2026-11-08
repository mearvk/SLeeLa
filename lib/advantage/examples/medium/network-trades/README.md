artifact;
pure;
gasoline;

# network-trades (medium)

The same scout -> negotiate -> receipt flow, now with real frictions: a
settlement fee, carry over time, or both. The receipt shows what actually
arrives, not the headline spread.

| Example | Good | Friction | Receipt (in -> out, profit) |
|---------|------|----------|-----------------------------|
| `copper-wire-trade.sleela` | copper wire | fee 1% | 7500 -> 7894.5, +394.5 |
| `steel-coil-trade.sleela` | steel coil | carry 0.5/unit-day | 31000 -> 34000, +3000 |
| `coffee-green-trade.sleela` | green coffee | fee 0.5% | 8000 -> 9313, +1313 |
| `cotton-bales-trade.sleela` | cotton bales | fee 1% + carry | 22500 -> 24571.5, +2071.5 |

Each `.sleela` has an equally-named `.md`.

> Illustrative Advantage(TM) examples with best-guess numbers. Not financial
> advice.

```sh
./working-binaries/sleela run lib/advantage/examples/medium/network-trades/steel-coil-trade.sleela
```
