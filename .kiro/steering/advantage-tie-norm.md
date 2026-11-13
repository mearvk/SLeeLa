---
inclusion: fileMatch
fileMatchPattern: "lib/advantage/**"
---

# Norm: ties go to supplier A

When two options in the Advantage(TM) courseware or demos have exactly equal
totals, the choice goes to **supplier A** (the first-listed option).

- Implement the choice as `a <= b` in a single `pick` method, so the student
  and the Purchase Master use the same rule.
- Never use a strict `<` that silently sends ties to the second option.
- Show the tie norm in any lesson that compares totals (see
  `lib/advantage/learner-vs-professional/series/06-purchase-onion/`).
- The rule decides the tie only. It is not a reason to skip checking that both
  sides computed the same total.

Set by the project owner, 2026-11-11.
