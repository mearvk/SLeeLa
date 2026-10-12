# UTF-4088 Semantic Concept Symbols and 4D Anchors

**Source-code realm:** `utf-4088/semantics/concept_symbols.cpp`  
**Machine-readable semantics:** `utf-4088/semantics/character_meanings.json`  
**Status:** 44 provisional concept seeds; glyphs are deterministic outputs of the C++ `seed_glyph` algorithm.

## Correction and scope

The earlier version concentrated on low-level C++ geometry structures and five probe-result glyphs. It omitted the semantic concept-symbol vocabulary requested for the 4D study. This revision adds explicit source records and visible glyphs for **wealth, wood, sex, pleasure, quality, Asia**, and related concepts. These are newly formalized starter seeds; they were **not** already present as mapped concept records in the previous C++ source. The old five trial glyphs remain separate in [the ordered probe results](results/symbols-ordered.md).

Each concept has a stable internal ID, a key, a careful distinction/definition, related concept keys, a provisional four-coordinate anchor, and a deterministic 8×12 glyph. The concept IDs are internal registry IDs—not UTF-4088 code points, Unicode values, or language-standard assignments.

## 1. How 4D concept geometry is represented

A source resolver point has the shape `(x, y, pressure, voltage)` in [`symbol_resolution.hpp`](../../utf-4088/algebra/symbol_resolution.hpp). This concept registry stores those four coordinates normalized to `[0,1]` as `(x,y,normalized_pressure,normalized_voltage)`. For this registry only, the anchor coordinates are **curated synthetic study fixtures**. Domain-related concepts are placed in nearby neighborhoods so the study can test distances, clustering ideas, and possible coincidences. These placements are design inputs—not physical measurements, learned semantic truths, or evidence that related concepts are interchangeable.

The existing resolver uses Euclidean point distance:
`d(a,b) = sqrt(Δx² + Δy² + Δpressure² + Δvoltage²)`.
It returns a path-similarity score; it does not by itself assign word meaning. The current graph engine coalesces repeated packed field-state IDs and repeated ordered edges, but it does not automatically turn these concept relationships into a semantic graph or implement a general near-coincidence detector.

## 2. Concept register

| ID | Key / symbol label | Domain | Provisional 4D anchor `(x,y,p,v)` | Related concepts |
|---|---|---|---|---|
| `UTF4088-CONCEPT-000001` | **Wealth** (`wealth`) | `economy` | `(0.100, 0.150, 0.200, 0.250)` | `money`, `income`, `property`, `resources`, `value`, `trade` |
| `UTF4088-CONCEPT-000002` | **Money** (`money`) | `economy` | `(0.112, 0.164, 0.206, 0.259)` | `wealth`, `trade`, `debt`, `value` |
| `UTF4088-CONCEPT-000003` | **Income** (`income`) | `economy` | `(0.124, 0.178, 0.212, 0.268)` | `wealth`, `trade`, `resources` |
| `UTF4088-CONCEPT-000004` | **Property** (`property`) | `economy` | `(0.136, 0.152, 0.218, 0.277)` | `wealth`, `resources`, `trade`, `wood` |
| `UTF4088-CONCEPT-000005` | **Trade** (`trade`) | `economy` | `(0.103, 0.166, 0.224, 0.286)` | `money`, `wealth`, `quality`, `region` |
| `UTF4088-CONCEPT-000006` | **Debt** (`debt`) | `economy` | `(0.115, 0.180, 0.230, 0.253)` | `money`, `wealth`, `property` |
| `UTF4088-CONCEPT-000007` | **Resources** (`resources`) | `economy` | `(0.127, 0.154, 0.236, 0.262)` | `wealth`, `property`, `wood`, `water` |
| `UTF4088-CONCEPT-000008` | **Wood** (`wood`) | `material_nature` | `(0.800, 0.150, 0.280, 0.300)` | `tree`, `forest`, `timber`, `plant`, `resources` |
| `UTF4088-CONCEPT-000009` | **Tree** (`tree`) | `material_nature` | `(0.812, 0.164, 0.286, 0.309)` | `wood`, `forest`, `plant`, `environment` |
| `UTF4088-CONCEPT-000010` | **Forest** (`forest`) | `material_nature` | `(0.824, 0.178, 0.292, 0.318)` | `tree`, `wood`, `plant`, `environment`, `asia` |
| `UTF4088-CONCEPT-000011` | **Timber** (`timber`) | `material_nature` | `(0.836, 0.152, 0.298, 0.327)` | `wood`, `tree`, `property`, `quality` |
| `UTF4088-CONCEPT-000012` | **Plant** (`plant`) | `material_nature` | `(0.803, 0.166, 0.304, 0.336)` | `tree`, `forest`, `wood`, `life`, `environment` |
| `UTF4088-CONCEPT-000013` | **Stone** (`stone`) | `material_nature` | `(0.815, 0.180, 0.310, 0.303)` | `wood`, `water`, `resources`, `quality` |
| `UTF4088-CONCEPT-000014` | **Water** (`water`) | `material_nature` | `(0.827, 0.154, 0.316, 0.312)` | `resources`, `life`, `environment`, `asia` |
| `UTF4088-CONCEPT-000015` | **Sex (context-dependent)** (`sex`) | `human_experience` | `(0.150, 0.800, 0.680, 0.740)` | `biological_sex`, `sexual_activity`, `sexuality`, `consent` |
| `UTF4088-CONCEPT-000016` | **Biological sex** (`biological_sex`) | `human_experience` | `(0.162, 0.814, 0.686, 0.749)` | `sex`, `reproduction`, `health` |
| `UTF4088-CONCEPT-000017` | **Sexual activity** (`sexual_activity`) | `human_experience` | `(0.174, 0.828, 0.692, 0.758)` | `sex`, `sexuality`, `consent`, `pleasure`, `intimacy` |
| `UTF4088-CONCEPT-000018` | **Sexuality** (`sexuality`) | `human_experience` | `(0.186, 0.802, 0.698, 0.767)` | `sex`, `sexual_activity`, `desire`, `intimacy` |
| `UTF4088-CONCEPT-000019` | **Pleasure** (`pleasure`) | `human_experience` | `(0.153, 0.816, 0.704, 0.776)` | `satisfaction`, `wellbeing`, `desire`, `pain` |
| `UTF4088-CONCEPT-000020` | **Desire** (`desire`) | `human_experience` | `(0.165, 0.830, 0.710, 0.743)` | `pleasure`, `sexuality`, `wealth`, `relationship` |
| `UTF4088-CONCEPT-000021` | **Intimacy** (`intimacy`) | `human_experience` | `(0.177, 0.804, 0.716, 0.752)` | `relationship`, `sexual_activity`, `sexuality`, `consent` |
| `UTF4088-CONCEPT-000022` | **Consent** (`consent`) | `human_experience` | `(0.189, 0.818, 0.722, 0.761)` | `sexual_activity`, `relationship`, `ethics` |
| `UTF4088-CONCEPT-000023` | **Reproduction** (`reproduction`) | `human_experience` | `(0.156, 0.832, 0.728, 0.770)` | `biological_sex`, `life`, `health` |
| `UTF4088-CONCEPT-000024` | **Wellbeing** (`wellbeing`) | `human_experience` | `(0.168, 0.806, 0.734, 0.779)` | `pleasure`, `health`, `satisfaction`, `quality`, `pain` |
| `UTF4088-CONCEPT-000025` | **Satisfaction** (`satisfaction`) | `human_experience` | `(0.180, 0.820, 0.740, 0.746)` | `pleasure`, `wellbeing`, `quality`, `desire` |
| `UTF4088-CONCEPT-000026` | **Pain** (`pain`) | `human_experience` | `(0.192, 0.834, 0.746, 0.755)` | `pleasure`, `health`, `wellbeing` |
| `UTF4088-CONCEPT-000027` | **Health** (`health`) | `human_experience` | `(0.159, 0.808, 0.752, 0.764)` | `wellbeing`, `pain`, `reproduction`, `quality` |
| `UTF4088-CONCEPT-000028` | **Relationship** (`relationship`) | `human_experience` | `(0.171, 0.822, 0.758, 0.773)` | `intimacy`, `consent`, `desire`, `trade` |
| `UTF4088-CONCEPT-000029` | **Quality** (`quality`) | `evaluation` | `(0.500, 0.500, 0.430, 0.460)` | `standard`, `excellence`, `defect`, `value`, `merit` |
| `UTF4088-CONCEPT-000030` | **Quantity** (`quantity`) | `evaluation` | `(0.512, 0.514, 0.436, 0.469)` | `quality`, `value`, `resources` |
| `UTF4088-CONCEPT-000031` | **Value** (`value`) | `evaluation` | `(0.524, 0.528, 0.442, 0.478)` | `wealth`, `money`, `quality`, `merit`, `trade` |
| `UTF4088-CONCEPT-000032` | **Merit** (`merit`) | `evaluation` | `(0.536, 0.502, 0.448, 0.487)` | `quality`, `value`, `excellence`, `standard` |
| `UTF4088-CONCEPT-000033` | **Standard** (`standard`) | `evaluation` | `(0.503, 0.516, 0.454, 0.496)` | `quality`, `excellence`, `defect`, `value` |
| `UTF4088-CONCEPT-000034` | **Defect** (`defect`) | `evaluation` | `(0.515, 0.530, 0.460, 0.463)` | `quality`, `standard`, `excellence` |
| `UTF4088-CONCEPT-000035` | **Excellence** (`excellence`) | `evaluation` | `(0.527, 0.504, 0.466, 0.472)` | `quality`, `merit`, `standard` |
| `UTF4088-CONCEPT-000036` | **Asia** (`asia`) | `geography_culture` | `(0.820, 0.800, 0.300, 0.340)` | `continent`, `region`, `country`, `geography`, `culture`, `language` |
| `UTF4088-CONCEPT-000037` | **Continent** (`continent`) | `geography_culture` | `(0.832, 0.814, 0.306, 0.349)` | `asia`, `geography`, `region` |
| `UTF4088-CONCEPT-000038` | **Region** (`region`) | `geography_culture` | `(0.844, 0.828, 0.312, 0.358)` | `asia`, `continent`, `country`, `geography`, `forest` |
| `UTF4088-CONCEPT-000039` | **Country** (`country`) | `geography_culture` | `(0.856, 0.802, 0.318, 0.367)` | `region`, `asia`, `culture`, `language`, `trade` |
| `UTF4088-CONCEPT-000040` | **Geography** (`geography`) | `geography_culture` | `(0.823, 0.816, 0.324, 0.376)` | `asia`, `continent`, `region`, `environment` |
| `UTF4088-CONCEPT-000041` | **Culture** (`culture`) | `geography_culture` | `(0.835, 0.830, 0.330, 0.343)` | `asia`, `region`, `country`, `language` |
| `UTF4088-CONCEPT-000042` | **Language** (`language`) | `geography_culture` | `(0.847, 0.804, 0.336, 0.352)` | `culture`, `region`, `asia` |
| `UTF4088-CONCEPT-000043` | **Environment** (`environment`) | `material_nature` | `(0.839, 0.168, 0.322, 0.321)` | `forest`, `tree`, `plant`, `water`, `geography` |
| `UTF4088-CONCEPT-000044` | **Life** (`life`) | `material_nature` | `(0.806, 0.182, 0.328, 0.330)` | `plant`, `tree`, `reproduction`, `health`, `environment` |

## 3. Generated concept glyphs

The glyph seed contract is `UTF4088-CONCEPT-V1:<key>`. The C++ registry passes that exact stable seed into `utf4088::seed_glyph`. A `#` is a set pixel and `.` is a background pixel. Each glyph has 8 columns and 12 rows (96 bits). The bitmap is a reproducible geometry fingerprint for the concept seed; it does not carry a meaning without the associated concept record.

### UTF4088-CONCEPT-000001 — Wealth (`wealth`)

- **Domain:** `economy`
- **Provisional definition:** Stock of economic assets and resources, net of liabilities where that accounting context applies; distinct from income and money.
- **4D anchor:** `(0.100, 0.150, 0.200, 0.250)`
- **Related keys:** `money`, `income`, `property`, `resources`, `value`, `trade`
- **Glyph seed:** `UTF4088-CONCEPT-V1:wealth`

```text
#.#..##.
##....#.
.#.....#
##.###.#
####.##.
.##.##..
.###....
......##
.#.#..#.
.#.#.###
#...#..#
#...#..#
```


### UTF4088-CONCEPT-000002 — Money (`money`)

- **Domain:** `economy`
- **Provisional definition:** A medium of exchange, unit of account, or store of value within a specified social or institutional system; not identical to wealth.
- **4D anchor:** `(0.112, 0.164, 0.206, 0.259)`
- **Related keys:** `wealth`, `trade`, `debt`, `value`
- **Glyph seed:** `UTF4088-CONCEPT-V1:money`

```text
#..#.#.#
#....#.#
...#...#
####..#.
####....
....#.#.
..##..#.
##...###
###.####
##...#.#
#.##...#
##..#..#
```


### UTF4088-CONCEPT-000003 — Income (`income`)

- **Domain:** `economy`
- **Provisional definition:** A flow of earnings or receipts over a time interval; distinct from accumulated wealth or a balance-sheet stock.
- **4D anchor:** `(0.124, 0.178, 0.212, 0.268)`
- **Related keys:** `wealth`, `trade`, `resources`
- **Glyph seed:** `UTF4088-CONCEPT-V1:income`

```text
....#.#.
...##.#.
#..##.#.
.###.#..
.###..##
.#..##.#
#.##.##.
...#.###
#.####.#
..######
.##.##.#
.#.#.##.
```


### UTF4088-CONCEPT-000004 — Property (`property`)

- **Domain:** `economy`
- **Provisional definition:** An asset or resource together with recognized ownership, use, or control rights.
- **4D anchor:** `(0.136, 0.152, 0.218, 0.277)`
- **Related keys:** `wealth`, `resources`, `trade`, `wood`
- **Glyph seed:** `UTF4088-CONCEPT-V1:property`

```text
##.###.#
.####...
.##.##..
...#...#
..#.####
#...#.##
###....#
###.###.
##.#.##.
..##..#.
#.#...##
...###..
```


### UTF4088-CONCEPT-000005 — Trade (`trade`)

- **Domain:** `economy`
- **Provisional definition:** An exchange of goods, services, or rights between parties under a specified transaction context.
- **4D anchor:** `(0.103, 0.166, 0.224, 0.286)`
- **Related keys:** `money`, `wealth`, `quality`, `region`
- **Glyph seed:** `UTF4088-CONCEPT-V1:trade`

```text
.#.....#
#####..#
..#####.
#.##.###
..####.#
....#.#.
..####.#
###..##.
..###.#.
#.#..##.
.###.##.
...###.#
```


### UTF4088-CONCEPT-000006 — Debt (`debt`)

- **Domain:** `economy`
- **Provisional definition:** An obligation to repay money, goods, or another specified value; distinct from an asset or from wealth itself.
- **4D anchor:** `(0.115, 0.180, 0.230, 0.253)`
- **Related keys:** `money`, `wealth`, `property`
- **Glyph seed:** `UTF4088-CONCEPT-V1:debt`

```text
.#...#..
....#..#
..#...##
..##..#.
.....###
###.#.#.
###....#
#..#...#
###.##.#
.#.#..##
.#..#...
##.##...
```


### UTF4088-CONCEPT-000007 — Resources (`resources`)

- **Domain:** `economy`
- **Provisional definition:** Useful assets, materials, capabilities, or supplies available for a purpose; the resource type should be explicit.
- **4D anchor:** `(0.127, 0.154, 0.236, 0.262)`
- **Related keys:** `wealth`, `property`, `wood`, `water`
- **Glyph seed:** `UTF4088-CONCEPT-V1:resources`

```text
.##..#..
..##..#.
.#..###.
....#.#.
.#...#.#
..#..#..
..##.#.#
.###.#..
###..##.
....##.#
....##..
....#..#
```


### UTF4088-CONCEPT-000008 — Wood (`wood`)

- **Domain:** `material_nature`
- **Provisional definition:** The structural material derived from tree tissues; distinct from a living tree, a forest ecosystem, or processed timber.
- **4D anchor:** `(0.800, 0.150, 0.280, 0.300)`
- **Related keys:** `tree`, `forest`, `timber`, `plant`, `resources`
- **Glyph seed:** `UTF4088-CONCEPT-V1:wood`

```text
####..##
#.#.#..#
.##.#..#
.#.####.
.##.#.#.
####.###
..####.#
..###.##
..##....
###....#
######.#
#..##...
```


### UTF4088-CONCEPT-000009 — Tree (`tree`)

- **Domain:** `material_nature`
- **Provisional definition:** A perennial woody plant with a trunk or comparable main stem and branching crown; biological details depend on taxon.
- **4D anchor:** `(0.812, 0.164, 0.286, 0.309)`
- **Related keys:** `wood`, `forest`, `plant`, `environment`
- **Glyph seed:** `UTF4088-CONCEPT-V1:tree`

```text
..#..#..
.####...
...#.#.#
###.####
...##.#.
.#..#.##
#.##.#.#
.###.#..
#.##..##
..#.#.#.
####....
#.#.####
```


### UTF4088-CONCEPT-000010 — Forest (`forest`)

- **Domain:** `material_nature`
- **Provisional definition:** An ecosystem or land area dominated by trees and associated organisms and processes; not a synonym for wood.
- **4D anchor:** `(0.824, 0.178, 0.292, 0.318)`
- **Related keys:** `tree`, `wood`, `plant`, `environment`, `asia`
- **Glyph seed:** `UTF4088-CONCEPT-V1:forest`

```text
.####...
#.####..
.#..#..#
##......
.#.#....
.#...##.
#..#.###
#...#..#
.####...
########
######..
.###.###
```


### UTF4088-CONCEPT-000011 — Timber (`timber`)

- **Domain:** `material_nature`
- **Provisional definition:** Wood prepared or designated for construction, woodworking, or other material use; usage may vary by region.
- **4D anchor:** `(0.836, 0.152, 0.298, 0.327)`
- **Related keys:** `wood`, `tree`, `property`, `quality`
- **Glyph seed:** `UTF4088-CONCEPT-V1:timber`

```text
####..##
..#.##..
.....###
..#.....
.#..##.#
###.#..#
#.##.#.#
...#.#.#
#####..#
#....#..
###..##.
.#.###.#
```


### UTF4088-CONCEPT-000012 — Plant (`plant`)

- **Domain:** `material_nature`
- **Provisional definition:** A plant organism or member of the plant kingdom, interpreted at the taxonomic scope specified by context.
- **4D anchor:** `(0.803, 0.166, 0.304, 0.336)`
- **Related keys:** `tree`, `forest`, `wood`, `life`, `environment`
- **Glyph seed:** `UTF4088-CONCEPT-V1:plant`

```text
##.#.##.
#..#.###
#..##.##
##.####.
##.#.#.#
.###.#.#
#..#...#
.###...#
#...#...
##..####
.##.#.##
.#...##.
```


### UTF4088-CONCEPT-000013 — Stone (`stone`)

- **Domain:** `material_nature`
- **Provisional definition:** A naturally occurring piece of rock or mineral material, distinguished from a geological formation.
- **4D anchor:** `(0.815, 0.180, 0.310, 0.303)`
- **Related keys:** `wood`, `water`, `resources`, `quality`
- **Glyph seed:** `UTF4088-CONCEPT-V1:stone`

```text
.##.#..#
####...#
...####.
.#.#####
#######.
#.###..#
.#......
#.###..#
##..####
##.#..##
.#...##.
####...#
```


### UTF4088-CONCEPT-000014 — Water (`water`)

- **Domain:** `material_nature`
- **Provisional definition:** The substance H2O or a specified body or supply of it; distinguish substance, resource, and geographic-feature senses.
- **4D anchor:** `(0.827, 0.154, 0.316, 0.312)`
- **Related keys:** `resources`, `life`, `environment`, `asia`
- **Glyph seed:** `UTF4088-CONCEPT-V1:water`

```text
.###..##
#...#.#.
...#...#
####..#.
..#.####
#####.##
.....###
#...###.
.##....#
##..#.##
#.#.####
.#.##.#.
```


### UTF4088-CONCEPT-000015 — Sex (context-dependent) (`sex`)

- **Domain:** `human_experience`
- **Provisional definition:** An intentionally broad, ambiguous head concept. Resolve to biological sex, sexual activity, or sexuality only when context supports that distinction.
- **4D anchor:** `(0.150, 0.800, 0.680, 0.740)`
- **Related keys:** `biological_sex`, `sexual_activity`, `sexuality`, `consent`
- **Glyph seed:** `UTF4088-CONCEPT-V1:sex`

```text
##...#.#
#.###...
.###..##
##.#....
#.##.###
#.##.#.#
#.##..##
##.###.#
#..##...
.###.#.#
###.####
.#..#.##
```


### UTF4088-CONCEPT-000016 — Biological sex (`biological_sex`)

- **Domain:** `human_experience`
- **Provisional definition:** Biological sex characteristics or classification in a stated medical, biological, or demographic context; not a synonym for gender identity.
- **4D anchor:** `(0.162, 0.814, 0.686, 0.749)`
- **Related keys:** `sex`, `reproduction`, `health`
- **Glyph seed:** `UTF4088-CONCEPT-V1:biological_sex`

```text
..#.###.
#..#.#..
....##.#
#....#..
..###...
.###..##
.###.#.#
..#..#.#
#..#.##.
#..###.#
##...#.#
.#.#.###
```


### UTF4088-CONCEPT-000017 — Sexual activity (`sexual_activity`)

- **Domain:** `human_experience`
- **Provisional definition:** Sexual behavior described neutrally for the relevant health, legal, or social context; consent must be represented separately.
- **4D anchor:** `(0.174, 0.828, 0.692, 0.758)`
- **Related keys:** `sex`, `sexuality`, `consent`, `pleasure`, `intimacy`
- **Glyph seed:** `UTF4088-CONCEPT-V1:sexual_activity`

```text
...##...
####.#.#
##.#.#.#
.####...
.###....
....##..
#...####
##...##.
#.#####.
##..#..#
.#..#.##
.####.#.
```


### UTF4088-CONCEPT-000018 — Sexuality (`sexuality`)

- **Domain:** `human_experience`
- **Provisional definition:** The broader domain of sexual identity, orientation, feelings, and expression as defined by the application; not reducible to one act or biological trait.
- **4D anchor:** `(0.186, 0.802, 0.698, 0.767)`
- **Related keys:** `sex`, `sexual_activity`, `desire`, `intimacy`
- **Glyph seed:** `UTF4088-CONCEPT-V1:sexuality`

```text
#.#.#.##
##.#...#
##..#.##
..###..#
....#.##
#...###.
#.###.#.
###.#.##
.##..##.
..###.##
##.###..
#.##....
```


### UTF4088-CONCEPT-000019 — Pleasure (`pleasure`)

- **Domain:** `human_experience`
- **Provisional definition:** A positive subjective hedonic experience; distinct from long-term wellbeing, satisfaction of a goal, and moral approval.
- **4D anchor:** `(0.153, 0.816, 0.704, 0.776)`
- **Related keys:** `satisfaction`, `wellbeing`, `desire`, `pain`
- **Glyph seed:** `UTF4088-CONCEPT-V1:pleasure`

```text
.##..###
.#......
##.###.#
..#.#..#
.......#
#####..#
..######
....#...
.#..#...
#.#...##
##...#..
#.###...
```


### UTF4088-CONCEPT-000020 — Desire (`desire`)

- **Domain:** `human_experience`
- **Provisional definition:** A motivational state directed toward an object, outcome, or experience; specify its object and context when useful.
- **4D anchor:** `(0.165, 0.830, 0.710, 0.743)`
- **Related keys:** `pleasure`, `sexuality`, `wealth`, `relationship`
- **Glyph seed:** `UTF4088-CONCEPT-V1:desire`

```text
.#.#.###
..#..###
..##..##
.#.##.#.
.##....#
.#...#..
.##...##
####.###
.#..#.#.
##.#.##.
.#.#.#..
.##.###.
```


### UTF4088-CONCEPT-000021 — Intimacy (`intimacy`)

- **Domain:** `human_experience`
- **Provisional definition:** Closeness, trust, or private connection between people, with emotional, social, and sexual dimensions distinguished by context.
- **4D anchor:** `(0.177, 0.804, 0.716, 0.752)`
- **Related keys:** `relationship`, `sexual_activity`, `sexuality`, `consent`
- **Glyph seed:** `UTF4088-CONCEPT-V1:intimacy`

```text
##.#....
..###..#
####...#
.#..##..
..###...
....#.#.
.#...###
####.#.#
.##...#.
#.#.##.#
...##.#.
.##..##.
```


### UTF4088-CONCEPT-000022 — Consent (`consent`)

- **Domain:** `human_experience`
- **Provisional definition:** Voluntary, informed, specific, and revocable agreement within a defined context; applicable legal requirements depend on jurisdiction.
- **4D anchor:** `(0.189, 0.818, 0.722, 0.761)`
- **Related keys:** `sexual_activity`, `relationship`, `ethics`
- **Glyph seed:** `UTF4088-CONCEPT-V1:consent`

```text
#.#...#.
..#...#.
.###..##
.#..####
...##...
#.#.#...
##.#####
#....#..
##.##..#
...#....
#.##...#
.#.#.##.
```


### UTF4088-CONCEPT-000023 — Reproduction (`reproduction`)

- **Domain:** `human_experience`
- **Provisional definition:** The biological process by which organisms produce offspring; separate from sexual activity, fertility status, and social parenthood.
- **4D anchor:** `(0.156, 0.832, 0.728, 0.770)`
- **Related keys:** `biological_sex`, `life`, `health`
- **Glyph seed:** `UTF4088-CONCEPT-V1:reproduction`

```text
##.....#
..##..#.
........
####.#..
.#.##.##
##.##..#
##.##.#.
.#.####.
#..#..##
##.#####
.###.#..
..#####.
```


### UTF4088-CONCEPT-000024 — Wellbeing (`wellbeing`)

- **Domain:** `human_experience`
- **Provisional definition:** A multidimensional state of health and quality of life; not identical to momentary pleasure or a single metric.
- **4D anchor:** `(0.168, 0.806, 0.734, 0.779)`
- **Related keys:** `pleasure`, `health`, `satisfaction`, `quality`, `pain`
- **Glyph seed:** `UTF4088-CONCEPT-V1:wellbeing`

```text
#....#..
..######
...#...#
...#.###
..#.####
.#......
.##.####
#######.
###.####
#.##.#.#
.#.#.##.
.##....#
```


### UTF4088-CONCEPT-000025 — Satisfaction (`satisfaction`)

- **Domain:** `human_experience`
- **Provisional definition:** A judgment or feeling that a need, expectation, or goal has been met; distinguish from pleasure and objective quality.
- **4D anchor:** `(0.180, 0.820, 0.740, 0.746)`
- **Related keys:** `pleasure`, `wellbeing`, `quality`, `desire`
- **Glyph seed:** `UTF4088-CONCEPT-V1:satisfaction`

```text
....#..#
.#....##
.###....
..#...##
#####...
........
###.#.##
#..###.#
#.#.###.
...#####
.##.....
.....##.
```


### UTF4088-CONCEPT-000026 — Pain (`pain`)

- **Domain:** `human_experience`
- **Provisional definition:** An unpleasant sensory or emotional experience associated with or resembling actual or potential tissue damage; context may be physical or psychological.
- **4D anchor:** `(0.192, 0.834, 0.746, 0.755)`
- **Related keys:** `pleasure`, `health`, `wellbeing`
- **Glyph seed:** `UTF4088-CONCEPT-V1:pain`

```text
#.#.##.#
##...#..
.##....#
.......#
##..#.##
###.#...
#####..#
.###..##
####.#.#
.##.....
.###...#
#.##..##
```


### UTF4088-CONCEPT-000027 — Health (`health`)

- **Domain:** `human_experience`
- **Provisional definition:** Physical, mental, and social functioning under the chosen health framework; not merely the absence of disease.
- **4D anchor:** `(0.159, 0.808, 0.752, 0.764)`
- **Related keys:** `wellbeing`, `pain`, `reproduction`, `quality`
- **Glyph seed:** `UTF4088-CONCEPT-V1:health`

```text
..#.#.##
.#######
#.####.#
##..#.#.
...##.#.
#..#..#.
..##..#.
###.#.##
#####.#.
##.##.##
.#..#.##
###.#...
```


### UTF4088-CONCEPT-000028 — Relationship (`relationship`)

- **Domain:** `human_experience`
- **Provisional definition:** A connection or association between people, entities, or concepts; specify the relationship type when it affects interpretation.
- **4D anchor:** `(0.171, 0.822, 0.758, 0.773)`
- **Related keys:** `intimacy`, `consent`, `desire`, `trade`
- **Glyph seed:** `UTF4088-CONCEPT-V1:relationship`

```text
#.#.####
#..#....
####....
.#...#..
##......
####..##
..#.....
#.##.#..
..####..
.##.#...
..#.###.
#.#..##.
```


### UTF4088-CONCEPT-000029 — Quality (`quality`)

- **Domain:** `evaluation`
- **Provisional definition:** A property or degree assessed against an explicit criterion, purpose, or standard; not an absolute universal score.
- **4D anchor:** `(0.500, 0.500, 0.430, 0.460)`
- **Related keys:** `standard`, `excellence`, `defect`, `value`, `merit`
- **Glyph seed:** `UTF4088-CONCEPT-V1:quality`

```text
.....##.
.####.##
...#..#.
#.#...#.
#..#.#.#
##..###.
..#####.
.##.###.
....####
.#.###..
..##...#
#.#.##.#
```


### UTF4088-CONCEPT-000030 — Quantity (`quantity`)

- **Domain:** `evaluation`
- **Provisional definition:** An amount or count measured in a declared unit or counting convention; distinct from quality.
- **4D anchor:** `(0.512, 0.514, 0.436, 0.469)`
- **Related keys:** `quality`, `value`, `resources`
- **Glyph seed:** `UTF4088-CONCEPT-V1:quantity`

```text
####.#.#
..#####.
###.##..
########
.#......
..##.###
####..##
.#.#.###
####.###
..#..###
...#...#
##..#.##
```


### UTF4088-CONCEPT-000031 — Value (`value`)

- **Domain:** `evaluation`
- **Provisional definition:** Worth or significance under a specified economic, practical, ethical, or personal framework; record the value type and viewpoint.
- **4D anchor:** `(0.524, 0.528, 0.442, 0.478)`
- **Related keys:** `wealth`, `money`, `quality`, `merit`, `trade`
- **Glyph seed:** `UTF4088-CONCEPT-V1:value`

```text
..#####.
#..#.#.#
.###....
...#....
..###...
#.......
#..#....
.#..##.#
.....###
#...#...
..#.....
#..##.##
```


### UTF4088-CONCEPT-000032 — Merit (`merit`)

- **Domain:** `evaluation`
- **Provisional definition:** A feature considered deserving of recognition under stated criteria; not a universal ranking of people or cultures.
- **4D anchor:** `(0.536, 0.502, 0.448, 0.487)`
- **Related keys:** `quality`, `value`, `excellence`, `standard`
- **Glyph seed:** `UTF4088-CONCEPT-V1:merit`

```text
#.##..#.
.....###
.###...#
#.###..#
.##.##.#
.#.#..#.
..#.##..
##..#.#.
.##...#.
##.#...#
.#.#...#
###..#..
```


### UTF4088-CONCEPT-000033 — Standard (`standard`)

- **Domain:** `evaluation`
- **Provisional definition:** A declared rule, criterion, or reference level against which an object, quality, or process may be assessed.
- **4D anchor:** `(0.503, 0.516, 0.454, 0.496)`
- **Related keys:** `quality`, `excellence`, `defect`, `value`
- **Glyph seed:** `UTF4088-CONCEPT-V1:standard`

```text
..#....#
.##.#...
#...#...
......#.
###..###
##.###..
.####...
......#.
#.#...##
#..#....
..#.###.
#..###.#
```


### UTF4088-CONCEPT-000034 — Defect (`defect`)

- **Domain:** `evaluation`
- **Provisional definition:** A departure from a stated requirement or intended function; classification depends on the declared standard and use.
- **4D anchor:** `(0.515, 0.530, 0.460, 0.463)`
- **Related keys:** `quality`, `standard`, `excellence`
- **Glyph seed:** `UTF4088-CONCEPT-V1:defect`

```text
#.#....#
.##.....
....#..#
...##.#.
.#....##
..#.###.
.#.#.#..
...###..
#.####.#
..###..#
####....
#..#.###
```


### UTF4088-CONCEPT-000035 — Excellence (`excellence`)

- **Domain:** `evaluation`
- **Provisional definition:** A high level of performance or quality against explicit criteria; the criteria and supporting evidence should be recorded.
- **4D anchor:** `(0.527, 0.504, 0.466, 0.472)`
- **Related keys:** `quality`, `merit`, `standard`
- **Glyph seed:** `UTF4088-CONCEPT-V1:excellence`

```text
#.#....#
#..#.##.
.##.#...
.......#
##.###..
....###.
...##.##
.#..##.#
#..##.#.
.#.#..#.
.#..##..
##.....#
```


### UTF4088-CONCEPT-000036 — Asia (`asia`)

- **Domain:** `geography_culture`
- **Provisional definition:** A broad geographic region or continent under a chosen geographic convention; do not infer one culture, language, or identity from the place label.
- **4D anchor:** `(0.820, 0.800, 0.300, 0.340)`
- **Related keys:** `continent`, `region`, `country`, `geography`, `culture`, `language`
- **Glyph seed:** `UTF4088-CONCEPT-V1:asia`

```text
####..##
.##.#.#.
.#.##.##
#..#..##
.###.###
.####..#
.##..#.#
#####...
.#......
#.#...##
#..####.
#.###...
```


### UTF4088-CONCEPT-000037 — Continent (`continent`)

- **Domain:** `geography_culture`
- **Provisional definition:** A large landmass or recognized continental region under a named geographic convention; boundaries may vary by convention.
- **4D anchor:** `(0.832, 0.814, 0.306, 0.349)`
- **Related keys:** `asia`, `geography`, `region`
- **Glyph seed:** `UTF4088-CONCEPT-V1:continent`

```text
#.#.#.#.
.#.....#
#.####.#
....#.##
#...##.#
#..#####
###..#..
##..#...
...##.#.
###...##
..##.###
.###..##
```


### UTF4088-CONCEPT-000038 — Region (`region`)

- **Domain:** `geography_culture`
- **Provisional definition:** A geographic or functional area defined by explicit boundaries or criteria; the label alone does not define political status or culture.
- **4D anchor:** `(0.844, 0.828, 0.312, 0.358)`
- **Related keys:** `asia`, `continent`, `country`, `geography`, `forest`
- **Glyph seed:** `UTF4088-CONCEPT-V1:region`

```text
.###.###
...#...#
#..###..
##.#.#..
###.#.##
##.#....
###..#.#
.#...#..
######..
###.##..
.######.
###.###.
```


### UTF4088-CONCEPT-000039 — Country (`country`)

- **Domain:** `geography_culture`
- **Provisional definition:** A country or state under a declared political or geographic model; country, nation, culture, and language are not interchangeable.
- **4D anchor:** `(0.856, 0.802, 0.318, 0.367)`
- **Related keys:** `region`, `asia`, `culture`, `language`, `trade`
- **Glyph seed:** `UTF4088-CONCEPT-V1:country`

```text
.#..#..#
##....##
..#####.
..####.#
.###.#..
...#...#
.#.##..#
##.#.#.#
#.#..##.
##.#.#.#
##.....#
.###..##
```


### UTF4088-CONCEPT-000040 — Geography (`geography`)

- **Domain:** `geography_culture`
- **Provisional definition:** The study or representation of places, spatial relations, environments, and human activity; distinct from any single place name.
- **4D anchor:** `(0.823, 0.816, 0.324, 0.376)`
- **Related keys:** `asia`, `continent`, `region`, `environment`
- **Glyph seed:** `UTF4088-CONCEPT-V1:geography`

```text
.#..###.
.##.....
.#.##..#
..#.##.#
.......#
###.#.##
##...###
.##.####
###...##
###.#.#.
#......#
..#..#..
```


### UTF4088-CONCEPT-000041 — Culture (`culture`)

- **Domain:** `geography_culture`
- **Provisional definition:** Shared and changing practices, meanings, institutions, and artifacts in a group or setting; avoid assuming a country has only one culture.
- **4D anchor:** `(0.835, 0.830, 0.330, 0.343)`
- **Related keys:** `asia`, `region`, `country`, `language`
- **Glyph seed:** `UTF4088-CONCEPT-V1:culture`

```text
##..#.#.
###.###.
....###.
.#...###
....#.##
#..###.#
####..#.
.###.##.
###.#...
.#......
###.#..#
#...####
```


### UTF4088-CONCEPT-000042 — Language (`language`)

- **Domain:** `geography_culture`
- **Provisional definition:** A system of communication with linguistic structure and community use; not equivalent to ethnicity, nationality, or culture.
- **4D anchor:** `(0.847, 0.804, 0.336, 0.352)`
- **Related keys:** `culture`, `region`, `asia`
- **Glyph seed:** `UTF4088-CONCEPT-V1:language`

```text
........
#.#..###
..#....#
......#.
.###.###
.#####.#
.#..#.##
#.#..##.
#...#.##
#.#..##.
#.#.#..#
##...#..
```


### UTF4088-CONCEPT-000043 — Environment (`environment`)

- **Domain:** `material_nature`
- **Provisional definition:** The surrounding physical, biological, and social conditions relevant to an organism or system; define the boundary and scale.
- **4D anchor:** `(0.839, 0.168, 0.322, 0.321)`
- **Related keys:** `forest`, `tree`, `plant`, `water`, `geography`
- **Glyph seed:** `UTF4088-CONCEPT-V1:environment`

```text
####.###
##...##.
##...##.
.#.##..#
..####.#
.#.#....
#..###.#
###..#..
##..###.
.#.#....
.#..####
..#.#..#
```


### UTF4088-CONCEPT-000044 — Life (`life`)

- **Domain:** `material_nature`
- **Provisional definition:** The state or processes associated with living organisms under the selected domain definition; no universal operational definition is assumed.
- **4D anchor:** `(0.806, 0.182, 0.328, 0.330)`
- **Related keys:** `plant`, `tree`, `reproduction`, `health`, `environment`
- **Glyph seed:** `UTF4088-CONCEPT-V1:life`

```text
.#.####.
##...###
.#.#....
.#.#...#
..###...
...##.#.
#####.#.
...#....
..###..#
###.###.
#..##...
#.######
```

## 4. Interpreting the requested concepts carefully

- **Wealth** describes a stock of resources/assets, not the flow called income or the exchange instrument called money.
- **Wood** is a material; **tree** is an organism; **forest** is an ecosystem; **timber** is wood prepared for material use.
- **Sex** is retained as an ambiguity-bearing head concept. The registry distinguishes **biological sex**, **sexual activity**, and **sexuality**. It does not equate biological sex with gender identity, and sexual activity has a separate **consent** concept.
- **Pleasure** is immediate subjective hedonic experience; **satisfaction** concerns whether a need/goal/expectation was met; **wellbeing** is broader and multidimensional.
- **Quality** is assessed against a stated criterion or **standard**. It is distinct from **quantity**, **value**, **merit**, **excellence**, and **defect**.
- **Asia** is represented as a geographic region/continent under a selected convention. It does not assert that Asia has one language, culture, nationality, or identity; related symbols keep **continent**, **region**, **country**, **geography**, **culture**, and **language** distinct.

The definitions and relations are starter annotations and should acquire evidence/provenance before any term is marked verified. Ambiguous language remains ambiguous rather than being forced into one universal sense.

## 5. Source implementation and stability

- [`concept_symbols.hpp`](../../utf-4088/semantics/concept_symbols.hpp) declares the public concept record and lookup interface.
- [`concept_symbols.cpp`](../../utf-4088/semantics/concept_symbols.cpp) contains the versioned seed list, curated anchor coordinates, and deterministic C++ glyph generation.
- [`character_meanings.json`](../../utf-4088/semantics/character_meanings.json) mirrors definitions, relations, anchors, glyph seeds, and glyph previews for review/tools.
- [`symbol_resolution.cpp`](../../utf-4088/algebra/symbol_resolution.cpp) contains the four-coordinate distance and path-scoring implementation.
- [`glyph8x12.cpp`](../../utf-4088/algebra/glyph8x12.cpp) implements deterministic glyph generation and topology metrics.
- [`digraph_engine.cpp`](../../utf-4088/digraph_engine.cpp) coalesces identical packed state IDs and repeated directed transitions.

IDs and glyph seeds are append-only within registry version 1. Do not renumber existing concepts or reuse retired IDs. A material definition, related-key, or anchor change should be reviewed as a semantic-registry revision. If anchor normalization changes, version the geometry contract and migrate consumers explicitly.

## 6. What coincidence currently means—and what remains future work

There are three separate mechanisms that must not be conflated:

1. **Exact concept-key identity:** e.g. `wealth` is the same registry key each time.
2. **Geometric proximity:** two anchor vectors are near one another under the declared Euclidean metric. The anchors here are deliberately clustered provisional inputs.
3. **Graph coalescence:** the existing digraph engine merges field states only when its quantized packed IDs match, and merges edges only when their ordered `(from,to)` pair matches.

The current source does **not** yet implement a general threshold for concept-anchor coincidence, a continuous geometric intersection engine, or automatic conversion of `related_keys` into semantic graph edges. The new C++ registry makes concepts and anchors available for such later work without falsely claiming those behaviors already exist.

## 7. Registry counts and boundaries

- **44** concept records, IDs `UTF4088-CONCEPT-000001` through `UTF4088-CONCEPT-000044`.
- **44** deterministic 8×12 glyph bitmaps, unique within this starter set.
- Concept anchors are synthetic normalized coordinates designed for exploratory domain neighborhoods.
- Definitions and semantic relationships are provisional annotations, not facts inferred from a bitmap.
- The existing 16,606-record front-end registry remains a separate experimental ID/shape/meaning-ID generator; its `meaning_id` hash is not automatically a populated semantic definition.
- UTF-4088 remains experimental and is not a Unicode standard or encoding.
