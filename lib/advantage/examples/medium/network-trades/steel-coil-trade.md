# steel-coil-trade.md -- network-trade 04: steel-coil

Companion to [`steel-coil-trade.sleela`](./steel-coil-trade.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

Steel-coil sits on the socio-economic ladder as heavy stock where TIME costs real carry per unit per day. The example
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
| CoilYard | 600 | 620 |
| ReRoller | 610 | 640 |
| ShipBroker | 760 | 780 |

- Best **buy** (lowest ask): **620**
- Best **sell** (highest bid): **760**
- Cross-venue **spread** (the social-economic ratio in raw form): **140**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = 620 + 0.5 * (760 - 620)
   = 690
```

`alpha = 0.5` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | 50 units @ 620 | **31000** |
| the TRADE (cleared price) | sell @ 690 | gross 34500 |
| TIME (carry over 20 days) | 0.5 per unit-day | 500 |
| friction (settlement fee) | rate 0 | 0 |
| cash OUT (net receipt) | gross - fees - carry | **34000** |
| profit | out - in | **3000** |
| return on capital | profit / in | 0.0968 |

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
./working-binaries/sleela run lib/advantage/examples/medium/network-trades/steel-coil-trade.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
