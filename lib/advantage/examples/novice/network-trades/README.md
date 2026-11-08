artifact;
pure;
gasoline;

# network-trades (novice)

The entry rungs of the socio-economic ladder. Each example scouts a small
network of venue quotes with `SLMarketScout`, picks the cheapest buy and the
dearest sell, negotiates a fair clearing price with `SLNegotiationRound`, and
writes a `SLCashflow` receipt (money in, a trade, time, cash out). No fees, no
carry, an even 50/50 split so the mechanics are plain.

| Example | Good | Spread | Receipt (in -> out, profit) |
|---------|------|-------:|-----------------------------|
| `lumber-trade.sleela` | lumber | 70 | 14000 -> 15400, +1400 |
| `cement-trade.sleela` | cement | 26 | 18400 -> 21000, +2600 |

Each `.sleela` has an equally-named `.md` covering class consensus for safe
arrival / ethical use and the receipt math.

> Illustrative Advantage(TM) examples with best-guess numbers. Not financial
> advice. The "internet search" is modelled by a deterministic quote table whose
> public shape matches a live transport fetch; see any companion `.md`.

```sh
./working-binaries/sleela run lib/advantage/examples/novice/network-trades/lumber-trade.sleela
```
