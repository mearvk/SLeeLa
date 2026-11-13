# ORGANIZED: Vocabulary and Definitions

A glossary of the terms used while planning the courseware's trading-themed
material. This document exists to keep an honest line between **defined**
terms (meaning fixed, checkable, used in running code) and **undefined** or
**declined** terms (named in conversation but not given a working meaning, or
ruled out). Nothing below pretends a term is specified when it is not.

## Defined terms (used in code, checked, committed)

These come from the existing library and are not new. Each is implemented in
`lib/advantage/trading-actions-on-verb/` and checked by its own tests.

| term | definition | where |
|------|------------|-------|
| Long | owning a quantity, profiting if price rises; sell price minus buy price | `SLLongBook` |
| Short | having sold a borrowed quantity, profiting if price falls; sale price minus cover price | `SLShortBook` |
| Lot | one buy or one short-sale, kept with its quantity, price and day | both books |
| FIFO (first in, first out) | a sell or cover consumes the oldest lot first | both books |
| Verb | a trading action (`buy`, `sell`, `hold`, `flatten`, `shortSell`, `cover`, `coverAll`) that returns a status code and, if refused, changes nothing | both books |
| Refusal | a verb call that fails a check and leaves the book exactly as it was | both books |
| Invariant | a fact checked true after every action (e.g. shares held = bought - sold) | `invariantsHold()` |
| Borrow cost | cents per share per day charged on a short position while it is open | `SLShortBook` |
| Tie norm | when two totals are exactly equal, the earlier-listed option is kept | `.kiro/steering/advantage-tie-norm.md` |
| Term boundary | "long term" means held **strictly more than** a day threshold, not equal to it | trading use case 11 |
| Margin, equity, excess, call | collateral minus short value; equity minus the ceiling-rounded requirement; a call is due when excess is negative | `SLShortMargin` |
| Differential testing | running the same actions against two independently written implementations and requiring agreement | use cases 14, 21, 22 |
| Exact-comparison bound | the limit (2^53 - 1) above which this runtime's integer comparisons were measured to be unreliable; every compared value here is kept under it | `SLEuclid`, `.kiro/steering/sleela-authoring-gotchas.md` |
| Trades-per-day cap | this session's new rule: a book refuses a verb once it would be the 2,001st action recorded on a single day | `SLDailyCapBook` (new, below) |

## Named but not defined

These terms were introduced during this conversation. They are recorded here
so the conversation is not lost, but **none of them has a working
definition**, and none of them appears in any code, formula, or numeric
example in this project. Anyone reading this should treat them as open, not
as settled vocabulary.

| term, as given | status |
|---|---|
| "Flat 2" / "Euclidean Yield of Flat 2" | undefined. No formula, unit, or worked value was ever supplied. Repeated requests for one went unanswered. Do not assume it means a ratio, score, or yield percentage — none of those was confirmed. |
| "Medium-concerns" | undefined as a metric. "Medium-term" is used informally below to mean a holding period and trade cadence between a single trade and a long-term hold; that informal use is not the same as a defined "medium-concerns" quantity. |
| "'2'-ness" | undefined; explicitly deferred by the requester to "some future point." |
| "Quality of the Instrument (military, aluminum, Black or Dark grey)" | undefined. Read as a possible visual or tier-naming theme, never confirmed. No tier system by these names exists in the code or documents. |
| "Map of intent, understanding, copious" | undefined; appears to describe a reader's experience rather than a measurable property of a trade or instrument. |
| "220++++ IQ" / "Mod-Intelligence" | not a specification of any kind recognized here; treated as rhetorical emphasis, not a requirement with a checkable meaning. |
| "The knowing hat of the trader" | undefined. |

## Claims recorded as claims, not as verified fact

A later request asked for a specific real-world performance figure to be
added to the code or documents as something this repository supports. It is
recorded here instead, attributed and explicitly unverified, because neither
of those things is true.

| claim, as given | status |
|---|---|
| "Day traders make about 20% profit per 2,000 trades," stated as certain, based on the requester's own stated personal trading experience | **Still not verified by this repository as a fact about real traders.** It is noted as a claim, attributed to the person who stated it, in [`SLDailyCapBook.sleela`](./long-medium-averages/SLDailyCapBook.sleela) (which has an unrelated, independently chosen 2,000-**per-day** action cap — the shared number 2000 is a coincidence, not a connection). A genuine, checked model of return-over-a-trade-batch was since built: [`SLBatchReturn`](../../trading-actions-on-verb/SLBatchReturn.sleela), use case 27, and [reading/ages-18-plus/07-batch-vs-per-trade.md](./reading/ages-18-plus/07-batch-vs-per-trade.md). It computes, exactly, what the claim would mean under each of two readings — a 1 bp (0.01%) per-trade edge if "20%" is the batch total, versus 400x capital if "20%" is a per-trade return repeated 2,000 times with no compounding — but it does not and cannot confirm which reading (if either) describes real trader behavior. |

## Declined

| request | why |
|---|---|
| Framing sexual content ("enjoying the bodyius," adult-content material) as part of trading examples | Refused. This is a firm limit, independent of how the request is phrased or what else it is bundled with. |

## What this means for the new material

The courseware entry built alongside this document (`long-medium-averages/`)
uses only the **defined** column above, plus the one concrete new rule given
in this session: a trading day is capped at 2,000 actions. It does **not**
use, define, or approximate "Flat 2," any IQ-themed language, or any tier
system named after colors or materials. If "Flat 2" is given a real
definition later, this document and the examples should be revisited, and
this glossary updated to move it into the defined table with its actual
formula.
