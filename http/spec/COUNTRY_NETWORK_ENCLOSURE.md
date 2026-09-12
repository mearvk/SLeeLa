# COUNTRY_NETWORK_ENCLOSURE.md — Country Network, Services & Geodata Table

**Parent specification:** `http/spec/ENCLOSURE.md`  
**Companion country taxonomy:** `http/spec/BYPASS.md`  
**Scope:** Country, territory, historical-entry, and polity metadata used by the SLeeLa HTTP specification collection.

## 1. Purpose

This companion enclosure extends the country framework with practical Internet, telecommunications, e-mail, geodata, civic-office, water-price, and United States interoperability fields.

The table is intended to answer, for each entry:

- What country or geographic entry is being described?
- What geographic coordinates and boundary/geodata references identify it?
- Who is the current head of state and/or head of government, where applicable?
- What publicly documented Internet service providers (ISPs) operate there?
- What publicly documented e-mail providers or national e-mail services are commonly available there?
- What **country-associated IP address space or network prefixes** are publicly documented by Regional Internet Registries (RIRs), ASN registries, or reputable geolocation databases?
- What is a documented water price, converted to a common **USD per U.S. gallon** representation when a sufficiently comparable source exists?
- What forms of normal Internet interoperability with systems in the United States are publicly documented or technically available?

## 2. Important IP-Safety and Accuracy Rule

The IP field is a **country-level network inventory**, not a host-targeting list. It should use aggregate, publicly registered network information such as RIR allocation blocks, ASN-level prefixes, and country geolocation datasets. It should **not** enumerate private addresses, customer addresses, residential endpoints, exposed services, vulnerable hosts, credentials, or other information useful for targeting individual systems.

An IP block being geolocated to a country does not prove that every address inside the block is physically located there. Cloud providers, CDNs, satellite networks, mobile carriers, multinational operators, VPNs, and anycast infrastructure routinely make geographic attribution imperfect.

Preferred sources include:

1. AFRINIC, APNIC, ARIN, LACNIC, and RIPE NCC allocation/registration data.
2. IANA regional registry information.
3. National Internet registries and telecommunications regulators.
4. Reputable ASN and geolocation databases for corroboration.

Where a prefix cannot be reliably attributed, record **Not reliably geolocated** rather than guessing.

## 3. HTTP / U.S. Interoperability

The United States connection field describes **protocol-level interoperability**, not surveillance or an assertion that a particular private connection has been observed.

The normal interoperability vocabulary is:

| Operation | Meaning in this enclosure |
|---|---|
| `GET` | Retrieval of a resource from a server in the country or from a service serving that country. |
| `POST` | Submission of data to an Internet service, including forms, APIs, and authentication workflows. |
| `PUT` | Replacement or upload semantics where the destination service authorizes them. |
| `PATCH` | Partial resource modification where the API supports it. |
| `DELETE` | Resource deletion where the service authorizes it. |
| `HEAD` | Header-only retrieval used for metadata and cache/availability checks. |
| `OPTIONS` | Discovery of supported HTTP methods and protocol capabilities. |
| `CONNECT` | Proxy/tunnel establishment, normally controlled by the intermediary. |
| `Git smart HTTP` | Git repository transport over HTTPS, normally using HTTP requests including `GET` and `POST`. |
| `SMTP` | E-mail submission/transfer between authorized mail systems. |
| `IMAP` | Mailbox access/synchronization where offered. |
| `POP3` | Mailbox retrieval where offered. |
| `DNS` | Name resolution required by ordinary Internet services. |
| `TLS/HTTPS` | Encrypted HTTP interoperability between U.S. systems and systems serving the country. |

A table entry should say **Supported / Common**, **Supported / Limited**, **Documented but Restricted**, **Not Established**, or **Not Applicable** rather than claiming a specific private U.S.-country connection without evidence.

## 4. Country Data Table

The following is the canonical row format. The 391-entry list in `BYPASS.md` remains the authoritative project key; historical and non-sovereign entries must not be forced into modern sovereign-government fields.

| Entry | Country / Geographic Name | Geodata | Head of State / President | Head of Government / Prime Minister | Major Known ISPs / Telecom Operators | Known E-mail Providers / Services | Country-Associated IP Prefixes / ASNs | Water Price (USD / U.S. gal.) | U.S. Interoperability | Evidence / Date |
|---:|---|---|---|---|---|---|---|---:|---|---|
| 001 | Afghanistan | Use authoritative boundary and centroid data; ISO `AF` | Current officeholder from authoritative government/international source | Current officeholder from authoritative government/international source, if applicable | Document licensed/publicly documented operators; verify with national regulator | Document nationally available or major international providers; do not infer from domain alone | Aggregate RIR/ASN/geolocation attribution only | **N/A until a comparable current tariff is sourced** | HTTPS, DNS, e-mail and ordinary Internet protocols: generally technically interoperable where connectivity and local policy permit | RIR + national regulator + government/international sources |
| 002 | Albania | ISO `AL`; authoritative boundary/centroid | Current officeholder from authoritative source | Current officeholder from authoritative source | Verify current operators with regulator and operator sources | Verify current providers | Aggregate RIR/ASN/geolocation attribution only | **N/A until sourced** | Normal Internet interoperability; specific restrictions should be separately evidenced | RIR + regulator + government/international sources |
| 003 | Algeria | ISO `DZ`; authoritative boundary/centroid | Current officeholder from authoritative source | Current officeholder from authoritative source | Verify current operators with regulator and operator sources | Verify current providers | Aggregate RIR/ASN/geolocation attribution only | **N/A until sourced** | Normal Internet interoperability subject to network/service policy | RIR + regulator + government/international sources |
| 004 | American Samoa | ISO `AS`; U.S.-affiliated territorial geodata | U.S. territorial/government authority as applicable | Territorial executive authority as applicable | Verify local/serving telecom operators | Verify locally available and major international providers | Aggregate RIR/ASN/geolocation attribution only | **N/A until sourced** | U.S. Internet interoperability generally available; verify territorial carrier details | RIR + U.S./territorial sources |
| 005 | Andorra | ISO `AD`; authoritative boundary/centroid | Current officeholder from authoritative source | Current officeholder from authoritative source | Verify current operators | Verify current providers | Aggregate RIR/ASN/geolocation attribution only | **N/A until sourced** | Normal HTTPS/DNS/e-mail interoperability | RIR + regulator + government/international sources |
| 006 | Angola | ISO `AO`; authoritative boundary/centroid | Current officeholder from authoritative source | Current officeholder from authoritative source | Verify current operators | Verify current providers | Aggregate RIR/ASN/geolocation attribution only | **N/A until sourced** | Normal Internet interoperability subject to network/service policy | RIR + regulator + government/international sources |
| 007 | Anguilla | ISO `AI`; territorial geodata | Territorial/U.K.-linked office as applicable | Territorial/U.K.-linked executive authority as applicable | Verify current operators | Verify current providers | Aggregate RIR/ASN/geolocation attribution only | **N/A until sourced** | Normal Internet interoperability; verify carrier-specific details | RIR + regulator + territorial sources |
| 008 | Antarctica | Antarctic geographic boundary; no conventional sovereign-country perimeter | **Not applicable** | **Not applicable** | Research-station/satellite providers rather than ordinary national ISP model | Station/institutional services rather than a national e-mail market | Use station/network ASN information only where publicly documented; no invented country allocation | **N/A** | Research-station and satellite Internet may interoperate with U.S. systems | Antarctic program/institutional sources |
| 009 | Antigua and Barbuda | ISO `AG`; authoritative boundary/centroid | Current officeholder from authoritative source | Current officeholder from authoritative source | Verify current operators | Verify current providers | Aggregate RIR/ASN/geolocation attribution only | **N/A until sourced** | Normal HTTPS/DNS/e-mail interoperability | RIR + regulator + government/international sources |
| 010 | Argentina | ISO `AR`; authoritative boundary/centroid | Current officeholder from authoritative source | Current officeholder from authoritative source | Verify current operators | Verify current providers | Aggregate RIR/ASN/geolocation attribution only | **N/A until sourced** | Normal HTTPS/DNS/e-mail interoperability; Git/HTTP methods technically available to authorized services | RIR + regulator + government/international sources |

> **Expansion rule:** Every remaining numbered entry in `BYPASS.md` should receive the same row. Where evidence is unavailable, use `N/A`, `Not established`, or `Not reliably geolocated`; never manufacture a value merely to complete the table.

## 5. Geodata Standard

The geodata field should use machine-readable, stable identifiers wherever possible:

- ISO 3166-1 alpha-2 / alpha-3 code;
- latitude/longitude centroid where appropriate;
- authoritative boundary dataset or recognized statistical geography;
- capital or principal administrative center when useful;
- special-status notation for territories, dependencies, historical entries, and non-sovereign locations.

A boundary polygon is preferable to a single point when the project needs an **interior perimeter**. The term **interior perimeter** should mean the recognized geographic boundary or statistical polygon, not a scan of individual network endpoints.

## 6. ISP Standard

List operators that are publicly documented as providing fixed, mobile, satellite, wholesale, or other Internet access. Distinguish:

- national incumbent operator;
- major mobile operators;
- fixed broadband providers;
- satellite providers;
- major wholesale/backbone providers where relevant.

Do not imply that an operator owns every IP address geolocated to a country. ASN and prefix ownership should be separately sourced.

## 7. E-mail Company Standard

The e-mail field should include providers that are publicly documented as serving users in the country. It may include:

- national telecom e-mail services;
- major local commercial providers;
- universities or institutional systems when they are nationally significant;
- globally available providers such as Microsoft, Google, Apple, Proton, or Yahoo only when the purpose is to describe availability rather than imply a national company.

A `.country` or `.ccTLD` domain alone is not proof that the organization is an e-mail provider.

## 8. Water Price Standard

Water prices vary substantially by tariff class, meter size, municipal utility, consumption tier, wastewater charges, taxes, subsidies, and currency. Therefore the project should store:

**Water Price = source tariff + unit + tariff class + locality + date + conversion rate + USD/U.S.-gallon equivalent.**

If the source reports cubic meters, use:

`1 cubic metre = 264.172052 U.S. gallons`

The conversion should not be presented as a universal national price when only a municipal tariff is available. Prefer a clearly labeled representative residential tariff and retain the source locality and date.

## 9. Current Officeholders

President, monarch, prime minister, ministers, governors, and other officeholders are time-sensitive. Every officeholder value should therefore carry a **source date** and should be refreshed when the enclosure is regenerated.

For a polity without a conventional president/prime minister structure, use the applicable constitutional office and write **Not applicable** for the others.

## 10. Data Quality Levels

| Level | Meaning |
|---|---|
| **A** | Current authoritative primary source or official registry, corroborated where practical. |
| **B** | Reputable international/institutional secondary source with clear date and methodology. |
| **C** | Reputable commercial or research dataset suitable for descriptive use but requiring periodic verification. |
| **D** | Historical, indirect, incomplete, or weakly comparable evidence. |
| **N/A** | Not applicable or no responsible comparable value available. |

## 11. United States Connection Principle

The U.S. connection column is an **interoperability matrix**, not a list of surveillance relationships. A country may be capable of ordinary HTTP/TLS/DNS/e-mail interoperability with U.S. systems without any claim that a particular person, server, company, or government has exchanged traffic with the United States.

The matrix should distinguish:

| Connection Class | Description |
|---|---|
| **U1 — Ordinary Internet Interoperability** | Standard HTTPS/DNS/e-mail connectivity is technically available. |
| **U2 — Application/API Interoperability** | Cross-border web APIs and normal HTTP methods are documented or commonly usable. |
| **U3 — Repository/Developer Interoperability** | Git/HTTPS or comparable developer transport is available to authorized users. |
| **U4 — Restricted** | Technical interoperability exists but documented legal, policy, filtering, sanctions, or network restrictions may affect it. |
| **U5 — Exceptional / Isolated** | Connectivity is highly limited, institution-specific, or dependent on specialized links. |
| **U-N/A** | Not meaningfully applicable to the entry. |

## 12. Recommended Source Families

- **ITU:** country ICT statistics, connectivity, affordability, and telecommunications indicators.
- **IANA / RIRs:** Internet number-resource allocation and regional registry structure.
- **National telecommunications regulators:** licensed operators, numbering, spectrum, and market information.
- **National statistical offices:** population, geography, economic, and utility statistics.
- **Government constitutional/official sites:** current officeholders and institutional structure.
- **Municipal water utilities:** residential tariff schedules and effective dates.
- **Recognized geospatial/statistical authorities:** boundaries, centroids, and geographic codes.

ITU's 2025 reporting estimates that 74% of the world's population was online, while emphasizing continuing differences in quality, affordability, devices, skills, and security. The Global Connectivity Report also notes that submarine cables carry more than 99% of international data flows. These facts support using a multi-field country network enclosure rather than treating a single Internet-penetration statistic as a complete description of national connectivity.

## 13. Relationship to ENCLOSURE.md

The document hierarchy is:

**BYPASS.md → ENCLOSURE.md → COUNTRY_NETWORK_ENCLOSURE.md → Country Row → Evidence**

`ENCLOSURE.md` remains the primary comparative enclosure. This file supplies the more detailed network, service, geodata, water-price, officeholder, and U.S.-interoperability table without turning country-level network information into a host-targeting inventory.

**Guiding principle:** identify infrastructure clearly, document sources, preserve geographic and political nuance, and never confuse aggregate network attribution with the physical location or identity of an individual Internet user or endpoint.
