# /4 · Physics model — geographic Areas

The **physics model** here reads *areas* of the real world: the 2D extents that
planetary and geographic bodies span. Where `/2` (Structures) held standing
constants and `/3` (Facets) held derived faces, `/4` (Areas) holds **extents** —
how much surface a country, a continent, the Earth, or a lake occupies.

## Data

- [`../data/country-areas.json`](../data/country-areas.json) — the 11 largest
  countries by total area (alphabetized by name).
- [`../data/earth-surface.xml`](../data/earth-surface.xml) — the Earth's total
  surface and its land/water/ice/arable partition (5 extents).
- [`../data/continents.txt`](../data/continents.txt) — the seven continents by
  area (alphabetized, size rank retained).
- [`../data/great-lakes.json`](../data/great-lakes.json) — the five Great Lakes
  plus other major North American lakes (8 total) by surface area.

| Area | Extent (km²) | Kind |
|---|---|---|
| Earth total surface | 510,072,000 | MEASURED (4πR², R≈6371 km) |
| Earth land | 148,940,000 | OBSERVED (29.2%) |
| Asia (continent) | 44,579,000 | OBSERVED |
| Antarctic ice sheet | 14,000,000 | OBSERVED |
| Russia (country) | 17,098,242 | OBSERVED |
| Algeria (country) | 2,381,741 | OBSERVED |
| United States (country) | 9,833,517 | OBSERVED |
| Lake Superior (surface) | 82,100 | OBSERVED |
| Great Bear Lake (surface) | 31,153 | OBSERVED |

## The distinction the model keeps

A geographic area is not a definition — it is **surveyed**. The Earth's total
surface is **MEASURED** (it follows from the mean radius); the land/water split
and the country, continent, and lake extents are **OBSERVED** published figures.
The model tags each so a surveyed extent is never confused with an exact,
formula-defined one — `FACT ≠ MODEL`.

## Provenance

Country totals from the United Nations Statistics Division (cross-referenced
with the FAO land/water split and the CIA World Factbook); Earth surface from
standard geophysics (mean radius) with the land/water percentages from Wikipedia
"Earth"; continents from standard geographic compilations; Great Lakes from the
U.S. Environmental Protection Agency and Encyclopaedia Britannica, with Great
Bear / Great Slave / Winnipeg lakes from Natural Resources Canada. Values
transcribed from these public sources.

---

*Copyright (C) 2026 MEARVK LLC*
*Author: Maximilian Eric Alexander Rupplin von Keffikon (Max Rupplin)*
