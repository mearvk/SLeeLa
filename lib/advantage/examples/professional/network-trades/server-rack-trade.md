# server-rack-trade.md -- network-trade 10: server-rack

Companion to [`server-rack-trade.sleela`](./server-rack-trade.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

Server-rack sits on the socio-economic ladder as capital equipment; a few units, large ticket. The example
scouts a small network of venues, picks the cheapest place to **buy** and the
dearest to **sell**, negotiates a fair clearing price, and writes a receipt.

## Where the "internet search" really happens

The example fills its quote table from a canned list so the demo is deterministic
and self-validating. The `SLMarketScout` surface (`addQuote / bestAsk / bestBid /
spread`) is **identical** whether those rows came from a canned table or a live
fetch, so in production the rows would be filled by the real transport layer
(`lib/advantage/transport`: HTTPS / SFTP / SSH acquire over a keystore-pinned
channel) reading venue price feeds. Nothing downstream changes. No live network
call is made from inside the SLeeLa VM.

## The venue network (best guesses)

| venue | bid (sell-to) | ask (buy-from) |
|-------|--------------:|---------------:|
| IntegratorA | 8200 | 8400 |
| ChannelB | 8600 | 8900 |
| DataCenter | 11200 | 11600 |

- Best **buy** (lowest ask): **8400**
- Best **sell** (highest bid): **11200**
- Cross-venue **spread** (the social-economic ratio in raw form): **2800**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = 8400 + 0.6 * (11200 - 8400)
   = 10080
```

`alpha = 0.6` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | 20 units @ 8400 | **168000** |
| the TRADE (cleared price) | sell @ 10080 | gross 201600 |
| TIME (carry over 50 days) | 0 per unit-day | 0 |
| friction (settlement fee) | rate 0.015 | 5544 |
| cash OUT (net receipt) | gross - fees - carry | **196056** |
| profit | out - in | **28056** |
| return on capital | profit / in | 0.167 |

```
cashOut = units*sell - feeRate*(units*buy + units*sell) - carry*units*days
```

## Class consensus -- safe arrival over use, ethical application

Professional tier. Large tickets, distribution-scale volume, and both frictions at once. Consensus for safe arrival: require written terms, escrow or staged settlement, independent price discovery across the whole venue network, and a documented audit trail (the receipt is the minimum record, not the maximum).

Ethical floor for every tier: trade only in lawful goods with willing
counterparties; never corner a market that others depend on for shelter, food,
power, or health; keep an honest receipt; and prefer a fair split (a positive
buyer surplus) over squeezing every cent, because a network you can return to is
worth more than one trade won outright.

## Run it

```sh
./working-binaries/sleela run lib/advantage/examples/professional/network-trades/server-rack-trade.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
