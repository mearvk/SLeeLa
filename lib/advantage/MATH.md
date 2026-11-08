# Advantage Trade Mathematics — a Math subject

> Bilateral trade economics for the `advantage` package. Treat a **buyer** and
> a **seller** as one coordinated system: first maximize the total surplus,
> then decide how to split it, with an extra model for perishable/stochastic
> demand.
>
> Implemented as SLeeLa subject classes under
> [`lib/advantage/math/`](./math) and validated end-to-end by
> [`math/trade-math-demo.sleela`](./math/trade-math-demo.sleela).

| Subject | Class | Scenario |
|---------|-------|----------|
| Joint Profit Maximization | `SLJointProfitMax` | Fixed demand — lock in maximum efficiency (`Q*`) |
| Nash Bargaining | `SLNashBargaining` | Variable pricing — negotiate contract terms from a known surplus |
| Bilateral Newsvendor | `SLBilateralNewsvendor` | Perishable / seasonal — avoid stockouts or excess holding waste |
| Shared math helpers | `SLTradeMath` | `expD`, `lnD`, `powAB`, `sqrtD` (no cross-package imports) |

Throughout, the buyer-revenue and seller-cost curves are modelled as
**quadratics**, so every optimum and split is a closed form and exactly
checkable:

```
R_B(Q) = rbA·Q − rbB·Q²      buyer revenue / utility (concave)
C_S(Q) = csA·Q + csB·Q²      seller cost (convex)
```

The worked example used in every section below fixes
`R_B(Q) = 100Q − Q²` and `C_S(Q) = 20Q + Q²`.

---

## 1. Joint Profit Maximization — the Integrated Surplus Formula

When costs are known on **both** ends, treat buyer and seller as a single
coordinated system and maximize the total economic surplus *before* dividing
it.

**Definition.**

```
Π_total(Q) = R_B(Q) − C_S(Q)
```

- `Q` — inventory / order quantity.
- `R_B(Q)` — the buyer's revenue (or utility valuation) from `Q` units.
- `C_S(Q)` — the seller's total cost to produce/supply `Q` units.
- `Π_total(Q)` — the total available pool of profit.

**Decision rule.** The optimal volume `Q*` is where marginal revenue equals
marginal supply cost:

```
dR_B/dQ = dC_S/dQ
```

For the quadratic curves this is linear in `Q`:

```
rbA − 2·rbB·Q  =  csA + 2·csB·Q
⇒  Q* = (rbA − csA) / (2·(rbB + csB))
```

**Sketch — the surplus is a cap; its peak sits at `Q*`:**

```
Π_total(Q)
  │           ___
  │        ,-'   `-.
  │      ,'         `.
  │    ,'    peak =   `.
  │   /   Π_total(Q*)    \
  │  /         |          \
  │ /          |           \
  └─────────── Q* ───────────── Q
  0
   MR > MC  →  |  ←  MR < MC
   (expand)      (contract)
```

To the left of `Q*`, each extra unit earns more than it costs (`MR > MC`) — keep
expanding. To the right, each extra unit costs more than it earns — pull back.
The peak is the most total value the relationship can create.

**Example** (`R_B = 100Q − Q²`, `C_S = 20Q + Q²`):

```
Q* = (100 − 20) / (2·(1 + 1)) = 80 / 4 = 20
Π_total(20) = (100·20 − 20²) − (20·20 + 20²)
            = (2000 − 400) − (400 + 400)
            = 1600 − 800 = 800
MR(20) = 100 − 2·20 = 60   MC(20) = 20 + 2·20 = 60   ✓ equal
```

```sleela
SLJointProfitMax jpm = new SLJointProfitMax();
jpm.configureCurves(100.0, 1.0, 20.0, 1.0);
double qstar = jpm.optimalQuantity();   // 20
double pool  = jpm.maxJointProfit();    // 800
```

---

## 2. Bilateral Monopoly & the Nash Bargaining Game

Once the optimal total profit is fixed at `Q*`, **how it is split** depends on
the wholesale price `P`. The individual profit curves are:

```
Seller:  Π_S(P, Q) = P·Q − C_S(Q)
Buyer:   Π_B(P, Q) = R_B(Q) − P·Q
```

At a fixed `Q*`, raising `P` moves value from buyer to seller one-for-one — it
does not change the size of the pool, only who holds it.

**Definition — the Nash Bargaining Solution** picks the price that maximizes
the geometric product of each party's surplus over its disagreement
(fallback) point:

```
max_P  [Π_S(P,Q*) − Π_S⁰]^α · [Π_B(P,Q*) − Π_B⁰]^(1−α)
```

- `Π_S⁰, Π_B⁰` — reservation profits (what each makes if the deal falls
  through: alternative vendors, scrap value, etc.).
- `α` — bargaining-power index in `[0,1]`. At `α = 0.5` with symmetric
  reservations, the surplus splits exactly down the middle.

**Closed form.** Because the pool is fixed at `Q*`, the generalized Nash
solution awards the seller a fraction `α` of the bargainable surplus
`S = Π_total(Q*) − Π_S⁰ − Π_B⁰`:

```
Π_S(P*) − Π_S⁰ = α·S
P* = ( Π_S⁰ + α·S + C_S(Q*) ) / Q*
```

**Sketch — sliding the split along the fixed pool:**

```
value
  │  Π_S  (rises with P)          Π_B (falls with P)
  │     \                        /
  │      \         P*           /
  │       \        │           /
  │        \       ▼          /
  │  Π_S⁰ ··\···············/···· Π_B⁰
  │          \   α : 1−α   /
  └───────────┴───────────┴──────────── P
      disagreement    the split that maximizes the Nash product
```

The whole bar between `Π_S⁰` and `Π_B⁰` is the surplus `S`; `α` chooses where
along it the deal lands.

**Example** (`Q* = 20`, `Π_total = 800`, reservations `0/0`):

```
α = 0.5:   S = 800,  seller share = 400
           P* = (0 + 400 + C_S(20)) / 20 = (400 + 800) / 20 = 60
           Π_S = 60·20 − 800 = 400     Π_B = 1600 − 1200 = 400   (even split)
α = 0.75:  seller share = 600
           P* = (600 + 800) / 20 = 70
           Π_S = 70·20 − 800 = 600     Π_B = 1600 − 1400 = 200
```

```sleela
SLNashBargaining nash = new SLNashBargaining();
nash.configure(jpm);
double p    = nash.nashPrice(0.0, 0.0, 0.5);          // 60  (400 / 400)
double pPow = nash.nashPrice(0.0, 0.0, 0.75);         // 70  (600 / 200)
double obj  = nash.nashObjective(p, qstar, 0.0, 0.0, 0.5);
```

---

## 3. Bilateral Newsvendor — Perishable / Stochastic Demand

If the inventory faces **uncertain** end-market demand but both parties'
holding and shortage costs are transparent, the profit curves are governed by
**overage** (one unit too many) and **underage** (one unit too few) costs on
each side.

| Metric | Seller | Buyer |
|--------|--------|-------|
| Overage `c_o` | `c_o,S = cs − vs` (prod. cost − seller salvage) | `c_o,B = P − vb` (wholesale − buyer salvage) |
| Underage `c_u` | `c_u,S = P − cs` (wholesale − prod. cost) | `c_u,B = r − P` (retail − wholesale) |

where `r` = retail price, `cs` = unit production cost, `P` = wholesale price,
`vs` = seller salvage, `vb` = buyer salvage.

**Definition — the Centralized Critical Ratio.** To prevent *double
marginalization* (both parties hedging separately and restricting inventory too
much), the system-wide optimum is the service level `F(Q*)`:

```
F(Q*) = c_u,system / (c_u,system + c_o,system)
      = (r − cs) / ((r − cs) + (cs − vb))
```

System underage `r − cs` is the full channel margin lost on a stockout; system
overage `cs − vb` is the production cost unrecovered beyond buyer salvage.

**Sketch — the critical ratio is the point on the demand CDF to stock to:**

```
F(Q)
 1 ┤                         ________________
   │                    _,-''
   │               _,-''
F* ┤----------●''                 F* = (r−cs)/((r−cs)+(cs−vb))
   │        ,'|
   │      ,'  |   stock Q* so P(demand ≤ Q*) = F*
   │    ,'    |
 0 ┤_,-'      |
   └──────────┴───────────────────────────────  Q
   0         Q*
```

Stock up to the quantity whose probability of covering demand equals `F*`.
A higher margin (`r − cs`) pushes `F*` toward 1 (stock more); a higher overage
cost pushes it toward 0 (stock less).

**Coordination.** A standalone buyer faces only its *own* ratio
`F_B = (r − P)/((r − P)+(P − vb))`, which sits **below** `F(Q*)` — that
shortfall *is* double marginalization. A **buyback** or **revenue-sharing**
contract lifts `F_B` up to the centralized `F(Q*)`, so the buyer is
financially incentivized to order exactly `Q*`.

**Example** (`r = 100`, `cs = 20`, `vb = 5`, `P = 60`):

```
system underage = 100 − 20 = 80
system overage  = 20 − 5  = 15
F(Q*) = 80 / (80 + 15) = 80 / 95 ≈ 0.8421

buyer-only:  c_u,B = 100 − 60 = 40,  c_o,B = 60 − 5 = 55
F_B = 40 / 95 ≈ 0.4211
coordination gap = 0.8421 − 0.4211 ≈ 0.4211   (a contract is needed)
```

```sleela
SLBilateralNewsvendor nv = new SLBilateralNewsvendor();
double f   = nv.criticalRatio(100.0, 20.0, 5.0);          // 0.8421
double gap = nv.coordinationGap(100.0, 20.0, 60.0, 5.0);  // 0.4211
```

---

## 4. The Profit Split, Visually

The pieces compose into distinct profit zones. The pool is fixed by Model 1;
Model 2 slides the boundary between the parties; Model 3 keeps the pool from
shrinking under uncertainty.

```
                  Π_total(Q*) = 800   (Model 1: the fixed pool)
  ┌───────────────────────────────────────────────────────────┐
  │                                                             │
  │   seller surplus            │            buyer surplus      │
  │   Π_S − Π_S⁰                │            Π_B − Π_B⁰         │
  │◄──────── α ────────────────►│◄──────── 1 − α ─────────────►│   (Model 2)
  │                            P* = 60                          │
  └───────────────────────────────────────────────────────────┘
       α = 0.5  → 400 / 400        α = 0.75 → 600 / 200

  Without coordination the buyer would order at F_B ≈ 0.42 instead of
  F* ≈ 0.84, shrinking the whole bar. A buyback / revenue-share contract
  restores it to the full 800.                                   (Model 3)
```

---

## Summary of tactical next steps

| Scenario | Objective | Model to use |
|----------|-----------|--------------|
| Fixed demand | Lock in maximum efficiency | Joint Profit Maximization (`Q*`) — `SLJointProfitMax` |
| Variable pricing | Negotiate terms out of a known surplus | Nash Bargaining — `SLNashBargaining` |
| Perishable / seasonal | Avoid stockouts or excess holding waste | Bilateral Newsvendor critical ratio — `SLBilateralNewsvendor` |

## Run the subject

```sh
./working-binaries/sleela run lib/advantage/math/trade-math-demo.sleela
```

`run` auto-links the sibling subject classes in `lib/advantage/math/`. The demo
prints each model and validates every result against the anchors above
(`Q* = 20`, `Π_total = 800`, Nash `P* = 60` splitting 400/400, `α = 0.75` →
600/200, critical ratio `80/95 ≈ 0.8421`) — ending in `ALL CHECKS PASSED`.

## Notes and honest limits

- Every class carries the mandatory **Metadata Class metapattern**
  (`.kiro/steering/metadata-class-standard.md`). The economic results are cited
  to their real sources — Cournot (1838) for `MR = MC`, Nash (1950) for the
  bargaining solution, Arrow–Harris–Marschak (1951) for the newsvendor ratio —
  so no `metaNationalSchedule` is invented.
- The newsvendor **critical ratio gives the optimal service level `F(Q*)`**, not
  a unit quantity directly; converting `F(Q*)` into an order size needs the
  demand distribution's inverse CDF, which is out of scope here.
- The quadratic curve forms are a modelling choice that makes `Q*`, `P*`, and
  the splits closed-form and exactly testable; a real engagement would fit the
  curves (or sample the marginals) from measured revenue and cost data.
