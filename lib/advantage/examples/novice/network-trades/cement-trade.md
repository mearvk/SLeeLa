# cement-trade.md -- network-trade 02: cement

Companion to [`cement-trade.sleela`](./cement-trade.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

Cement sits on the socio-economic ladder as a bulk building material, bought by the pallet. The example
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
| DepotWest | 88 | 92 |
| DepotEast | 90 | 95 |
| PortSilo | 118 | 124 |

- Best **buy** (lowest ask): **92**
- Best **sell** (highest bid): **118**
- Cross-venue **spread** (the social-economic ratio in raw form): **26**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = 92 + 0.5 * (118 - 92)
   = 105
```

`alpha = 0.5` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | 200 units @ 92 | **18400** |
| the TRADE (cleared price) | sell @ 105 | gross 21000 |
| TIME (carry over 8 days) | 0 per unit-day | 0 |
| friction (settlement fee) | rate 0 | 0 |
| cash OUT (net receipt) | gross - fees - carry | **21000** |
| profit | out - in | **2600** |
| return on capital | profit / in | 0.1413 |

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
./working-binaries/sleela run lib/advantage/examples/novice/network-trades/cement-trade.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
