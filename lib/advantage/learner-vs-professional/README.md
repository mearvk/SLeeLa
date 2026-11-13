# Learner vs. Professional

Side-by-side comparison of how a **learner** and a **professional** work the
same Advantage™ problem with the Sequitur onion model.

> **Looking for the data?** [`INSTRUMENT.md`](./INSTRUMENT.md) is a single,
> neatly organized index of every dataset, worked example, exercise set, and
> self-checking entry across this entire folder — the Device Instructor
> series, the children's/adults' units, the age-sorted reading material, and
> the `long-medium-averages`/`long-term-potencies` courseware topics.

## Current layout

The original `learner/`/`professional/`/`comparison/` scaffold below was a
proposal that was never built this way; what actually exists is organized
by topic instead, under `series/` and `courseware/`:

| path | purpose |
|------|---------|
| `series/` | the Device Instructor series: 32 lessons on registries, agreement, and 26 trading decisions |
| `courseware/children/`, `courseware/adults/` | 5 units each, paired logic/process lessons for two audiences |
| `courseware/reading/` | 24 readers sorted by age, 5-7 through 18+ |
| `courseware/long-medium-averages/` | 20 entries on the daily-trade-count teaching rule |
| `courseware/long-term-potencies/` | 6 entries defining term potency and age potency |

## Notes

- Any new `.sleela` class added here should carry the Metadata Class
  metapattern (see `SLAdvantageLayer.sleela` for a reference).
- Runnable demos need a `main` method; library classes are validated through a
  sibling demo.
- Related material: [`../examples/novice`](../examples/novice) and
  [`../examples/professional`](../examples/professional).
