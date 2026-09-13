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

| ID | Country/Jurisdiction | ISO Alpha-2 | ISO Alpha-3 | ISO Numeric Code | Currency Code | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Status |
| --- | --- | --- | --- | --- | --- | ---: | ---: | ---: | ---: | --- | --- | --- |
| 001 | Afghanistan | AF | AFG | 004 | AFN | $17.78B | 416.87 | -6.60 | 83.76 | Low income | PENDING | PARTIAL |
| 002 | Albania | AL | ALB | 008 | ALL | $30.54B | $12,998 | 2.15 | 79.53 | Upper middle income | PENDING | PARTIAL |
| 003 | Algeria | DZ | DZA | 012 | DZD | $287.03B | $6,051 | 1.42 | 38.98 | Upper middle income | PENDING | PARTIAL |
| 004 | American Samoa | AS | ASM | 016 | USD | $871.00M | $18,017 | N/A | 124.68 | High income | PENDING | PARTIAL |
| 005 | Andorra | AD | AND | 020 | EUR | $4.50B | $54,292 | N/A | N/A | High income | PENDING | PARTIAL |
| 006 | Angola | AO | AGO | 024 | AOA | $122.17B | $3,129 | 20.16 | 34.65 | Lower middle income | PENDING | PARTIAL |
| 007 | Anguilla | AI | AIA | 660 | XCD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 008 | Antarctica | AQ | ATA | 010 | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 009 | Antigua and Barbuda | AG | ATG | 028 | XCD | $2.34B | $24,819 | 1.37 | 118.30 | High income | PENDING | PARTIAL |
| 010 | Argentina | AR | ARG | 032 | ARS | $683.10B | $14,898 | 219.88 | 30.36 | Upper middle income | PENDING | PARTIAL |
| 011 | Armenia | AM | ARM | 051 | AMD | $29.24B | $9,474 | 3.31 | 101.71 | Upper middle income | PENDING | PARTIAL |
| 012 | Aruba | AW | ABW | 533 | AWG | $4.17B | $38,591 | 4.26 | 164.14 | High income | PENDING | PARTIAL |
| 013 | Australia | AU | AUS | 036 | AUD | $1798.52B | $65,130 | 2.87 | 45.90 | High income | PENDING | PARTIAL |
| 014 | Austria | AT | AUT | 040 | EUR | $579.47B | $62,930 | 3.53 | 107.02 | High income | PENDING | PARTIAL |
| 015 | Azerbaijan | AZ | AZE | 031 | AZN | $75.94B | $7,411 | 5.62 | 79.83 | Upper middle income | PENDING | PARTIAL |
| 016 | Bahamas | BS | BHS | 044 | BSD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 017 | Bahrain | BH | BHR | 048 | BHD | $48.97B | $30,597 | -0.14 | 157.48 | High income | PENDING | PARTIAL |
| 018 | Bangladesh | BD | BGD | 050 | BDT | $456.32B | $2,597 | 8.77 | 27.95 | Lower middle income | PENDING | PARTIAL |
| 019 | Barbados | BB | BRB | 052 | BBD | $8.02B | $28,365 | 0.85 | N/A | High income | PENDING | PARTIAL |
| 020 | Belarus | BY | BLR | 112 | BYN | $93.40B | $10,279 | 6.60 | 112.11 | Upper middle income | PENDING | PARTIAL |
| 021 | Belgium | BE | BEL | 056 | EUR | $725.47B | $60,750 | 2.47 | 152.92 | High income | PENDING | PARTIAL |
| 022 | Belize | BZ | BLZ | 084 | BZD | $3.33B | $7,865 | 1.06 | 108.97 | Upper middle income | PENDING | PARTIAL |
| 023 | Benin | BJ | BEN | 204 | XOF | $24.57B | $1,658 | 1.10 | 37.12 | Lower middle income | PENDING | PARTIAL |
| 024 | Bermuda | BM | BMU | 060 | BMD | $9.19B | $142,250 | N/A | 80.21 | High income | PENDING | PARTIAL |
| 025 | Bhutan | BT | BTN | 064 | INR,BTN | $3.58B | $4,493 | 3.56 | 79.36 | Lower middle income | PENDING | PARTIAL |
| 026 | Bolivia | BO | BOL | 068 | BOB | $64.77B | $5,148 | 19.52 | 46.98 | Lower middle income | PENDING | PARTIAL |
| 027 | Bonaire, Sint Eustatius and Saba | BQ | BES | 535 | USD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 028 | Bosnia and Herzegovina | BA | BIH | 070 | BAM | $32.60B | $10,382 | 6.11 | 95.12 | Upper middle income | PENDING | PARTIAL |
| 029 | Botswana | BW | BWA | 072 | BWP | $19.93B | $7,778 | 2.66 | 69.54 | Upper middle income | PENDING | PARTIAL |
| 030 | Bouvet Island | BV | BVT | 074 | NOK | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 031 | Brazil | BR | BRA | 076 | BRL | $2279.92B | $10,713 | 5.02 | 35.29 | Upper middle income | PENDING | PARTIAL |
| 032 | British Indian Ocean Territory | IO | IOT | 086 | USD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 033 | Brunei | BN | BRN | 096 | BND | $15.03B | $32,235 | -0.30 | 123.61 | High income | PENDING | PARTIAL |
| 034 | Bulgaria | BG | BGR | 100 | EUR | $130.78B | $20,328 | 4.58 | 102.51 | High income | PENDING | PARTIAL |
| 035 | Burkina Faso | BF | BFA | 854 | XOF | $27.63B | $1,148 | -0.59 | 65.00 | Low income | PENDING | PARTIAL |
| 036 | Burundi | BI | BDI | 108 | BIF | $3.36B | 233.82 | 34.13 | N/A | Low income | PENDING | PARTIAL |
| 037 | Cabo Verde | CV | CPV | 132 | CVE | $3.06B | $5,796 | 2.34 | 95.33 | Upper middle income | PENDING | PARTIAL |
| 038 | Cambodia | KH | KHM | 116 | KHR | $51.27B | $2,872 | 2.35 | 136.09 | Lower middle income | PENDING | PARTIAL |
| 039 | Cameroon | CM | CMR | 120 | XAF | $58.93B | $1,972 | 3.40 | 33.38 | Lower middle income | PENDING | PARTIAL |
| 040 | Canada | CA | CAN | 124 | CAD | $2319.90B | $55,698 | 2.07 | 63.51 | High income | PENDING | PARTIAL |
| 041 | Cayman Islands | KY | CYM | 136 | KYD | $7.77B | $104,293 | -0.63 | 105.47 | High income | PENDING | PARTIAL |
| 042 | Central African Republic | CF | CAF | 140 | XAF | $3.07B | 556.13 | 1.00 | 52.91 | Low income | PENDING | PARTIAL |
| 043 | Chad | TD | TCD | 148 | XAF | $21.47B | $1,022 | -3.91 | 46.13 | Low income | PENDING | PARTIAL |
| 044 | Chile | CL | CHL | 152 | CLP | $357.37B | $17,995 | 4.21 | 64.98 | High income | PENDING | PARTIAL |
| 045 | China | CN | CHN | 156 | CNY | $19498.04B | $13,862 | 0.06 | 37.96 | Upper middle income | PENDING | PARTIAL |
| 046 | Christmas Island | CX | CXR | 162 | AUD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 047 | Cocos (Keeling) Islands | CC | CCK | 166 | AUD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 048 | Colombia | CO | COL | 170 | COP | $457.41B | $8,562 | 5.14 | 35.37 | Upper middle income | PENDING | PARTIAL |
| 049 | Comoros | KM | COM | 174 | KMF | $1.81B | $2,056 | 3.25 | 40.89 | Lower middle income | PENDING | PARTIAL |
| 050 | Congo | CG | COG | 178 | XAF | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 051 | Cook Islands | CK | COK | 184 | NZD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 052 | Costa Rica | CR | CRI | 188 | CRC | $102.90B | $19,970 | -0.07 | 71.49 | High income | PENDING | PARTIAL |
| 053 | Côte d'Ivoire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 054 | Croatia | HR | HRV | 191 | EUR | $105.06B | $27,104 | 3.69 | 100.99 | High income | PENDING | PARTIAL |
| 055 | Cuba | CU | CUB | 192 | CUP | $107.35B | $9,605 | N/A | 124.86 | Upper middle income | PENDING | PARTIAL |
| 056 | Curaçao | CW | CUW | 531 | XCG | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 057 | Cyprus | CY | CYP | 196 | EUR | $41.23B | $41,783 | 0.13 | 189.50 | High income | PENDING | PARTIAL |
| 058 | Czechia | CZ | CZE | 203 | CZK | $391.03B | $35,917 | 2.46 | 128.04 | High income | PENDING | PARTIAL |
| 059 | Democratic Republic of the Congo | CD | COD | 180 | CDF | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 060 | Denmark | DK | DNK | 208 | DKK | $462.53B | $76,970 | 1.89 | 129.22 | High income | PENDING | PARTIAL |
| 061 | Djibouti | DJ | DJI | 262 | DJF | $4.62B | $3,906 | -0.31 | 206.30 | Lower middle income | PENDING | PARTIAL |
| 062 | Dominica | DM | DMA | 212 | XCD | $723.86M | $10,989 | 2.53 | N/A | Upper middle income | PENDING | PARTIAL |
| 063 | Dominican Republic | DO | DOM | 214 | DOP | $127.41B | $11,059 | 3.87 | 51.49 | Upper middle income | PENDING | PARTIAL |
| 064 | Ecuador | EC | ECU | 218 | USD | $130.32B | $7,125 | 0.71 | 59.36 | Upper middle income | PENDING | PARTIAL |
| 065 | Egypt | EG | EGY | 818 | EGP | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 066 | El Salvador | SV | SLV | 222 | SVC,USD | $36.71B | $5,767 | 0.26 | 88.16 | Upper middle income | PENDING | PARTIAL |
| 067 | Equatorial Guinea | GQ | GNQ | 226 | XAF | $12.82B | $6,615 | 2.92 | 74.93 | Upper middle income | PENDING | PARTIAL |
| 068 | Eritrea | ER | ERI | 232 | ERN | $2.07B | 688.68 | N/A | 47.39 | Low income | PENDING | PARTIAL |
| 069 | Estonia | EE | EST | 233 | EUR | $47.03B | $34,418 | 4.83 | 155.76 | High income | PENDING | PARTIAL |
| 070 | Eswatini | SZ | SWZ | 748 | SZL | $5.16B | $4,108 | 2.60 | 107.48 | Lower middle income | PENDING | PARTIAL |
| 071 | Ethiopia | ET | ETH | 231 | ETB | $126.36B | 932.73 | 13.23 | 33.46 | Low income | PENDING | PARTIAL |
| 072 | Falkland Islands | FK | FLK | 238 | FKP | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 073 | Faroe Islands | FO | FRO | 234 | DKK | $4.05B | $74,175 | N/A | 105.75 | High income | PENDING | PARTIAL |
| 074 | Fiji | FJ | FJI | 242 | FJD | $6.20B | $6,642 | -1.38 | 118.96 | Upper middle income | PENDING | PARTIAL |
| 075 | Finland | FI | FIN | 246 | EUR | $317.04B | $56,149 | 0.34 | 83.40 | High income | PENDING | PARTIAL |
| 076 | France | FR | FRA | 250 | EUR | $3366.32B | $48,986 | 0.94 | 67.19 | High income | PENDING | PARTIAL |
| 077 | French Guiana | GF | GUF | 254 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 078 | French Polynesia | PF | PYF | 258 | XPF | $6.32B | $22,440 | N/A | 64.47 | High income | PENDING | PARTIAL |
| 079 | French Southern Territories | TF | ATF | 260 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 080 | Gabon | GA | GAB | 266 | XAF | $21.43B | $8,263 | 1.77 | 84.89 | Upper middle income | PENDING | PARTIAL |
| 081 | Gambia | GM | GMB | 270 | GMD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 082 | Georgia | GE | GEO | 268 | GEL | $38.14B | $9,692 | 3.87 | 100.63 | Upper middle income | PENDING | PARTIAL |
| 083 | Germany | DE | DEU | 276 | EUR | $5050.92B | $60,496 | 2.17 | 78.51 | High income | PENDING | PARTIAL |
| 084 | Ghana | GH | GHA | 288 | GHS | $114.21B | $3,257 | 14.20 | 68.97 | Lower middle income | PENDING | PARTIAL |
| 085 | Gibraltar | GI | GIB | 292 | GIP | N/A | N/A | N/A | N/A | High income | PENDING | PARTIAL |
| 086 | Greece | GR | GRC | 300 | EUR | $280.64B | $26,948 | 2.48 | 83.45 | High income | PENDING | PARTIAL |
| 087 | Greenland | GL | GRL | 304 | DKK | $3.33B | $58,499 | N/A | 91.89 | High income | PENDING | PARTIAL |
| 088 | Grenada | GD | GRD | 308 | XCD | $1.42B | $12,107 | 0.61 | N/A | Upper middle income | PENDING | PARTIAL |
| 089 | Guadeloupe | GP | GLP | 312 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 090 | Guam | GU | GUM | 316 | USD | $6.91B | $41,833 | N/A | 71.87 | High income | PENDING | PARTIAL |
| 091 | Guatemala | GT | GTM | 320 | GTQ | $123.31B | $6,598 | 1.49 | 46.53 | Upper middle income | PENDING | PARTIAL |
| 092 | Guernsey | GG | GGY | 831 | GBP | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 093 | Guinea | GN | GIN | 324 | GNF | $28.35B | $1,877 | 3.55 | 103.68 | Lower middle income | PENDING | PARTIAL |
| 094 | Guinea-Bissau | GW | GNB | 624 | XOF | $2.53B | $1,124 | 0.87 | 40.88 | Low income | PENDING | PARTIAL |
| 095 | Guyana | GY | GUY | 328 | GYD | $27.10B | $32,414 | 3.33 | 194.35 | High income | PENDING | PARTIAL |
| 096 | Haiti | HT | HTI | 332 | HTG,USD | $32.08B | $2,694 | 28.64 | 15.89 | Lower middle income | PENDING | PARTIAL |
| 097 | Heard Island and McDonald Islands | HM | HMD | 334 | AUD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 098 | Holy See | VA | VAT | 336 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 099 | Honduras | HN | HND | 340 | HNL | $39.60B | $3,598 | 4.60 | 91.51 | Lower middle income | PENDING | PARTIAL |
| 100 | Hong Kong | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 101 | Hungary | HU | HUN | 348 | HUF | $246.49B | $25,907 | 4.41 | 140.78 | High income | PENDING | PARTIAL |
| 102 | Iceland | IS | ISL | 352 | ISK | $38.58B | $98,323 | 4.09 | 82.49 | High income | PENDING | PARTIAL |
| 103 | India | IN | IND | 356 | INR | $3956.07B | $2,702 | 2.40 | 46.27 | Lower middle income | PENDING | PARTIAL |
| 104 | Indonesia | ID | IDN | 360 | IDR | $1445.64B | $5,060 | 1.91 | 43.39 | Upper middle income | PENDING | PARTIAL |
| 105 | Iran | IR | IRN | 364 | IRR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 106 | Iraq | IQ | IRQ | 368 | IQD | $254.37B | $5,410 | 0.30 | 77.14 | Upper middle income | PENDING | PARTIAL |
| 107 | Ireland | IE | IRL | 372 | EUR | $721.70B | $131,592 | 2.21 | 239.96 | High income | PENDING | PARTIAL |
| 108 | Isle of Man | IM | IMN | 833 | GBP | $7.58B | $90,015 | N/A | N/A | High income | PENDING | PARTIAL |
| 109 | Israel | IL | ISR | 376 | ILS | $610.78B | $60,337 | 3.04 | 52.99 | High income | PENDING | PARTIAL |
| 110 | Italy | IT | ITA | 380 | EUR | $2551.56B | $43,309 | 1.53 | 62.52 | High income | PENDING | PARTIAL |
| 111 | Jamaica | JM | JAM | 388 | JMD | $22.70B | $8,003 | 4.00 | N/A | Upper middle income | PENDING | PARTIAL |
| 112 | Japan | JP | JPN | 392 | JPY | $4435.16B | $35,951 | 3.17 | 44.85 | High income | PENDING | PARTIAL |
| 113 | Jersey | JE | JEY | 832 | GBP | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 114 | Jordan | JO | JOR | 400 | JOD | $61.61B | $5,348 | 1.77 | 145.99 | Upper middle income | PENDING | PARTIAL |
| 115 | Kazakhstan | KZ | KAZ | 398 | KZT | $306.24B | $14,692 | 11.39 | 57.16 | Upper middle income | PENDING | PARTIAL |
| 116 | Kenya | KE | KEN | 404 | KES | $135.94B | $2,363 | 4.07 | 37.52 | Lower middle income | PENDING | PARTIAL |
| 117 | Kiribati | KI | KIR | 296 | AUD | $349.23M | $2,559 | 2.46 | 102.11 | Lower middle income | PENDING | PARTIAL |
| 118 | North Korea | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 119 | South Korea | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 120 | Kuwait | KW | KWT | 414 | KWD | $157.21B | $32,312 | 2.36 | 93.99 | High income | PENDING | PARTIAL |
| 121 | Kyrgyzstan | KG | KGZ | 417 | KGS | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 122 | Lao People's Democratic Republic | LA | LAO | 418 | LAK | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 123 | Latvia | LV | LVA | 428 | EUR | $48.62B | $26,312 | 3.75 | 129.72 | High income | PENDING | PARTIAL |
| 124 | Lebanon | LB | LBN | 422 | LBP | $25.97B | $4,473 | 14.60 | 82.23 | Lower middle income | PENDING | PARTIAL |
| 125 | Lesotho | LS | LSO | 426 | LSL,ZAR | $2.57B | $1,089 | 4.27 | 153.98 | Lower middle income | PENDING | PARTIAL |
| 126 | Liberia | LR | LBR | 430 | LRD | $5.25B | 915.33 | 8.32 | N/A | Low income | PENDING | PARTIAL |
| 127 | Libya | LY | LBY | 434 | LYD | $48.10B | $6,449 | 1.84 | 137.66 | Upper middle income | PENDING | PARTIAL |
| 128 | Liechtenstein | LI | LIE | 438 | CHF | $8.91B | $220,167 | N/A | N/A | High income | PENDING | PARTIAL |
| 129 | Lithuania | LT | LTU | 440 | EUR | $95.21B | $32,959 | 3.79 | 142.11 | High income | PENDING | PARTIAL |
| 130 | Luxembourg | LU | LUX | 442 | EUR | $101.16B | $147,252 | 2.26 | 349.89 | High income | PENDING | PARTIAL |
| 131 | Macao | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 132 | Madagascar | MG | MDG | 450 | MGA | $19.62B | 599.27 | 8.05 | 54.24 | Low income | PENDING | PARTIAL |
| 133 | Malawi | MW | MWI | 454 | MWK | $14.92B | 671.51 | 28.37 | 39.40 | Low income | PENDING | PARTIAL |
| 134 | Malaysia | MY | MYS | 458 | MYR | $472.19B | $13,125 | 1.38 | 136.40 | Upper middle income | PENDING | PARTIAL |
| 135 | Maldives | MV | MDV | 462 | MVR | $7.74B | $14,615 | 4.01 | 156.47 | Upper middle income | PENDING | PARTIAL |
| 136 | Mali | ML | MLI | 466 | XOF | $30.07B | $1,193 | 3.28 | 48.77 | Low income | PENDING | PARTIAL |
| 137 | Malta | MT | MLT | 470 | EUR | $27.77B | $47,907 | 2.36 | 219.01 | High income | PENDING | PARTIAL |
| 138 | Marshall Islands | MH | MHL | 584 | USD | $308.00M | $8,489 | N/A | 115.91 | Upper middle income | PENDING | PARTIAL |
| 139 | Martinique | MQ | MTQ | 474 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 140 | Mauritania | MR | MRT | 478 | MRU | $11.68B | $2,198 | 1.55 | 94.02 | Lower middle income | PENDING | PARTIAL |
| 141 | Mauritius | MU | MUS | 480 | MUR | $16.16B | $12,991 | 3.67 | 139.94 | Upper middle income | PENDING | PARTIAL |
| 142 | Mayotte | YT | MYT | 175 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 143 | Mexico | MX | MEX | 484 | MXN | $1832.64B | $13,889 | 3.81 | 79.92 | Upper middle income | PENDING | PARTIAL |
| 144 | Micronesia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 145 | Moldova | MD | MDA | N/A | N/A | $20.35B | $8,622 | 7.76 | 92.67 | Upper middle income | PENDING | PARTIAL |
| 146 | Monaco | MC | MCO | 492 | EUR | $11.13B | $288,002 | N/A | N/A | High income | PENDING | PARTIAL |
| 147 | Mongolia | MN | MNG | 496 | MNT | $25.37B | $7,108 | 8.61 | 130.71 | Upper middle income | PENDING | PARTIAL |
| 148 | Montenegro | ME | MNE | 499 | EUR | $9.23B | $14,817 | 3.90 | 105.77 | Upper middle income | PENDING | PARTIAL |
| 149 | Montserrat | MS | MSR | 500 | XCD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 150 | Morocco | MA | MAR | 504 | MAD | $182.37B | $4,672 | 0.70 | 93.10 | Lower middle income | PENDING | PARTIAL |
| 151 | Mozambique | MZ | MOZ | 508 | MZN | $22.34B | 626.91 | 4.37 | 97.10 | Low income | PENDING | PARTIAL |
| 152 | Myanmar | MM | MMR | 104 | MMK | $81.67B | $1,489 | 8.83 | N/A | Lower middle income | PENDING | PARTIAL |
| 153 | Namibia | NA | NAM | 516 | NAD,ZAR | $15.08B | $4,876 | 3.51 | 108.58 | Lower middle income | PENDING | PARTIAL |
| 154 | Nauru | NR | NRU | 520 | AUD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 155 | Nepal | NP | NPL | 524 | NPR | $45.49B | $1,536 | 2.65 | 42.55 | Lower middle income | PENDING | PARTIAL |
| 156 | Netherlands | NL | NLD | 528 | EUR | $1332.77B | $73,684 | 3.26 | 150.12 | High income | PENDING | PARTIAL |
| 157 | New Caledonia | NC | NCL | 540 | XPF | $8.55B | $29,213 | 0.58 | 40.37 | High income | PENDING | PARTIAL |
| 158 | New Zealand | NZ | NZL | 554 | NZD | $264.06B | $49,591 | 2.84 | 50.66 | High income | PENDING | PARTIAL |
| 159 | Nicaragua | NI | NIC | 558 | NIO | $22.24B | $3,173 | 2.08 | 96.45 | Lower middle income | PENDING | PARTIAL |
| 160 | Niger | NE | NER | 562 | XOF | $21.65B | 775.35 | -4.45 | 33.91 | Low income | PENDING | PARTIAL |
| 161 | Nigeria | NG | NGA | 566 | NGN | $290.79B | $1,224 | 23.01 | 26.17 | Lower middle income | PENDING | PARTIAL |
| 162 | Niue | NU | NIU | 570 | NZD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 163 | Norfolk Island | NF | NFK | 574 | AUD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 164 | North Macedonia | MK | MKD | 807 | MKD | $19.10B | $10,490 | 4.07 | 133.04 | Upper middle income | PENDING | PARTIAL |
| 165 | Northern Mariana Islands | MP | MNP | 580 | USD | $1.10B | $23,786 | N/A | 93.16 | High income | PENDING | PARTIAL |
| 166 | Norway | NO | NOR | 578 | NOK | $530.76B | $94,594 | 3.06 | 79.05 | High income | PENDING | PARTIAL |
| 167 | Oman | OM | OMN | 512 | OMR | $109.60B | $19,947 | 0.97 | 114.85 | High income | PENDING | PARTIAL |
| 168 | Pakistan | PK | PAK | 586 | PKR | $407.31B | $1,596 | 3.55 | 27.19 | Lower middle income | PENDING | PARTIAL |
| 169 | Palau | PW | PLW | 585 | USD | $345.00M | $19,532 | -0.42 | 105.81 | High income | PENDING | PARTIAL |
| 170 | Palestine | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 171 | Panama | PA | PAN | 591 | PAB,USD | $90.46B | $19,790 | -0.19 | 83.67 | High income | PENDING | PARTIAL |
| 172 | Papua New Guinea | PG | PNG | 598 | PGK | $32.50B | $3,020 | 4.42 | 131.08 | Lower middle income | PENDING | PARTIAL |
| 173 | Paraguay | PY | PRY | 600 | PYG | $49.28B | $7,027 | 4.04 | 76.48 | Upper middle income | PENDING | PARTIAL |
| 174 | Peru | PE | PER | 604 | PEN | $334.85B | $9,684 | 1.53 | 51.82 | Upper middle income | PENDING | PARTIAL |
| 175 | Philippines | PH | PHL | 608 | PHP | $487.09B | $4,171 | 1.66 | 66.79 | Upper middle income | PENDING | PARTIAL |
| 176 | Pitcairn | PN | PCN | 612 | NZD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 177 | Poland | PL | POL | 616 | PLN | $1035.49B | $28,420 | 3.81 | 97.03 | High income | PENDING | PARTIAL |
| 178 | Portugal | PT | PRT | 620 | EUR | $346.64B | $32,082 | 2.34 | 86.40 | High income | PENDING | PARTIAL |
| 179 | Puerto Rico | PR | PRI | 630 | USD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 180 | Qatar | QA | QAT | 634 | QAR | $215.56B | $72,525 | 1.27 | 100.21 | High income | PENDING | PARTIAL |
| 181 | Réunion | RE | REU | 638 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 182 | Romania | RO | ROU | 642 | RON | $428.68B | $22,538 | 7.19 | 76.21 | High income | PENDING | PARTIAL |
| 183 | Russian Federation | RU | RUS | 643 | RUB | $2561.31B | $17,547 | 8.72 | 33.71 | High income | PENDING | PARTIAL |
| 184 | Rwanda | RW | RWA | 646 | RWF | $16.37B | $1,124 | 5.91 | 56.67 | Low income | PENDING | PARTIAL |
| 185 | Saint Barthélemy | BL | BLM | 652 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 186 | Saint Helena, Ascension and Tristan da Cunha | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 187 | Saint Kitts and Nevis | KN | KNA | 659 | XCD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 188 | Saint Lucia | LC | LCA | 662 | XCD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 189 | Saint Martin | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 190 | Saint Pierre and Miquelon | PM | SPM | 666 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 191 | Saint Vincent and the Grenadines | VC | VCT | 670 | XCD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 192 | Samoa | WS | WSM | 882 | WST | $1.29B | $5,873 | 2.21 | 70.63 | Upper middle income | PENDING | PARTIAL |
| 193 | San Marino | SM | SMR | 674 | EUR | $2.03B | $59,871 | 2.30 | 340.98 | High income | PENDING | PARTIAL |
| 194 | Sao Tome and Principe | ST | STP | 678 | STN | $981.29M | $4,084 | 11.05 | 46.57 | Lower middle income | PENDING | PARTIAL |
| 195 | Saudi Arabia | SA | SAU | 682 | SAR | $1276.94B | $34,537 | 2.08 | 57.92 | High income | PENDING | PARTIAL |
| 196 | Senegal | SN | SEN | 686 | XOF | $37.01B | $1,955 | 1.46 | 71.07 | Lower middle income | PENDING | PARTIAL |
| 197 | Serbia | RS | SRB | 688 | RSD | $99.95B | $15,262 | 3.89 | 112.92 | Upper middle income | PENDING | PARTIAL |
| 198 | Seychelles | SC | SYC | 690 | SCR | $2.39B | $19,449 | 0.30 | 172.29 | High income | PENDING | PARTIAL |
| 199 | Sierra Leone | SL | SLE | 694 | SLE | $7.46B | 846.30 | 7.49 | 49.10 | Low income | PENDING | PARTIAL |
| 200 | Singapore | SG | SGP | 702 | SGD | $603.87B | $98,814 | 0.90 | 320.31 | High income | PENDING | PARTIAL |
| 201 | Sint Maarten | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 202 | Slovakia | SK | SVK | 703 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 203 | Slovenia | SI | SVN | 705 | EUR | $79.65B | $37,376 | 2.37 | 151.92 | High income | PENDING | PARTIAL |
| 204 | Solomon Islands | SB | SLB | 090 | SBD | $1.75B | $2,086 | 3.36 | 111.35 | Lower middle income | PENDING | PARTIAL |
| 205 | Somalia | SO | SOM | 706 | SOS | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 206 | South Africa | ZA | ZAF | 710 | ZAR | $427.18B | $6,598 | 3.21 | 60.91 | Upper middle income | PENDING | PARTIAL |
| 207 | South Georgia and the South Sandwich Islands | GS | SGS | 239 | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 208 | South Sudan | SS | SSD | 728 | SSP | $12.00B | $1,080 | 91.44 | 65.55 | Low income | PENDING | PARTIAL |
| 209 | Spain | ES | ESP | 724 | EUR | $1906.45B | $38,627 | 2.70 | 69.45 | High income | PENDING | PARTIAL |
| 210 | Sri Lanka | LK | LKA | 144 | LKR | $108.83B | $5,002 | -4.76 | 41.81 | Upper middle income | PENDING | PARTIAL |
| 211 | Sudan | SD | SDN | 729 | SDG | $60.16B | $1,165 | 138.81 | 2.38 | Low income | PENDING | PARTIAL |
| 212 | Suriname | SR | SUR | 740 | SRD | $4.52B | $7,070 | 9.21 | 90.96 | Upper middle income | PENDING | PARTIAL |
| 213 | Svalbard and Jan Mayen | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 214 | Sweden | SE | SWE | 752 | SEK | $669.00B | $63,133 | 0.68 | 104.34 | High income | PENDING | PARTIAL |
| 215 | Switzerland | CH | CHE | 756 | CHF | $1043.53B | $114,769 | 0.15 | 147.16 | High income | PENDING | PARTIAL |
| 216 | Syrian Arab Republic | SY | SYR | 760 | SYP | $23.74B | $1,057 | 13.42 | 35.44 | Low income | PENDING | PARTIAL |
| 217 | Taiwan | TW | TWN | 158 | TWD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 218 | Tajikistan | TJ | TJK | 762 | TJS | $17.66B | $1,637 | 6.00 | 59.09 | Lower middle income | PENDING | PARTIAL |
| 219 | Tanzania | TZ | TZA | N/A | N/A | $90.14B | $1,319 | 3.33 | 38.97 | Lower middle income | PENDING | PARTIAL |
| 220 | Thailand | TH | THA | 764 | THB | $577.01B | $8,057 | -0.13 | 138.41 | Upper middle income | PENDING | PARTIAL |
| 221 | Timor-Leste | TL | TLS | 626 | USD | $1.90B | $1,341 | 0.43 | 95.45 | Lower middle income | PENDING | PARTIAL |
| 222 | Togo | TG | TGO | 768 | XOF | $11.89B | $1,384 | 0.43 | 59.93 | Lower middle income | PENDING | PARTIAL |
| 223 | Tokelau | TK | TKL | 772 | NZD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 224 | Tonga | TO | TON | 776 | TOP | $679.22M | $6,547 | 5.59 | 77.58 | Upper middle income | PENDING | PARTIAL |
| 225 | Trinidad and Tobago | TT | TTO | 780 | TTD | $25.94B | $18,967 | 0.99 | N/A | High income | PENDING | PARTIAL |
| 226 | Tunisia | TN | TUN | 788 | TND | $57.50B | $4,657 | 5.15 | 101.45 | Lower middle income | PENDING | PARTIAL |
| 227 | Türkiye | TR | TUR | 792 | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 228 | Turkmenistan | TM | TKM | 795 | TMT | $49.83B | $6,540 | N/A | 28.17 | Upper middle income | PENDING | PARTIAL |
| 229 | Turks and Caicos Islands | TC | TCA | 796 | USD | $1.75B | $37,507 | N/A | N/A | High income | PENDING | PARTIAL |
| 230 | Tuvalu | TV | TUV | 798 | AUD | $57.35M | $6,041 | 0.50 | N/A | Upper middle income | PENDING | PARTIAL |
| 231 | Uganda | UG | UGA | 800 | UGX | $61.99B | $1,206 | 3.58 | 50.20 | Low income | PENDING | PARTIAL |
| 232 | Ukraine | UA | UKR | 804 | UAH | $214.23B | $5,866 | 12.73 | 77.18 | Upper middle income | PENDING | PARTIAL |
| 233 | United Arab Emirates | AE | ARE | 784 | AED | $552.32B | $50,274 | 1.25 | 199.04 | High income | PENDING | PARTIAL |
| 234 | United Kingdom | GB | GBR | N/A | N/A | $4002.59B | $57,602 | 3.88 | 62.46 | High income | PENDING | PARTIAL |
| 235 | United States | US | USA | N/A | N/A | $30769.70B | $90,027 | 2.95 | 25.02 | High income | PENDING | PARTIAL |
| 236 | United States Minor Outlying Islands | UM | UMI | 581 | USD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 237 | Uruguay | UY | URY | 858 | UYU,UYW | $85.35B | $25,216 | 4.65 | 49.72 | High income | PENDING | PARTIAL |
| 238 | Uzbekistan | UZ | UZB | 860 | UZS | $147.04B | $3,968 | 8.80 | 57.40 | Lower middle income | PENDING | PARTIAL |
| 239 | Vanuatu | VU | VUT | 548 | VUV | $1.35B | $4,039 | 0.66 | 73.38 | Lower middle income | PENDING | PARTIAL |
| 240 | Venezuela | VE | VEN | 862 | VES,VED | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 241 | Vietnam | VN | VNM | N/A | N/A | $514.70B | $5,066 | 3.31 | 190.34 | Upper middle income | PENDING | PARTIAL |
| 242 | Virgin Islands, British | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 243 | Virgin Islands, U.S. | VI | VIR | N/A | N/A | $4.67B | $44,321 | N/A | 205.63 | High income | PENDING | PARTIAL |
| 244 | Wallis and Futuna | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 245 | Western Sahara | EH | ESH | 732 | MAD | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 246 | Yemen | YE | YEM | 887 | YER | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 247 | Zambia | ZM | ZMB | 894 | ZMW | $28.88B | $1,318 | 13.91 | 62.53 | Lower middle income | PENDING | PARTIAL |
| 248 | Zimbabwe | ZW | ZWE | 716 | ZWG | $51.22B | $3,021 | 104.71 | 41.50 | Lower middle income | PENDING | PARTIAL |
| 249 | Åland Islands | AX | ALA | 248 | EUR | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 250 | Roman Republic | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 251 | Roman Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 252 | Byzantine Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 253 | Holy Roman Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 254 | Western Roman Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 255 | Eastern Roman Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 256 | Ancient Egypt | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 257 | Kingdom of Kush | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 258 | Carthaginian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 259 | Kingdom of Aksum | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 260 | Mali Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 261 | Songhai Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 262 | Ghana Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 263 | Kanem-Bornu Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 264 | Benin Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 265 | Oyo Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 266 | Ashanti Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 267 | Ethiopian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 268 | Kingdom of Kongo | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 269 | Mutapa Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 270 | Maravi Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 271 | Zulu Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 272 | Sokoto Caliphate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 273 | Almohad Caliphate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 274 | Almoravid Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 275 | Fatimid Caliphate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 276 | Mamluk Sultanate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 277 | Abbasid Caliphate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 278 | Umayyad Caliphate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 279 | Ottoman Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 280 | Safavid Iran | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 281 | Qajar Iran | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 282 | Mughal Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 283 | Maurya Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 284 | Gupta Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 285 | Chola Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 286 | Pala Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 287 | Delhi Sultanate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 288 | Maratha Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 289 | Vijayanagara Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 290 | Kushan Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 291 | Parthian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 292 | Sasanian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 293 | Achaemenid Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 294 | Median Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 295 | Hittite Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 296 | Phoenicia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 297 | Kingdom of Lydia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 298 | Seleucid Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 299 | Ptolemaic Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 300 | Ancient Greece | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 301 | Macedonian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 302 | Spartan State | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 303 | Athens | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 304 | Carthage | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 305 | Han Dynasty | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 306 | Qin Dynasty | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 307 | Tang Dynasty | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 308 | Song Dynasty | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 309 | Yuan Dynasty | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 310 | Ming Dynasty | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 311 | Qing Dynasty | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 312 | Three Kingdoms China | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 313 | Korea under Joseon | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 314 | Goryeo | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 315 | Silla | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 316 | Baekje | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 317 | Goguryeo | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 318 | Ryukyu Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 319 | Khmer Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 320 | Pagan Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 321 | Ayutthaya Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 322 | Sukhothai Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 323 | Majapahit | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 324 | Srivijaya | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 325 | Malacca Sultanate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 326 | Brunei Sultanate | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 327 | Lan Xang | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 328 | Dai Viet | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 329 | Champa | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 330 | Nguyen Vietnam | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 331 | Mataram Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 332 | Hawaiian Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 333 | Kingdom of Tahiti | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 334 | Tu'i Tonga Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 335 | Samoan Kingdom | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 336 | Maori Confederations | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 337 | Inca Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 338 | Aztec Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 339 | Maya City-States | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 340 | Mississippian Cultures | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 341 | Tarascan State | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 342 | Gran Colombia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 343 | Viceroyalty of New Spain | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 344 | Viceroyalty of Peru | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 345 | Viceroyalty of New Granada | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 346 | Viceroyalty of the Río de la Plata | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 347 | Captaincy General of Guatemala | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 348 | Kingdom of Brazil | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 349 | United Provinces of Central America | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 350 | Federal Republic of Central America | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 351 | United States of Colombia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 352 | New Granada | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 353 | Kingdom of Prussia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 354 | German Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 355 | Austro-Hungarian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 356 | Austrian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 357 | Kingdom of Hungary | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 358 | Kingdom of Bohemia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 359 | Polish-Lithuanian Commonwealth | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 360 | Kingdom of Poland | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 361 | Grand Duchy of Lithuania | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 362 | Russian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 363 | Soviet Union | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 364 | Yugoslavia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 365 | Czechoslovakia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 366 | East Germany | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 367 | West Germany | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 368 | Kingdom of Italy | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 369 | Kingdom of Sardinia | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 370 | Papal States | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 371 | Kingdom of the Two Sicilies | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 372 | French Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 373 | Kingdom of France | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 374 | Spanish Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 375 | Kingdom of Spain | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 376 | Portuguese Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 377 | Dutch Republic | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 378 | British Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 379 | Irish Free State | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 380 | United Arab Republic | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 381 | United Arab Federation | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 382 | North Yemen | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 383 | South Yemen | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 384 | Democratic Kampuchea | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 385 | South Vietnam | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 386 | North Vietnam | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 387 | Republic of Vietnam | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 388 | Kingdom of Afghanistan | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 389 | Tibetan Government | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 390 | Manchu-led Qing successor states | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 391 | Persian Empire | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
