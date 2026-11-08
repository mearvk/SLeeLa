# lumber-trade.md -- network-trade 01: lumber

Companion to [`lumber-trade.sleela`](./lumber-trade.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

Lumber sits on the socio-economic ladder as the entry rung of the ladder: a raw structural commodity. The example
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
| MillYard | 340 | 350 |
| BigBoxA | 345 | 360 |
| Regional | 420 | 430 |

- Best **buy** (lowest ask): **350**
- Best **sell** (highest bid): **420**
- Cross-venue **spread** (the social-economic ratio in raw form): **70**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = 350 + 0.5 * (420 - 350)
   = 385
```

`alpha = 0.5` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | 40 units @ 350 | **14000** |
| the TRADE (cleared price) | sell @ 385 | gross 15400 |
| TIME (carry over 10 days) | 0 per unit-day | 0 |
| friction (settlement fee) | rate 0 | 0 |
| cash OUT (net receipt) | gross - fees - carry | **15400** |
| profit | out - in | **1400** |
| return on capital | profit / in | 0.1 |

```
cashOut = units*sell - feeRate*(units*buy + units*sell) - carry*units*days
```

## Class consensus -- safe arrival over use, ethical application

Novice tier. The trade is modelled with no fees and an even 50/50 split so the mechanics are visible. Consensus for safe arrival: a novice must treat the cleared price as an INDICATION, re-verify both venue quotes live before committing capital, and never deploy money that cannot be lost.

Ethical floor for every tier: trade only in lawful goods with willing
counterparties; never corner a market that others depend on for shelter, food,
power, or health; keep an honest receipt; and prefer a fair split (a positive
buyer surplus) over squeezing every cent, because a network you can return to is
worth more than one trade won outright.

## Run it

```sh
./working-binaries/sleela run lib/advantage/examples/novice/network-trades/lumber-trade.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
