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
| `advantage-demo.sleela` | `AdvantageDemo` | Runnable demo + validation (`main`) |

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

## Run it

```sh
./working-binaries/sleela run lib/advantage/advantage-demo.sleela
```

`run` auto-links the sibling classes in this directory (no imports needed).
The demo prints the Sequitur, exercises both models, navigates the onion, and
validates the arithmetic (developer pass = 25, user advantage = 30).

## Notes

- Every class carries the mandatory **Metadata Class metapattern**
  (`.kiro/steering/metadata-class-standard.md`).
- Editing files here touches the `lib/**` gates: after changes, regenerate
  `lib/LIBRARY.SYMBOLS.md` (`tools/generate-library-symbols.py`), update the
  four `EXPECTED_*` in `test-suites/test-library-inventory.sh` and the header
  counts in `lib/LIBRARY.INDEX.md`, and regenerate `security/sha256-manifest.json`.
