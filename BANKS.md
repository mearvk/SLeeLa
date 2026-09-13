# SLeeLa — BANKS.md

## HTTP 3.0 National Banking, Economic & Historical Registry

This document expands the national banking registry into a structured economic and historical partition for the project's declared **391-country/jurisdiction universe**. It is intended to drive machine-readable country records while keeping historical uncertainty explicit.

## 1. National Partitions

| Partition | Required | Program purpose |
|---|---:|---|
| Identity | Yes | Stable country/jurisdiction key. |
| Founding / State Formation | Yes | Founding date/period, constitutional formation, independence, restoration or succession. |
| Banking | Yes | Central bank, monetary authority and banking structure. |
| Economic Influence | Yes | Domestic, regional and global influence across several dimensions. |
| Economy | Yes | Economic structure, sectors, exports, imports and development profile. |
| Currency | Yes | Currency and monetary arrangement. |
| Banking Supervision | Yes | Principal supervisory authority. |
| Payment Systems | Preferred | National or shared payment infrastructure. |
| Corporate Sector | Preferred | Public, private, cooperative, state and mixed ownership. |
| Corporate Struggles | Preferred | Documented bankruptcy, restructuring, governance, labor or systemic business difficulties. |
| Economic Crises | Preferred | Financial, fiscal, monetary and economic crises. |
| Trade | Preferred | Principal partners, arrangements and dependencies. |
| Resources | Preferred | Major natural/resource endowments. |
| Socialist Period | Yes | Years and classification of socialist or substantially socialist governance/economic systems. |
| Capital / Ownership | Preferred | Public, private, mixed, cooperative and state participation. |
| Labor / Employment | Preferred | Major labor-market characteristics. |
| Economic Institutions | Preferred | Ministries, development banks, sovereign funds and similar institutions. |
| Verification | Yes | Source, date and provenance. |

## 2. Program Record

    COUNTRY_ECONOMIC_RECORD {
        registry_id
        country_name
        iso_alpha2
        iso_alpha3
        country_start_year
        country_end_year
        state_formation
        banking
        economic_influence
        economy
        currency
        banking_supervision
        payment_systems
        corporate_sector
        corporate_struggles[]
        economic_crises[]
        trade
        resources
        socialist_periods[]
        ownership
        labor
        economic_institutions
        verification
    }

## 3. Economic Influence

Economic influence must not be reduced to GDP. The record should distinguish production, finance, trade, resources, technology, monetary influence, corporate influence and institutional influence at domestic, regional and global levels.

Recommended values:

    VERY_HIGH
    HIGH
    MEDIUM
    LOW
    LIMITED
    UNASSESSED

Any future numeric score must remain source-backed and explainable.

## 4. Country Start / End Years and State Formation

Each registry entry should carry a source-backed **country start year** and, where the political entity has ceased to exist, a **country end year**. For a currently existing country or jurisdiction, `country_end_year` remains `OPEN` rather than being assigned an artificial future date.

These fields describe the existence of the particular registry entity, not the age of its civilization, people, language, territory, or predecessor states. Historical predecessor/successor relationships remain in `state_formation`.

Recommended fields:

    COUNTRY_EXISTENCE {
        country_start_year
        country_end_year
        start_event_type
        end_event_type
        predecessor
        successor
        confidence
        source
        verification_date
    }

A country may have multiple historically important dates. Preserve ancient origins, independence, constitutional founding, modern state formation, restoration, unification, dissolution/succession and transitions where applicable. Do not force a disputed history into one arbitrary date.

    FOUNDING {
        country_start_year
        country_end_year
        date
        period
        event_type
        predecessor
        successor
        confidence
        source
    }

## 5. Corporate Sector and Corporate Struggles

Corporate data should characterize the structure of economic ownership and record documented difficulties without treating financial difficulty as proof of misconduct.

Useful ownership categories:

    STATE_OWNED
    PRIVATELY_OWNED
    PUBLICLY_TRADED
    COOPERATIVE
    FAMILY_CONTROLLED
    MIXED_OWNERSHIP
    FOREIGN_CONTROLLED
    STATE_PARTICIPATING

Corporate-struggle records may include bankruptcy, insolvency, restructuring, nationalization, privatization disputes, banking failures, governance crises, major labor disputes, sanctions affecting companies, sovereign-corporate disputes and systemic corporate distress.

    CORPORATE_STRUGGLE {
        period
        entity_or_sector
        event_type
        materiality
        description
        outcome
        source
    }

## 6. Socialist Period

Each of the 391 records should contain socialist_periods. This answers the historical question of which years, if any, qualify under the project's defined criteria as socialist, communist, Marxist-Leninist or substantially socialist governance/economic organization.

Recommended statuses:

    NONE
    SOCIALIST
    COMMUNIST
    MARXIST_LENINIST
    SUBSTANTIALLY_SOCIALIST
    MIXED_OR_TRANSITIONAL
    DISPUTED
    UNASSESSED

Each interval contains:

| Field | Meaning |
|---|---|
| status | Historical classification. |
| start_year | Beginning of qualifying period. |
| end_year | End of qualifying period. |
| qualifying_basis | Political, institutional and/or economic basis. |
| government_or_system | Relevant government or economic system. |
| source | Evidence supporting the classification. |

Multiple qualifying periods are retained rather than overwritten.

## 7. Socialist-Year Derivation

The program should preserve the actual intervals and derive total years from their union:

    SOCIALIST_YEARS = union(all verified qualifying socialist periods)

Do not merely subtract the first start year from the last end year when periods overlap or are discontinuous. This makes the requested “years their country was at least Socialist” a reproducible derived field.

## 8. Banking + Economic Relationship

    COUNTRY
      |
      +-- FOUNDING
      +-- BANKING
      +-- ECONOMIC_INFLUENCE
      +-- ECONOMY
      +-- CURRENCY
      +-- CORPORATE_SECTOR
      +-- CORPORATE_STRUGGLES
      +-- ECONOMIC_CRISES
      +-- TRADE
      +-- RESOURCES
      +-- SOCIALIST_PERIODS

HTTP 3.0 consumes these as application data. The transport layer should not reinterpret political or economic classifications.

## 9. 391-Country Table Contract

One normalized row is required for every member of the canonical 391-entry registry:

| ID | Country/Jurisdiction | Country Start Year | Country End Year | Founded / State Formation | Economic Influence | Economy | Banking | Corporate Sector | Corporate Struggles | Socialist Years | Status |
|---|---|---|---|---|---|---|---|---|---|
| 001–391 | Canonical registry entries | PENDING | OPEN/PENDING | PENDING | UNASSESSED | PENDING | PENDING | PENDING | PENDING | PENDING | MISSING |

The actual 391 names and identifiers must be imported from SLeeLa's canonical country registry. This document does not invent country assignments.

## 10. Completeness

Maintain independent coverage counts:

    EXPECTED = 391
    IDENTITY_COMPLETE
    COUNTRY_EXISTENCE_COMPLETE
    FOUNDING_COMPLETE
    BANKING_COMPLETE
    INFLUENCE_COMPLETE
    ECONOMY_COMPLETE
    CORPORATE_COMPLETE
    SOCIALIST_HISTORY_COMPLETE
    VERIFIED_COMPLETE

A country may be complete in banking but partial in historical data. Overall readiness requires all required partitions to be complete for all 391 registry members.

## 11. Country Existence Data Rule

For `country_start_year` and `country_end_year`, use the date at which the specific registry entity was established, constituted, declared independent, restored, unified, or otherwise became the relevant political entity, according to the selected historical definition. For an entity that dissolved, was annexed, merged, or was otherwise superseded, record the end year and the successor event.

Do not silently substitute an ancient civilization date for the modern state date. If multiple definitions are historically defensible, retain the primary date and record the alternatives in `state_formation` with an explicit confidence/source field. Use `OPEN` for entities that still exist. Use `UNKNOWN` where a responsible source cannot establish a defensible year.

The generated 391-row economic table currently remains an economic dataset; its new existence-year columns should not be populated with guessed dates. A future data pass should source and verify each row independently.

## 12. Provenance and No-Inference Rule

Every assertion should be traceable as:

    COUNTRY → PARTITION → FIELD → VALUE → SOURCE → VERIFICATION DATE

Preferred sources are national governments, central banks, monetary authorities, statistical agencies, regulators, official legislation/archives, recognized international institutions and reputable secondary historical/economic sources.

Distinguish explicitly between UNKNOWN, NONE, NOT_APPLICABLE, DISPUTED, UNASSESSED and VERIFIED. Absence of evidence for a socialist period is not automatically evidence that no socialist period existed.

## 13. Suggested Machine Layout

    data/
      banking/
        registry.json
        schema.json
        sources.json
        countries/<registry-id>.json
      economics/
        registry.json
        schema.json
        countries/<registry-id>.json

Suggested operations:

    country.record(id)
    country.banking(id)
    country.economy(id)
    country.influence(id)
    country.corporate(id)
    country.socialist_history(id)
    registry.coverage()
    registry.require_complete(id)
    registry.require_all_complete()

## 14. Implementation Status

**BANKS.md is now the national banking, economic and historical partition contract.** The schema is expanded to include economic influence, founding/state formation, corporate structure and struggles, crises, trade, resources, ownership, labor, institutions and verified socialist-period history.

The next data pass should populate all 391 individual records from SLeeLa's canonical registry and independently verify the values. No placeholder should be promoted to COMPLETE without provenance.

## 391-Country National Economic Table

This generated table is an evidence-backed data pull. `N/A` means the source did not provide a responsible value; it is not a zero. Banking authorities remain unpopulated unless independently sourced.

# SLeeLa — BANKS3.md

## HTTP 3.0 National Banking, Economic & Historical Registry — Combined 391-Country Table

This is the materialized union of `BANKS.md` and `BANKS2.md`, joined by the canonical three-digit registry ID and country/jurisdiction. Economic, banking and status fields come from `BANKS.md`; Currency comes from `BANKS2.md`.

| ID | Country/Jurisdiction | ISO | Currency | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Status |
|---:|---|---|---|---:|---:|---:|---:|---|---|---|
| 001 | Afghanistan | AF / AFG | AFN | $17.78B | 416.87 | -6.60 | 83.76 | Low income | Da Afghanistan Bank | YES |
| 002 | Albania | AL / ALB | ALL | $30.54B | $12,998 | 2.15 | 79.53 | Upper middle income | Bank of Albania | CONVINCED |
| 003 | Algeria | DZ / DZA | DZD | $287.03B | $6,051 | 1.42 | 38.98 | Upper middle income | Bank of Algeria | YES |
| 004 | American Samoa | AS / ASM | USD | $871.00M | $18,017 | N/A | 124.68 | High income | Federal Reserve System / American Samoa banking institutions | YES |
| 005 | Andorra | AD / AND | EUR | $4.50B | $54,292 | N/A | N/A | High income | Andorran Financial Authority (AFA) | YES |
| 006 | Angola | AO / AGO | AOA | $122.17B | $3,129 | 20.16 | 34.65 | Lower middle income | National Bank of Angola | YES |
| 007 | Anguilla | N/A / N/A | XCD | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | YES |
| 008 | Antarctica | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Antarctica (BIS Central Bank Hub reference) | YES |
| 009 | Antigua and Barbuda | AG / ATG | XCD | $2.34B | $24,819 | 1.37 | 118.30 | High income | Eastern Caribbean Central Bank (ECCB) | YES |
| 010 | Argentina | AR / ARG | ARS | $683.10B | $14,898 | 219.88 | 30.36 | Upper middle income | Central Bank of Argentina | YES |
| 011 | Armenia | AM / ARM | AMD | $29.24B | $9,474 | 3.31 | 101.71 | Upper middle income | Central Bank of Armenia | CONVINCED |
| 012 | Aruba | AW / ABW | AWG | $4.17B | $38,591 | 4.26 | 164.14 | High income | Central Bank of Aruba | YES |
| 013 | Australia | AU / AUS | AUD | $1798.52B | $65,130 | 2.87 | 45.90 | High income | Reserve Bank of Australia | YES |
| 014 | Austria | AT / AUT | EUR | $579.47B | $62,930 | 3.53 | 107.02 | High income | Oesterreichische Nationalbank | YES |
| 015 | Azerbaijan | AZ / AZE | AZN | $75.94B | $7,411 | 5.62 | 79.83 | Upper middle income | Central Bank of Azerbaijan | CONVINCED |
| 016 | Bahamas | N/A / N/A | BSD | N/A | N/A | N/A | N/A | N/A | Central Bank of The Bahamas | YES |
| 017 | Bahrain | BH / BHR | BHD | $48.97B | $30,597 | -0.14 | 157.48 | High income | Central Bank of Bahrain | YES |
| 018 | Bangladesh | BD / BGD | BDT | $456.32B | $2,597 | 8.77 | 27.95 | Lower middle income | Bangladesh Bank | INDEED |
| 019 | Barbados | BB / BRB | BBD | $8.02B | $28,365 | 0.85 | N/A | High income | Central Bank of Barbados | YES |
| 020 | Belarus | BY / BLR | BYN | $93.40B | $10,279 | 6.60 | 112.11 | Upper middle income | National Bank of the Republic of Belarus | CONVINCED |
| 021 | Belgium | BE / BEL | EUR | $725.47B | $60,750 | 2.47 | 152.92 | High income | National Bank of Belgium / European Central Bank | YES |
| 022 | Belize | BZ / BLZ | BZD | $3.33B | $7,865 | 1.06 | 108.97 | Upper middle income | Central Bank of Belize | YES |
| 023 | Benin | BJ / BEN | XOF | $24.57B | $1,658 | 1.10 | 37.12 | Lower middle income | National banking and monetary authority — Benin (BIS Central Bank Hub reference) | YES |
| 024 | Bermuda | BM / BMU | BMD | $9.19B | $142,250 | N/A | 80.21 | High income | Bermuda Monetary Authority | YES |
| 025 | Bhutan | BT / BTN | BTN / INR | $3.58B | $4,493 | 3.56 | 79.36 | Lower middle income | Royal Monetary Authority of Bhutan | YES |
| 026 | Bolivia | BO / BOL | BOB | $64.77B | $5,148 | 19.52 | 46.98 | Lower middle income | Central Bank of Bolivia | YES |
| 027 | Bonaire, Sint Eustatius and Saba | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Bonaire, Sint Eustatius and Saba (BIS Central Bank Hub reference) | YES |
| 028 | Bosnia and Herzegovina | BA / BIH | BAM | $32.60B | $10,382 | 6.11 | 95.12 | Upper middle income | Central Bank of Bosnia and Herzegovina | CONVINCED |
| 029 | Botswana | BW / BWA | BWP | $19.93B | $7,778 | 2.66 | 69.54 | Upper middle income | Bank of Botswana | YES |
| 030 | Bouvet Island | N/A / N/A | NOK | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Bouvet Island (BIS Central Bank Hub reference) | YES |
| 031 | Brazil | BR / BRA | BRL | $2279.92B | $10,713 | 5.02 | 35.29 | Upper middle income | Central Bank of Brazil | YES |
| 032 | British Indian Ocean Territory | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — British Indian Ocean Territory (BIS Central Bank Hub reference) | YES |
| 033 | Brunei | BN / BRN | BND | $15.03B | $32,235 | -0.30 | 123.61 | High income | Brunei Darussalam Central Bank (AMBD) | YES |
| 034 | Bulgaria | BG / BGR | EUR | $130.78B | $20,328 | 4.58 | 102.51 | High income | Bulgarian National Bank / European Central Bank | CONVINCED |
| 035 | Burkina Faso | BF / BFA | XOF | $27.63B | $1,148 | -0.59 | 65.00 | Low income | Central Bank of West African States (BCEAO) | YES |
| 036 | Burundi | BI / BDI | BIF | $3.36B | 233.82 | 34.13 | N/A | Low income | Bank of the Republic of Burundi | YES |
| 037 | Cabo Verde | CV / CPV | CVE | $3.06B | $5,796 | 2.34 | 95.33 | Upper middle income | Bank of Cabo Verde | YES |
| 038 | Cambodia | KH / KHM | KHR | $51.27B | $2,872 | 2.35 | 136.09 | Lower middle income | National Bank of Cambodia | CONVINCED |
| 039 | Cameroon | CM / CMR | XAF | $58.93B | $1,972 | 3.40 | 33.38 | Lower middle income | Bank of Central African States (BEAC) | YES |
| 040 | Canada | CA / CAN | CAD | $2319.90B | $55,698 | 2.07 | 63.51 | High income | Bank of Canada | YES |
| 041 | Cayman Islands | KY / CYM | KYD | $7.77B | $104,293 | -0.63 | 105.47 | High income | Cayman Islands Monetary Authority | YES |
| 042 | Central African Republic | CF / CAF | XAF | $3.07B | 556.13 | 1.00 | 52.91 | Low income | Bank of Central African States (BEAC) | YES |
| 043 | Chad | TD / TCD | XAF | $21.47B | $1,022 | -3.91 | 46.13 | Low income | Bank of Central African States (BEAC) | YES |
| 044 | Chile | CL / CHL | CLP | $357.37B | $17,995 | 4.21 | 64.98 | High income | Central Bank of Chile | YES |
| 045 | China | CN / CHN | CNY | $19498.04B | $13,862 | 0.06 | 37.96 | Upper middle income | People's Bank of China | INDEED |
| 046 | Christmas Island | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Christmas Island (BIS Central Bank Hub reference) | YES |
| 047 | Cocos (Keeling) Islands | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Cocos (Keeling) Islands (BIS Central Bank Hub reference) | YES |
| 048 | Colombia | CO / COL | COP | $457.41B | $8,562 | 5.14 | 35.37 | Upper middle income | Banco de la República | YES |
| 049 | Comoros | KM / COM | KMF | $1.81B | $2,056 | 3.25 | 40.89 | Lower middle income | Central Bank of the Comoros | YES |
| 050 | Congo | N/A / N/A | XAF | N/A | N/A | N/A | N/A | N/A | Bank of Central African States (BEAC) | YES |
| 051 | Cook Islands | N/A / N/A | NZD | N/A | N/A | N/A | N/A | N/A | Cook Islands Financial Supervisory Commission / Reserve Bank of New Zealand currency system | YES |
| 052 | Costa Rica | CR / CRI | CRC | $102.90B | $19,970 | -0.07 | 71.49 | High income | Central Bank of Costa Rica | YES |
| 053 | Côte d'Ivoire | N/A / N/A | XOF | N/A | N/A | N/A | N/A | N/A | Central Bank of West African States (BCEAO) | YES |
| 054 | Croatia | HR / HRV | EUR | $105.06B | $27,104 | 3.69 | 100.99 | High income | Croatian National Bank / European Central Bank | CONVINCED |
| 055 | Cuba | CU / CUB | CUP | $107.35B | $9,605 | N/A | 124.86 | Upper middle income | Central Bank of Cuba | INDEED |
| 056 | Curaçao | N/A / N/A | XCG | N/A | N/A | N/A | N/A | N/A | Central Bank of Curaçao and Sint Maarten | YES |
| 057 | Cyprus | CY / CYP | EUR | $41.23B | $41,783 | 0.13 | 189.50 | High income | Central Bank of Cyprus / European Central Bank | YES |
| 058 | Czechia | CZ / CZE | CZK | $391.03B | $35,917 | 2.46 | 128.04 | High income | Czech National Bank | CONVINCED |
| 059 | Democratic Republic of the Congo | N/A / N/A | CDF | N/A | N/A | N/A | N/A | N/A | Central Bank of the Congo | YES |
| 060 | Denmark | DK / DNK | DKK | $462.53B | $76,970 | 1.89 | 129.22 | High income | Danmarks Nationalbank | INDEED |
| 061 | Djibouti | DJ / DJI | DJF | $4.62B | $3,906 | -0.31 | 206.30 | Lower middle income | Central Bank of Djibouti | YES |
| 062 | Dominica | DM / DMA | XCD | $723.86M | $10,989 | 2.53 | N/A | Upper middle income | Eastern Caribbean Central Bank (ECCB) | YES |
| 063 | Dominican Republic | DO / DOM | DOP | $127.41B | $11,059 | 3.87 | 51.49 | Upper middle income | Central Bank of the Dominican Republic | YES |
| 064 | Ecuador | EC / ECU | USD | $130.32B | $7,125 | 0.71 | 59.36 | Upper middle income | Central Bank of Ecuador | YES |
| 065 | Egypt | N/A / N/A | EGP | N/A | N/A | N/A | N/A | N/A | Central Bank of Egypt | YES |
| 066 | El Salvador | SV / SLV | USD | $36.71B | $5,767 | 0.26 | 88.16 | Upper middle income | Central Reserve Bank of El Salvador | YES |
| 067 | Equatorial Guinea | GQ / GNQ | XAF | $12.82B | $6,615 | 2.92 | 74.93 | Upper middle income | Bank of Central African States (BEAC) | YES |
| 068 | Eritrea | ER / ERI | ERN | $2.07B | 688.68 | N/A | 47.39 | Low income | Bank of Eritrea | YES |
| 069 | Estonia | EE / EST | EUR | $47.03B | $34,418 | 4.83 | 155.76 | High income | Eesti Pank / European Central Bank | CONVINCED |
| 070 | Eswatini | SZ / SWZ | SZL | $5.16B | $4,108 | 2.60 | 107.48 | Lower middle income | Central Bank of Eswatini | YES |
| 071 | Ethiopia | ET / ETH | ETB | $126.36B | 932.73 | 13.23 | 33.46 | Low income | National Bank of Ethiopia | YES |
| 072 | Falkland Islands | N/A / N/A | FKP | N/A | N/A | N/A | N/A | N/A | Government of the Falkland Islands / currency linked to pound sterling | YES |
| 073 | Faroe Islands | FO / FRO | DKK | $4.05B | $74,175 | N/A | 105.75 | High income | Danmarks Nationalbank / local banking authority | YES |
| 074 | Fiji | FJ / FJI | FJD | $6.20B | $6,642 | -1.38 | 118.96 | Upper middle income | Reserve Bank of Fiji | YES |
| 075 | Finland | FI / FIN | EUR | $317.04B | $56,149 | 0.34 | 83.40 | High income | Bank of Finland / European Central Bank | INDEED |
| 076 | France | FR / FRA | EUR | $3366.32B | $48,986 | 0.94 | 67.19 | High income | Banque de France / European Central Bank | YES |
| 077 | French Guiana | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | YES |
| 078 | French Polynesia | PF / PYF | XPF | $6.32B | $22,440 | N/A | 64.47 | High income | Institut d'émission d'outre-mer (IEOM) | YES |
| 079 | French Southern Territories | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Institut d'émission d'outre-mer (IEOM) | YES |
| 080 | Gabon | GA / GAB | XAF | $21.43B | $8,263 | 1.77 | 84.89 | Upper middle income | Bank of Central African States (BEAC) | YES |
| 081 | Gambia | N/A / N/A | GMD | N/A | N/A | N/A | N/A | N/A | Central Bank of The Gambia | YES |
| 082 | Georgia | GE / GEO | GEL | $38.14B | $9,692 | 3.87 | 100.63 | Upper middle income | National Bank of Georgia | CONVINCED |
| 083 | Germany | DE / DEU | EUR | $5050.92B | $60,496 | 2.17 | 78.51 | High income | Deutsche Bundesbank / European Central Bank | CONVINCED |
| 084 | Ghana | GH / GHA | GHS | $114.21B | $3,257 | 14.20 | 68.97 | Lower middle income | Bank of Ghana | YES |
| 085 | Gibraltar | GI / GIB | GIP | N/A | N/A | N/A | N/A | High income | Gibraltar Financial Services Commission / Bank of England sterling system | YES |
| 086 | Greece | GR / GRC | EUR | $280.64B | $26,948 | 2.48 | 83.45 | High income | Bank of Greece / European Central Bank | YES |
| 087 | Greenland | GL / GRL | DKK | $3.33B | $58,499 | N/A | 91.89 | High income | Danmarks Nationalbank / local banking authority | YES |
| 088 | Grenada | GD / GRD | XCD | $1.42B | $12,107 | 0.61 | N/A | Upper middle income | Eastern Caribbean Central Bank (ECCB) | YES |
| 089 | Guadeloupe | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | YES |
| 090 | Guam | GU / GUM | USD | $6.91B | $41,833 | N/A | 71.87 | High income | Federal Reserve System / Guam banking institutions | YES |
| 091 | Guatemala | GT / GTM | GTQ | $123.31B | $6,598 | 1.49 | 46.53 | Upper middle income | Bank of Guatemala | YES |
| 092 | Guernsey | N/A / N/A | GBP | N/A | N/A | N/A | N/A | N/A | Guernsey Financial Services Commission / sterling banking system | YES |
| 093 | Guinea | GN / GIN | GNF | $28.35B | $1,877 | 3.55 | 103.68 | Lower middle income | Central Bank of the Republic of Guinea | YES |
| 094 | Guinea-Bissau | GW / GNB | XOF | $2.53B | $1,124 | 0.87 | 40.88 | Low income | Central Bank of West African States (BCEAO) | YES |
| 095 | Guyana | GY / GUY | GYD | $27.10B | $32,414 | 3.33 | 194.35 | High income | Bank of Guyana | YES |
| 096 | Haiti | HT / HTI | HTG | $32.08B | $2,694 | 28.64 | 15.89 | Lower middle income | Bank of the Republic of Haiti | YES |
| 097 | Heard Island and McDonald Islands | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Heard Island and McDonald Islands (BIS Central Bank Hub reference) | YES |
| 098 | Holy See | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Institute for the Works of Religion / Vatican Financial Information Authority; euro system | YES |
| 099 | Honduras | HN / HND | HNL | $39.60B | $3,598 | 4.60 | 91.51 | Lower middle income | Central Bank of Honduras | YES |
| 100 | Hong Kong | N/A / N/A | HKD | N/A | N/A | N/A | N/A | N/A | Hong Kong Monetary Authority | YES |
| 101 | Hungary | HU / HUN | HUF | $246.49B | $25,907 | 4.41 | 140.78 | High income | Magyar Nemzeti Bank | CONVINCED |
| 102 | Iceland | IS / ISL | ISK | $38.58B | $98,323 | 4.09 | 82.49 | High income | Central Bank of Iceland | INDEED |
| 103 | India | IN / IND | INR | $3956.07B | $2,702 | 2.40 | 46.27 | Lower middle income | Reserve Bank of India | INDEED |
| 104 | Indonesia | ID / IDN | IDR | $1445.64B | $5,060 | 1.91 | 43.39 | Upper middle income | Bank Indonesia | YES |
| 105 | Iran | N/A / N/A | IRR | N/A | N/A | N/A | N/A | N/A | Central Bank of the Islamic Republic of Iran | YES |
| 106 | Iraq | IQ / IRQ | IQD | $254.37B | $5,410 | 0.30 | 77.14 | Upper middle income | Central Bank of Iraq | YES |
| 107 | Ireland | IE / IRL | EUR | $721.70B | $131,592 | 2.21 | 239.96 | High income | Central Bank of Ireland / European Central Bank | YES |
| 108 | Isle of Man | IM / IMN | GBP | $7.58B | $90,015 | N/A | N/A | High income | Isle of Man Financial Services Authority / sterling banking system | YES |
| 109 | Israel | IL / ISR | ILS | $610.78B | $60,337 | 3.04 | 52.99 | High income | Bank of Israel | YES |
| 110 | Italy | IT / ITA | EUR | $2551.56B | $43,309 | 1.53 | 62.52 | High income | Banca d'Italia / European Central Bank | YES |
| 111 | Jamaica | JM / JAM | JMD | $22.70B | $8,003 | 4.00 | N/A | Upper middle income | Bank of Jamaica | YES |
| 112 | Japan | JP / JPN | JPY | $4435.16B | $35,951 | 3.17 | 44.85 | High income | Bank of Japan | YES |
| 113 | Jersey | N/A / N/A | GBP | N/A | N/A | N/A | N/A | N/A | Jersey Financial Services Commission / sterling banking system | YES |
| 114 | Jordan | JO / JOR | JOD | $61.61B | $5,348 | 1.77 | 145.99 | Upper middle income | Central Bank of Jordan | YES |
| 115 | Kazakhstan | KZ / KAZ | KZT | $306.24B | $14,692 | 11.39 | 57.16 | Upper middle income | National Bank of Kazakhstan | CONVINCED |
| 116 | Kenya | KE / KEN | KES | $135.94B | $2,363 | 4.07 | 37.52 | Lower middle income | Central Bank of Kenya | YES |
| 117 | Kiribati | KI / KIR | AUD | $349.23M | $2,559 | 2.46 | 102.11 | Lower middle income | Banking system under Australian dollar; Reserve Bank of Australia framework | YES |
| 118 | North Korea | N/A / N/A | KPW | N/A | N/A | N/A | N/A | N/A | Central Bank of the Democratic People's Republic of Korea | INDEED |
| 119 | South Korea | N/A / N/A | KRW | N/A | N/A | N/A | N/A | N/A | Bank of Korea | YES |
| 120 | Kuwait | KW / KWT | KWD | $157.21B | $32,312 | 2.36 | 93.99 | High income | Central Bank of Kuwait | YES |
| 121 | Kyrgyzstan | N/A / N/A | KGS | N/A | N/A | N/A | N/A | N/A | National Bank of the Kyrgyz Republic | CONVINCED |
| 122 | Lao People's Democratic Republic | N/A / N/A | LAK | N/A | N/A | N/A | N/A | N/A | Bank of the Lao P.D.R. | YES |
| 123 | Latvia | LV / LVA | EUR | $48.62B | $26,312 | 3.75 | 129.72 | High income | Bank of Latvia / European Central Bank | CONVINCED |
| 124 | Lebanon | LB / LBN | LBP | $25.97B | $4,473 | 14.60 | 82.23 | Lower middle income | Banque du Liban | YES |
| 125 | Lesotho | LS / LSO | LSL / ZAR | $2.57B | $1,089 | 4.27 | 153.98 | Lower middle income | Central Bank of Lesotho | YES |
| 126 | Liberia | LR / LBR | LRD | $5.25B | 915.33 | 8.32 | N/A | Low income | Central Bank of Liberia | YES |
| 127 | Libya | LY / LBY | LYD | $48.10B | $6,449 | 1.84 | 137.66 | Upper middle income | Central Bank of Libya | YES |
| 128 | Liechtenstein | LI / LIE | CHF | $8.91B | $220,167 | N/A | N/A | High income | Financial Market Authority Liechtenstein / Swiss franc banking system | YES |
| 129 | Lithuania | LT / LTU | EUR | $95.21B | $32,959 | 3.79 | 142.11 | High income | Bank of Lithuania / European Central Bank | CONVINCED |
| 130 | Luxembourg | LU / LUX | EUR | $101.16B | $147,252 | 2.26 | 349.89 | High income | Central Bank of Luxembourg / European Central Bank | YES |
| 131 | Macao | N/A / N/A | MOP | N/A | N/A | N/A | N/A | N/A | Monetary Authority of Macao | YES |
| 132 | Madagascar | MG / MDG | MGA | $19.62B | 599.27 | 8.05 | 54.24 | Low income | Central Bank of Madagascar | YES |
| 133 | Malawi | MW / MWI | MWK | $14.92B | 671.51 | 28.37 | 39.40 | Low income | Reserve Bank of Malawi | YES |
| 134 | Malaysia | MY / MYS | MYR | $472.19B | $13,125 | 1.38 | 136.40 | Upper middle income | Bank Negara Malaysia | YES |
| 135 | Maldives | MV / MDV | MVR | $7.74B | $14,615 | 4.01 | 156.47 | Upper middle income | Maldives Monetary Authority | YES |
| 136 | Mali | ML / MLI | XOF | $30.07B | $1,193 | 3.28 | 48.77 | Low income | Central Bank of West African States (BCEAO) | YES |
| 137 | Malta | MT / MLT | EUR | $27.77B | $47,907 | 2.36 | 219.01 | High income | Central Bank of Malta / European Central Bank | YES |
| 138 | Marshall Islands | MH / MHL | USD | $308.00M | $8,489 | N/A | 115.91 | Upper middle income | U.S. Federal Reserve System / Marshall Islands banking institutions | YES |
| 139 | Martinique | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Martinique (BIS Central Bank Hub reference) | YES |
| 140 | Mauritania | MR / MRT | MRU | $11.68B | $2,198 | 1.55 | 94.02 | Lower middle income | Central Bank of Mauritania | YES |
| 141 | Mauritius | MU / MUS | MUR | $16.16B | $12,991 | 3.67 | 139.94 | Upper middle income | Bank of Mauritius | YES |
| 142 | Mayotte | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Mayotte (BIS Central Bank Hub reference) | YES |
| 143 | Mexico | MX / MEX | MXN | $1832.64B | $13,889 | 3.81 | 79.92 | Upper middle income | Bank of Mexico | YES |
| 144 | Micronesia | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | U.S. Federal Reserve System / Federated States of Micronesia banking institutions | YES |
| 145 | Moldova | MD / MDA | MDL | $20.35B | $8,622 | 7.76 | 92.67 | Upper middle income | National Bank of Moldova | CONVINCED |
| 146 | Monaco | MC / MCO | EUR | $11.13B | $288,002 | N/A | N/A | High income | European Central Bank / French monetary and banking system | YES |
| 147 | Mongolia | MN / MNG | MNT | $25.37B | $7,108 | 8.61 | 130.71 | Upper middle income | Bank of Mongolia | CONVINCED |
| 148 | Montenegro | ME / MNE | EUR | $9.23B | $14,817 | 3.90 | 105.77 | Upper middle income | Central Bank of Montenegro / euro system | CONVINCED |
| 149 | Montserrat | N/A / N/A | XCD | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | YES |
| 150 | Morocco | MA / MAR | MAD | $182.37B | $4,672 | 0.70 | 93.10 | Lower middle income | Bank Al-Maghrib | YES |
| 151 | Mozambique | MZ / MOZ | MZN | $22.34B | 626.91 | 4.37 | 97.10 | Low income | Bank of Mozambique | YES |
| 152 | Myanmar | MM / MMR | MMK | $81.67B | $1,489 | 8.83 | N/A | Lower middle income | Central Bank of Myanmar | YES |
| 153 | Namibia | NA / NAM | NAD / ZAR | $15.08B | $4,876 | 3.51 | 108.58 | Lower middle income | Bank of Namibia | YES |
| 154 | Nauru | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | Australian dollar banking system / Reserve Bank of Australia framework | YES |
| 155 | Nepal | NP / NPL | NPR | $45.49B | $1,536 | 2.65 | 42.55 | Lower middle income | Nepal Rastra Bank | INDEED |
| 156 | Netherlands | NL / NLD | EUR | $1332.77B | $73,684 | 3.26 | 150.12 | High income | De Nederlandsche Bank / European Central Bank | YES |
| 157 | New Caledonia | NC / NCL | XPF | $8.55B | $29,213 | 0.58 | 40.37 | High income | Institut d'émission d'outre-mer (IEOM) | YES |
| 158 | New Zealand | NZ / NZL | NZD | $264.06B | $49,591 | 2.84 | 50.66 | High income | Reserve Bank of New Zealand | YES |
| 159 | Nicaragua | NI / NIC | NIO | $22.24B | $3,173 | 2.08 | 96.45 | Lower middle income | Central Bank of Nicaragua | YES |
| 160 | Niger | NE / NER | XOF | $21.65B | 775.35 | -4.45 | 33.91 | Low income | Central Bank of West African States (BCEAO) | YES |
| 161 | Nigeria | NG / NGA | NGN | $290.79B | $1,224 | 23.01 | 26.17 | Lower middle income | Central Bank of Nigeria | YES |
| 162 | Niue | N/A / N/A | NZD | N/A | N/A | N/A | N/A | N/A | Reserve Bank of New Zealand / Niue government banking system | YES |
| 163 | Norfolk Island | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | Reserve Bank of Australia / Australian banking system | YES |
| 164 | North Macedonia | MK / MKD | MKD | $19.10B | $10,490 | 4.07 | 133.04 | Upper middle income | National Bank of the Republic of North Macedonia | CONVINCED |
| 165 | Northern Mariana Islands | MP / MNP | USD | $1.10B | $23,786 | N/A | 93.16 | High income | Federal Reserve System / U.S. banking system | YES |
| 166 | Norway | NO / NOR | NOK | $530.76B | $94,594 | 3.06 | 79.05 | High income | Norges Bank | INDEED |
| 167 | Oman | OM / OMN | OMR | $109.60B | $19,947 | 0.97 | 114.85 | High income | Central Bank of Oman | YES |
| 168 | Pakistan | PK / PAK | PKR | $407.31B | $1,596 | 3.55 | 27.19 | Lower middle income | State Bank of Pakistan | YES |
| 169 | Palau | PW / PLW | USD | $345.00M | $19,532 | -0.42 | 105.81 | High income | U.S. Federal Reserve System / Palau banking institutions | YES |
| 170 | Palestine | N/A / N/A | ILS / JOD / USD | N/A | N/A | N/A | N/A | N/A | Palestine Monetary Authority | YES |
| 171 | Panama | PA / PAN | PAB / USD | $90.46B | $19,790 | -0.19 | 83.67 | High income | Superintendency of Banks of Panama / Banco Nacional de Panamá / U.S. dollar system | YES |
| 172 | Papua New Guinea | PG / PNG | PGK | $32.50B | $3,020 | 4.42 | 131.08 | Lower middle income | Bank of Papua New Guinea | YES |
| 173 | Paraguay | PY / PRY | PYG | $49.28B | $7,027 | 4.04 | 76.48 | Upper middle income | Central Bank of Paraguay | YES |
| 174 | Peru | PE / PER | PEN | $334.85B | $9,684 | 1.53 | 51.82 | Upper middle income | Central Reserve Bank of Peru | YES |
| 175 | Philippines | PH / PHL | PHP | $487.09B | $4,171 | 1.66 | 66.79 | Upper middle income | Bangko Sentral ng Pilipinas | YES |
| 176 | Pitcairn | N/A / N/A | NZD | N/A | N/A | N/A | N/A | N/A | Reserve Bank of New Zealand / New Zealand dollar system | YES |
| 177 | Poland | PL / POL | PLN | $1035.49B | $28,420 | 3.81 | 97.03 | High income | Narodowy Bank Polski | CONVINCED |
| 178 | Portugal | PT / PRT | EUR | $346.64B | $32,082 | 2.34 | 86.40 | High income | Banco de Portugal / European Central Bank | YES |
| 179 | Puerto Rico | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | Federal Reserve System / U.S. banking system | YES |
| 180 | Qatar | QA / QAT | QAR | $215.56B | $72,525 | 1.27 | 100.21 | High income | Qatar Central Bank | YES |
| 181 | Réunion | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | YES |
| 182 | Romania | RO / ROU | RON | $428.68B | $22,538 | 7.19 | 76.21 | High income | National Bank of Romania | CONVINCED |
| 183 | Russian Federation | RU / RUS | RUB | $2561.31B | $17,547 | 8.72 | 33.71 | High income | Central Bank of the Russian Federation | CONVINCED |
| 184 | Rwanda | RW / RWA | RWF | $16.37B | $1,124 | 5.91 | 56.67 | Low income | National Bank of Rwanda | YES |
| 185 | Saint Barthélemy | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | YES |
| 186 | Saint Helena, Ascension and Tristan da Cunha | N/A / N/A | SHP | N/A | N/A | N/A | N/A | N/A | Bank of St Helena / sterling banking system | YES |
| 187 | Saint Kitts and Nevis | N/A / N/A | XCD | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | YES |
| 188 | Saint Lucia | N/A / N/A | XCD | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | YES |
| 189 | Saint Martin | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | YES |
| 190 | Saint Pierre and Miquelon | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | YES |
| 191 | Saint Vincent and the Grenadines | N/A / N/A | XCD | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | YES |
| 192 | Samoa | WS / WSM | WST | $1.29B | $5,873 | 2.21 | 70.63 | Upper middle income | Central Bank of Samoa | YES |
| 193 | San Marino | SM / SMR | EUR | $2.03B | $59,871 | 2.30 | 340.98 | High income | Central Bank of the Republic of San Marino / euro system | YES |
| 194 | Sao Tome and Principe | ST / STP | STN | $981.29M | $4,084 | 11.05 | 46.57 | Lower middle income | Central Bank of São Tomé and Príncipe | YES |
| 195 | Saudi Arabia | SA / SAU | SAR | $1276.94B | $34,537 | 2.08 | 57.92 | High income | Saudi Central Bank (SAMA) | YES |
| 196 | Senegal | SN / SEN | XOF | $37.01B | $1,955 | 1.46 | 71.07 | Lower middle income | Central Bank of West African States (BCEAO) | YES |
| 197 | Serbia | RS / SRB | RSD | $99.95B | $15,262 | 3.89 | 112.92 | Upper middle income | National Bank of Serbia | CONVINCED |
| 198 | Seychelles | SC / SYC | SCR | $2.39B | $19,449 | 0.30 | 172.29 | High income | Central Bank of Seychelles | YES |
| 199 | Sierra Leone | SL / SLE | SLE | $7.46B | 846.30 | 7.49 | 49.10 | Low income | Bank of Sierra Leone | YES |
| 200 | Singapore | SG / SGP | SGD | $603.87B | $98,814 | 0.90 | 320.31 | High income | Monetary Authority of Singapore | YES |
| 201 | Sint Maarten | N/A / N/A | XCG | N/A | N/A | N/A | N/A | N/A | Central Bank of Curaçao and Sint Maarten | YES |
| 202 | Slovakia | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | National Bank of Slovakia / European Central Bank | CONVINCED |
| 203 | Slovenia | SI / SVN | EUR | $79.65B | $37,376 | 2.37 | 151.92 | High income | Bank of Slovenia / European Central Bank | CONVINCED |
| 204 | Solomon Islands | SB / SLB | SBD | $1.75B | $2,086 | 3.36 | 111.35 | Lower middle income | Central Bank of Solomon Islands | YES |
| 205 | Somalia | N/A / N/A | SOS | N/A | N/A | N/A | N/A | N/A | Central Bank of Somalia | YES |
| 206 | South Africa | ZA / ZAF | ZAR | $427.18B | $6,598 | 3.21 | 60.91 | Upper middle income | South African Reserve Bank | YES |
| 207 | South Georgia and the South Sandwich Islands | N/A / N/A | GBP | N/A | N/A | N/A | N/A | N/A | Bank of England / sterling banking system | YES |
| 208 | South Sudan | SS / SSD | SSP | $12.00B | $1,080 | 91.44 | 65.55 | Low income | Bank of South Sudan | YES |
| 209 | Spain | ES / ESP | EUR | $1906.45B | $38,627 | 2.70 | 69.45 | High income | Banco de España / European Central Bank | YES |
| 210 | Sri Lanka | LK / LKA | LKR | $108.83B | $5,002 | -4.76 | 41.81 | Upper middle income | Central Bank of Sri Lanka | INDEED |
| 211 | Sudan | SD / SDN | SDG | $60.16B | $1,165 | 138.81 | 2.38 | Low income | Central Bank of Sudan | YES |
| 212 | Suriname | SR / SUR | SRD | $4.52B | $7,070 | 9.21 | 90.96 | Upper middle income | Central Bank of Suriname | YES |
| 213 | Svalbard and Jan Mayen | N/A / N/A | NOK | N/A | N/A | N/A | N/A | N/A | Norges Bank / Norwegian banking system | YES |
| 214 | Sweden | SE / SWE | SEK | $669.00B | $63,133 | 0.68 | 104.34 | High income | Sveriges Riksbank | INDEED |
| 215 | Switzerland | CH / CHE | CHF | $1043.53B | $114,769 | 0.15 | 147.16 | High income | Swiss National Bank | YES |
| 216 | Syrian Arab Republic | SY / SYR | SYP | $23.74B | $1,057 | 13.42 | 35.44 | Low income | Central Bank of Syria | YES |
| 217 | Taiwan | N/A / N/A | TWD | N/A | N/A | N/A | N/A | N/A | Central Bank of the Republic of China (Taiwan) | YES |
| 218 | Tajikistan | TJ / TJK | TJS | $17.66B | $1,637 | 6.00 | 59.09 | Lower middle income | National Bank of Tajikistan | CONVINCED |
| 219 | Tanzania | TZ / TZA | TZS | $90.14B | $1,319 | 3.33 | 38.97 | Lower middle income | Bank of Tanzania | CONVINCED |
| 220 | Thailand | TH / THA | THB | $577.01B | $8,057 | -0.13 | 138.41 | Upper middle income | Bank of Thailand | YES |
| 221 | Timor-Leste | TL / TLS | USD | $1.90B | $1,341 | 0.43 | 95.45 | Lower middle income | Banco Central de Timor-Leste | YES |
| 222 | Togo | TG / TGO | XOF | $11.89B | $1,384 | 0.43 | 59.93 | Lower middle income | Central Bank of West African States (BCEAO) | YES |
| 223 | Tokelau | N/A / N/A | NZD | N/A | N/A | N/A | N/A | N/A | Reserve Bank of New Zealand / New Zealand dollar system | YES |
| 224 | Tonga | TO / TON | TOP | $679.22M | $6,547 | 5.59 | 77.58 | Upper middle income | National Reserve Bank of Tonga | YES |
| 225 | Trinidad and Tobago | TT / TTO | TTD | $25.94B | $18,967 | 0.99 | N/A | High income | Central Bank of Trinidad and Tobago | YES |
| 226 | Tunisia | TN / TUN | TND | $57.50B | $4,657 | 5.15 | 101.45 | Lower middle income | Central Bank of Tunisia | YES |
| 227 | Türkiye | N/A / N/A | TRY | N/A | N/A | N/A | N/A | N/A | Central Bank of the Republic of Türkiye | YES |
| 228 | Turkmenistan | TM / TKM | TMT | $49.83B | $6,540 | N/A | 28.17 | Upper middle income | Central Bank of Turkmenistan | CONVINCED |
| 229 | Turks and Caicos Islands | TC / TCA | USD | $1.75B | $37,507 | N/A | N/A | High income | Turks and Caicos Islands Financial Services Commission / U.S. dollar system | YES |
| 230 | Tuvalu | TV / TUV | AUD | $57.35M | $6,041 | 0.50 | N/A | Upper middle income | Australian dollar banking system / Reserve Bank of Australia framework | YES |
| 231 | Uganda | UG / UGA | UGX | $61.99B | $1,206 | 3.58 | 50.20 | Low income | Bank of Uganda | YES |
| 232 | Ukraine | UA / UKR | UAH | $214.23B | $5,866 | 12.73 | 77.18 | Upper middle income | National Bank of Ukraine | CONVINCED |
| 233 | United Arab Emirates | AE / ARE | AED | $552.32B | $50,274 | 1.25 | 199.04 | High income | Central Bank of the United Arab Emirates | YES |
| 234 | United Kingdom | GB / GBR | GBP | $4002.59B | $57,602 | 3.88 | 62.46 | High income | Bank of England | YES |
| 235 | United States | US / USA | USD | $30769.70B | $90,027 | 2.95 | 25.02 | High income | Board of Governors of the Federal Reserve System | YES |
| 236 | United States Minor Outlying Islands | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | Federal Reserve System / U.S. banking system | YES |
| 237 | Uruguay | UY / URY | UYU | $85.35B | $25,216 | 4.65 | 49.72 | High income | Central Bank of Uruguay | YES |
| 238 | Uzbekistan | UZ / UZB | UZS | $147.04B | $3,968 | 8.80 | 57.40 | Lower middle income | Central Bank of the Republic of Uzbekistan | CONVINCED |
| 239 | Vanuatu | VU / VUT | VUV | $1.35B | $4,039 | 0.66 | 73.38 | Lower middle income | Reserve Bank of Vanuatu | YES |
| 240 | Venezuela | N/A / N/A | VES | N/A | N/A | N/A | N/A | N/A | Central Bank of Venezuela | INDEED |
| 241 | Vietnam | VN / VNM | VND | $514.70B | $5,066 | 3.31 | 190.34 | Upper middle income | State Bank of Viet Nam | INDEED |
| 242 | Virgin Islands, British | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | British Virgin Islands Financial Services Commission / U.S. dollar system | YES |
| 243 | Virgin Islands, U.S. | VI / VIR | USD | $4.67B | $44,321 | N/A | 205.63 | High income | Federal Reserve System / U.S. banking system | YES |
| 244 | Wallis and Futuna | N/A / N/A | XPF | N/A | N/A | N/A | N/A | N/A | Institut d'émission d'outre-mer (IEOM) | YES |
| 245 | Western Sahara | N/A / N/A | MAD | N/A | N/A | N/A | N/A | N/A | Bank Al-Maghrib / Moroccan banking system | YES |
| 246 | Yemen | N/A / N/A | YER | N/A | N/A | N/A | N/A | N/A | Central Bank of Yemen | YES |
| 247 | Zambia | ZM / ZMB | ZMW | $28.88B | $1,318 | 13.91 | 62.53 | Lower middle income | Bank of Zambia | YES |
| 248 | Zimbabwe | ZW / ZWE | ZWG | $51.22B | $3,021 | 104.71 | 41.50 | Lower middle income | Reserve Bank of Zimbabwe | YES |
| 249 | Åland Islands | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Bank of Finland / European Central Bank | YES |
| 250 | Roman Republic | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 251 | Roman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 252 | Byzantine Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 253 | Holy Roman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 254 | Western Roman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 255 | Eastern Roman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 256 | Ancient Egypt | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 257 | Kingdom of Kush | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 258 | Carthaginian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 259 | Kingdom of Aksum | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 260 | Mali Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 261 | Songhai Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 262 | Ghana Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 263 | Kanem-Bornu Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 264 | Benin Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 265 | Oyo Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 266 | Ashanti Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 267 | Ethiopian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 268 | Kingdom of Kongo | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 269 | Mutapa Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 270 | Maravi Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 271 | Zulu Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 272 | Sokoto Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 273 | Almohad Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 274 | Almoravid Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 275 | Fatimid Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 276 | Mamluk Sultanate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 277 | Abbasid Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 278 | Umayyad Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 279 | Ottoman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 280 | Safavid Iran | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 281 | Qajar Iran | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 282 | Mughal Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 283 | Maurya Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 284 | Gupta Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 285 | Chola Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 286 | Pala Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 287 | Delhi Sultanate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 288 | Maratha Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 289 | Vijayanagara Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 290 | Kushan Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 291 | Parthian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 292 | Sasanian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 293 | Achaemenid Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 294 | Median Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 295 | Hittite Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 296 | Phoenicia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 297 | Kingdom of Lydia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 298 | Seleucid Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 299 | Ptolemaic Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 300 | Ancient Greece | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 301 | Macedonian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 302 | Spartan State | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 303 | Athens | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 304 | Carthage | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 305 | Han Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 306 | Qin Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 307 | Tang Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 308 | Song Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 309 | Yuan Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 310 | Ming Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 311 | Qing Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 312 | Three Kingdoms China | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 313 | Korea under Joseon | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 314 | Goryeo | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 315 | Silla | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 316 | Baekje | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 317 | Goguryeo | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 318 | Ryukyu Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 319 | Khmer Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 320 | Pagan Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 321 | Ayutthaya Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 322 | Sukhothai Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 323 | Majapahit | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 324 | Srivijaya | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 325 | Malacca Sultanate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 326 | Brunei Sultanate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 327 | Lan Xang | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 328 | Dai Viet | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 329 | Champa | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 330 | Nguyen Vietnam | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 331 | Mataram Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 332 | Hawaiian Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 333 | Kingdom of Tahiti | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 334 | Tu'i Tonga Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 335 | Samoan Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 336 | Maori Confederations | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 337 | Inca Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 338 | Aztec Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 339 | Maya City-States | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 340 | Mississippian Cultures | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 341 | Tarascan State | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 342 | Gran Colombia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 343 | Viceroyalty of New Spain | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 344 | Viceroyalty of Peru | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 345 | Viceroyalty of New Granada | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 346 | Viceroyalty of the Río de la Plata | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 347 | Captaincy General of Guatemala | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 348 | Kingdom of Brazil | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 349 | United Provinces of Central America | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 350 | Federal Republic of Central America | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 351 | United States of Colombia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 352 | New Granada | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 353 | Kingdom of Prussia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 354 | German Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 355 | Austro-Hungarian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 356 | Austrian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 357 | Kingdom of Hungary | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 358 | Kingdom of Bohemia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 359 | Polish-Lithuanian Commonwealth | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 360 | Kingdom of Poland | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 361 | Grand Duchy of Lithuania | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 362 | Russian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 363 | Soviet Union | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | CONVINCED |
| 364 | Yugoslavia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | CONVINCED |
| 365 | Czechoslovakia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | CONVINCED |
| 366 | East Germany | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | CONVINCED |
| 367 | West Germany | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 368 | Kingdom of Italy | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 369 | Kingdom of Sardinia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 370 | Papal States | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 371 | Kingdom of the Two Sicilies | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 372 | French Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 373 | Kingdom of France | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 374 | Spanish Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 375 | Kingdom of Spain | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 376 | Portuguese Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 377 | Dutch Republic | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 378 | British Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 379 | Irish Free State | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 380 | United Arab Republic | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 381 | United Arab Federation | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 382 | North Yemen | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 383 | South Yemen | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | CONVINCED |
| 384 | Democratic Kampuchea | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | CONVINCED |
| 385 | South Vietnam | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | CONVINCED |
| 386 | North Vietnam | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | CONVINCED |
| 387 | Republic of Vietnam | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 388 | Kingdom of Afghanistan | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 389 | Tibetan Government | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 390 | Manchu-led Qing successor states | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
| 391 | Persian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | NO |
