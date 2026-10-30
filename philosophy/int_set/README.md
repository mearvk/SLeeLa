# INT set: character schema + real age structure → JSON / XML

A small, dependency-free data module that emits the **INT set** as both
`int_set.json` and `int_set.xml`.

## What it is

- A **16-item authored character set** — Start, Business, Model, Conversation,
  Help, Diner, Exit, Concentration, Chores, Method, A-ness, Consideration,
  Departure, Winning Runs for Home Team, Education, Scent of IQ — connected
  laterally into an **ordinal INT range** (low 70 … high 160).
- A **real human age structure** (UN DYB 2024 world totals) that the schema
  sits on top of.

## Honest boundary (important)

- The age layer uses **real, public-domain statistics** (UN / US Census). See
  [`SOURCES.md`](SOURCES.md).
- The character set and the "INT range like IQ" are **authored and
  illustrative**. No statistics bureau measures traits like "Scent of IQ,"
  and there is no population dataset for them.
- The INT range is marked `psychometric: false`. Per the module README §3.2,
  this software does not accept IQ as a validated scale and assigns no person a
  score. The range is an ordinal ordering of authored traits, not an IQ.

## Age rules

- A person starts at a natural `startAge` and grows older (`ageGrowsOlder`).
- **Younger than 16** → not mentioned (`route: not-mentioned`, the 0–14 band is
  flagged `mentioned: false`).
- **Older than 85** → a basic-anatomy check-in and routed out by **Fellows** or
  **Worship** (`basicAnatomyCheckin: true`, `route: fellows|worship`).

## Build, test, generate

    make          # build the generator and tests
    make test     # run the test suite
    make data     # (re)write int_set.json and int_set.xml
    ./build/int-set-generate --stdout json   # print JSON to stdout
    ./build/int-set-generate --stdout xml    # print XML to stdout

`int_set.json` and `int_set.xml` are committed so they can be consumed without
building. Both are validated well-formed (JSON and XML) by the test/CI path.
