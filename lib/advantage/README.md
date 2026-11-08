# lib/advantage — the Sequitur onion model

The `advantage` package encapsulates the process of **advantaging a unique
dividend with respect to a model**: an ordered, layered procedure you can
navigate inward (deeper) and outward (shallower), and iterate up or down.

"Sequitur" means *it follows* — each layer follows the previous in logical
order. The layers form an **onion**, outer skin to inner core:

| depth | layer | kind | role |
|------:|-------|------|------|
| 0 | `dividend` | value | the value under consideration |
| 1 | `review` | validate | look again; confirm the premise |
| 2 | `concerns` | risk | surface risks and caveats |
| 3 | `maths` | quant | the quantitative core |
| 4 | `additions` | grow | what the layer adds to the dividend |
| 5 | `subtraction` | shrink | what the layer takes away |
| 6 | `regrouping` | reorg | reorganize concepts; carry/borrow across the model |
| 7 | `iteration` | loop | decide to go further down or back up the onion |

## Classes

| File | Class | Purpose |
|------|-------|---------|
| `SLPackage.sleela` | `SLPackage` | Package facade (compiler / Loader / Nordshrift) |
| `SLAdvantageLayer.sleela` | `SLAdvantageLayer` | One concentric onion layer (depth, name, kind, contribution) |
| `SLAdvantageDividend.sleela` | `SLAdvantageDividend` | The unique dividend held with respect to a named model |
| `SLAdvantageSequitur.sleela` | `SLAdvantageSequitur` | The ordered, navigable onion; cursor moves inward/outward; runs passes |
| `SLAdvantageUserModel.sleela` | `SLAdvantageUserModel` | **User's model** — simple, outcome-facing encapsulation |
| `SLAdvantageDeveloperModel.sleela` | `SLAdvantageDeveloperModel` | **Developer's model** — full-control encapsulation of the process |
| `SLAdvantageTrade.sleela` | `SLAdvantageTrade` | **Bilateral trade economics** — joint profit max, Nash bargaining, newsvendor critical ratio |
| `advantage-demo.sleela` | `AdvantageDemo` | Runnable demo + validation of the onion model (`main`) |
| `advantage-trade-demo.sleela` | `AdvantageTradeDemo` | Runnable demo + validation of the trade math (`main`) |

## The two models

- **User's model** hides the onion. State a dividend and the model it respects,
  nudge a few plain dials (`grow`, `shrink`, `rethink`), press `advantage()`,
  and read the result. `advantageHarder(n)` lets the process iterate and keeps
  the best outcome. A curious user can still reach the full controls via
  `advanced()`.
- **Developer's model** exposes the whole machine: build the Sequitur, tune
  each layer's signed contribution (`setReview`, `setConcerns`, `setMaths`,
  `setAdditions`, `setSubtraction`, `setRegrouping`, `setIteration`), walk the
  cursor (`deeper`, `shallower`, `seek`), single-step (`step`), run a full
  inward pass (`runPass`), and iterate (`iterate`).

Both operate on the same `SLAdvantageSequitur` + `SLAdvantageDividend` engine —
the user's model is a thin, friendly skin over the developer's model.

## Dynamically altering the model (to the owner's advantage)

The model can be re-shaped, re-pointed, and fully restored at runtime:

- `setOwner(name)` / `ownedBy(name)` — name the party the model is tuned to
  favor. This also re-snapshots, folding the owner into the restitution point.
- `updateModel(model, label, base)` (developer) / `updateModel(model, base)`
  (user) — re-point the dividend onto a new frame of reference and starting
  value at runtime. The onion shape is preserved and the original identity is
  retained for restitution. Pass `null` to leave a field unchanged.
- `favorOwner(strength)` (developer) / `favorMe(strength)` (user) — a dynamic
  alteration that tips the model toward the owner: the growth-side layers
  (`maths`, `additions`, `regrouping`) each gain `+strength` and a cost held in
  `subtraction` is softened toward zero by up to `strength`. It never flips a
  cost into a gain, and it leaves `review`, `concerns`, and `iteration`
  untouched — favoring must not fabricate a clean review or silence a concern.
  Returns the net contribution gained.
- `commitBaseline()` — adopt the current (altered) state as the new restitution
  point, so a later `restitute()` returns here instead of to the `init()` state.
- `restitute()` — **full restitute**: restore *both* the dividend's identity
  (label, model, owner, base) *and* the entire onion shape to the captured
  baseline, and clear the pass counter. A complete, known-good undo of every
  dynamic alteration.

The demo's final sections exercise all of this: a baseline advantage of 10,
`favorOwner(4)` raising it to 26 (shape gain +16), an `updateModel` onto a
promo model, and a `restitute` that returns the model and advantage to their
committed baseline.

## Bilateral trade economics (`SLAdvantageTrade`)

Math for advantaging a dividend across a **buyer** and a **seller** as one
coordinated system, then dividing the surplus. Curves are quadratic so every
result is closed-form and exactly checkable:

```
R_B(Q) = rbA*Q - rbB*Q^2     (buyer revenue / utility)
C_S(Q) = csA*Q + csB*Q^2     (seller cost)
```

1. **Joint Profit Maximization — the integrated surplus.**
   `jointProfit(Q) = buyerRevenue(Q) − sellerCost(Q)`; the optimal quantity
   `optimalQuantity()` solves `marginalRevenue(Q) = marginalCost(Q)`, i.e.
   `Q* = (rbA − csA) / (2·(rbB + csB))`. `maxJointProfit()` evaluates the pool
   at `Q*`.
2. **Bilateral monopoly & Nash bargaining.** `sellerProfit(P,Q)=P·Q−C_S(Q)`,
   `buyerProfit(P,Q)=R_B(Q)−P·Q`. `nashObjective(...)` is the geometric product
   `[Π_S−Π_S⁰]^α · [Π_B−Π_B⁰]^(1−α)`; `nashPrice(s0,b0,α)` returns the
   closed-form wholesale price that awards the seller a fraction `α` of the
   bargainable surplus (α=0.5 splits it down the middle).
3. **Bilateral newsvendor (stochastic demand).** Overage/underage costs on both
   ends (`overageSeller`, `underageSeller`, `overageBuyer`, `underageBuyer`) and
   the centralized critical ratio
   `criticalRatio(r,cs,vb) = (r−cs)/((r−cs)+(cs−vb))` that avoids double
   marginalization. `coordinationGap(...)` measures how far the buyer's
   standalone ratio falls short of the system ratio (how much a buyback /
   revenue-share contract must lift it).

Self-contained `expD`/`lnD`/`powAB` helpers (Taylor + atanh series, range-
reduced) supply the fractional-exponent and root math with no cross-package
imports.

## Run it

```sh
./working-binaries/sleela run lib/advantage/advantage-demo.sleela        # onion model
./working-binaries/sleela run lib/advantage/advantage-trade-demo.sleela  # trade math
```

`run` auto-links the sibling classes in this directory (no imports needed).
The onion demo validates the layered arithmetic (developer pass = 25, user
advantage = 30). The trade demo validates every model against hand-computed
anchors: Q* = 20, Π_total = 800, Nash price = 60 (400/400 split), critical
ratio = 80/95 ≈ 0.8421.

## Notes

- Every class carries the mandatory **Metadata Class metapattern**
  (`.kiro/steering/metadata-class-standard.md`).
- Editing files here touches the `lib/**` gates: after changes, regenerate
  `lib/LIBRARY.SYMBOLS.md` (`tools/generate-library-symbols.py`), update the
  four `EXPECTED_*` in `test-suites/test-library-inventory.sh` and the header
  counts in `lib/LIBRARY.INDEX.md`, and regenerate `security/sha256-manifest.json`.
