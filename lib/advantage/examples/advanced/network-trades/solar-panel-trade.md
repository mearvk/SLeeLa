# solar-panel-trade.md -- network-trade 07: solar-panel

Companion to [`solar-panel-trade.sleela`](./solar-panel-trade.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

Solar-panel sits on the socio-economic ladder as a manufactured good; seller-leaning power (alpha=0.6). The example
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
| FactoryA | 180 | 190 |
| Distributor | 195 | 205 |
| InstallerBuy | 250 | 260 |

- Best **buy** (lowest ask): **190**
- Best **sell** (highest bid): **250**
- Cross-venue **spread** (the social-economic ratio in raw form): **60**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = 190 + 0.6 * (250 - 190)
   = 226
```

`alpha = 0.6` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | 400 units @ 190 | **76000** |
| the TRADE (cleared price) | sell @ 226 | gross 90400 |
| TIME (carry over 45 days) | 0 per unit-day | 0 |
| friction (settlement fee) | rate 0.01 | 1664 |
| cash OUT (net receipt) | gross - fees - carry | **88736** |
| profit | out - in | **12736** |
| return on capital | profit / in | 0.1676 |

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
./working-binaries/sleela run lib/advantage/examples/advanced/network-trades/solar-panel-trade.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
