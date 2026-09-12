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

## 4. Founding / State Formation

A country may have multiple historically important dates. Preserve ancient origins, independence, constitutional founding, modern state formation, restoration, unification, dissolution/succession and transitions where applicable. Do not force a disputed history into one arbitrary date.

    FOUNDING {
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

| ID | Country/Jurisdiction | Founded / State Formation | Economic Influence | Economy | Banking | Corporate Sector | Corporate Struggles | Socialist Years | Status |
|---|---|---|---|---|---|---|---|---|---|
| 001–391 | Canonical registry entries | PENDING | UNASSESSED | PENDING | PENDING | PENDING | PENDING | PENDING | MISSING |

The actual 391 names and identifiers must be imported from SLeeLa's canonical country registry. This document does not invent country assignments.

## 10. Completeness

Maintain independent coverage counts:

    EXPECTED = 391
    IDENTITY_COMPLETE
    FOUNDING_COMPLETE
    BANKING_COMPLETE
    INFLUENCE_COMPLETE
    ECONOMY_COMPLETE
    CORPORATE_COMPLETE
    SOCIALIST_HISTORY_COMPLETE
    VERIFIED_COMPLETE

A country may be complete in banking but partial in historical data. Overall readiness requires all required partitions to be complete for all 391 registry members.

## 11. Provenance and No-Inference Rule

Every assertion should be traceable as:

    COUNTRY → PARTITION → FIELD → VALUE → SOURCE → VERIFICATION DATE

Preferred sources are national governments, central banks, monetary authorities, statistical agencies, regulators, official legislation/archives, recognized international institutions and reputable secondary historical/economic sources.

Distinguish explicitly between UNKNOWN, NONE, NOT_APPLICABLE, DISPUTED, UNASSESSED and VERIFIED. Absence of evidence for a socialist period is not automatically evidence that no socialist period existed.

## 12. Suggested Machine Layout

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

## 13. Implementation Status

**BANKS.md is now the national banking, economic and historical partition contract.** The schema is expanded to include economic influence, founding/state formation, corporate structure and struggles, crises, trade, resources, ownership, labor, institutions and verified socialist-period history.

The next data pass should populate all 391 individual records from SLeeLa's canonical registry and independently verify the values. No placeholder should be promoted to COMPLETE without provenance.
## 391-Country National Economic Table

| ID | Country/Jurisdiction | Founded / State Formation | Economic Influence | Economy | Banking | Corporate Sector | Corporate Struggles | Socialist Years | Status |
|---|---|---|---|---|---|---|---|---|---|
| 001 | Afghanistan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 002 | Albania | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 003 | Algeria | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 004 | American Samoa | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 005 | Andorra | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 006 | Angola | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 007 | Anguilla | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 008 | Antarctica | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 009 | Antigua and Barbuda | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 010 | Argentina | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 011 | Armenia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 012 | Aruba | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 013 | Australia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 014 | Austria | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 015 | Azerbaijan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 016 | Bahamas | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 017 | Bahrain | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 018 | Bangladesh | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 019 | Barbados | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 020 | Belarus | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 021 | Belgium | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 022 | Belize | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 023 | Benin | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 024 | Bermuda | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 025 | Bhutan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 026 | Bolivia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 027 | Bonaire, Sint Eustatius and Saba | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 028 | Bosnia and Herzegovina | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 029 | Botswana | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 030 | Bouvet Island | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 031 | Brazil | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 032 | British Indian Ocean Territory | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 033 | Brunei | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 034 | Bulgaria | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 035 | Burkina Faso | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 036 | Burundi | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 037 | Cabo Verde | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 038 | Cambodia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 039 | Cameroon | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 040 | Canada | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 041 | Cayman Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 042 | Central African Republic | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 043 | Chad | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 044 | Chile | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 045 | China | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 046 | Christmas Island | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 047 | Cocos (Keeling) Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 048 | Colombia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 049 | Comoros | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 050 | Congo | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 051 | Cook Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 052 | Costa Rica | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 053 | Côte d'Ivoire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 054 | Croatia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 055 | Cuba | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 056 | Curaçao | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 057 | Cyprus | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 058 | Czechia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 059 | Democratic Republic of the Congo | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 060 | Denmark | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 061 | Djibouti | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 062 | Dominica | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 063 | Dominican Republic | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 064 | Ecuador | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 065 | Egypt | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 066 | El Salvador | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 067 | Equatorial Guinea | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 068 | Eritrea | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 069 | Estonia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 070 | Eswatini | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 071 | Ethiopia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 072 | Falkland Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 073 | Faroe Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 074 | Fiji | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 075 | Finland | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 076 | France | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 077 | French Guiana | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 078 | French Polynesia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 079 | French Southern Territories | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 080 | Gabon | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 081 | Gambia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 082 | Georgia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 083 | Germany | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 084 | Ghana | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 085 | Gibraltar | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 086 | Greece | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 087 | Greenland | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 088 | Grenada | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 089 | Guadeloupe | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 090 | Guam | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 091 | Guatemala | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 092 | Guernsey | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 093 | Guinea | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 094 | Guinea-Bissau | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 095 | Guyana | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 096 | Haiti | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 097 | Heard Island and McDonald Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 098 | Holy See | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 099 | Honduras | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 100 | Hong Kong | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 101 | Hungary | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 102 | Iceland | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 103 | India | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 104 | Indonesia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 105 | Iran | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 106 | Iraq | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 107 | Ireland | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 108 | Isle of Man | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 109 | Israel | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 110 | Italy | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 111 | Jamaica | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 112 | Japan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 113 | Jersey | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 114 | Jordan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 115 | Kazakhstan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 116 | Kenya | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 117 | Kiribati | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 118 | North Korea | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 119 | South Korea | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 120 | Kuwait | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 121 | Kyrgyzstan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 122 | Lao People's Democratic Republic | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 123 | Latvia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 124 | Lebanon | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 125 | Lesotho | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 126 | Liberia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 127 | Libya | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 128 | Liechtenstein | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 129 | Lithuania | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 130 | Luxembourg | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 131 | Macao | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 132 | Madagascar | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 133 | Malawi | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 134 | Malaysia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 135 | Maldives | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 136 | Mali | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 137 | Malta | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 138 | Marshall Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 139 | Martinique | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 140 | Mauritania | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 141 | Mauritius | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 142 | Mayotte | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 143 | Mexico | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 144 | Micronesia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 145 | Moldova | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 146 | Monaco | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 147 | Mongolia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 148 | Montenegro | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 149 | Montserrat | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 150 | Morocco | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 151 | Mozambique | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 152 | Myanmar | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 153 | Namibia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 154 | Nauru | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 155 | Nepal | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 156 | Netherlands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 157 | New Caledonia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 158 | New Zealand | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 159 | Nicaragua | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 160 | Niger | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 161 | Nigeria | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 162 | Niue | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 163 | Norfolk Island | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 164 | North Macedonia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 165 | Northern Mariana Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 166 | Norway | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 167 | Oman | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 168 | Pakistan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 169 | Palau | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 170 | Palestine | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 171 | Panama | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 172 | Papua New Guinea | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 173 | Paraguay | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 174 | Peru | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 175 | Philippines | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 176 | Pitcairn | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 177 | Poland | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 178 | Portugal | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 179 | Puerto Rico | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 180 | Qatar | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 181 | Réunion | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 182 | Romania | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 183 | Russian Federation | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 184 | Rwanda | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 185 | Saint Barthélemy | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 186 | Saint Helena, Ascension and Tristan da Cunha | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 187 | Saint Kitts and Nevis | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 188 | Saint Lucia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 189 | Saint Martin | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 190 | Saint Pierre and Miquelon | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 191 | Saint Vincent and the Grenadines | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 192 | Samoa | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 193 | San Marino | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 194 | Sao Tome and Principe | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 195 | Saudi Arabia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 196 | Senegal | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 197 | Serbia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 198 | Seychelles | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 199 | Sierra Leone | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 200 | Singapore | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 201 | Sint Maarten | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 202 | Slovakia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 203 | Slovenia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 204 | Solomon Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 205 | Somalia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 206 | South Africa | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 207 | South Georgia and the South Sandwich Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 208 | South Sudan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 209 | Spain | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 210 | Sri Lanka | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 211 | Sudan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 212 | Suriname | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 213 | Svalbard and Jan Mayen | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 214 | Sweden | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 215 | Switzerland | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 216 | Syrian Arab Republic | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 217 | Taiwan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 218 | Tajikistan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 219 | Tanzania | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 220 | Thailand | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 221 | Timor-Leste | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 222 | Togo | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 223 | Tokelau | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 224 | Tonga | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 225 | Trinidad and Tobago | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 226 | Tunisia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 227 | Türkiye | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 228 | Turkmenistan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 229 | Turks and Caicos Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 230 | Tuvalu | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 231 | Uganda | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 232 | Ukraine | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 233 | United Arab Emirates | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 234 | United Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 235 | United States | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 236 | United States Minor Outlying Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 237 | Uruguay | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 238 | Uzbekistan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 239 | Vanuatu | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 240 | Venezuela | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 241 | Vietnam | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 242 | Virgin Islands, British | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 243 | Virgin Islands, U.S. | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 244 | Wallis and Futuna | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 245 | Western Sahara | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 246 | Yemen | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 247 | Zambia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 248 | Zimbabwe | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 249 | Åland Islands | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 250 | Roman Republic | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 251 | Roman Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 252 | Byzantine Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 253 | Holy Roman Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 254 | Western Roman Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 255 | Eastern Roman Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 256 | Ancient Egypt | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 257 | Kingdom of Kush | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 258 | Carthaginian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 259 | Kingdom of Aksum | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 260 | Mali Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 261 | Songhai Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 262 | Ghana Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 263 | Kanem-Bornu Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 264 | Benin Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 265 | Oyo Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 266 | Ashanti Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 267 | Ethiopian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 268 | Kingdom of Kongo | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 269 | Mutapa Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 270 | Maravi Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 271 | Zulu Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 272 | Sokoto Caliphate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 273 | Almohad Caliphate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 274 | Almoravid Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 275 | Fatimid Caliphate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 276 | Mamluk Sultanate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 277 | Abbasid Caliphate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 278 | Umayyad Caliphate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 279 | Ottoman Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 280 | Safavid Iran | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 281 | Qajar Iran | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 282 | Mughal Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 283 | Maurya Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 284 | Gupta Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 285 | Chola Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 286 | Pala Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 287 | Delhi Sultanate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 288 | Maratha Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 289 | Vijayanagara Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 290 | Kushan Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 291 | Parthian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 292 | Sasanian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 293 | Achaemenid Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 294 | Median Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 295 | Hittite Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 296 | Phoenicia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 297 | Kingdom of Lydia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 298 | Seleucid Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 299 | Ptolemaic Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 300 | Ancient Greece | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 301 | Macedonian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 302 | Spartan State | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 303 | Athens | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 304 | Carthage | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 305 | Han Dynasty | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 306 | Qin Dynasty | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 307 | Tang Dynasty | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 308 | Song Dynasty | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 309 | Yuan Dynasty | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 310 | Ming Dynasty | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 311 | Qing Dynasty | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 312 | Three Kingdoms China | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 313 | Korea under Joseon | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 314 | Goryeo | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 315 | Silla | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 316 | Baekje | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 317 | Goguryeo | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 318 | Ryukyu Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 319 | Khmer Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 320 | Pagan Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 321 | Ayutthaya Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 322 | Sukhothai Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 323 | Majapahit | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 324 | Srivijaya | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 325 | Malacca Sultanate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 326 | Brunei Sultanate | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 327 | Lan Xang | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 328 | Dai Viet | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 329 | Champa | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 330 | Nguyen Vietnam | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 331 | Mataram Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 332 | Hawaiian Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 333 | Kingdom of Tahiti | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 334 | Tu'i Tonga Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 335 | Samoan Kingdom | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 336 | Maori Confederations | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 337 | Inca Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 338 | Aztec Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 339 | Maya City-States | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 340 | Mississippian Cultures | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 341 | Tarascan State | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 342 | Gran Colombia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 343 | Viceroyalty of New Spain | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 344 | Viceroyalty of Peru | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 345 | Viceroyalty of New Granada | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 346 | Viceroyalty of the Río de la Plata | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 347 | Captaincy General of Guatemala | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 348 | Kingdom of Brazil | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 349 | United Provinces of Central America | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 350 | Federal Republic of Central America | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 351 | United States of Colombia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 352 | New Granada | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 353 | Kingdom of Prussia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 354 | German Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 355 | Austro-Hungarian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 356 | Austrian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 357 | Kingdom of Hungary | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 358 | Kingdom of Bohemia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 359 | Polish-Lithuanian Commonwealth | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 360 | Kingdom of Poland | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 361 | Grand Duchy of Lithuania | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 362 | Russian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 363 | Soviet Union | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 364 | Yugoslavia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 365 | Czechoslovakia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 366 | East Germany | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 367 | West Germany | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 368 | Kingdom of Italy | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 369 | Kingdom of Sardinia | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 370 | Papal States | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 371 | Kingdom of the Two Sicilies | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 372 | French Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 373 | Kingdom of France | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 374 | Spanish Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 375 | Kingdom of Spain | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 376 | Portuguese Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 377 | Dutch Republic | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 378 | British Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 379 | Irish Free State | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 380 | United Arab Republic | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 381 | United Arab Federation | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 382 | North Yemen | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 383 | South Yemen | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 384 | Democratic Kampuchea | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 385 | South Vietnam | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 386 | North Vietnam | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 387 | Republic of Vietnam | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 388 | Kingdom of Afghanistan | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 389 | Tibetan Government | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 390 | Manchu-led Qing successor states | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
| 391 | Persian Empire | PENDING | UNASSESSED | PENDING | PENDING | PENDING | 0 event(s) | NONE / UNASSESSED | PARTIAL |
