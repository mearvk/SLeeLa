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

# SLeeLa — BANKS.md

## HTTP 3.0 National Banking, Economic & Historical Registry — Combined 391-Country Table

This is the materialized union of `BANKS.md` and `BANKS2.md`, joined by the canonical three-digit registry ID and country/jurisdiction. Economic, banking and status fields come from `BANKS.md`; Currency comes from `BANKS2.md`.

| ID | Country/Jurisdiction | ISO | Currency | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Status |
|---:|---|---|---|---:|---:|---:|---:|---|---|---|
| 001 | Afghanistan | 1919 | OPEN | $17.78B | 416.87 | -6.60 | 83.76 | Low income | Da Afghanistan Bank | PARTIAL |
| 002 | Albania | 1912 | OPEN | $30.54B | $12,998 | 2.15 | 79.53 | Upper middle income | Bank of Albania | PARTIAL |
| 003 | Algeria | 1962 | OPEN | $287.03B | $6,051 | 1.42 | 38.98 | Upper middle income | Bank of Algeria | PARTIAL |
| 004 | American Samoa | AS / ASM | USD | $871.00M | $18,017 | N/A | 124.68 | High income | Federal Reserve System / American Samoa banking institutions | PARTIAL |
| 005 | Andorra | 1278 | OPEN | $4.50B | $54,292 | N/A | N/A | High income | Andorran Financial Authority (AFA) | PARTIAL |
| 006 | Angola | 1975 | OPEN | $122.17B | $3,129 | 20.16 | 34.65 | Lower middle income | National Bank of Angola | PARTIAL |
| 007 | Anguilla | N/A / N/A | XCD | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | MISSING |
| 008 | Antarctica | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Antarctica (BIS Central Bank Hub reference) | MISSING |
| 009 | Antigua and Barbuda | 1981 | OPEN | $2.34B | $24,819 | 1.37 | 118.30 | High income | Eastern Caribbean Central Bank (ECCB) | PARTIAL |
| 010 | Argentina | 1816 | OPEN | $683.10B | $14,898 | 219.88 | 30.36 | Upper middle income | Central Bank of Argentina | PARTIAL |
| 011 | Armenia | 1991 | OPEN | $29.24B | $9,474 | 3.31 | 101.71 | Upper middle income | Central Bank of Armenia | PARTIAL |
| 012 | Aruba | AW / ABW | AWG | $4.17B | $38,591 | 4.26 | 164.14 | High income | Central Bank of Aruba | PARTIAL |
| 013 | Australia | 1901 | OPEN | $1798.52B | $65,130 | 2.87 | 45.90 | High income | Reserve Bank of Australia | PARTIAL |
| 014 | Austria | 1918 | OPEN | $579.47B | $62,930 | 3.53 | 107.02 | High income | Oesterreichische Nationalbank | PARTIAL |
| 015 | Azerbaijan | 1991 | OPEN | $75.94B | $7,411 | 5.62 | 79.83 | Upper middle income | Central Bank of Azerbaijan | PARTIAL |
| 016 | Bahamas | 1973 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of The Bahamas | MISSING |
| 017 | Bahrain | 1971 | OPEN | $48.97B | $30,597 | -0.14 | 157.48 | High income | Central Bank of Bahrain | PARTIAL |
| 018 | Bangladesh | 1971 | OPEN | $456.32B | $2,597 | 8.77 | 27.95 | Lower middle income | Bangladesh Bank | PARTIAL |
| 019 | Barbados | 1966 | OPEN | $8.02B | $28,365 | 0.85 | N/A | High income | Central Bank of Barbados | PARTIAL |
| 020 | Belarus | 1991 | OPEN | $93.40B | $10,279 | 6.60 | 112.11 | Upper middle income | National Bank of the Republic of Belarus | PARTIAL |
| 021 | Belgium | 1830 | OPEN | $725.47B | $60,750 | 2.47 | 152.92 | High income | National Bank of Belgium / European Central Bank | PARTIAL |
| 022 | Belize | 1981 | OPEN | $3.33B | $7,865 | 1.06 | 108.97 | Upper middle income | Central Bank of Belize | PARTIAL |
| 023 | Benin | 1960 | OPEN | $24.57B | $1,658 | 1.10 | 37.12 | Lower middle income | National banking and monetary authority — Benin (BIS Central Bank Hub reference) | PARTIAL |
| 024 | Bermuda | BM / BMU | BMD | $9.19B | $142,250 | N/A | 80.21 | High income | Bermuda Monetary Authority | PARTIAL |
| 025 | Bhutan | 1910 | OPEN | $3.58B | $4,493 | 3.56 | 79.36 | Lower middle income | Royal Monetary Authority of Bhutan | PARTIAL |
| 026 | Bolivia | 1825 | OPEN | $64.77B | $5,148 | 19.52 | 46.98 | Lower middle income | Central Bank of Bolivia | PARTIAL |
| 027 | Bonaire, Sint Eustatius and Saba | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Bonaire, Sint Eustatius and Saba (BIS Central Bank Hub reference) | MISSING |
| 028 | Bosnia and Herzegovina | 1992 | OPEN | $32.60B | $10,382 | 6.11 | 95.12 | Upper middle income | Central Bank of Bosnia and Herzegovina | PARTIAL |
| 029 | Botswana | 1966 | OPEN | $19.93B | $7,778 | 2.66 | 69.54 | Upper middle income | Bank of Botswana | PARTIAL |
| 030 | Bouvet Island | N/A / N/A | NOK | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Bouvet Island (BIS Central Bank Hub reference) | MISSING |
| 031 | Brazil | 1822 | OPEN | $2279.92B | $10,713 | 5.02 | 35.29 | Upper middle income | Central Bank of Brazil | PARTIAL |
| 032 | British Indian Ocean Territory | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — British Indian Ocean Territory (BIS Central Bank Hub reference) | MISSING |
| 033 | Brunei | 1984 | OPEN | $15.03B | $32,235 | -0.30 | 123.61 | High income | Brunei Darussalam Central Bank (AMBD) | PARTIAL |
| 034 | Bulgaria | 1908 | OPEN | $130.78B | $20,328 | 4.58 | 102.51 | High income | Bulgarian National Bank / European Central Bank | PARTIAL |
| 035 | Burkina Faso | 1960 | OPEN | $27.63B | $1,148 | -0.59 | 65.00 | Low income | Central Bank of West African States (BCEAO) | PARTIAL |
| 036 | Burundi | 1962 | OPEN | $3.36B | 233.82 | 34.13 | N/A | Low income | Bank of the Republic of Burundi | PARTIAL |
| 037 | Cabo Verde | 1975 | OPEN | $3.06B | $5,796 | 2.34 | 95.33 | Upper middle income | Bank of Cabo Verde | PARTIAL |
| 038 | Cambodia | 1953 | OPEN | $51.27B | $2,872 | 2.35 | 136.09 | Lower middle income | National Bank of Cambodia | PARTIAL |
| 039 | Cameroon | 1960 | OPEN | $58.93B | $1,972 | 3.40 | 33.38 | Lower middle income | Bank of Central African States (BEAC) | PARTIAL |
| 040 | Canada | 1867 | OPEN | $2319.90B | $55,698 | 2.07 | 63.51 | High income | Bank of Canada | PARTIAL |
| 041 | Cayman Islands | KY / CYM | KYD | $7.77B | $104,293 | -0.63 | 105.47 | High income | Cayman Islands Monetary Authority | PARTIAL |
| 042 | Central African Republic | 1960 | OPEN | $3.07B | 556.13 | 1.00 | 52.91 | Low income | Bank of Central African States (BEAC) | PARTIAL |
| 043 | Chad | 1960 | OPEN | $21.47B | $1,022 | -3.91 | 46.13 | Low income | Bank of Central African States (BEAC) | PARTIAL |
| 044 | Chile | 1810 | OPEN | $357.37B | $17,995 | 4.21 | 64.98 | High income | Central Bank of Chile | PARTIAL |
| 045 | China | -1523 | OPEN | $19498.04B | $13,862 | 0.06 | 37.96 | Upper middle income | People's Bank of China | PARTIAL |
| 046 | Christmas Island | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Christmas Island (BIS Central Bank Hub reference) | MISSING |
| 047 | Cocos (Keeling) Islands | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Cocos (Keeling) Islands (BIS Central Bank Hub reference) | MISSING |
| 048 | Colombia | 1810 | OPEN | $457.41B | $8,562 | 5.14 | 35.37 | Upper middle income | Banco de la República | PARTIAL |
| 049 | Comoros | 1975 | OPEN | $1.81B | $2,056 | 3.25 | 40.89 | Lower middle income | Central Bank of the Comoros | PARTIAL |
| 050 | Congo | 1960 | OPEN | N/A | N/A | N/A | N/A | N/A | Bank of Central African States (BEAC) | MISSING |
| 051 | Cook Islands | N/A / N/A | NZD | N/A | N/A | N/A | N/A | N/A | Cook Islands Financial Supervisory Commission / Reserve Bank of New Zealand currency system | MISSING |
| 052 | Costa Rica | 1821 | OPEN | $102.90B | $19,970 | -0.07 | 71.49 | High income | Central Bank of Costa Rica | PARTIAL |
| 053 | Côte d'Ivoire | N/A / N/A | XOF | N/A | N/A | N/A | N/A | N/A | Central Bank of West African States (BCEAO) | MISSING |
| 054 | Croatia | 1991 | OPEN | $105.06B | $27,104 | 3.69 | 100.99 | High income | Croatian National Bank / European Central Bank | PARTIAL |
| 055 | Cuba | 1902 | OPEN | $107.35B | $9,605 | N/A | 124.86 | Upper middle income | Central Bank of Cuba | PARTIAL |
| 056 | Curaçao | N/A / N/A | XCG | N/A | N/A | N/A | N/A | N/A | Central Bank of Curaçao and Sint Maarten | MISSING |
| 057 | Cyprus | 1960 | OPEN | $41.23B | $41,783 | 0.13 | 189.50 | High income | Central Bank of Cyprus / European Central Bank | PARTIAL |
| 058 | Czechia | 1993 | OPEN | $391.03B | $35,917 | 2.46 | 128.04 | High income | Czech National Bank | PARTIAL |
| 059 | Democratic Republic of the Congo | N/A / N/A | CDF | N/A | N/A | N/A | N/A | N/A | Central Bank of the Congo | MISSING |
| 060 | Denmark | 800 | OPEN | $462.53B | $76,970 | 1.89 | 129.22 | High income | Danmarks Nationalbank | PARTIAL |
| 061 | Djibouti | 1977 | OPEN | $4.62B | $3,906 | -0.31 | 206.30 | Lower middle income | Central Bank of Djibouti | PARTIAL |
| 062 | Dominica | 1978 | OPEN | $723.86M | $10,989 | 2.53 | N/A | Upper middle income | Eastern Caribbean Central Bank (ECCB) | PARTIAL |
| 063 | Dominican Republic | 1844 | OPEN | $127.41B | $11,059 | 3.87 | 51.49 | Upper middle income | Central Bank of the Dominican Republic | PARTIAL |
| 064 | Ecuador | 1822 | OPEN | $130.32B | $7,125 | 0.71 | 59.36 | Upper middle income | Central Bank of Ecuador | PARTIAL |
| 065 | Egypt | 1922 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of Egypt | MISSING |
| 066 | El Salvador | 1841 | OPEN | $36.71B | $5,767 | 0.26 | 88.16 | Upper middle income | Central Reserve Bank of El Salvador | PARTIAL |
| 067 | Equatorial Guinea | 1968 | OPEN | $12.82B | $6,615 | 2.92 | 74.93 | Upper middle income | Bank of Central African States (BEAC) | PARTIAL |
| 068 | Eritrea | 1993 | OPEN | $2.07B | 688.68 | N/A | 47.39 | Low income | Bank of Eritrea | PARTIAL |
| 069 | Estonia | 1991 | OPEN | $47.03B | $34,418 | 4.83 | 155.76 | High income | Eesti Pank / European Central Bank | PARTIAL |
| 070 | Eswatini | 1968 | OPEN | $5.16B | $4,108 | 2.60 | 107.48 | Lower middle income | Central Bank of Eswatini | PARTIAL |
| 071 | Ethiopia | -1000 | OPEN | $126.36B | 932.73 | 13.23 | 33.46 | Low income | National Bank of Ethiopia | PARTIAL |
| 072 | Falkland Islands | N/A / N/A | FKP | N/A | N/A | N/A | N/A | N/A | Government of the Falkland Islands / currency linked to pound sterling | MISSING |
| 073 | Faroe Islands | FO / FRO | DKK | $4.05B | $74,175 | N/A | 105.75 | High income | Danmarks Nationalbank / local banking authority | PARTIAL |
| 074 | Fiji | FJ / FJI | FJD | $6.20B | $6,642 | -1.38 | 118.96 | Upper middle income | Reserve Bank of Fiji | PARTIAL |
| 075 | Finland | 1917 | OPEN | $317.04B | $56,149 | 0.34 | 83.40 | High income | Bank of Finland / European Central Bank | PARTIAL |
| 076 | France | 843 | OPEN | $3366.32B | $48,986 | 0.94 | 67.19 | High income | Banque de France / European Central Bank | PARTIAL |
| 077 | French Guiana | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | MISSING |
| 078 | French Polynesia | PF / PYF | XPF | $6.32B | $22,440 | N/A | 64.47 | High income | Institut d'émission d'outre-mer (IEOM) | PARTIAL |
| 079 | French Southern Territories | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Institut d'émission d'outre-mer (IEOM) | MISSING |
| 080 | Gabon | 1960 | OPEN | $21.43B | $8,263 | 1.77 | 84.89 | Upper middle income | Bank of Central African States (BEAC) | PARTIAL |
| 081 | Gambia | 1965 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of The Gambia | MISSING |
| 082 | Georgia | 1991 | OPEN | $38.14B | $9,692 | 3.87 | 100.63 | Upper middle income | National Bank of Georgia | PARTIAL |
| 083 | Germany | 1955 | OPEN | $5050.92B | $60,496 | 2.17 | 78.51 | High income | Deutsche Bundesbank / European Central Bank | PARTIAL |
| 084 | Ghana | 1957 | OPEN | $114.21B | $3,257 | 14.20 | 68.97 | Lower middle income | Bank of Ghana | PARTIAL |
| 085 | Gibraltar | GI / GIB | GIP | N/A | N/A | N/A | N/A | High income | Gibraltar Financial Services Commission / Bank of England sterling system | PARTIAL |
| 086 | Greece | 1830 | OPEN | $280.64B | $26,948 | 2.48 | 83.45 | High income | Bank of Greece / European Central Bank | PARTIAL |
| 087 | Greenland | GL / GRL | DKK | $3.33B | $58,499 | N/A | 91.89 | High income | Danmarks Nationalbank / local banking authority | PARTIAL |
| 088 | Grenada | 1974 | OPEN | $1.42B | $12,107 | 0.61 | N/A | Upper middle income | Eastern Caribbean Central Bank (ECCB) | PARTIAL |
| 089 | Guadeloupe | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | MISSING |
| 090 | Guam | GU / GUM | USD | $6.91B | $41,833 | N/A | 71.87 | High income | Federal Reserve System / Guam banking institutions | PARTIAL |
| 091 | Guatemala | 1821 | OPEN | $123.31B | $6,598 | 1.49 | 46.53 | Upper middle income | Bank of Guatemala | PARTIAL |
| 092 | Guernsey | N/A / N/A | GBP | N/A | N/A | N/A | N/A | N/A | Guernsey Financial Services Commission / sterling banking system | MISSING |
| 093 | Guinea | 1958 | OPEN | $28.35B | $1,877 | 3.55 | 103.68 | Lower middle income | Central Bank of the Republic of Guinea | PARTIAL |
| 094 | Guinea-Bissau | 1974 | OPEN | $2.53B | $1,124 | 0.87 | 40.88 | Low income | Central Bank of West African States (BCEAO) | PARTIAL |
| 095 | Guyana | 1966 | OPEN | $27.10B | $32,414 | 3.33 | 194.35 | High income | Bank of Guyana | PARTIAL |
| 096 | Haiti | 1804 | OPEN | $32.08B | $2,694 | 28.64 | 15.89 | Lower middle income | Bank of the Republic of Haiti | PARTIAL |
| 097 | Heard Island and McDonald Islands | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Heard Island and McDonald Islands (BIS Central Bank Hub reference) | MISSING |
| 098 | Holy See | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Institute for the Works of Religion / Vatican Financial Information Authority; euro system | MISSING |
| 099 | Honduras | 1838 | OPEN | $39.60B | $3,598 | 4.60 | 91.51 | Lower middle income | Central Bank of Honduras | PARTIAL |
| 100 | Hong Kong | N/A / N/A | HKD | N/A | N/A | N/A | N/A | N/A | Hong Kong Monetary Authority | MISSING |
| 101 | Hungary | 1918 | OPEN | $246.49B | $25,907 | 4.41 | 140.78 | High income | Magyar Nemzeti Bank | PARTIAL |
| 102 | Iceland | 1944 | OPEN | $38.58B | $98,323 | 4.09 | 82.49 | High income | Central Bank of Iceland | PARTIAL |
| 103 | India | 1947 | OPEN | $3956.07B | $2,702 | 2.40 | 46.27 | Lower middle income | Reserve Bank of India | PARTIAL |
| 104 | Indonesia | 1945 | OPEN | $1445.64B | $5,060 | 1.91 | 43.39 | Upper middle income | Bank Indonesia | PARTIAL |
| 105 | Iran | 1906 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of the Islamic Republic of Iran | MISSING |
| 106 | Iraq | 1932 | OPEN | $254.37B | $5,410 | 0.30 | 77.14 | Upper middle income | Central Bank of Iraq | PARTIAL |
| 107 | Ireland | 1921 | OPEN | $721.70B | $131,592 | 2.21 | 239.96 | High income | Central Bank of Ireland / European Central Bank | PARTIAL |
| 108 | Isle of Man | IM / IMN | GBP | $7.58B | $90,015 | N/A | N/A | High income | Isle of Man Financial Services Authority / sterling banking system | PARTIAL |
| 109 | Israel | 1948 | OPEN | $610.78B | $60,337 | 3.04 | 52.99 | High income | Bank of Israel | PARTIAL |
| 110 | Italy | 1861 | OPEN | $2551.56B | $43,309 | 1.53 | 62.52 | High income | Banca d'Italia / European Central Bank | PARTIAL |
| 111 | Jamaica | 1962 | OPEN | $22.70B | $8,003 | 4.00 | N/A | Upper middle income | Bank of Jamaica | PARTIAL |
| 112 | Japan | -660 | OPEN | $4435.16B | $35,951 | 3.17 | 44.85 | High income | Bank of Japan | PARTIAL |
| 113 | Jersey | N/A / N/A | GBP | N/A | N/A | N/A | N/A | N/A | Jersey Financial Services Commission / sterling banking system | MISSING |
| 114 | Jordan | 1946 | OPEN | $61.61B | $5,348 | 1.77 | 145.99 | Upper middle income | Central Bank of Jordan | PARTIAL |
| 115 | Kazakhstan | 1991 | OPEN | $306.24B | $14,692 | 11.39 | 57.16 | Upper middle income | National Bank of Kazakhstan | PARTIAL |
| 116 | Kenya | 1963 | OPEN | $135.94B | $2,363 | 4.07 | 37.52 | Lower middle income | Central Bank of Kenya | PARTIAL |
| 117 | Kiribati | 1979 | OPEN | $349.23M | $2,559 | 2.46 | 102.11 | Lower middle income | Banking system under Australian dollar; Reserve Bank of Australia framework | PARTIAL |
| 118 | North Korea | 1948 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of the Democratic People's Republic of Korea | MISSING |
| 119 | South Korea | 1948 | OPEN | N/A | N/A | N/A | N/A | N/A | Bank of Korea | MISSING |
| 120 | Kuwait | 1961 | OPEN | $157.21B | $32,312 | 2.36 | 93.99 | High income | Central Bank of Kuwait | PARTIAL |
| 121 | Kyrgyzstan | 1991 | OPEN | N/A | N/A | N/A | N/A | N/A | National Bank of the Kyrgyz Republic | MISSING |
| 122 | Lao People's Democratic Republic | N/A / N/A | LAK | N/A | N/A | N/A | N/A | N/A | Bank of the Lao P.D.R. | MISSING |
| 123 | Latvia | 1991 | OPEN | $48.62B | $26,312 | 3.75 | 129.72 | High income | Bank of Latvia / European Central Bank | PARTIAL |
| 124 | Lebanon | 1941 | OPEN | $25.97B | $4,473 | 14.60 | 82.23 | Lower middle income | Banque du Liban | PARTIAL |
| 125 | Lesotho | 1966 | OPEN | $2.57B | $1,089 | 4.27 | 153.98 | Lower middle income | Central Bank of Lesotho | PARTIAL |
| 126 | Liberia | 1847 | OPEN | $5.25B | 915.33 | 8.32 | N/A | Low income | Central Bank of Liberia | PARTIAL |
| 127 | Libya | 1951 | OPEN | $48.10B | $6,449 | 1.84 | 137.66 | Upper middle income | Central Bank of Libya | PARTIAL |
| 128 | Liechtenstein | 1806 | OPEN | $8.91B | $220,167 | N/A | N/A | High income | Financial Market Authority Liechtenstein / Swiss franc banking system | PARTIAL |
| 129 | Lithuania | 1991 | OPEN | $95.21B | $32,959 | 3.79 | 142.11 | High income | Bank of Lithuania / European Central Bank | PARTIAL |
| 130 | Luxembourg | 1867 | OPEN | $101.16B | $147,252 | 2.26 | 349.89 | High income | Central Bank of Luxembourg / European Central Bank | PARTIAL |
| 131 | Macao | N/A / N/A | MOP | N/A | N/A | N/A | N/A | N/A | Monetary Authority of Macao | MISSING |
| 132 | Madagascar | 1960 | OPEN | $19.62B | 599.27 | 8.05 | 54.24 | Low income | Central Bank of Madagascar | PARTIAL |
| 133 | Malawi | 1964 | OPEN | $14.92B | 671.51 | 28.37 | 39.40 | Low income | Reserve Bank of Malawi | PARTIAL |
| 134 | Malaysia | 1957 | OPEN | $472.19B | $13,125 | 1.38 | 136.40 | Upper middle income | Bank Negara Malaysia | PARTIAL |
| 135 | Maldives | 1965 | OPEN | $7.74B | $14,615 | 4.01 | 156.47 | Upper middle income | Maldives Monetary Authority | PARTIAL |
| 136 | Mali | 1960 | OPEN | $30.07B | $1,193 | 3.28 | 48.77 | Low income | Central Bank of West African States (BCEAO) | PARTIAL |
| 137 | Malta | 1964 | OPEN | $27.77B | $47,907 | 2.36 | 219.01 | High income | Central Bank of Malta / European Central Bank | PARTIAL |
| 138 | Marshall Islands | 1990 | OPEN | $308.00M | $8,489 | N/A | 115.91 | Upper middle income | U.S. Federal Reserve System / Marshall Islands banking institutions | PARTIAL |
| 139 | Martinique | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Martinique (BIS Central Bank Hub reference) | MISSING |
| 140 | Mauritania | 1960 | OPEN | $11.68B | $2,198 | 1.55 | 94.02 | Lower middle income | Central Bank of Mauritania | PARTIAL |
| 141 | Mauritius | 1968 | OPEN | $16.16B | $12,991 | 3.67 | 139.94 | Upper middle income | Bank of Mauritius | PARTIAL |
| 142 | Mayotte | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | National banking and monetary authority — Mayotte (BIS Central Bank Hub reference) | MISSING |
| 143 | Mexico | 1810 | OPEN | $1832.64B | $13,889 | 3.81 | 79.92 | Upper middle income | Bank of Mexico | PARTIAL |
| 144 | Micronesia | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | U.S. Federal Reserve System / Federated States of Micronesia banking institutions | MISSING |
| 145 | Moldova | 1991 | OPEN | $20.35B | $8,622 | 7.76 | 92.67 | Upper middle income | National Bank of Moldova | PARTIAL |
| 146 | Monaco | 1861 | OPEN | $11.13B | $288,002 | N/A | N/A | High income | European Central Bank / French monetary and banking system | PARTIAL |
| 147 | Mongolia | 1921 | OPEN | $25.37B | $7,108 | 8.61 | 130.71 | Upper middle income | Bank of Mongolia | PARTIAL |
| 148 | Montenegro | 2006 | OPEN | $9.23B | $14,817 | 3.90 | 105.77 | Upper middle income | Central Bank of Montenegro / euro system | PARTIAL |
| 149 | Montserrat | N/A / N/A | XCD | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | MISSING |
| 150 | Morocco | 1956 | OPEN | $182.37B | $4,672 | 0.70 | 93.10 | Lower middle income | Bank Al-Maghrib | PARTIAL |
| 151 | Mozambique | 1975 | OPEN | $22.34B | 626.91 | 4.37 | 97.10 | Low income | Bank of Mozambique | PARTIAL |
| 152 | Myanmar | 1948 | OPEN | $81.67B | $1,489 | 8.83 | N/A | Lower middle income | Central Bank of Myanmar | PARTIAL |
| 153 | Namibia | 1990 | OPEN | $15.08B | $4,876 | 3.51 | 108.58 | Lower middle income | Bank of Namibia | PARTIAL |
| 154 | Nauru | 1968 | OPEN | N/A | N/A | N/A | N/A | N/A | Australian dollar banking system / Reserve Bank of Australia framework | MISSING |
| 155 | Nepal | 1923 | OPEN | $45.49B | $1,536 | 2.65 | 42.55 | Lower middle income | Nepal Rastra Bank | PARTIAL |
| 156 | Netherlands | 1581 | OPEN | $1332.77B | $73,684 | 3.26 | 150.12 | High income | De Nederlandsche Bank / European Central Bank | PARTIAL |
| 157 | New Caledonia | NC / NCL | XPF | $8.55B | $29,213 | 0.58 | 40.37 | High income | Institut d'émission d'outre-mer (IEOM) | PARTIAL |
| 158 | New Zealand | 1907 | OPEN | $264.06B | $49,591 | 2.84 | 50.66 | High income | Reserve Bank of New Zealand | PARTIAL |
| 159 | Nicaragua | 1838 | OPEN | $22.24B | $3,173 | 2.08 | 96.45 | Lower middle income | Central Bank of Nicaragua | PARTIAL |
| 160 | Niger | 1960 | OPEN | $21.65B | 775.35 | -4.45 | 33.91 | Low income | Central Bank of West African States (BCEAO) | PARTIAL |
| 161 | Nigeria | 1960 | OPEN | $290.79B | $1,224 | 23.01 | 26.17 | Lower middle income | Central Bank of Nigeria | PARTIAL |
| 162 | Niue | N/A / N/A | NZD | N/A | N/A | N/A | N/A | N/A | Reserve Bank of New Zealand / Niue government banking system | MISSING |
| 163 | Norfolk Island | N/A / N/A | AUD | N/A | N/A | N/A | N/A | N/A | Reserve Bank of Australia / Australian banking system | MISSING |
| 164 | North Macedonia | 1991 | OPEN | $19.10B | $10,490 | 4.07 | 133.04 | Upper middle income | National Bank of the Republic of North Macedonia | PARTIAL |
| 165 | Northern Mariana Islands | MP / MNP | USD | $1.10B | $23,786 | N/A | 93.16 | High income | Federal Reserve System / U.S. banking system | PARTIAL |
| 166 | Norway | 1905 | OPEN | $530.76B | $94,594 | 3.06 | 79.05 | High income | Norges Bank | PARTIAL |
| 167 | Oman | 1951 | OPEN | $109.60B | $19,947 | 0.97 | 114.85 | High income | Central Bank of Oman | PARTIAL |
| 168 | Pakistan | 1947 | OPEN | $407.31B | $1,596 | 3.55 | 27.19 | Lower middle income | State Bank of Pakistan | PARTIAL |
| 169 | Palau | 1994 | OPEN | $345.00M | $19,532 | -0.42 | 105.81 | High income | U.S. Federal Reserve System / Palau banking institutions | PARTIAL |
| 170 | Palestine | N/A / N/A | ILS / JOD / USD | N/A | N/A | N/A | N/A | N/A | Palestine Monetary Authority | MISSING |
| 171 | Panama | 1903 | OPEN | $90.46B | $19,790 | -0.19 | 83.67 | High income | Superintendency of Banks of Panama / Banco Nacional de Panamá / U.S. dollar system | PARTIAL |
| 172 | Papua New Guinea | 1975 | OPEN | $32.50B | $3,020 | 4.42 | 131.08 | Lower middle income | Bank of Papua New Guinea | PARTIAL |
| 173 | Paraguay | 1811 | OPEN | $49.28B | $7,027 | 4.04 | 76.48 | Upper middle income | Central Bank of Paraguay | PARTIAL |
| 174 | Peru | 1821 | OPEN | $334.85B | $9,684 | 1.53 | 51.82 | Upper middle income | Central Reserve Bank of Peru | PARTIAL |
| 175 | Philippines | 1946 | OPEN | $487.09B | $4,171 | 1.66 | 66.79 | Upper middle income | Bangko Sentral ng Pilipinas | PARTIAL |
| 176 | Pitcairn | N/A / N/A | NZD | N/A | N/A | N/A | N/A | N/A | Reserve Bank of New Zealand / New Zealand dollar system | MISSING |
| 177 | Poland | 1918 | OPEN | $1035.49B | $28,420 | 3.81 | 97.03 | High income | Narodowy Bank Polski | PARTIAL |
| 178 | Portugal | 1143 | OPEN | $346.64B | $32,082 | 2.34 | 86.40 | High income | Banco de Portugal / European Central Bank | PARTIAL |
| 179 | Puerto Rico | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | Federal Reserve System / U.S. banking system | MISSING |
| 180 | Qatar | 1971 | OPEN | $215.56B | $72,525 | 1.27 | 100.21 | High income | Qatar Central Bank | PARTIAL |
| 181 | Réunion | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | MISSING |
| 182 | Romania | 1878 | OPEN | $428.68B | $22,538 | 7.19 | 76.21 | High income | National Bank of Romania | PARTIAL |
| 183 | Russian Federation | RU / RUS | RUB | $2561.31B | $17,547 | 8.72 | 33.71 | High income | Central Bank of the Russian Federation | PARTIAL |
| 184 | Rwanda | 1962 | OPEN | $16.37B | $1,124 | 5.91 | 56.67 | Low income | National Bank of Rwanda | PARTIAL |
| 185 | Saint Barthélemy | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | MISSING |
| 186 | Saint Helena, Ascension and Tristan da Cunha | N/A / N/A | SHP | N/A | N/A | N/A | N/A | N/A | Bank of St Helena / sterling banking system | MISSING |
| 187 | Saint Kitts and Nevis | 1983 | OPEN | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | MISSING |
| 188 | Saint Lucia | 1979 | OPEN | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | MISSING |
| 189 | Saint Martin | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | MISSING |
| 190 | Saint Pierre and Miquelon | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Banque de France / European Central Bank | MISSING |
| 191 | Saint Vincent and the Grenadines | 1979 | OPEN | N/A | N/A | N/A | N/A | N/A | Eastern Caribbean Central Bank (ECCB) | MISSING |
| 192 | Samoa | 1962 | OPEN | $1.29B | $5,873 | 2.21 | 70.63 | Upper middle income | Central Bank of Samoa | PARTIAL |
| 193 | San Marino | 885 | OPEN | $2.03B | $59,871 | 2.30 | 340.98 | High income | Central Bank of the Republic of San Marino / euro system | PARTIAL |
| 194 | Sao Tome and Principe | 1975 | OPEN | $981.29M | $4,084 | 11.05 | 46.57 | Lower middle income | Central Bank of São Tomé and Príncipe | PARTIAL |
| 195 | Saudi Arabia | 1932 | OPEN | $1276.94B | $34,537 | 2.08 | 57.92 | High income | Saudi Central Bank (SAMA) | PARTIAL |
| 196 | Senegal | 1960 | OPEN | $37.01B | $1,955 | 1.46 | 71.07 | Lower middle income | Central Bank of West African States (BCEAO) | PARTIAL |
| 197 | Serbia | 2006 | OPEN | $99.95B | $15,262 | 3.89 | 112.92 | Upper middle income | National Bank of Serbia | PARTIAL |
| 198 | Seychelles | 1976 | OPEN | $2.39B | $19,449 | 0.30 | 172.29 | High income | Central Bank of Seychelles | PARTIAL |
| 199 | Sierra Leone | 1961 | OPEN | $7.46B | 846.30 | 7.49 | 49.10 | Low income | Bank of Sierra Leone | PARTIAL |
| 200 | Singapore | 1965 | OPEN | $603.87B | $98,814 | 0.90 | 320.31 | High income | Monetary Authority of Singapore | PARTIAL |
| 201 | Sint Maarten | N/A / N/A | XCG | N/A | N/A | N/A | N/A | N/A | Central Bank of Curaçao and Sint Maarten | MISSING |
| 202 | Slovakia | 1993 | OPEN | N/A | N/A | N/A | N/A | N/A | National Bank of Slovakia / European Central Bank | MISSING |
| 203 | Slovenia | 1991 | OPEN | $79.65B | $37,376 | 2.37 | 151.92 | High income | Bank of Slovenia / European Central Bank | PARTIAL |
| 204 | Solomon Islands | 1978 | OPEN | $1.75B | $2,086 | 3.36 | 111.35 | Lower middle income | Central Bank of Solomon Islands | PARTIAL |
| 205 | Somalia | 1960 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of Somalia | MISSING |
| 206 | South Africa | 1910 | OPEN | $427.18B | $6,598 | 3.21 | 60.91 | Upper middle income | South African Reserve Bank | PARTIAL |
| 207 | South Georgia and the South Sandwich Islands | N/A / N/A | GBP | N/A | N/A | N/A | N/A | N/A | Bank of England / sterling banking system | MISSING |
| 208 | South Sudan | 2011 | OPEN | $12.00B | $1,080 | 91.44 | 65.55 | Low income | Bank of South Sudan | PARTIAL |
| 209 | Spain | 1492 | OPEN | $1906.45B | $38,627 | 2.70 | 69.45 | High income | Banco de España / European Central Bank | PARTIAL |
| 210 | Sri Lanka | LK / LKA | LKR | $108.83B | $5,002 | -4.76 | 41.81 | Upper middle income | Central Bank of Sri Lanka | PARTIAL |
| 211 | Sudan | 1956 | OPEN | $60.16B | $1,165 | 138.81 | 2.38 | Low income | Central Bank of Sudan | PARTIAL |
| 212 | Suriname | 1975 | OPEN | $4.52B | $7,070 | 9.21 | 90.96 | Upper middle income | Central Bank of Suriname | PARTIAL |
| 213 | Svalbard and Jan Mayen | N/A / N/A | NOK | N/A | N/A | N/A | N/A | N/A | Norges Bank / Norwegian banking system | MISSING |
| 214 | Sweden | 836 | OPEN | $669.00B | $63,133 | 0.68 | 104.34 | High income | Sveriges Riksbank | PARTIAL |
| 215 | Switzerland | 1499 | OPEN | $1043.53B | $114,769 | 0.15 | 147.16 | High income | Swiss National Bank | PARTIAL |
| 216 | Syrian Arab Republic | 1941 | OPEN | $23.74B | $1,057 | 13.42 | 35.44 | Low income | Central Bank of Syria | PARTIAL |
| 217 | Taiwan | N/A / N/A | TWD | N/A | N/A | N/A | N/A | N/A | Central Bank of the Republic of China (Taiwan) | MISSING |
| 218 | Tajikistan | 1991 | OPEN | $17.66B | $1,637 | 6.00 | 59.09 | Lower middle income | National Bank of Tajikistan | PARTIAL |
| 219 | Tanzania | 1961 | OPEN | $90.14B | $1,319 | 3.33 | 38.97 | Lower middle income | Bank of Tanzania | PARTIAL |
| 220 | Thailand | 1350 | OPEN | $577.01B | $8,057 | -0.13 | 138.41 | Upper middle income | Bank of Thailand | PARTIAL |
| 221 | Timor-Leste | TL / TLS | USD | $1.90B | $1,341 | 0.43 | 95.45 | Lower middle income | Banco Central de Timor-Leste | PARTIAL |
| 222 | Togo | 1960 | OPEN | $11.89B | $1,384 | 0.43 | 59.93 | Lower middle income | Central Bank of West African States (BCEAO) | PARTIAL |
| 223 | Tokelau | N/A / N/A | NZD | N/A | N/A | N/A | N/A | N/A | Reserve Bank of New Zealand / New Zealand dollar system | MISSING |
| 224 | Tonga | 1970 | OPEN | $679.22M | $6,547 | 5.59 | 77.58 | Upper middle income | National Reserve Bank of Tonga | PARTIAL |
| 225 | Trinidad and Tobago | 1962 | OPEN | $25.94B | $18,967 | 0.99 | N/A | High income | Central Bank of Trinidad and Tobago | PARTIAL |
| 226 | Tunisia | 1956 | OPEN | $57.50B | $4,657 | 5.15 | 101.45 | Lower middle income | Central Bank of Tunisia | PARTIAL |
| 227 | Türkiye | 1923 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of the Republic of Türkiye | MISSING |
| 228 | Turkmenistan | 1991 | OPEN | $49.83B | $6,540 | N/A | 28.17 | Upper middle income | Central Bank of Turkmenistan | PARTIAL |
| 229 | Turks and Caicos Islands | TC / TCA | USD | $1.75B | $37,507 | N/A | N/A | High income | Turks and Caicos Islands Financial Services Commission / U.S. dollar system | PARTIAL |
| 230 | Tuvalu | 1978 | OPEN | $57.35M | $6,041 | 0.50 | N/A | Upper middle income | Australian dollar banking system / Reserve Bank of Australia framework | PARTIAL |
| 231 | Uganda | 1962 | OPEN | $61.99B | $1,206 | 3.58 | 50.20 | Low income | Bank of Uganda | PARTIAL |
| 232 | Ukraine | 1991 | OPEN | $214.23B | $5,866 | 12.73 | 77.18 | Upper middle income | National Bank of Ukraine | PARTIAL |
| 233 | United Arab Emirates | 1971 | OPEN | $552.32B | $50,274 | 1.25 | 199.04 | High income | Central Bank of the United Arab Emirates | PARTIAL |
| 234 | United Kingdom | 1066 | OPEN | $4002.59B | $57,602 | 3.88 | 62.46 | High income | Bank of England | PARTIAL |
| 235 | United States | 1776 | OPEN | $30769.70B | $90,027 | 2.95 | 25.02 | High income | Board of Governors of the Federal Reserve System | PARTIAL |
| 236 | United States Minor Outlying Islands | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | Federal Reserve System / U.S. banking system | MISSING |
| 237 | Uruguay | 1828 | OPEN | $85.35B | $25,216 | 4.65 | 49.72 | High income | Central Bank of Uruguay | PARTIAL |
| 238 | Uzbekistan | 1991 | OPEN | $147.04B | $3,968 | 8.80 | 57.40 | Lower middle income | Central Bank of the Republic of Uzbekistan | PARTIAL |
| 239 | Vanuatu | 1980 | OPEN | $1.35B | $4,039 | 0.66 | 73.38 | Lower middle income | Reserve Bank of Vanuatu | PARTIAL |
| 240 | Venezuela | 1811 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of Venezuela | MISSING |
| 241 | Vietnam | 1945 | OPEN | $514.70B | $5,066 | 3.31 | 190.34 | Upper middle income | State Bank of Viet Nam | PARTIAL |
| 242 | Virgin Islands, British | N/A / N/A | USD | N/A | N/A | N/A | N/A | N/A | British Virgin Islands Financial Services Commission / U.S. dollar system | MISSING |
| 243 | Virgin Islands, U.S. | VI / VIR | USD | $4.67B | $44,321 | N/A | 205.63 | High income | Federal Reserve System / U.S. banking system | PARTIAL |
| 244 | Wallis and Futuna | N/A / N/A | XPF | N/A | N/A | N/A | N/A | N/A | Institut d'émission d'outre-mer (IEOM) | MISSING |
| 245 | Western Sahara | N/A / N/A | MAD | N/A | N/A | N/A | N/A | N/A | Bank Al-Maghrib / Moroccan banking system | MISSING |
| 246 | Yemen | 1918 | OPEN | N/A | N/A | N/A | N/A | N/A | Central Bank of Yemen | MISSING |
| 247 | Zambia | 1964 | OPEN | $28.88B | $1,318 | 13.91 | 62.53 | Lower middle income | Bank of Zambia | PARTIAL |
| 248 | Zimbabwe | 1980 | OPEN | $51.22B | $3,021 | 104.71 | 41.50 | Lower middle income | Reserve Bank of Zimbabwe | PARTIAL |
| 249 | Åland Islands | N/A / N/A | EUR | N/A | N/A | N/A | N/A | N/A | Bank of Finland / European Central Bank | MISSING |
| 250 | Roman Republic | -509 | -27 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 251 | Roman Empire | -27 | 1453 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 252 | Byzantine Empire | 395 | 1453 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 253 | Holy Roman Empire | 800 | 1806 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 254 | Western Roman Empire | 395 | 476 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 255 | Eastern Roman Empire | 395 | 1453 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 256 | Ancient Egypt | -3100 | -30 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 257 | Kingdom of Kush | -1070 | 350 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 258 | Carthaginian Empire | -814 | -146 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 259 | Kingdom of Aksum | 100 | 960 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 260 | Mali Empire | 1235 | 1645 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 261 | Songhai Empire | 1464 | 1591 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 262 | Ghana Empire | 300 | 1200 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 263 | Kanem-Bornu Empire | 700 | 1893 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 264 | Benin Kingdom | 1180 | 1897 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 265 | Oyo Empire | 1300 | 1896 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 266 | Ashanti Empire | 1701 | 1957 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 267 | Ethiopian Empire | 1270 | 1974 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 268 | Kingdom of Kongo | 1390 | 1914 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 269 | Mutapa Empire | 1430 | 1760 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 270 | Maravi Kingdom | 1480 | 1891 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 271 | Zulu Kingdom | 1816 | 1897 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 272 | Sokoto Caliphate | 1804 | 1903 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 273 | Almohad Caliphate | 1121 | 1269 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 274 | Almoravid Empire | 1040 | 1147 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 275 | Fatimid Caliphate | 909 | 1171 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 276 | Mamluk Sultanate | 1250 | 1517 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 277 | Abbasid Caliphate | 750 | 1258 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 278 | Umayyad Caliphate | 661 | 750 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 279 | Ottoman Empire | 1299 | 1922 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 280 | Safavid Iran | 1501 | 1736 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 281 | Qajar Iran | 1789 | 1925 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 282 | Mughal Empire | 1526 | 1857 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 283 | Maurya Empire | -322 | -185 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 284 | Gupta Empire | 320 | 550 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 285 | Chola Empire | 300 | 1279 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 286 | Pala Empire | 750 | 1161 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 287 | Delhi Sultanate | 1206 | 1526 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 288 | Maratha Empire | 1674 | 1818 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 289 | Vijayanagara Empire | 1336 | 1646 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 290 | Kushan Empire | 30 | 375 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 291 | Parthian Empire | -247 | 224 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 292 | Sasanian Empire | 224 | 651 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 293 | Achaemenid Empire | -550 | -330 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 294 | Median Empire | -678 | -549 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 295 | Hittite Empire | -1600 | -1178 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 296 | Phoenicia | -1500 | -300 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 297 | Kingdom of Lydia | -1200 | -546 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 298 | Seleucid Empire | -312 | -63 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 299 | Ptolemaic Kingdom | -305 | -30 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 300 | Ancient Greece | -800 | -146 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 301 | Macedonian Empire | -359 | -323 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 302 | Spartan State | -900 | -192 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 303 | Athens | -508 | -322 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 304 | Carthage | -814 | -146 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 305 | Han Dynasty | -202 | 220 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 306 | Qin Dynasty | -221 | -206 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 307 | Tang Dynasty | 618 | 907 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 308 | Song Dynasty | 960 | 1279 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 309 | Yuan Dynasty | 1271 | 1368 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 310 | Ming Dynasty | 1368 | 1644 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 311 | Qing Dynasty | 1636 | 1912 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 312 | Three Kingdoms China | 220 | 280 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 313 | Korea under Joseon | 1392 | 1897 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 314 | Goryeo | 918 | 1392 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 315 | Silla | -57 | 935 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 316 | Baekje | -18 | 660 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 317 | Goguryeo | -37 | 668 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 318 | Ryukyu Kingdom | 1429 | 1879 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 319 | Khmer Empire | 802 | 1431 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 320 | Pagan Kingdom | 849 | 1297 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 321 | Ayutthaya Kingdom | 1351 | 1767 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 322 | Sukhothai Kingdom | 1238 | 1438 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 323 | Majapahit | 1293 | 1527 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 324 | Srivijaya | 650 | 1377 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 325 | Malacca Sultanate | 1400 | 1511 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 326 | Brunei Sultanate | 1368 | 1888 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 327 | Lan Xang | 1353 | 1707 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 328 | Dai Viet | 1054 | 1804 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 329 | Champa | 192 | 1832 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 330 | Nguyen Vietnam | 1802 | 1945 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 331 | Mataram Kingdom | 716 | 1016 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 332 | Hawaiian Kingdom | 1795 | 1893 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 333 | Kingdom of Tahiti | 1788 | 1880 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 334 | Tu'i Tonga Empire | 950 | 1865 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 335 | Samoan Kingdom | 1857 | 1910 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 336 | Maori Confederations | 1500 | 1840 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 337 | Inca Empire | 1438 | 1533 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 338 | Aztec Empire | 1428 | 1521 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 339 | Maya City-States | -2000 | 1697 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 340 | Mississippian Cultures | 800 | 1600 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 341 | Tarascan State | 1300 | 1530 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 342 | Gran Colombia | 1819 | 1831 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 343 | Viceroyalty of New Spain | 1535 | 1821 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 344 | Viceroyalty of Peru | 1542 | 1824 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 345 | Viceroyalty of New Granada | 1717 | 1819 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 346 | Viceroyalty of the Río de la Plata | 1776 | 1814 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 347 | Captaincy General of Guatemala | 1543 | 1821 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 348 | Kingdom of Brazil | 1815 | 1822 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 349 | United Provinces of Central America | 1823 | 1824 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 350 | Federal Republic of Central America | 1824 | 1839 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 351 | United States of Colombia | 1863 | 1886 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 352 | New Granada | 1831 | 1858 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 353 | Kingdom of Prussia | 1525 | 1918 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 354 | German Empire | 1871 | 1918 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 355 | Austro-Hungarian Empire | 1867 | 1918 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 356 | Austrian Empire | 1804 | 1867 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 357 | Kingdom of Hungary | 1000 | 1918 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 358 | Kingdom of Bohemia | 1198 | 1918 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 359 | Polish-Lithuanian Commonwealth | 1569 | 1795 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 360 | Kingdom of Poland | 1025 | 1795 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 361 | Grand Duchy of Lithuania | 1236 | 1795 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 362 | Russian Empire | 1721 | 1917 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 363 | Soviet Union | 1922 | 1991 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 364 | Yugoslavia | 1918 | 2003 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 365 | Czechoslovakia | 1918 | 1993 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 366 | East Germany | 1949 | 1990 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 367 | West Germany | 1949 | 1990 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 368 | Kingdom of Italy | 1861 | 1946 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 369 | Kingdom of Sardinia | 1720 | 1861 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 370 | Papal States | 756 | 1870 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 371 | Kingdom of the Two Sicilies | 1816 | 1861 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 372 | French Empire | 1804 | 1815 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 373 | Kingdom of France | 843 | 1792 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 374 | Spanish Empire | 1492 | 1976 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 375 | Kingdom of Spain | 1479 | 1931 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 376 | Portuguese Empire | 1415 | 1999 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 377 | Dutch Republic | 1581 | 1795 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 378 | British Empire | 1707 | 1997 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 379 | Irish Free State | 1922 | 1937 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 380 | United Arab Republic | 1958 | 1961 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 381 | United Arab Federation | 1958 | 1961 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 382 | North Yemen | 1918 | 1990 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 383 | South Yemen | 1967 | 1990 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 384 | Democratic Kampuchea | 1975 | 1979 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 385 | South Vietnam | 1954 | 1975 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 386 | North Vietnam | 1945 | 1976 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 387 | Republic of Vietnam | 1955 | 1975 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 388 | Kingdom of Afghanistan | 1926 | 1973 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 389 | Tibetan Government | 1912 | 1951 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 390 | Manchu-led Qing successor states | 1932 | 1945 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |
| 391 | Persian Empire | -550 | 1935 | N/A | N/A | N/A | N/A | N/A | Historical jurisdiction — no current banking authority | MISSING |

## 391-Country National Economic Table

This generated table is an evidence-backed data pull. `N/A` means the source did not provide a responsible value; it is not a zero. Banking authorities remain unpopulated unless independently sourced.

| ID | Country/Jurisdiction | ISO | Currency | GDP | GDP/Capita | Inflation | Trade/GDP | World Bank Income | Banking | Status |
|---|---|---|---|---:|---:|---:|---:|---|---|---|
| 001 | Afghanistan | AF / AFG | N/A | $17.78B | 416.87 | -6.60 | 83.76 | Low income | PENDING | PARTIAL |
| 002 | Albania | AL / ALB | N/A | $30.54B | $12,998 | 2.15 | 79.53 | Upper middle income | PENDING | PARTIAL |
| 003 | Algeria | DZ / DZA | N/A | $287.03B | $6,051 | 1.42 | 38.98 | Upper middle income | PENDING | PARTIAL |
| 004 | American Samoa | AS / ASM | N/A | $871.00M | $18,017 | N/A | 124.68 | High income | PENDING | PARTIAL |
| 005 | Andorra | AD / AND | N/A | $4.50B | $54,292 | N/A | N/A | High income | PENDING | PARTIAL |
| 006 | Angola | AO / AGO | N/A | $122.17B | $3,129 | 20.16 | 34.65 | Lower middle income | PENDING | PARTIAL |
| 007 | Anguilla | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 008 | Antarctica | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 009 | Antigua and Barbuda | AG / ATG | N/A | $2.34B | $24,819 | 1.37 | 118.30 | High income | PENDING | PARTIAL |
| 010 | Argentina | AR / ARG | N/A | $683.10B | $14,898 | 219.88 | 30.36 | Upper middle income | PENDING | PARTIAL |
| 011 | Armenia | AM / ARM | N/A | $29.24B | $9,474 | 3.31 | 101.71 | Upper middle income | PENDING | PARTIAL |
| 012 | Aruba | AW / ABW | N/A | $4.17B | $38,591 | 4.26 | 164.14 | High income | PENDING | PARTIAL |
| 013 | Australia | AU / AUS | N/A | $1798.52B | $65,130 | 2.87 | 45.90 | High income | PENDING | PARTIAL |
| 014 | Austria | AT / AUT | N/A | $579.47B | $62,930 | 3.53 | 107.02 | High income | PENDING | PARTIAL |
| 015 | Azerbaijan | AZ / AZE | N/A | $75.94B | $7,411 | 5.62 | 79.83 | Upper middle income | PENDING | PARTIAL |
| 016 | Bahamas | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 017 | Bahrain | BH / BHR | N/A | $48.97B | $30,597 | -0.14 | 157.48 | High income | PENDING | PARTIAL |
| 018 | Bangladesh | BD / BGD | N/A | $456.32B | $2,597 | 8.77 | 27.95 | Lower middle income | PENDING | PARTIAL |
| 019 | Barbados | BB / BRB | N/A | $8.02B | $28,365 | 0.85 | N/A | High income | PENDING | PARTIAL |
| 020 | Belarus | BY / BLR | N/A | $93.40B | $10,279 | 6.60 | 112.11 | Upper middle income | PENDING | PARTIAL |
| 021 | Belgium | BE / BEL | N/A | $725.47B | $60,750 | 2.47 | 152.92 | High income | PENDING | PARTIAL |
| 022 | Belize | BZ / BLZ | N/A | $3.33B | $7,865 | 1.06 | 108.97 | Upper middle income | PENDING | PARTIAL |
| 023 | Benin | BJ / BEN | N/A | $24.57B | $1,658 | 1.10 | 37.12 | Lower middle income | PENDING | PARTIAL |
| 024 | Bermuda | BM / BMU | N/A | $9.19B | $142,250 | N/A | 80.21 | High income | PENDING | PARTIAL |
| 025 | Bhutan | BT / BTN | N/A | $3.58B | $4,493 | 3.56 | 79.36 | Lower middle income | PENDING | PARTIAL |
| 026 | Bolivia | BO / BOL | N/A | $64.77B | $5,148 | 19.52 | 46.98 | Lower middle income | PENDING | PARTIAL |
| 027 | Bonaire, Sint Eustatius and Saba | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 028 | Bosnia and Herzegovina | BA / BIH | N/A | $32.60B | $10,382 | 6.11 | 95.12 | Upper middle income | PENDING | PARTIAL |
| 029 | Botswana | BW / BWA | N/A | $19.93B | $7,778 | 2.66 | 69.54 | Upper middle income | PENDING | PARTIAL |
| 030 | Bouvet Island | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 031 | Brazil | BR / BRA | N/A | $2279.92B | $10,713 | 5.02 | 35.29 | Upper middle income | PENDING | PARTIAL |
| 032 | British Indian Ocean Territory | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 033 | Brunei | BN / BRN | N/A | $15.03B | $32,235 | -0.30 | 123.61 | High income | PENDING | PARTIAL |
| 034 | Bulgaria | BG / BGR | N/A | $130.78B | $20,328 | 4.58 | 102.51 | High income | PENDING | PARTIAL |
| 035 | Burkina Faso | BF / BFA | N/A | $27.63B | $1,148 | -0.59 | 65.00 | Low income | PENDING | PARTIAL |
| 036 | Burundi | BI / BDI | N/A | $3.36B | 233.82 | 34.13 | N/A | Low income | PENDING | PARTIAL |
| 037 | Cabo Verde | CV / CPV | N/A | $3.06B | $5,796 | 2.34 | 95.33 | Upper middle income | PENDING | PARTIAL |
| 038 | Cambodia | KH / KHM | N/A | $51.27B | $2,872 | 2.35 | 136.09 | Lower middle income | PENDING | PARTIAL |
| 039 | Cameroon | CM / CMR | N/A | $58.93B | $1,972 | 3.40 | 33.38 | Lower middle income | PENDING | PARTIAL |
| 040 | Canada | CA / CAN | N/A | $2319.90B | $55,698 | 2.07 | 63.51 | High income | PENDING | PARTIAL |
| 041 | Cayman Islands | KY / CYM | N/A | $7.77B | $104,293 | -0.63 | 105.47 | High income | PENDING | PARTIAL |
| 042 | Central African Republic | CF / CAF | N/A | $3.07B | 556.13 | 1.00 | 52.91 | Low income | PENDING | PARTIAL |
| 043 | Chad | TD / TCD | N/A | $21.47B | $1,022 | -3.91 | 46.13 | Low income | PENDING | PARTIAL |
| 044 | Chile | CL / CHL | N/A | $357.37B | $17,995 | 4.21 | 64.98 | High income | PENDING | PARTIAL |
| 045 | China | CN / CHN | N/A | $19498.04B | $13,862 | 0.06 | 37.96 | Upper middle income | PENDING | PARTIAL |
| 046 | Christmas Island | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 047 | Cocos (Keeling) Islands | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 048 | Colombia | CO / COL | N/A | $457.41B | $8,562 | 5.14 | 35.37 | Upper middle income | PENDING | PARTIAL |
| 049 | Comoros | KM / COM | N/A | $1.81B | $2,056 | 3.25 | 40.89 | Lower middle income | PENDING | PARTIAL |
| 050 | Congo | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 051 | Cook Islands | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 052 | Costa Rica | CR / CRI | N/A | $102.90B | $19,970 | -0.07 | 71.49 | High income | PENDING | PARTIAL |
| 053 | Côte d'Ivoire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 054 | Croatia | HR / HRV | N/A | $105.06B | $27,104 | 3.69 | 100.99 | High income | PENDING | PARTIAL |
| 055 | Cuba | CU / CUB | N/A | $107.35B | $9,605 | N/A | 124.86 | Upper middle income | PENDING | PARTIAL |
| 056 | Curaçao | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 057 | Cyprus | CY / CYP | N/A | $41.23B | $41,783 | 0.13 | 189.50 | High income | PENDING | PARTIAL |
| 058 | Czechia | CZ / CZE | N/A | $391.03B | $35,917 | 2.46 | 128.04 | High income | PENDING | PARTIAL |
| 059 | Democratic Republic of the Congo | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 060 | Denmark | DK / DNK | N/A | $462.53B | $76,970 | 1.89 | 129.22 | High income | PENDING | PARTIAL |
| 061 | Djibouti | DJ / DJI | N/A | $4.62B | $3,906 | -0.31 | 206.30 | Lower middle income | PENDING | PARTIAL |
| 062 | Dominica | DM / DMA | N/A | $723.86M | $10,989 | 2.53 | N/A | Upper middle income | PENDING | PARTIAL |
| 063 | Dominican Republic | DO / DOM | N/A | $127.41B | $11,059 | 3.87 | 51.49 | Upper middle income | PENDING | PARTIAL |
| 064 | Ecuador | EC / ECU | N/A | $130.32B | $7,125 | 0.71 | 59.36 | Upper middle income | PENDING | PARTIAL |
| 065 | Egypt | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 066 | El Salvador | SV / SLV | N/A | $36.71B | $5,767 | 0.26 | 88.16 | Upper middle income | PENDING | PARTIAL |
| 067 | Equatorial Guinea | GQ / GNQ | N/A | $12.82B | $6,615 | 2.92 | 74.93 | Upper middle income | PENDING | PARTIAL |
| 068 | Eritrea | ER / ERI | N/A | $2.07B | 688.68 | N/A | 47.39 | Low income | PENDING | PARTIAL |
| 069 | Estonia | EE / EST | N/A | $47.03B | $34,418 | 4.83 | 155.76 | High income | PENDING | PARTIAL |
| 070 | Eswatini | SZ / SWZ | N/A | $5.16B | $4,108 | 2.60 | 107.48 | Lower middle income | PENDING | PARTIAL |
| 071 | Ethiopia | ET / ETH | N/A | $126.36B | 932.73 | 13.23 | 33.46 | Low income | PENDING | PARTIAL |
| 072 | Falkland Islands | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 073 | Faroe Islands | FO / FRO | N/A | $4.05B | $74,175 | N/A | 105.75 | High income | PENDING | PARTIAL |
| 074 | Fiji | FJ / FJI | N/A | $6.20B | $6,642 | -1.38 | 118.96 | Upper middle income | PENDING | PARTIAL |
| 075 | Finland | FI / FIN | N/A | $317.04B | $56,149 | 0.34 | 83.40 | High income | PENDING | PARTIAL |
| 076 | France | FR / FRA | N/A | $3366.32B | $48,986 | 0.94 | 67.19 | High income | PENDING | PARTIAL |
| 077 | French Guiana | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 078 | French Polynesia | PF / PYF | N/A | $6.32B | $22,440 | N/A | 64.47 | High income | PENDING | PARTIAL |
| 079 | French Southern Territories | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 080 | Gabon | GA / GAB | N/A | $21.43B | $8,263 | 1.77 | 84.89 | Upper middle income | PENDING | PARTIAL |
| 081 | Gambia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 082 | Georgia | GE / GEO | N/A | $38.14B | $9,692 | 3.87 | 100.63 | Upper middle income | PENDING | PARTIAL |
| 083 | Germany | DE / DEU | N/A | $5050.92B | $60,496 | 2.17 | 78.51 | High income | PENDING | PARTIAL |
| 084 | Ghana | GH / GHA | N/A | $114.21B | $3,257 | 14.20 | 68.97 | Lower middle income | PENDING | PARTIAL |
| 085 | Gibraltar | GI / GIB | N/A | N/A | N/A | N/A | N/A | High income | PENDING | PARTIAL |
| 086 | Greece | GR / GRC | N/A | $280.64B | $26,948 | 2.48 | 83.45 | High income | PENDING | PARTIAL |
| 087 | Greenland | GL / GRL | N/A | $3.33B | $58,499 | N/A | 91.89 | High income | PENDING | PARTIAL |
| 088 | Grenada | GD / GRD | N/A | $1.42B | $12,107 | 0.61 | N/A | Upper middle income | PENDING | PARTIAL |
| 089 | Guadeloupe | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 090 | Guam | GU / GUM | N/A | $6.91B | $41,833 | N/A | 71.87 | High income | PENDING | PARTIAL |
| 091 | Guatemala | GT / GTM | N/A | $123.31B | $6,598 | 1.49 | 46.53 | Upper middle income | PENDING | PARTIAL |
| 092 | Guernsey | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 093 | Guinea | GN / GIN | N/A | $28.35B | $1,877 | 3.55 | 103.68 | Lower middle income | PENDING | PARTIAL |
| 094 | Guinea-Bissau | GW / GNB | N/A | $2.53B | $1,124 | 0.87 | 40.88 | Low income | PENDING | PARTIAL |
| 095 | Guyana | GY / GUY | N/A | $27.10B | $32,414 | 3.33 | 194.35 | High income | PENDING | PARTIAL |
| 096 | Haiti | HT / HTI | N/A | $32.08B | $2,694 | 28.64 | 15.89 | Lower middle income | PENDING | PARTIAL |
| 097 | Heard Island and McDonald Islands | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 098 | Holy See | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 099 | Honduras | HN / HND | N/A | $39.60B | $3,598 | 4.60 | 91.51 | Lower middle income | PENDING | PARTIAL |
| 100 | Hong Kong | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 101 | Hungary | HU / HUN | N/A | $246.49B | $25,907 | 4.41 | 140.78 | High income | PENDING | PARTIAL |
| 102 | Iceland | IS / ISL | N/A | $38.58B | $98,323 | 4.09 | 82.49 | High income | PENDING | PARTIAL |
| 103 | India | IN / IND | N/A | $3956.07B | $2,702 | 2.40 | 46.27 | Lower middle income | PENDING | PARTIAL |
| 104 | Indonesia | ID / IDN | N/A | $1445.64B | $5,060 | 1.91 | 43.39 | Upper middle income | PENDING | PARTIAL |
| 105 | Iran | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 106 | Iraq | IQ / IRQ | N/A | $254.37B | $5,410 | 0.30 | 77.14 | Upper middle income | PENDING | PARTIAL |
| 107 | Ireland | IE / IRL | N/A | $721.70B | $131,592 | 2.21 | 239.96 | High income | PENDING | PARTIAL |
| 108 | Isle of Man | IM / IMN | N/A | $7.58B | $90,015 | N/A | N/A | High income | PENDING | PARTIAL |
| 109 | Israel | IL / ISR | N/A | $610.78B | $60,337 | 3.04 | 52.99 | High income | PENDING | PARTIAL |
| 110 | Italy | IT / ITA | N/A | $2551.56B | $43,309 | 1.53 | 62.52 | High income | PENDING | PARTIAL |
| 111 | Jamaica | JM / JAM | N/A | $22.70B | $8,003 | 4.00 | N/A | Upper middle income | PENDING | PARTIAL |
| 112 | Japan | JP / JPN | N/A | $4435.16B | $35,951 | 3.17 | 44.85 | High income | PENDING | PARTIAL |
| 113 | Jersey | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 114 | Jordan | JO / JOR | N/A | $61.61B | $5,348 | 1.77 | 145.99 | Upper middle income | PENDING | PARTIAL |
| 115 | Kazakhstan | KZ / KAZ | N/A | $306.24B | $14,692 | 11.39 | 57.16 | Upper middle income | PENDING | PARTIAL |
| 116 | Kenya | KE / KEN | N/A | $135.94B | $2,363 | 4.07 | 37.52 | Lower middle income | PENDING | PARTIAL |
| 117 | Kiribati | KI / KIR | N/A | $349.23M | $2,559 | 2.46 | 102.11 | Lower middle income | PENDING | PARTIAL |
| 118 | North Korea | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 119 | South Korea | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 120 | Kuwait | KW / KWT | N/A | $157.21B | $32,312 | 2.36 | 93.99 | High income | PENDING | PARTIAL |
| 121 | Kyrgyzstan | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 122 | Lao People's Democratic Republic | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 123 | Latvia | LV / LVA | N/A | $48.62B | $26,312 | 3.75 | 129.72 | High income | PENDING | PARTIAL |
| 124 | Lebanon | LB / LBN | N/A | $25.97B | $4,473 | 14.60 | 82.23 | Lower middle income | PENDING | PARTIAL |
| 125 | Lesotho | LS / LSO | N/A | $2.57B | $1,089 | 4.27 | 153.98 | Lower middle income | PENDING | PARTIAL |
| 126 | Liberia | LR / LBR | N/A | $5.25B | 915.33 | 8.32 | N/A | Low income | PENDING | PARTIAL |
| 127 | Libya | LY / LBY | N/A | $48.10B | $6,449 | 1.84 | 137.66 | Upper middle income | PENDING | PARTIAL |
| 128 | Liechtenstein | LI / LIE | N/A | $8.91B | $220,167 | N/A | N/A | High income | PENDING | PARTIAL |
| 129 | Lithuania | LT / LTU | N/A | $95.21B | $32,959 | 3.79 | 142.11 | High income | PENDING | PARTIAL |
| 130 | Luxembourg | LU / LUX | N/A | $101.16B | $147,252 | 2.26 | 349.89 | High income | PENDING | PARTIAL |
| 131 | Macao | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 132 | Madagascar | MG / MDG | N/A | $19.62B | 599.27 | 8.05 | 54.24 | Low income | PENDING | PARTIAL |
| 133 | Malawi | MW / MWI | N/A | $14.92B | 671.51 | 28.37 | 39.40 | Low income | PENDING | PARTIAL |
| 134 | Malaysia | MY / MYS | N/A | $472.19B | $13,125 | 1.38 | 136.40 | Upper middle income | PENDING | PARTIAL |
| 135 | Maldives | MV / MDV | N/A | $7.74B | $14,615 | 4.01 | 156.47 | Upper middle income | PENDING | PARTIAL |
| 136 | Mali | ML / MLI | N/A | $30.07B | $1,193 | 3.28 | 48.77 | Low income | PENDING | PARTIAL |
| 137 | Malta | MT / MLT | N/A | $27.77B | $47,907 | 2.36 | 219.01 | High income | PENDING | PARTIAL |
| 138 | Marshall Islands | MH / MHL | N/A | $308.00M | $8,489 | N/A | 115.91 | Upper middle income | PENDING | PARTIAL |
| 139 | Martinique | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 140 | Mauritania | MR / MRT | N/A | $11.68B | $2,198 | 1.55 | 94.02 | Lower middle income | PENDING | PARTIAL |
| 141 | Mauritius | MU / MUS | N/A | $16.16B | $12,991 | 3.67 | 139.94 | Upper middle income | PENDING | PARTIAL |
| 142 | Mayotte | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 143 | Mexico | MX / MEX | N/A | $1832.64B | $13,889 | 3.81 | 79.92 | Upper middle income | PENDING | PARTIAL |
| 144 | Micronesia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 145 | Moldova | MD / MDA | N/A | $20.35B | $8,622 | 7.76 | 92.67 | Upper middle income | PENDING | PARTIAL |
| 146 | Monaco | MC / MCO | N/A | $11.13B | $288,002 | N/A | N/A | High income | PENDING | PARTIAL |
| 147 | Mongolia | MN / MNG | N/A | $25.37B | $7,108 | 8.61 | 130.71 | Upper middle income | PENDING | PARTIAL |
| 148 | Montenegro | ME / MNE | N/A | $9.23B | $14,817 | 3.90 | 105.77 | Upper middle income | PENDING | PARTIAL |
| 149 | Montserrat | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 150 | Morocco | MA / MAR | N/A | $182.37B | $4,672 | 0.70 | 93.10 | Lower middle income | PENDING | PARTIAL |
| 151 | Mozambique | MZ / MOZ | N/A | $22.34B | 626.91 | 4.37 | 97.10 | Low income | PENDING | PARTIAL |
| 152 | Myanmar | MM / MMR | N/A | $81.67B | $1,489 | 8.83 | N/A | Lower middle income | PENDING | PARTIAL |
| 153 | Namibia | NA / NAM | N/A | $15.08B | $4,876 | 3.51 | 108.58 | Lower middle income | PENDING | PARTIAL |
| 154 | Nauru | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 155 | Nepal | NP / NPL | N/A | $45.49B | $1,536 | 2.65 | 42.55 | Lower middle income | PENDING | PARTIAL |
| 156 | Netherlands | NL / NLD | N/A | $1332.77B | $73,684 | 3.26 | 150.12 | High income | PENDING | PARTIAL |
| 157 | New Caledonia | NC / NCL | N/A | $8.55B | $29,213 | 0.58 | 40.37 | High income | PENDING | PARTIAL |
| 158 | New Zealand | NZ / NZL | N/A | $264.06B | $49,591 | 2.84 | 50.66 | High income | PENDING | PARTIAL |
| 159 | Nicaragua | NI / NIC | N/A | $22.24B | $3,173 | 2.08 | 96.45 | Lower middle income | PENDING | PARTIAL |
| 160 | Niger | NE / NER | N/A | $21.65B | 775.35 | -4.45 | 33.91 | Low income | PENDING | PARTIAL |
| 161 | Nigeria | NG / NGA | N/A | $290.79B | $1,224 | 23.01 | 26.17 | Lower middle income | PENDING | PARTIAL |
| 162 | Niue | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 163 | Norfolk Island | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 164 | North Macedonia | MK / MKD | N/A | $19.10B | $10,490 | 4.07 | 133.04 | Upper middle income | PENDING | PARTIAL |
| 165 | Northern Mariana Islands | MP / MNP | N/A | $1.10B | $23,786 | N/A | 93.16 | High income | PENDING | PARTIAL |
| 166 | Norway | NO / NOR | N/A | $530.76B | $94,594 | 3.06 | 79.05 | High income | PENDING | PARTIAL |
| 167 | Oman | OM / OMN | N/A | $109.60B | $19,947 | 0.97 | 114.85 | High income | PENDING | PARTIAL |
| 168 | Pakistan | PK / PAK | N/A | $407.31B | $1,596 | 3.55 | 27.19 | Lower middle income | PENDING | PARTIAL |
| 169 | Palau | PW / PLW | N/A | $345.00M | $19,532 | -0.42 | 105.81 | High income | PENDING | PARTIAL |
| 170 | Palestine | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 171 | Panama | PA / PAN | N/A | $90.46B | $19,790 | -0.19 | 83.67 | High income | PENDING | PARTIAL |
| 172 | Papua New Guinea | PG / PNG | N/A | $32.50B | $3,020 | 4.42 | 131.08 | Lower middle income | PENDING | PARTIAL |
| 173 | Paraguay | PY / PRY | N/A | $49.28B | $7,027 | 4.04 | 76.48 | Upper middle income | PENDING | PARTIAL |
| 174 | Peru | PE / PER | N/A | $334.85B | $9,684 | 1.53 | 51.82 | Upper middle income | PENDING | PARTIAL |
| 175 | Philippines | PH / PHL | N/A | $487.09B | $4,171 | 1.66 | 66.79 | Upper middle income | PENDING | PARTIAL |
| 176 | Pitcairn | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 177 | Poland | PL / POL | N/A | $1035.49B | $28,420 | 3.81 | 97.03 | High income | PENDING | PARTIAL |
| 178 | Portugal | PT / PRT | N/A | $346.64B | $32,082 | 2.34 | 86.40 | High income | PENDING | PARTIAL |
| 179 | Puerto Rico | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 180 | Qatar | QA / QAT | N/A | $215.56B | $72,525 | 1.27 | 100.21 | High income | PENDING | PARTIAL |
| 181 | Réunion | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 182 | Romania | RO / ROU | N/A | $428.68B | $22,538 | 7.19 | 76.21 | High income | PENDING | PARTIAL |
| 183 | Russian Federation | RU / RUS | N/A | $2561.31B | $17,547 | 8.72 | 33.71 | High income | PENDING | PARTIAL |
| 184 | Rwanda | RW / RWA | N/A | $16.37B | $1,124 | 5.91 | 56.67 | Low income | PENDING | PARTIAL |
| 185 | Saint Barthélemy | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 186 | Saint Helena, Ascension and Tristan da Cunha | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 187 | Saint Kitts and Nevis | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 188 | Saint Lucia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 189 | Saint Martin | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 190 | Saint Pierre and Miquelon | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 191 | Saint Vincent and the Grenadines | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 192 | Samoa | WS / WSM | N/A | $1.29B | $5,873 | 2.21 | 70.63 | Upper middle income | PENDING | PARTIAL |
| 193 | San Marino | SM / SMR | N/A | $2.03B | $59,871 | 2.30 | 340.98 | High income | PENDING | PARTIAL |
| 194 | Sao Tome and Principe | ST / STP | N/A | $981.29M | $4,084 | 11.05 | 46.57 | Lower middle income | PENDING | PARTIAL |
| 195 | Saudi Arabia | SA / SAU | N/A | $1276.94B | $34,537 | 2.08 | 57.92 | High income | PENDING | PARTIAL |
| 196 | Senegal | SN / SEN | N/A | $37.01B | $1,955 | 1.46 | 71.07 | Lower middle income | PENDING | PARTIAL |
| 197 | Serbia | RS / SRB | N/A | $99.95B | $15,262 | 3.89 | 112.92 | Upper middle income | PENDING | PARTIAL |
| 198 | Seychelles | SC / SYC | N/A | $2.39B | $19,449 | 0.30 | 172.29 | High income | PENDING | PARTIAL |
| 199 | Sierra Leone | SL / SLE | N/A | $7.46B | 846.30 | 7.49 | 49.10 | Low income | PENDING | PARTIAL |
| 200 | Singapore | SG / SGP | N/A | $603.87B | $98,814 | 0.90 | 320.31 | High income | PENDING | PARTIAL |
| 201 | Sint Maarten | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 202 | Slovakia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 203 | Slovenia | SI / SVN | N/A | $79.65B | $37,376 | 2.37 | 151.92 | High income | PENDING | PARTIAL |
| 204 | Solomon Islands | SB / SLB | N/A | $1.75B | $2,086 | 3.36 | 111.35 | Lower middle income | PENDING | PARTIAL |
| 205 | Somalia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 206 | South Africa | ZA / ZAF | N/A | $427.18B | $6,598 | 3.21 | 60.91 | Upper middle income | PENDING | PARTIAL |
| 207 | South Georgia and the South Sandwich Islands | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 208 | South Sudan | SS / SSD | N/A | $12.00B | $1,080 | 91.44 | 65.55 | Low income | PENDING | PARTIAL |
| 209 | Spain | ES / ESP | N/A | $1906.45B | $38,627 | 2.70 | 69.45 | High income | PENDING | PARTIAL |
| 210 | Sri Lanka | LK / LKA | N/A | $108.83B | $5,002 | -4.76 | 41.81 | Upper middle income | PENDING | PARTIAL |
| 211 | Sudan | SD / SDN | N/A | $60.16B | $1,165 | 138.81 | 2.38 | Low income | PENDING | PARTIAL |
| 212 | Suriname | SR / SUR | N/A | $4.52B | $7,070 | 9.21 | 90.96 | Upper middle income | PENDING | PARTIAL |
| 213 | Svalbard and Jan Mayen | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 214 | Sweden | SE / SWE | N/A | $669.00B | $63,133 | 0.68 | 104.34 | High income | PENDING | PARTIAL |
| 215 | Switzerland | CH / CHE | N/A | $1043.53B | $114,769 | 0.15 | 147.16 | High income | PENDING | PARTIAL |
| 216 | Syrian Arab Republic | SY / SYR | N/A | $23.74B | $1,057 | 13.42 | 35.44 | Low income | PENDING | PARTIAL |
| 217 | Taiwan | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 218 | Tajikistan | TJ / TJK | N/A | $17.66B | $1,637 | 6.00 | 59.09 | Lower middle income | PENDING | PARTIAL |
| 219 | Tanzania | TZ / TZA | N/A | $90.14B | $1,319 | 3.33 | 38.97 | Lower middle income | PENDING | PARTIAL |
| 220 | Thailand | TH / THA | N/A | $577.01B | $8,057 | -0.13 | 138.41 | Upper middle income | PENDING | PARTIAL |
| 221 | Timor-Leste | TL / TLS | N/A | $1.90B | $1,341 | 0.43 | 95.45 | Lower middle income | PENDING | PARTIAL |
| 222 | Togo | TG / TGO | N/A | $11.89B | $1,384 | 0.43 | 59.93 | Lower middle income | PENDING | PARTIAL |
| 223 | Tokelau | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 224 | Tonga | TO / TON | N/A | $679.22M | $6,547 | 5.59 | 77.58 | Upper middle income | PENDING | PARTIAL |
| 225 | Trinidad and Tobago | TT / TTO | N/A | $25.94B | $18,967 | 0.99 | N/A | High income | PENDING | PARTIAL |
| 226 | Tunisia | TN / TUN | N/A | $57.50B | $4,657 | 5.15 | 101.45 | Lower middle income | PENDING | PARTIAL |
| 227 | Türkiye | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 228 | Turkmenistan | TM / TKM | N/A | $49.83B | $6,540 | N/A | 28.17 | Upper middle income | PENDING | PARTIAL |
| 229 | Turks and Caicos Islands | TC / TCA | N/A | $1.75B | $37,507 | N/A | N/A | High income | PENDING | PARTIAL |
| 230 | Tuvalu | TV / TUV | N/A | $57.35M | $6,041 | 0.50 | N/A | Upper middle income | PENDING | PARTIAL |
| 231 | Uganda | UG / UGA | N/A | $61.99B | $1,206 | 3.58 | 50.20 | Low income | PENDING | PARTIAL |
| 232 | Ukraine | UA / UKR | N/A | $214.23B | $5,866 | 12.73 | 77.18 | Upper middle income | PENDING | PARTIAL |
| 233 | United Arab Emirates | AE / ARE | N/A | $552.32B | $50,274 | 1.25 | 199.04 | High income | PENDING | PARTIAL |
| 234 | United Kingdom | GB / GBR | N/A | $4002.59B | $57,602 | 3.88 | 62.46 | High income | PENDING | PARTIAL |
| 235 | United States | US / USA | N/A | $30769.70B | $90,027 | 2.95 | 25.02 | High income | PENDING | PARTIAL |
| 236 | United States Minor Outlying Islands | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 237 | Uruguay | UY / URY | N/A | $85.35B | $25,216 | 4.65 | 49.72 | High income | PENDING | PARTIAL |
| 238 | Uzbekistan | UZ / UZB | N/A | $147.04B | $3,968 | 8.80 | 57.40 | Lower middle income | PENDING | PARTIAL |
| 239 | Vanuatu | VU / VUT | N/A | $1.35B | $4,039 | 0.66 | 73.38 | Lower middle income | PENDING | PARTIAL |
| 240 | Venezuela | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 241 | Vietnam | VN / VNM | N/A | $514.70B | $5,066 | 3.31 | 190.34 | Upper middle income | PENDING | PARTIAL |
| 242 | Virgin Islands, British | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 243 | Virgin Islands, U.S. | VI / VIR | N/A | $4.67B | $44,321 | N/A | 205.63 | High income | PENDING | PARTIAL |
| 244 | Wallis and Futuna | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 245 | Western Sahara | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 246 | Yemen | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 247 | Zambia | ZM / ZMB | N/A | $28.88B | $1,318 | 13.91 | 62.53 | Lower middle income | PENDING | PARTIAL |
| 248 | Zimbabwe | ZW / ZWE | N/A | $51.22B | $3,021 | 104.71 | 41.50 | Lower middle income | PENDING | PARTIAL |
| 249 | Åland Islands | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 250 | Roman Republic | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 251 | Roman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 252 | Byzantine Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 253 | Holy Roman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 254 | Western Roman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 255 | Eastern Roman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 256 | Ancient Egypt | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 257 | Kingdom of Kush | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 258 | Carthaginian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 259 | Kingdom of Aksum | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 260 | Mali Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 261 | Songhai Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 262 | Ghana Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 263 | Kanem-Bornu Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 264 | Benin Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 265 | Oyo Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 266 | Ashanti Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 267 | Ethiopian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 268 | Kingdom of Kongo | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 269 | Mutapa Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 270 | Maravi Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 271 | Zulu Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 272 | Sokoto Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 273 | Almohad Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 274 | Almoravid Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 275 | Fatimid Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 276 | Mamluk Sultanate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 277 | Abbasid Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 278 | Umayyad Caliphate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 279 | Ottoman Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 280 | Safavid Iran | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 281 | Qajar Iran | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 282 | Mughal Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 283 | Maurya Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 284 | Gupta Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 285 | Chola Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 286 | Pala Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 287 | Delhi Sultanate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 288 | Maratha Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 289 | Vijayanagara Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 290 | Kushan Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 291 | Parthian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 292 | Sasanian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 293 | Achaemenid Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 294 | Median Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 295 | Hittite Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 296 | Phoenicia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 297 | Kingdom of Lydia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 298 | Seleucid Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 299 | Ptolemaic Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 300 | Ancient Greece | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 301 | Macedonian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 302 | Spartan State | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 303 | Athens | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 304 | Carthage | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 305 | Han Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 306 | Qin Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 307 | Tang Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 308 | Song Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 309 | Yuan Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 310 | Ming Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 311 | Qing Dynasty | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 312 | Three Kingdoms China | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 313 | Korea under Joseon | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 314 | Goryeo | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 315 | Silla | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 316 | Baekje | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 317 | Goguryeo | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 318 | Ryukyu Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 319 | Khmer Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 320 | Pagan Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 321 | Ayutthaya Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 322 | Sukhothai Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 323 | Majapahit | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 324 | Srivijaya | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 325 | Malacca Sultanate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 326 | Brunei Sultanate | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 327 | Lan Xang | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 328 | Dai Viet | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 329 | Champa | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 330 | Nguyen Vietnam | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 331 | Mataram Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 332 | Hawaiian Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 333 | Kingdom of Tahiti | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 334 | Tu'i Tonga Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 335 | Samoan Kingdom | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 336 | Maori Confederations | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 337 | Inca Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 338 | Aztec Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 339 | Maya City-States | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 340 | Mississippian Cultures | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 341 | Tarascan State | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 342 | Gran Colombia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 343 | Viceroyalty of New Spain | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 344 | Viceroyalty of Peru | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 345 | Viceroyalty of New Granada | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 346 | Viceroyalty of the Río de la Plata | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 347 | Captaincy General of Guatemala | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 348 | Kingdom of Brazil | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 349 | United Provinces of Central America | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 350 | Federal Republic of Central America | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 351 | United States of Colombia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 352 | New Granada | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 353 | Kingdom of Prussia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 354 | German Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 355 | Austro-Hungarian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 356 | Austrian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 357 | Kingdom of Hungary | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 358 | Kingdom of Bohemia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 359 | Polish-Lithuanian Commonwealth | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 360 | Kingdom of Poland | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 361 | Grand Duchy of Lithuania | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 362 | Russian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 363 | Soviet Union | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 364 | Yugoslavia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 365 | Czechoslovakia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 366 | East Germany | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 367 | West Germany | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 368 | Kingdom of Italy | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 369 | Kingdom of Sardinia | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 370 | Papal States | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 371 | Kingdom of the Two Sicilies | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 372 | French Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 373 | Kingdom of France | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 374 | Spanish Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 375 | Kingdom of Spain | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 376 | Portuguese Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 377 | Dutch Republic | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 378 | British Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 379 | Irish Free State | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 380 | United Arab Republic | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 381 | United Arab Federation | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 382 | North Yemen | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 383 | South Yemen | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 384 | Democratic Kampuchea | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 385 | South Vietnam | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 386 | North Vietnam | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 387 | Republic of Vietnam | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 388 | Kingdom of Afghanistan | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 389 | Tibetan Government | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 390 | Manchu-led Qing successor states | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
| 391 | Persian Empire | N/A / N/A | N/A | N/A | N/A | N/A | N/A | N/A | PENDING | MISSING |
