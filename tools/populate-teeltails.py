#!/usr/bin/env python3
"""Populate TEELTAILS.md.ms.max from the project's 391-entry BYPASS taxonomy.

BYPASS.md is the authoritative project taxonomy.  The current-country dataset
is used only to enrich entries with ISO codes where a current match exists.
Historical countries, territories, and project-specific entries remain in the
matrix with PROJECT / N/A rather than being silently dropped.

The generator deliberately uses conservative, source-oriented classifications.
It does not invent country-specific prices or historical deployments.  Where
country-specific evidence has not yet been curated, fields are marked
"Not yet curated".
"""

from pathlib import Path
import json
import re
import urllib.request

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "http/spec/TEELTAILS.md.ms.max"
BYPASS = ROOT / "http/spec/BYPASS.md"
COUNTRIES_URL = "https://raw.githubusercontent.com/mledoze/countries/master/countries.json"
OVERRIDES = ROOT / "http/spec/country_network_overrides.json"
BEGIN = "<!-- BEGIN GENERATED TEELTAILS COUNTRY MATRIX -->"
END = "<!-- END GENERATED TEELTAILS COUNTRY MATRIX -->"
EXPECTED_ENTRIES = 391


def get_json(url):
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-TEELTAILS/1.1"})
    with urllib.request.urlopen(req, timeout=30) as response:
        return json.load(response)


def normalize_name(value):
    value = value.casefold().replace("&", "and")
    value = re.sub(r"[^a-z0-9]+", " ", value)
    return " ".join(value.split())


def load_bypass_entries():
    if not BYPASS.exists():
        raise RuntimeError(f"Missing authoritative taxonomy: {BYPASS}")

    pattern = re.compile(r"^###\s+(\d+)\.\s+(.+?)\s*$", re.MULTILINE)
    entries = [(int(number), name.strip()) for number, name in pattern.findall(BYPASS.read_text(encoding="utf-8"))]
    entries.sort(key=lambda item: item[0])

    if len(entries) != EXPECTED_ENTRIES:
        raise RuntimeError(
            f"BYPASS taxonomy contains {len(entries)} numbered entries; expected {EXPECTED_ENTRIES}"
        )

    expected_numbers = list(range(1, EXPECTED_ENTRIES + 1))
    actual_numbers = [number for number, _ in entries]
    if actual_numbers != expected_numbers:
        raise RuntimeError("BYPASS taxonomy numbering is not a continuous 001-391 sequence")

    return entries


def load_iso_dataset():
    data = get_json(COUNTRIES_URL)
    if not isinstance(data, list):
        raise RuntimeError("Country dataset returned an unexpected response type")

    by_name = {}
    for item in data:
        if not isinstance(item, dict):
            continue
        name = item.get("name", {}).get("common")
        cca2 = item.get("cca2")
        cca3 = item.get("cca3")
        if name and cca2 and cca3:
            record = (cca2, cca3)
            by_name[normalize_name(name)] = record

            official = item.get("name", {}).get("official")
            if official:
                by_name.setdefault(normalize_name(official), record)

    # Project taxonomy names that differ from the current dataset's common name.
    aliases = {
        "cape verde": "cabo verde",
        "czech republic": "czechia",
        "eswatini": "eswatini",
        "iran": "iran",
        "laos": "laos",
        "moldova": "moldova",
        "north korea": "north korea",
        "russia": "russia",
        "south korea": "south korea",
        "syria": "syria",
        "taiwan": "taiwan",
        "tanzania": "tanzania",
        "the gambia": "gambia",
        "turkey": "turkey",
        "venezuela": "venezuela",
        "vatican city": "vatican city",
    }

    for source_name, dataset_name in aliases.items():
        key = normalize_name(dataset_name)
        if key in by_name:
            by_name[normalize_name(source_name)] = by_name[key]

    return by_name


def load_overrides():
    if not OVERRIDES.exists():
        return {}
    try:
        data = json.loads(OVERRIDES.read_text(encoding="utf-8"))
        return data if isinstance(data, dict) else {}
    except Exception:
        return {}


def cell(value):
    if value is None or value == "":
        return "Not yet curated"
    return str(value).replace("|", "\\|").replace("\n", " ")


def row(number, country, iso, overrides):
    o = overrides.get(iso.split(" / ")[0], {}) if iso and iso != "PROJECT / N/A" else {}
    wifi = o.get("wifi", "Not yet curated")
    cellular = o.get("cellular", "Not yet curated")
    wimax = o.get("wimax", "Not yet curated")
    entry = o.get("internet_entry", "Not yet curated")
    standard = o.get("internet_standard", "Not yet curated")
    premium = o.get("internet_premium", "Not yet curated")
    quality = o.get("internet_quality", "Not yet curated")
    return (
        f"| {number:03d} | {cell(country)} | {iso} | {cell(wifi)} | {cell(cellular)} | "
        f"{cell(wimax)} | {cell(entry)} | {cell(standard)} | {cell(premium)} | {cell(quality)} |"
    )


def build_matrix(entries, iso_by_name, overrides):
    lines = [
        BEGIN,
        "",
        "**Generated from `http/spec/BYPASS.md`; ISO codes are enrichment data only.**",
        "",
        "| # | Country / World Entry | ISO | Wi-Fi standards | Cellular technology | WiMAX / FWA | Entry Internet tier | Standard Internet tier | Premium Internet tier | Quality |",
        "|---:|---|---|---|---|---|---|---|---|---|",
    ]

    matched = 0
    for number, country in entries:
        iso_pair = iso_by_name.get(normalize_name(country))
        if iso_pair:
            iso = f"{iso_pair[0]} / {iso_pair[1]}"
            matched += 1
        else:
            iso = "PROJECT / N/A"
        lines.append(row(number, country, iso, overrides))

    lines += ["", f"**Taxonomy entries:** {len(entries)}  |  **Current ISO-enriched matches:** {matched}  |  **Project/historical/non-ISO entries:** {len(entries) - matched}", "", END]
    return "\n".join(lines)


def replace_generated(text, generated):
    if BEGIN in text and END in text:
        before = text.split(BEGIN, 1)[0].rstrip()
        after = text.split(END, 1)[1].lstrip()
        return before + "\n\n" + generated + ("\n\n" + after if after else "\n")
    return text.rstrip() + "\n\n" + generated + "\n"


def main():
    entries = load_bypass_entries()
    iso_by_name = load_iso_dataset()
    overrides = load_overrides()
    existing = OUT.read_text(encoding="utf-8") if OUT.exists() else "# TEELTAILS.md.ms.max\n"
    generated = build_matrix(entries, iso_by_name, overrides)
    OUT.write_text(replace_generated(existing, generated), encoding="utf-8")
    matched = sum(1 for _, name in entries if normalize_name(name) in iso_by_name)
    print(f"Generated TEELTAILS country matrix: {len(entries)} entries ({matched} ISO-enriched)")


if __name__ == "__main__":
    main()
