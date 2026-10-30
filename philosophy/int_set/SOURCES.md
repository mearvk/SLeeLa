# INT-set data sources and boundaries

This folder builds a machine-readable (JSON + XML) description of an **INT set**:
a lateral character schema connected to an ordinal "INT range," layered on top
of a **real** human age structure.

## What is real vs. synthetic

The honest split matters, and the module keeps the two strictly separated.

### Real: age structure

Human age structure is taken from authoritative, public-domain statistics
bureaus. No bureau publishes an "intelligence" dataset keyed to the character
traits below, so only the **age** layer is sourced from real data.

- **United Nations, Demographic Yearbook 2024, Table 02** — world population
  and its percentage distribution by age group (0–14, 15–64, 65+), 2024.
  World total 8,162 million; 0–14 = 2,017M; 15–64 = 5,312M; 65+ = 833M.
  <https://unstats.un.org/unsd/demographic-social/products/dyb/documents/DYB2024/table02.pdf>
- **United Nations, World Population Prospects 2024** — official estimates and
  projections by single year of age and by 5-year age groups, 237 countries.
  <https://population.un.org/WPP/>
- **U.S. Census Bureau** — Resident Population by Single Year of Age and Sex
  (NC-EST2023-AGESEX-RES), single-year ages 0–99 and 100+.
  <https://www.census.gov/data/tables/time-series/demo/popest/2020s-national-detail.html>

Content from these sources was summarized/rephrased for compliance with
licensing restrictions; the figures themselves are public-domain official
statistics.

### Synthetic: the character set and the "INT range"

The 16-item character set — Start, Business, Model, Conversation, Help, Diner,
Exit, Concentration, Chores, Method, A-ness, Consideration, Departure, Winning
Runs for Home Team, Education, Scent of IQ — is an **authored, illustrative
schema**. No statistics bureau, census, or scientific instrument measures these
traits, and there is no population dataset for them.

The "INT range like IQ" is therefore represented as a declared **ordinal
lateral index**, not a psychometric IQ score. Per the module README (§3.2), this
software does not accept IQ as a validated scale and assigns no person a score.
The index is a schema-internal ordering of the authored traits, explicitly
labelled `psychometric: false`.

## Age handling rules (as specified)

- A person starts at a natural **age** and grows older over time.
- Ages **younger than 16** are **not mentioned** (excluded from the mentioned
  set; the age band is still present in the real structure, flagged
  `mentioned: false`).
- Ages **older than 85** are checked in on **basic anatomy** and **routed out**
  by the `Fellows` or `Worship` handlers.

These rules are declarative schema policy, not a clinical or legal
determination. The module makes no judgement about any real person.
