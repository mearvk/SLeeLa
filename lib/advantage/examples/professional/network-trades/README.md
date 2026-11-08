artifact;
pure;
gasoline;

# network-trades (professional)

The top rungs: large tickets, distribution-scale volume, and both frictions at
once. The receipt is the minimum audit record, not the maximum.

| Example | Good | Receipt (in -> out, profit) |
|---------|------|-----------------------------|
| `server-rack-trade.sleela` | server racks | 168000 -> 196056, +28056 |
| `smartphone-trade.sleela` | smartphones | 1020000 -> 1208520, +188520 |
| `television-trade.sleela` | televisions | 336000 -> 425880, +89880 |

Each `.sleela` has an equally-named `.md` including the per-tier consensus:
require written terms, escrow or staged settlement, independent price discovery
across the whole venue network, and a documented audit trail.

The `television-trade` example is the ladder's top rung -- a finished consumer
television, with both a settlement fee and carry over time.

> Illustrative Advantage(TM) examples with best-guess numbers. Not financial
> advice.

```sh
./working-binaries/sleela run lib/advantage/examples/professional/network-trades/television-trade.sleela
```
