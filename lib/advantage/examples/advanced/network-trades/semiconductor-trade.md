# semiconductor-trade.md -- network-trade 09: semiconductor

Companion to [`semiconductor-trade.sleela`](./semiconductor-trade.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

Semiconductor sits on the socio-economic ladder as a scarce input: strong seller power and both frictions. The example
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
| FoundryLot | 40 | 44 |
| BrokerDesk | 46 | 50 |
| BoardMaker | 64 | 68 |

- Best **buy** (lowest ask): **44**
- Best **sell** (highest bid): **64**
- Cross-venue **spread** (the social-economic ratio in raw form): **20**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = 44 + 0.65 * (64 - 44)
   = 57
```

`alpha = 0.65` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | 2500 units @ 44 | **110000** |
| the TRADE (cleared price) | sell @ 57 | gross 142500 |
| TIME (carry over 60 days) | 0.02 per unit-day | 3000 |
| friction (settlement fee) | rate 0.012 | 3030 |
| cash OUT (net receipt) | gross - fees - carry | **136470** |
| profit | out - in | **26470** |
| return on capital | profit / in | 0.2406 |

```
cashOut = units*sell - feeRate*(units*buy + units*sell) - carry*units*days
```

## Class consensus -- safe arrival over use, ethical application

Advanced tier. Seller-leaning bargaining power (alpha > 0.5) and larger positions mean concentration risk. Consensus for safe arrival: diversify counterparties, cap single-venue exposure, and stress the receipt against a quote that moves against you before you clear.

Ethical floor for every tier: trade only in lawful goods with willing
counterparties; never corner a market that others depend on for shelter, food,
power, or health; keep an honest receipt; and prefer a fair split (a positive
buyer surplus) over squeezing every cent, because a network you can return to is
worth more than one trade won outright.

## Run it

```sh
./working-binaries/sleela run lib/advantage/examples/advanced/network-trades/semiconductor-trade.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
