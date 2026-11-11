# Learner vs. Professional

Side-by-side comparison of how a **learner** and a **professional** work the
same Advantage™ problem with the Sequitur onion model.

This folder is a scaffold. The structure below is a proposal and is meant to
be adjusted.

## Intended layout

| path | purpose |
|------|---------|
| `learner/` | the learner approach: fewer layers, hand-set contributions, no risk pass |
| `professional/` | the professional approach: full onion traversal, grounded contributions, iteration |
| `comparison/` | the same scenario run both ways, with the dividend difference explained |

## Notes

- Any new `.sleela` class added here should carry the Metadata Class
  metapattern (see `SLAdvantageLayer.sleela` for a reference).
- Runnable demos need a `main` method; library classes are validated through a
  sibling demo.
- Related material: [`../examples/novice`](../examples/novice) and
  [`../examples/professional`](../examples/professional).
