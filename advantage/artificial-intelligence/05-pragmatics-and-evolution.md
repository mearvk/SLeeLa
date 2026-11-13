# Lesson 05 — Pragmatics and evolution (advanced)

> Series: [index](README.md) · prev: [04](04-the-high-speed-trader.md) · next: [06 — Fail closed, and the brilliant](06-fail-closed-and-the-brilliant.md)

A **pragmatic** is one evolution of judgement. The model has 24 in three phases:
**logic** (is it coherent), **use** (does it fit the purpose), **ground** (is it
secure and grounded). You choose 8 to 24 with `configure(P, tpm)`.

## Reading the registry

`SLHstPragmatics` holds names, phases, hard minimums and the priority order. Note
the order interleaves phases and the first 8 are the minimal safe core: L1
well-formed, G1 venue trust, G2 kill switch, U1 size, G3 price collar, L2 net
edge, U2 freshness, G4 position limit.

## Evolution

`learn(realizedBps)` multiplies each active weight by `1 +/- eta*(f - 0.5)`: a
feature that was high on a win gains weight, on a loss loses it, bounded 0.25..4.
This is the same evidence idea as lesson 01, applied online.

## Exercises

1. Run P = 8, 16, 24. Which pragmatics veto most? Which never fire?
2. Compare learning ON and OFF. Why must the OFF baseline have its drawdown guard
   disabled to be a fair comparison?
3. The demo shows weights L4 and L5 saturating at 4.0. What does saturation
   hide, and how would you detect an over-fit weight?

## Advanced

Add a 25th pragmatic of your own as a feature in 0..1 with a hard minimum, then
decide where it belongs in the priority order and justify it. (Remember
`configure` caps the count at 24: you must raise that deliberately.)
