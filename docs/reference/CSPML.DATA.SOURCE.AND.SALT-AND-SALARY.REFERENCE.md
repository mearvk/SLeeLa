<img align="right" src="../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# CSPML Data Source Reference and Salt & Salary Research Framework

**Document class:** Cross-repository data-source and research reference  
**Maintained by:** SLeeLa project  
**Primary external data source:** [CSPML — City System Planning Markup Language](https://github.com/mearvk/CSPML)  
**Primary dataset:** [USA Cities XML Loads](https://github.com/mearvk/CSPML/tree/master/usa-cities-xml-loads)  
**Source branch:** `master`  
**Status:** Reference and integration guidance; not a claim that a live connector or automated import is already implemented.

---

## 1. Purpose

This document establishes CSPML as a recognized external data source for work performed in the SLeeLa repository. It also defines how CSPML may support the **Salt & Salary** research and application work: place-based economic context, compensation research, household resources, education, labor-market context, and the relationship between essential commodities and the value of work.

SLeeLa is the software, language, runtime, and application environment. CSPML is a separately maintained data project. The two repositories remain independently versioned and governed. A SLeeLa component may read, transform, validate, cache, or analyze CSPML data, but must preserve the identity and provenance of the source records.

CSPML is a private GitHub repository. Access requires an authorized GitHub account and repository permission. A link in this document is a reference, not a grant of access.

## 2. Authoritative repository references

| Resource | Reference | Intended use |
|---|---|---|
| CSPML repository | [mearvk/CSPML](https://github.com/mearvk/CSPML) | Project identity, schemas, generators, workflows, and source documentation |
| U.S. place dataset | [usa-cities-xml-loads](https://github.com/mearvk/CSPML/tree/master/usa-cities-xml-loads) | National, state, and individual Census-place records |
| Dataset README | [USA Cities XML Loads README](https://github.com/mearvk/CSPML/blob/master/usa-cities-xml-loads/README.md) | Record structure, measures, source vintages, generation, validation, and limitations |
| National index | [index.xml](https://github.com/mearvk/CSPML/blob/master/usa-cities-xml-loads/index.xml) | State/covered-area navigation and national index |
| Public-school directory | [education/usa-schools-by-state.xml](https://github.com/mearvk/CSPML/blob/master/usa-cities-xml-loads/education/usa-schools-by-state.xml) | Public elementary and secondary school directory by state |
| Place schema | [schema/](https://github.com/mearvk/CSPML/tree/master/usa-cities-xml-loads/schema) | XML structure and validation contracts |

The dataset README is the first stop for changes to the CSPML data model. Do not rely on a copied summary here when the current source README, schema, or XML record provides a more recent and precise definition.

## 3. What CSPML contributes

The USA Cities XML Loads dataset is organized around U.S. Census places rather than an informal assumption that every named place is an incorporated city. Its documented structure includes:

- A national index and alphabetical state/covered-area indexes.
- Individual place XML records, with state and GEOID retained to distinguish duplicate place names.
- Place identity, Census classification, geographic codes, land and water area, and internal-point coordinates.
- 2024 ACS 5-year measures, where published and available, including population, median household income, median age, housing units, poverty, educational attainment, civilian labor force, unemployment, broadband subscriptions, median owner-occupied home value, median gross rent, and owner/renter counts.
- Published margins of error where available.
- A separate state-level research-indicator file, with distinct source series kept separate rather than silently averaged.
- A separate public-school directory based on the NCES Common Core of Data 2024–25 Public Elementary/Secondary School Universe.

The source documentation describes 50-state research indicators and the broader place geography as including the District of Columbia and Puerto Rico where represented by the Census Gazetteer. Consumers must inspect the actual index and record coverage rather than assume every geography or field is populated.

Dataset counts are intentionally not frozen in this reference. Counts change as the generator and source data are refreshed; obtain current totals from the repository's indexes, validation output, and workflow artifacts.

## 4. Salt & Salary: intended research use

**Salt & Salary** is treated here as a research and application domain that connects the material conditions of places with the economic value and adequacy of work. CSPML can supply geographic and socioeconomic context for that work.

Potential analytical dimensions include:

1. **Place and population context** — identify the relevant Census place, state, GEOID, population, and geographic classification.
2. **Household economic context** — use median household income, poverty, housing values, and gross rent to describe the local economic setting.
3. **Workforce context** — use civilian labor-force and unemployment measures as contextual indicators, retaining their published year, definition, and uncertainty.
4. **Education context** — use educational-attainment measures and the separate public-school directory to describe local educational infrastructure. Do not treat school-directory presence as a direct measure of educational outcomes.
5. **Compensation context** — compare a proposed or observed salary with local income and housing measures only after defining the comparison population, time period, household/person basis, and geographic match.
6. **Essential-goods context** — use separately sourced and dated commodity-price or household-expenditure data when evaluating the cost of essential goods. CSPML does not, by itself, provide a complete historical or current salt-price series.
7. **Longitudinal comparison** — align data vintages before comparing places or years; never imply that an ACS 5-year estimate is a point-in-time census count.

CSPML's median household income is not an individual salary, an average wage, or a guaranteed measure of disposable income. It must retain its original statistical meaning in every Salt & Salary output.

## 5. Salt and salary: historical and conceptual care

The words *salt* and *salary* are etymologically related through Latin `sal` and `salarium`. That relationship does **not** establish that Roman soldiers were routinely paid directly in physical salt. The popular direct-payment story is disputed and should not be presented as settled historical fact.

For Salt & Salary materials:

- Distinguish established linguistic relationships from conjectures about the exact historical payment mechanism.
- Do not state that Roman soldiers were universally or routinely paid in salt unless a specific primary source supports the precise claim.
- Treat historical salt production, taxation, trade, prices, transport, and access as separate research questions requiring period- and region-specific historical sources.
- Keep modern wages, modern commodity prices, and ancient economic arrangements in separate time series and analytical categories.
- Use the salt/salary connection as a useful conceptual frame for essential resources and compensation, not as a substitute for evidence.

A useful secondary discussion of the historical claim is Peter Gainsford, [“Salt and salary: were Roman soldiers paid in salt?”](https://kiwihellenist.blogspot.com/2017/01/salt-and-salary.html). Its central caution is that the familiar direct-salt-payment story is not demonstrated by the often-repeated Pliny quotation. A concise etymological overview with the same qualification is available at [Wordorigins — “salary”](https://www.wordorigins.org/big-list-entries/salary). Consult primary texts and specialist scholarship before making a definitive historical assertion.

## 6. Data use, provenance, and attribution rules

Every CSPML-derived value used by SLeeLa or a Salt & Salary application should carry, directly or through a traceable record:

- Source repository and branch or immutable commit identifier.
- Source file path and, where practical, record identifier/GEOID.
- Original publisher or dataset named by CSPML.
- Geography and geographic classification.
- Measure name, unit, population/universe, and definition.
- Reference year or multi-year period.
- Estimate and margin of error, where supplied.
- Retrieval or import date.
- Transformation, calculation, and join method.
- Missingness, suppression, or quality notes.

Preserve source values separately from derived values. Derived ratios, rankings, indexes, modeled values, and salary comparisons must identify their formula and inputs. Never overwrite an original source field with a calculated result.

When a value is absent, suppressed, stale, unmatched, or not supported by the source, represent it as unavailable (for example, `N/A`) with a reason where possible. Do not infer a value from a neighboring city, state average, school rating, or unrelated geographic unit.

## 7. Geographic matching and statistical boundaries

The Census place is the primary unit for place-level joins. Use GEOID as the stable key when available; do not join solely on a city name. Preserve state and place classification because identical names can occur in multiple states and because a Census-designated place is not necessarily an incorporated municipality.

For school records, use the school identifier and the documented school location. If a school is associated with a city-level measure, document the geographic crosswalk and whether the school point, mailing address, district, county, or another boundary was used. Do not imply that a school district and a Census place are equivalent.

For salary and labor comparisons, define whether the measure refers to an individual, worker, household, full-time worker, or all residents. A city median household income must not be relabeled as a local salary benchmark.

## 8. Integration guidance for SLeeLa

CSPML should be consumed as an external data source, not copied wholesale into the SLeeLa language or runtime repository.

A future or existing integration should follow these boundaries:

- **Acquisition:** retrieve a pinned CSPML revision or a deliberately selected release/snapshot. Avoid silently tracking a moving branch in reproducible builds.
- **Parsing:** use the SLeeLa XML facilities and the CSPML schema/documentation as the data contract. Reject malformed XML and invalid required identifiers.
- **Validation:** validate source records before transformation; report record counts, duplicate identifiers, missing fields, and index-to-record consistency.
- **Normalization:** normalize for internal queries only while retaining the original source value, source path, and provenance.
- **Storage:** keep imported snapshots and generated analysis outputs distinct from authoritative CSPML source files.
- **Updates:** make refreshes explicit and auditable. A new source revision should produce a traceable import/update report.
- **Security:** do not embed credentials, access tokens, or private repository URLs containing credentials in source code, generated documentation, logs, or committed configuration.
- **Licensing and access:** verify the current license and redistribution terms before copying CSPML records into a public artifact or distributing a derived dataset. Repository access does not itself establish redistribution permission.

No live API, package dependency, automatic synchronization, or runtime binding is implied merely by this reference document. Such functionality requires a separately designed, implemented, and tested integration.

## 9. Recommended source hierarchy for Salt & Salary

Use CSPML for the place-level demographic, household-income, housing, workforce, education, and public-school context described above. Supplement it with purpose-built sources for the questions CSPML does not answer.

| Research question | Preferred source class | Handling |
|---|---|---|
| Place identity and boundaries | U.S. Census Gazetteer / CSPML place records | Preserve GEOID and place type |
| Household income and poverty | ACS 5-year / CSPML fields | Preserve estimate, year, and margin of error |
| Housing cost context | ACS 5-year / CSPML fields | Keep rent and owner value as separate measures |
| Labor force and unemployment | ACS or official labor statistics | Preserve universe, period, and geography |
| Public-school identity and location | NCES CCD / CSPML school directory | Do not infer athletic class, SAT, or outcomes |
| Postsecondary institutions | NCES IPEDS or another authoritative directory | Match institutions separately from resident attainment |
| Current wages and occupational pay | BLS and other authoritative labor datasets | Match occupation, geography, period, and worker basis |
| Current prices and cost of essentials | Official price series or transparent local price observations | Preserve item definition, unit, date, and collection method |
| Historical salt economy | Primary historical records and specialist scholarship | Keep historical evidence distinct from modern statistics |

## 10. Quality controls before publication

Before a Salt & Salary report or application publishes a CSPML-derived comparison:

- Confirm the source revision and the relevant record's GEOID.
- Confirm that the measure's year, geography, unit, and population basis match the comparison.
- Preserve margins of error and note when overlapping uncertainty makes small differences inconclusive.
- Separate estimates from observed administrative counts.
- Separate household income from personal wages and salaries.
- Avoid causal claims from descriptive place-level statistics.
- Do not rank places unless the ranking method, coverage, missing-data treatment, and tie rules are stated.
- Do not create a lifestyle score or cost-of-living index by intuition; define and source every component.
- Keep state-level research indicators separate from city/place records and do not assign state estimates to individuals or localities.
- Label unavailable data honestly and avoid filling gaps with unsupported proxies.
- Re-run the CSPML validation workflow or its documented validator after a source refresh; record the observed result rather than assuming success.

## 11. Citation and implementation convention

When SLeeLa documentation, source code comments, reports, or Salt & Salary outputs rely on CSPML, cite the dataset as:

> **City System Planning Markup Language (CSPML), USA Cities XML Loads**, `mearvk/CSPML`, `master`, [dataset documentation](https://github.com/mearvk/CSPML/tree/master/usa-cities-xml-loads). Record the specific source file, GEOID, source vintage, and commit or snapshot used.

For reproducible analysis, record the exact CSPML commit SHA in the output metadata. A branch URL is a navigation reference, not an immutable citation.

## 12. Maintenance

Update this document when the CSPML dataset layout, schemas, source vintages, school directory, or access model materially changes. The CSPML repository remains authoritative for its own data definitions and generation status. SLeeLa remains authoritative for its own parser, runtime, and integration behavior.

**Principle:** CSPML supplies traceable place-based evidence; SLeeLa supplies the tools to process it; Salt & Salary supplies a research context. None of these roles permits a source estimate to be silently redefined, a missing value to be invented, or a historical hypothesis to be reported as fact.
