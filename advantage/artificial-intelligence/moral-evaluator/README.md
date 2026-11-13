# Moral Evaluator (courseware model)

Judges whether a trade is **centered on the human and on the living person**,
with up to **41 yes/no points**, and scores the merit at risk across the overall
trades. Run from this folder:

```sh
export SLEELA_HOME=<repo root>
$SLEELA_HOME/working-binaries/sleela run moral-evaluator.sleela
```

One file, five classes (outside `/lib`, several files run together duplicate
library units):

| Class | Role |
|---|---|
| `SLMoralTrade` | The facts of one trade; defaults describe an exemplary trade |
| `SLTallScience` | Tall Science: the United States held as an **Item** with its lists |
| `SLMoralEvaluator` | The judge: 41 points, cardinal vetoes, dimension floor, merit, verdict, review |
| `SLMoralLedger` | Overall trades over time: counts, mean and recent merit, risk of merit |
| `MoralDemo` | 39 checks |

## The 41 points in five dimensions

| Dimension | Points | Asks |
|---|--:|---|
| Trades | 9 | Life safe, human is the end, consent, no deception, no coercion, fair value, remedy, no third-party harm, both benefit |
| Time | 7 | Urgency not exploited, cooling off, time to decide, timestamped, bounded commitment, fresh quote, sustainable pace |
| Listers | 8 | Identified, in good standing, truthful, not prohibited, provenance, conflict disclosed, more than one lister, accountable |
| United States | 8 | Inside US boundary, lawful subject, no sanctioned counterparty, records, fees, no manipulation, protections, respects the United States |
| Tall Science | 9 | See below |

## Tall Science: the United States as an Item

`SLTallScience` is a catalog record, not a sale. The United States is declared an
**Item** (named `the United States`) carrying lists of subject **nouns**, **names**,
**scientific individuals** and **moral individuals**, plus a boundary flag and a
respect flag. Nouns and names are read **respectively**: entry *i* pairs with entry
*i*, so the lists must be the same length. The seeded catalog pairs `nation` with
`the United States`, `legislature` with `the United States Congress`, and
`judiciary` with `the Supreme Court of the United States`. Individuals are listed
as **role labels** (or public-record names the caller supplies); the model does not
profile anyone.

## Verdicts: yes/no, or yes/no/complete

- `0` **NO**, `1` **YES**, `2` **COMPLETE** (all 41 points active and yes).
- `configure(points, states)`: points 5..41, states 2 or 3. In 2-state mode
  COMPLETE is reported as YES.

Order of judgement:

1. **Cardinal veto**: five points (life safe, human is the end, consent, no
   deception, lawful subject) are always evaluated first. Any No is a NO, however
   high the merit. That is the human- and life-centered orientation.
2. **Dimension floor**: every dimension with active points must reach 0.5.
3. **Merit**: the mean of the dimension fractions must reach the threshold (0.80).
   `risk = 1 - merit`.

## Evolution

`review(verdict, wasCorrect)`: a person who says a YES was wrong raises the
threshold 0.01; a wrong NO lowers it 0.01 (kept in 0.60..0.95). Cardinals and the
floor never move.

## Measured here (one run)

20,000 synthetic trades: 14,011 COMPLETE, 4,202 YES, 1,787 NO; mean merit 0.990;
roughly 70,000 trades per second with all 41 points. Synthetic mix, illustrative only.

## Interpretations I chose (change them if they are wrong)

- "To center and to live man" = human-centered and life-centered, implemented as
  the five cardinal vetoes.
- "Binary Complete" = a third verdict, COMPLETE, meaning every point is Yes.
- "Listers" = the parties who list the item being traded.
- The point rules and thresholds are teaching choices, not a legal or moral authority.

## Honest limits

A rule-based model on caller-supplied facts. It does not detect lies in its
inputs, give legal advice, or decide what is right; it makes a stated set of
judgements explicit, ordered, and auditable.
