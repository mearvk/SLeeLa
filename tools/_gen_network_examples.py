#!/usr/bin/env python3
"""One-shot generator for the advantage/network-trades examples.

Emits a self-validating .sleela example per rung of the lumber->television
ladder, with EXACT hand-anchors computed here so the embedded validation always
matches runtime output. Not part of the build; kept in tools/ for reproducibility.
"""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
EX = os.path.join(ROOT, "lib", "advantage", "examples")


def fmt(x):
    """Human-facing number for comments/labels: integers without a point,
    else a short rounded decimal."""
    xr = round(float(x), 4)
    if xr == int(xr):
        return str(int(xr))
    return ("%.4f" % xr).rstrip("0").rstrip(".")


def dbl(x):
    """A valid SLeeLa DOUBLE literal (always has a decimal point)."""
    xr = round(float(x), 4)
    if xr == int(xr):
        return str(int(xr)) + ".0"
    return ("%.4f" % xr).rstrip("0")


# Each scenario: level/folder, file stem, good label, title rung index,
# venues [(name,bid,ask)], alpha, units, days, feeRate, carry(per unit per day),
# scienceRating, metaId, a one-line "ratio" note for the header.
S = [
    dict(level="novice", stem="lumber-trade", good="lumber", idx=1,
         venues=[("MillYard",340,350),("BigBoxA",345,360),("Regional",420,430)],
         alpha=0.5, units=40, days=10, fee=0.0, carry=0.0, sci=6, mid=10620,
         rung="the entry rung of the ladder: a raw structural commodity"),
    dict(level="novice", stem="cement-trade", good="cement", idx=2,
         venues=[("DepotWest",88,92),("DepotEast",90,95),("PortSilo",118,124)],
         alpha=0.5, units=200, days=8, fee=0.0, carry=0.0, sci=6, mid=10621,
         rung="a bulk building material, bought by the pallet"),

    dict(level="medium", stem="copper-wire-trade", good="copper-wire", idx=3,
         venues=[("MetalsHub",7.20,7.50),("SpoolCo",7.40,7.80),("ExportDesk",8.60,8.90)],
         alpha=0.5, units=1000, days=15, fee=0.01, carry=0.0, sci=7, mid=10622,
         rung="a refined industrial input; now with a settlement fee"),
    dict(level="medium", stem="steel-coil-trade", good="steel-coil", idx=4,
         venues=[("CoilYard",600,620),("ReRoller",610,640),("ShipBroker",760,780)],
         alpha=0.5, units=50, days=20, fee=0.0, carry=0.5, sci=7, mid=10623,
         rung="heavy stock where TIME costs real carry per unit per day"),
    dict(level="medium", stem="coffee-green-trade", good="coffee-green", idx=5,
         venues=[("FarmGate",3.80,4.00),("CoopDesk",4.10,4.30),("RoasterBuy",5.40,5.60)],
         alpha=0.5, units=2000, days=30, fee=0.005, carry=0.0, sci=7, mid=10624,
         rung="a soft commodity with seasonality baked into the spread"),
    dict(level="medium", stem="cotton-bales-trade", good="cotton-bales", idx=6,
         venues=[("GinHouse",72,75),("Warehouse",74,78),("MillBuyer",94,98)],
         alpha=0.5, units=300, days=25, fee=0.01, carry=0.04, sci=7, mid=10625,
         rung="both frictions together: fee AND carry on the same trade"),

    dict(level="advanced", stem="solar-panel-trade", good="solar-panel", idx=7,
         venues=[("FactoryA",180,190),("Distributor",195,205),("InstallerBuy",250,260)],
         alpha=0.6, units=400, days=45, fee=0.01, carry=0.0, sci=8, mid=10626,
         rung="a manufactured good; seller-leaning power (alpha=0.6)"),
    dict(level="advanced", stem="lithium-cell-trade", good="lithium-cell", idx=8,
         venues=[("CellFab",9.00,9.50),("PackMaker",9.80,10.20),("OEMBuyer",12.40,12.80)],
         alpha=0.6, units=5000, days=40, fee=0.01, carry=0.0, sci=8, mid=10627,
         rung="a high-volume cell; small unit price, large position"),
    dict(level="advanced", stem="semiconductor-trade", good="semiconductor", idx=9,
         venues=[("FoundryLot",40,44),("BrokerDesk",46,50),("BoardMaker",64,68)],
         alpha=0.65, units=2500, days=60, fee=0.012, carry=0.02, sci=8, mid=10628,
         rung="a scarce input: strong seller power and both frictions"),

    dict(level="professional", stem="server-rack-trade", good="server-rack", idx=10,
         venues=[("IntegratorA",8200,8400),("ChannelB",8600,8900),("DataCenter",11200,11600)],
         alpha=0.6, units=20, days=50, fee=0.015, carry=0.0, sci=9, mid=10629,
         rung="capital equipment; a few units, large ticket"),
    dict(level="professional", stem="smartphone-trade", good="smartphone", idx=11,
         venues=[("AssemblerA",320,340),("WholesaleB",360,380),("CarrierBuy",470,490)],
         alpha=0.6, units=3000, days=35, fee=0.02, carry=0.0, sci=9, mid=10630,
         rung="a finished consumer device; distribution-scale volume"),
    dict(level="professional", stem="television-trade", good="television", idx=12,
         venues=[("PanelFab",260,280),("DistB",300,320),("RetailBuy",430,460)],
         alpha=0.6, units=1200, days=42, fee=0.02, carry=0.05, sci=9, mid=10631,
         rung="the top rung: a finished consumer television, fee AND carry"),
]

CLASS_NAMES = {}  # stem -> ClassName


def class_name(stem):
    parts = stem.replace("-", " ").split()
    return "".join(p.capitalize() for p in parts)


META = """  < ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- >
  String metaVersion = "1.0.0";
  int metaId = {mid};
  String metaAuthor = "Kiro Agent";
  String metaDate = "2026-11-08";
  String metaCopyright = "SLeeLa project";
  String metaTrademark = "none";
  String metaCorporation = "none";
  < metaOrigin: National | <nation> | International | Galactic >
  String metaOrigin = "International";
  String metaBirthDate = "2026-11-08";
  int metaScienceRating = {sci};
  String metaNationalSchedule = "none";

  String getMetaVersion() {{ return metaVersion; }}
  void setMetaVersion(String v) {{ metaVersion = v; }}
  int getMetaId() {{ return metaId; }}
  void setMetaId(int v) {{ metaId = v; }}
  String getAuthor() {{ return metaAuthor; }}
  void setAuthor(String v) {{ metaAuthor = v; }}
  String getDate() {{ return metaDate; }}
  void setDate(String v) {{ metaDate = v; }}
  String getCopyright() {{ return metaCopyright; }}
  void setCopyright(String v) {{ metaCopyright = v; }}
  String getTrademark() {{ return metaTrademark; }}
  void setTrademark(String v) {{ metaTrademark = v; }}
  String getCorporation() {{ return metaCorporation; }}
  void setCorporation(String v) {{ metaCorporation = v; }}
  String getOrigin() {{ return metaOrigin; }}
  void setOrigin(String v) {{ metaOrigin = v; }}
  String getBirthDate() {{ return metaBirthDate; }}
  void setBirthDate(String v) {{ metaBirthDate = v; }}
  int getScienceRating() {{ return metaScienceRating; }}
  void setScienceRating(int v) {{ metaScienceRating = v; }}
  String getNationalSchedule() {{ return metaNationalSchedule; }}
  void setNationalSchedule(String v) {{ metaNationalSchedule = v; }}
"""


def anchors(s):
    bids = [v[1] for v in s["venues"]]
    asks = [v[2] for v in s["venues"]]
    best_ask = min(asks)
    best_bid = max(bids)
    spread = best_bid - best_ask
    alpha = s["alpha"]
    price = best_ask + alpha * (best_bid - best_ask)
    units = s["units"]
    buy = best_ask
    sell = price
    days = s["days"]
    fee = s["fee"]
    carry = s["carry"]
    money_in = units * buy
    gross = units * sell
    fees = fee * (money_in + gross)
    carry_cost = carry * units * days
    cash_out = gross - fees - carry_cost
    profit = cash_out - money_in
    roc = profit / money_in if money_in else 0.0
    R = lambda v: round(float(v), 4)
    return dict(best_ask=R(best_ask), best_bid=R(best_bid), spread=R(spread),
                price=R(price), money_in=R(money_in), gross=R(gross), fees=R(fees),
                carry_cost=R(carry_cost), cash_out=R(cash_out), profit=R(profit),
                roc=R(roc), buy=R(buy), sell=R(sell), units=units, days=days,
                fee=fee, carry=carry)


def check_clean(s, a):
    """Reject any scenario whose anchors don't print cleanly (we want exact
    string matches, so keep values at values that print without float noise)."""
    for key in ("best_ask","best_bid","spread","price","money_in","cash_out","profit"):
        v = a[key]
        if abs(v - round(v, 4)) > 1e-9:
            raise SystemExit(f"{s['stem']}: {key}={v} does not print cleanly")


def gen(s):
    cn = class_name(s["stem"])
    a = anchors(s)
    check_clean(s, a)
    idx = s["idx"]
    good = s["good"]
    level = s["level"]
    venues_src = "\n".join(
        f'    scout.addQuote("{v[0]}", {dbl(v[1])}, {dbl(v[2])});'
        for v in s["venues"])

    # tolerances: 0.01 so printed values validate within a cent
    tol = "0.01"

    header = f"""<< lib/advantage/examples/{level}/network-trades/{s['stem']}.sleela
   SLeeLa Standard Library Definition.
   Definition: NETWORK-TRADE {idx:02d} of 12 -- {good.upper()}. Scout a network of
   venue quotes, pick the cheapest buy and dearest sell, negotiate a fair
   clearing price, and emit a cash receipt. Rung: {s['rung']}. >>
#sleela 3.0
material;
artifact;
pure;
gasoline;
<< THE RATIO ({good}). Bargaining-power alpha = {fmt(s['alpha'])} (seller share).
   Best buy ask = {fmt(a['best_ask'])}; best sell bid = {fmt(a['best_bid'])}; spread = {fmt(a['spread'])}.
   Clearing P* = ask + alpha*(bid-ask) = {fmt(a['price'])}.
   Cashflow: units={fmt(a['units'])} buy={fmt(a['buy'])} sell={fmt(a['sell'])} days={fmt(a['days'])}
   feeRate={fmt(a['fee'])} carry={fmt(a['carry'])} -> in={fmt(a['money_in'])} out={fmt(a['cash_out'])}
   profit={fmt(a['profit'])}. SLMarketScout / SLNegotiationRound / SLCashflow
   resolve from lib/advantage/network by the /lib scan. See {s['stem']}.md. >>"""

    body = f"""class {cn} {{
{META.format(mid=s['mid'], sci=s['sci'])}
  boolean near(double a, double b, double tol) {{
    double d = a - b;
    if (d < 0.0) {{ d = 0.0 - d; }}
    return d < tol;
  }}

  void main() {{
    print("== network-trade {idx:02d}: {good.upper()} ==");

    SLMarketScout scout = new SLMarketScout();
    scout.forGood("{good}");
{venues_src}
    print("  scout : " + scout.describe());

    double ask = scout.bestAsk();
    double bid = scout.bestBid();

    SLNegotiationRound neg = new SLNegotiationRound();
    neg.configure(ask, bid, {dbl(s['alpha'])});
    double price = neg.clearingPrice();
    print("  deal  : " + neg.describe());

    SLCashflow cf = new SLCashflow();
    cf.configure({dbl(a['units'])}, ask, price, {dbl(a['days'])});
    cf.setFrictions({dbl(a['fee'])}, {dbl(a['carry'])});
    print("  money : " + cf.receipt("{good}"));

    print("");
    print("== validation ==");
    boolean okAsk = near(ask, {dbl(a['best_ask'])}, {tol});
    boolean okBid = near(bid, {dbl(a['best_bid'])}, {tol});
    boolean okSpread = near(scout.spread(), {dbl(a['spread'])}, {tol});
    boolean okPrice = near(price, {dbl(a['price'])}, {tol});
    boolean okIn = near(cf.moneyIn(), {dbl(a['money_in'])}, {tol});
    boolean okOut = near(cf.cashOut(), {dbl(a['cash_out'])}, {tol});
    boolean okProfit = near(cf.profit(), {dbl(a['profit'])}, {tol});
    print("  best ask   ? " + okAsk);
    print("  best bid   ? " + okBid);
    print("  spread     ? " + okSpread);
    print("  clearing   ? " + okPrice);
    print("  money in   ? " + okIn);
    print("  cash out   ? " + okOut);
    print("  profit     ? " + okProfit);
    if (okAsk) {{ if (okBid) {{ if (okSpread) {{ if (okPrice) {{
      if (okIn) {{ if (okOut) {{ if (okProfit) {{
        print("ALL CHECKS PASSED");
      }} }} }} }} }} }} }}
  }}
}}
"""
    out_dir = os.path.join(EX, level, "network-trades")
    os.makedirs(out_dir, exist_ok=True)
    path = os.path.join(out_dir, s["stem"] + ".sleela")
    with open(path, "w") as f:
        f.write(header + "\n" + body)
    return path, a, cn


# Per-level "class consensus" for safe arrival over use and ethical application.
CONSENSUS = {
    "novice": (
        "Novice tier. The trade is modelled with no fees and an even 50/50 "
        "split so the mechanics are visible. Consensus for safe arrival: a "
        "novice must treat the cleared price as an INDICATION, re-verify both "
        "venue quotes live before committing capital, and never deploy money "
        "that cannot be lost."),
    "medium": (
        "Medium tier. Real frictions enter -- a settlement fee and/or carry "
        "over time -- so the receipt shows what actually arrives, not the "
        "headline spread. Consensus for safe arrival: size the position to the "
        "net (post-friction) margin, confirm delivery terms, and hold only as "
        "long as the carry math stays positive."),
    "advanced": (
        "Advanced tier. Seller-leaning bargaining power (alpha > 0.5) and "
        "larger positions mean concentration risk. Consensus for safe arrival: "
        "diversify counterparties, cap single-venue exposure, and stress the "
        "receipt against a quote that moves against you before you clear."),
    "professional": (
        "Professional tier. Large tickets, distribution-scale volume, and both "
        "frictions at once. Consensus for safe arrival: require written terms, "
        "escrow or staged settlement, independent price discovery across the "
        "whole venue network, and a documented audit trail (the receipt is the "
        "minimum record, not the maximum)."),
}


def gen_md(s, a, cn):
    level = s["level"]
    good = s["good"]
    idx = s["idx"]
    venues_tbl = "\n".join(
        f"| {v[0]} | {fmt(v[1])} | {fmt(v[2])} |" for v in s["venues"])
    md = f"""# {s['stem']}.md -- network-trade {idx:02d}: {good}

Companion to [`{s['stem']}.sleela`](./{s['stem']}.sleela). Same name, same trade,
told for a human reader.

> `artifact; pure; gasoline;` -- an illustrative Advantage(TM) example. The
> numbers are **best-guess scenario values**, not a live market. This is not
> financial, investment, or trading advice.

## The rung

{good.capitalize()} sits on the socio-economic ladder as {s['rung']}. The example
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
{venues_tbl}

- Best **buy** (lowest ask): **{fmt(a['best_ask'])}**
- Best **sell** (highest bid): **{fmt(a['best_bid'])}**
- Cross-venue **spread** (the social-economic ratio in raw form): **{fmt(a['spread'])}**

## The negotiation (the performant part)

Clearing is a **closed-form** step, not a search loop:

```
P* = sellerMin + alpha * (buyerMax - sellerMin)
   = {fmt(a['best_ask'])} + {fmt(s['alpha'])} * ({fmt(a['best_bid'])} - {fmt(a['best_ask'])})
   = {fmt(a['price'])}
```

`alpha = {fmt(s['alpha'])}` is the bargaining-power index -- the seller's share of
the surplus. Because it is closed form, a whole fan of counterparties clears in
`O(parties)` with no iteration; that is what makes network negotiation
performant at scale.

## Engineering grease: money in, a trade, time, cash out

The receipt is the ledger identity of one round trip:

| stage | quantity | value |
|-------|----------|------:|
| money IN (capital deployed) | {fmt(a['units'])} units @ {fmt(a['buy'])} | **{fmt(a['money_in'])}** |
| the TRADE (cleared price) | sell @ {fmt(a['sell'])} | gross {fmt(a['gross'])} |
| TIME (carry over {fmt(a['days'])} days) | {fmt(s['carry'])} per unit-day | {fmt(a['carry_cost'])} |
| friction (settlement fee) | rate {fmt(s['fee'])} | {fmt(a['fees'])} |
| cash OUT (net receipt) | gross - fees - carry | **{fmt(a['cash_out'])}** |
| profit | out - in | **{fmt(a['profit'])}** |
| return on capital | profit / in | {fmt(a['roc'])} |

```
cashOut = units*sell - feeRate*(units*buy + units*sell) - carry*units*days
```

## Class consensus -- safe arrival over use, ethical application

{CONSENSUS[level]}

Ethical floor for every tier: trade only in lawful goods with willing
counterparties; never corner a market that others depend on for shelter, food,
power, or health; keep an honest receipt; and prefer a fair split (a positive
buyer surplus) over squeezing every cent, because a network you can return to is
worth more than one trade won outright.

## Run it

```sh
./working-binaries/sleela run lib/advantage/examples/{level}/network-trades/{s['stem']}.sleela
```

Expected: the scout line, the negotiation line, the receipt line, seven `true`
checks, and a final `ALL CHECKS PASSED`.
"""
    path = os.path.join(EX, level, "network-trades", s["stem"] + ".md")
    with open(path, "w") as f:
        f.write(md)
    return path


if __name__ == "__main__":
    for s in S:
        path, a, cn = gen(s)
        mdp = gen_md(s, a, cn)
        rel = os.path.relpath(path, ROOT)
        print(f"{rel}  class={cn}  in={fmt(a['money_in'])} out={fmt(a['cash_out'])} profit={fmt(a['profit'])}")
