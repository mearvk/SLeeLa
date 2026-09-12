#!/usr/bin/env python3
"""Generate the 391-entry BYPASS holiday companion from BYPASS.md.

The generator intentionally lists major public/civic and major religious/cultural
celebrations rather than claiming to enumerate every local observance.
"""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "http/spec/BYPASS.md"
TARGET = ROOT / "http/spec/BYPASS-HOLIDAYS.md"

MUSLIM = ("afghanistan albania algeria azerbaijan bahrain bangladesh brunei "
          "comoros djibouti egypt gambia guinea iran iraq jordan kazakhstan "
          "kuwait kyrgyzstan lebanon libya malaysia maldives mali mauritania "
          "morocco niger nigeria oman pakistan palestine qatar saudi senegal "
          "sierra leone somalia sudan syria tajikistan tunisia turkey "
          "turkmenistan uzbekistan yemen kosovo")
HINDU = "india nepal mauritius fiji guyana suriname trinidad"
BUDDHIST = "bhutan cambodia laos myanmar sri lanka thailand mongolia"
EAST_ASIA = "china hong kong japan korea taiwan vietnam singapore"
ORTHODOX = "russia ukraine belarus serbia romania bulgaria greece georgia cyprus armenia montenegro north macedonia moldova eritrea ethiopia"
EUROPE_CHRISTIAN = ("france belgium luxembourg germany netherlands switzerland austria italy "
                    "spain portugal poland czech slovak hungary croatia slovenia malta "
                    "monaco andorra ireland")


def has(name: str, words: str) -> bool:
    n = name.lower()
    return any(w in n for w in words.split())


def profile(name: str, number: int):
    n = name.lower()
    if number > 249:
        return (
            ["Historical commemorations of the polity", "Anniversary/remembrance observances in successor communities"],
            ["Major religious celebrations of associated historical communities"],
            ["Heritage, remembrance, and cultural festivals connected with the historical polity"],
        )

    civic = ["New Year's Day", "National/Independence Day", "Labour/Workers' Day"]
    religious = []
    cultural = ["Local cultural and seasonal festivals"]

    if has(name, MUSLIM):
        religious += ["Ramadan", "Eid al-Fitr", "Eid al-Adha", "Islamic New Year", "Mawlid (where observed)"]
    if has(name, HINDU):
        religious += ["Diwali", "Holi", "Navratri/Dussehra", "Janmashtami"]
    if has(name, BUDDHIST):
        religious += ["Vesak/Buddha Day", "Buddhist New Year", "Lunar/full-moon observances"]
    if has(name, EAST_ASIA):
        cultural += ["Lunar New Year", "Mid-Autumn Festival"]
    if "ethiopia" in n or "eritrea" in n:
        religious += ["Ethiopian/Eritrean Christmas", "Timkat", "Meskel"]
    if "israel" in n:
        religious = ["Rosh Hashanah", "Yom Kippur", "Sukkot", "Passover", "Shavuot", "Hanukkah"]
    if has(name, ORTHODOX):
        religious += ["Orthodox Easter", "Orthodox Christmas or Armenian Christmas"]
    if has(name, EUROPE_CHRISTIAN):
        religious += ["Christmas", "Good Friday/Easter period", "All Saints' Day or local Christian observances"]
    if not religious:
        religious = ["Christmas", "Easter or locally significant Christian observances"]

    if "brazil" in n:
        cultural += ["Carnival", "Festa Junina"]
    if "mexico" in n:
        cultural += ["Día de los Muertos", "Our Lady of Guadalupe celebrations"]
    if "japan" in n:
        cultural += ["Golden Week", "Obon"]
    if "united states" in n:
        civic += ["Memorial Day", "Thanksgiving", "Juneteenth"]
    if "canada" in n:
        civic += ["Canada Day", "National Day for Truth and Reconciliation"]
    if "australia" in n:
        civic += ["Australia Day", "ANZAC Day"]
    if "new zealand" in n:
        civic += ["Waitangi Day", "ANZAC Day"]
    if "south africa" in n:
        civic += ["Freedom Day", "Heritage Day", "Day of Reconciliation"]

    def unique(values):
        return list(dict.fromkeys(values))
    return unique(civic), unique(religious), unique(cultural)


def main():
    source = SOURCE.read_text(encoding="utf-8")
    entries = re.findall(r"^###\s+(\d{3})\.\s+(.+?)\s*$", source, flags=re.MULTILINE)
    if len(entries) != 391:
        raise SystemExit(f"Expected 391 BYPASS entries; found {len(entries)}")

    out = [
        "# BYPASS — Major Holidays and Religious Celebrations",
        "",
        "**Status:** Companion to `BYPASS.md`",
        "",
        "**Method:** Each heading identifies one of the 391 project entries. The listed celebrations are major national/public or widely observed religious/cultural observances, not an exhaustive inventory of every local or optional observance. Dates vary by year, especially for lunar/lunisolar calendars.",
        "",
        "National holiday schedules can change, and religious dates can move between Gregorian dates. Final operational use should consult the competent national authority or recognized calendar source for the relevant year.",
        "",
        "## World Entries",
        "",
    ]
    for number, name in entries:
        civ, religious, cultural = profile(name, int(number))
        selected = civ[:4] + religious[:4] + cultural[:2]
        out += [
            f"### {number}. {name}",
            "",
            f"**Major Celebrations:** {'; '.join(selected)}.",
            "",
        ]
    TARGET.write_text("\n".join(out), encoding="utf-8")
    print(f"Generated {TARGET} with {len(entries)} entries")


if __name__ == "__main__":
    main()
