#!/usr/bin/env python3
"""Generate the country-level enclosure appendix from http/spec/BYPASS.md.

The parent document remains authoritative for the 391-entry taxonomy. This
script extracts every numbered entry and creates a generated appendix inside
ENCLOSURE.md. Current entries (1-249) receive project-level comparative
fields; historical/non-sovereign entries receive N/A fields where modern
comparisons would be misleading.
"""

from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
BYPASS = ROOT / "http" / "spec" / "BYPASS.md"
ENCLOSURE = ROOT / "http" / "spec" / "ENCLOSURE.md"

TECH_4 = {
    "United States", "Canada", "Australia", "New Zealand", "Japan",
    "Korea, Republic of", "Singapore", "Switzerland", "Norway", "Sweden",
    "Denmark", "Finland", "Netherlands", "Luxembourg", "Germany", "Austria",
    "Belgium", "France", "United Kingdom", "Ireland", "Iceland", "Estonia",
    "Israel", "Taiwan, Province of China", "Andorra", "Liechtenstein", "Monaco",
    "San Marino"
}

TECH_3 = {
    "Spain", "Italy", "Portugal", "Slovenia", "Czechia", "Slovakia", "Poland",
    "Lithuania", "Latvia", "Croatia", "Greece", "Malta", "Cyprus",
    "United Arab Emirates", "Qatar", "Bahrain", "Saudi Arabia", "Kuwait", "Oman",
    "Brunei", "Brunei Darussalam", "Malaysia", "Thailand", "China", "Türkiye",
    "Russian Federation", "Russia", "Chile", "Uruguay", "Argentina", "Brazil",
    "Mexico", "South Africa", "Costa Rica", "Panama", "Colombia", "Georgia",
    "Armenia", "Azerbaijan", "Kazakhstan", "Serbia", "Montenegro", "Hungary",
    "Romania", "Bulgaria"
}

TECH_1 = {
    "Afghanistan", "Burundi", "Central African Republic", "Chad",
    "Congo, The Democratic Republic of the", "Eritrea", "Guinea-Bissau",
    "Haiti", "Liberia", "Libya", "Madagascar", "Malawi", "Mali", "Mozambique",
    "Niger", "Sierra Leone", "Somalia", "South Sudan", "Sudan", "Yemen",
    "Zimbabwe", "Guinea", "Togo", "Gambia", "Equatorial Guinea", "Sao Tome and Principe",
    "Comoros", "Korea, Democratic People's Republic of", "Democratic Kampuchea"
}

SOCIALIST = {
    "China", "Cuba", "Korea, Democratic People's Republic of", "Lao People's Democratic Republic",
    "Viet Nam", "Vietnam", "Democratic Kampuchea", "Soviet Union"
}

SOCIAL_DEMOCRATIC = {
    "Sweden", "Norway", "Denmark", "Finland", "Iceland", "Netherlands", "Belgium",
    "France", "Germany", "Austria", "Luxembourg", "Ireland", "New Zealand", "Australia",
    "Canada", "Spain", "Portugal", "Italy", "Slovenia", "Czechia", "Slovakia", "Estonia",
    "Lithuania", "Latvia", "Poland", "Croatia"
}

MARKET = {
    "United States", "Singapore", "Switzerland", "Hong Kong", "Australia", "New Zealand",
    "United Kingdom", "Ireland", "Canada", "Chile", "Panama", "Uruguay", "Japan",
    "Korea, Republic of", "Israel"
}

KIND_4 = SOCIAL_DEMOCRATIC | {"Canada", "Australia", "New Zealand", "Japan", "Korea, Republic of", "Switzerland", "Singapore", "Uruguay", "Costa Rica"}


def tech_grade(name: str, number: int) -> str:
    if number > 249:
        return "N/A"
    if name in TECH_4:
        return "4"
    if name in TECH_1:
        return "1"
    if name in TECH_3:
        return "3"
    return "2"


def socialism(name: str, number: int) -> str:
    if number > 249:
        return "S-N/A"
    if name in SOCIALIST:
        return "S-4"
    if name in SOCIAL_DEMOCRATIC:
        return "S-3"
    if name in MARKET:
        return "S-1"
    return "S-2"


def kindness(name: str, number: int) -> str:
    if number > 249:
        return "K-N/A"
    if name in KIND_4:
        return "K-4"
    if name in TECH_1:
        return "K-1"
    if name in TECH_4 or name in TECH_3:
        return "K-3"
    return "K-2"


def technology_note(name: str, number: int, grade: str) -> str:
    if number > 249:
        return "Historical/non-sovereign entry; modern technology comparison is not applicable without a defined historical reference year."
    if grade == "4":
        return "Advanced digital infrastructure and technology ecosystem; treated as comparable to the project's U.S. reference level."
    if grade == "3":
        return "Strong or advanced digital environment with meaningful differences in scale, access, affordability, or infrastructure."
    if grade == "1":
        return "Major constraints in infrastructure, affordability, access, skills, reliability, or security; current evidence should be consulted."
    return "Mixed or developing digital environment; substantial variation may exist by geography, income, infrastructure, and access."


def socialism_note(name: str, number: int, grade: str) -> str:
    if number > 249:
        return "Historical/non-sovereign entry; no modern economic-system grade is asserted."
    if grade == "S-4":
        return "Strong or explicit socialist/state-led economic structure; terminology is institutional, not a label for individuals."
    if grade == "S-3":
        return "Strong public-sector and social-democratic role within a mixed market economy."
    if grade == "S-1":
        return "Predominantly market-oriented institutional structure with substantial private-sector activity."
    return "Mixed public/private economic structure with public services and market institutions operating together."


def kindness_note(name: str, number: int, grade: str) -> str:
    if number > 249:
        return "Historical/non-sovereign entry; civic comparison is limited to historical context rather than a modern population judgment."
    if grade == "K-4":
        return "Strong institutional emphasis on welfare, dignity, accessibility, education, social protection, and civic inclusion."
    if grade == "K-3":
        return "Substantial institutional or civic-welfare orientation, with important areas of unevenness or limitation."
    if grade == "K-1":
        return "Institutional expression is limited or substantially constrained; this is not a judgment about individual people."
    return "Mixed or uneven institutional expression of welfare, dignity, accessibility, and civic support."


def main() -> None:
    text = BYPASS.read_text(encoding="utf-8")
    entries = [(int(n), name.strip()) for n, name in re.findall(r"^### (\d{3})\. (.+)$", text, re.MULTILINE)]
    if len(entries) != 391:
        raise SystemExit(f"Expected 391 BYPASS entries; found {len(entries)}")

    lines = [
        "## 12. Generated Country-by-Country Data",
        "",
        "This section is generated from the numbered entries in `BYPASS.md`. It is deliberately a **project classification layer**, not an official international ranking. ITU's 2026 ICT Development Index covers 159 economies, so entries without comparable official data use a transparent project assessment or N/A where a modern comparison would be misleading.",
        "",
        "| Entry | Internet & Technology Standard | Technology Grade | Socialism Standard | Kind / Civic Standard | Country-Level Note |",
        "|---|---|---:|---|---|---|",
    ]
    for number, name in entries:
        tg = tech_grade(name, number)
        sg = socialism(name, number)
        kg = kindness(name, number)
        lines.append(
            f"| {number:03d}. {name.replace('|', '/') } | {technology_note(name, number, tg)} | **{tg}** | **{sg}** — {socialism_note(name, number, sg)} | **{kg}** — {kindness_note(name, number, kg)} | Country/entry-specific evidence should be added as it becomes available. |"
        )

    generated = "\n".join(lines) + "\n"
    enclosure = ENCLOSURE.read_text(encoding="utf-8")
    start = "## 12. Generated Country-by-Country Data"
    end = "## 13. Guiding Principle"
    if start in enclosure and end in enclosure:
        prefix = enclosure.split(start, 1)[0]
        suffix = enclosure.split(end, 1)[1]
        enclosure = prefix + generated + "\n" + end + suffix
    elif start in enclosure:
        enclosure = enclosure.split(start, 1)[0] + generated
    else:
        enclosure = enclosure.rstrip() + "\n\n" + generated

    ENCLOSURE.write_text(enclosure, encoding="utf-8")
    print(f"Generated {len(entries)} country/entry records in {ENCLOSURE}")


if __name__ == "__main__":
    main()
