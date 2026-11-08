# smartphone-trade.md -- network-trade 11: smartphone

Companion to [`smartphone-trade.sleela`](./smartphone-trade.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

Smartphone sits on the socio-economic ladder as a finished consumer device; distribution-scale volume. The example
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
| AssemblerA | 320 | 340 |
| WholesaleB | 360 | 380 |
| CarrierBuy | 470 | 490 |

- Best **buy** (lowest ask): **340**
- Best **sell** (highest bid): **470**
- Cross-venue **spread** (the social-economic ratio in raw form): **130**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = 340 + 0.6 * (470 - 340)
   = 418
```

`alpha = 0.6` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | 3000 units @ 340 | **1020000** |
| the TRADE (cleared price) | sell @ 418 | gross 1254000 |
| TIME (carry over 35 days) | 0 per unit-day | 0 |
| friction (settlement fee) | rate 0.02 | 45480 |
| cash OUT (net receipt) | gross - fees - carry | **1208520** |
| profit | out - in | **188520** |
| return on capital | profit / in | 0.1848 |

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
./working-binaries/sleela run lib/advantage/examples/professional/network-trades/smartphone-trade.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
