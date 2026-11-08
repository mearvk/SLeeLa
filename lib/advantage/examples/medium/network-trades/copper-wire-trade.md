# copper-wire-trade.md -- network-trade 03: copper-wire

Companion to [`copper-wire-trade.sleela`](./copper-wire-trade.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

Copper-wire sits on the socio-economic ladder as a refined industrial input; now with a settlement fee. The example
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
| MetalsHub | 7.2 | 7.5 |
| SpoolCo | 7.4 | 7.8 |
| ExportDesk | 8.6 | 8.9 |

- Best **buy** (lowest ask): **7.5**
- Best **sell** (highest bid): **8.6**
- Cross-venue **spread** (the social-economic ratio in raw form): **1.1**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = 7.5 + 0.5 * (8.6 - 7.5)
   = 8.05
```

`alpha = 0.5` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | 1000 units @ 7.5 | **7500** |
| the TRADE (cleared price) | sell @ 8.05 | gross 8050 |
| TIME (carry over 15 days) | 0 per unit-day | 0 |
| friction (settlement fee) | rate 0.01 | 155.5 |
| cash OUT (net receipt) | gross - fees - carry | **7894.5** |
| profit | out - in | **394.5** |
| return on capital | profit / in | 0.0526 |

```
cashOut = units*sell - feeRate*(units*buy + units*sell) - carry*units*days
```

## Class consensus -- safe arrival over use, ethical application

Medium tier. Real frictions enter -- a settlement fee and/or carry over time -- so the receipt shows what actually arrives, not the headline spread. Consensus for safe arrival: size the position to the net (post-friction) margin, confirm delivery terms, and hold only as long as the carry math stays positive.

Ethical floor for every tier: trade only in lawful goods with willing
counterparties; never corner a market that others depend on for shelter, food,
power, or health; keep an honest receipt; and prefer a fair split (a positive
buyer surplus) over squeezing every cent, because a network you can return to is
worth more than one trade won outright.

## Run it

```sh
./working-binaries/sleela run lib/advantage/examples/medium/network-trades/copper-wire-trade.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
