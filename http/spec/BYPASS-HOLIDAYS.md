# BYPASS — Major Holidays and Religious Celebrations

**Status:** Holiday and celebration companion to `BYPASS.md`

## Purpose

This document defines the required structure for the **391 world entries** in `BYPASS.md`. Each country, territory, or historical polity receives its own heading containing its major public/civic holidays, major religious celebrations, and major cultural celebrations.

## Scope

The holiday layer is deliberately broader than national public holidays. A celebration may be widely practiced without being a statutory day off. Conversely, a public holiday does not imply that every resident follows the religion or tradition associated with it.

Pew Research Center's 2026 cross-national analysis identified nearly 2,500 public holidays across 190 UN member countries and notes that national schedules vary and that many religious holidays follow lunar calendars. citeturn0search0turn0search24

UNESCO's Intangible Cultural Heritage records provide an additional reference for culturally significant festivals, processions, rituals, and community celebrations. citeturn0search4turn0search5

## Entry Format

Every BYPASS entry should use this form:

### NNN. Country or World Entry

**Major Public and Civic Holidays:** National Day; Independence Day; Labour/Workers' Day; major remembrance or civic holidays.

**Major Religious Celebrations:** Major religious festivals observed nationally or widely by significant communities.

**Major Cultural Celebrations:** Major festivals, seasonal celebrations, heritage events, and traditional observances.

## Holiday Classification

- **Public:** Nationally recognized non-working holiday.
- **Religious:** Major observance associated with a religious tradition, whether or not it is a national holiday.
- **Cultural:** Major traditional, seasonal, historical, or community celebration.
- **Historical:** Celebration or remembrance associated with a former polity or historical community.
- **Regional:** Significant celebration whose recognition is subnational rather than nationwide.

## Calendar Rule

Holiday dates MUST be represented with their calendar basis where relevant. Islamic, Hebrew, Buddhist, Hindu, Chinese, Ethiopian, Armenian, Orthodox, and other calendar systems can cause celebrations to move relative to the Gregorian calendar.

## Diversity Rule

No country's list should imply that one religion defines every resident. Where multiple major traditions are present, the heading should preserve that plurality.

Pew's religious-composition research covers 201 countries and territories and emphasizes that religious affiliation estimates are based on thousands of censuses and surveys and remain estimates rather than exact individual classifications. citeturn0search1turn0search6

## 391-Entry Generation

The companion generator is:

`tools/generate_bypass_holidays.py`

It reads the existing 391 numbered headings from `BYPASS.md` and produces the country-by-country holiday document without changing the underlying BYPASS taxonomy.

The generation workflow is:

`.github/workflows/generate-bypass-holidays.yml`

The generated document is intentionally maintained separately from `BYPASS.md` so the civic-wisdom document remains readable while the holiday calendar can evolve independently.

## Accuracy Rule

“Major” does not mean “every known.” Local calendars contain thousands of additional celebrations. The generated list is a maintained baseline and should be expanded with authoritative national calendars, recognized religious authorities, government gazettes, and UNESCO cultural-heritage records when a fuller country-specific treatment is required.

## Guiding Principle

**Every world entry deserves its own cultural calendar context, while no calendar should be treated as a measure of human worth, legal privilege, or network trust.**
