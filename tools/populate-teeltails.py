#!/usr/bin/env python3
"""Populate TEELTAILS.md.ms.max with a 391-entry country technology matrix.

The generator deliberately uses conservative, source-oriented classifications.
It does not invent country-specific prices or historical deployments.  Where
country-specific evidence has not yet been curated, fields are marked
"Not yet curated" and the document records the methodology for later sourcing.
"""

from pathlib import Path
import json
import urllib.request

OUT = Path("http/spec/TEELTAILS.md.ms.max")
COUNTRIES_URL = "https://raw.githubusercontent.com/mledoze/countries/master/countries.json"
OVERRIDES = Path("http/spec/country_network_overrides.json")
BEGIN = "<!-- BEGIN GENERATED TEELTAILS COUNTRY MATRIX -->"
END = "<!-- END GENERATED TEELTAILS COUNTRY MATRIX -->"


def get_json(url):
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-TEELTAILS/1.0"})
    with urllib.request.urlopen(req, timeout=30) as r:
        return json.load(r)


def load_countries():
    data = get_json(COUNTRIES_URL)
    if not isinstance(data, list):
        raise RuntimeError("Country dataset returned an unexpected response type")
    countries = []
    for item in data:
        if not isinstance(item, dict):
            continue
        name = item.get("name", {}).get("common")
        cca2 = item.get("cca2")
        cca3 = item.get("cca3")
        if name and cca2 and cca3:
            countries.append((name, cca2, cca3))
    return sorted(countries, key=lambda x: x[0].casefold())


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


def row(i, country, cca2, cca3, overrides):
    o = overrides.get(cca2, {}) if isinstance(overrides, dict) else {}
    wifi = o.get("wifi", "Not yet curated")
    cellular = o.get("cellular", "Not yet curated")
    wimax = o.get("wimax", "Not yet curated")
    entry = o.get("internet_entry", "Not yet curated")
    standard = o.get("internet_standard", "Not yet curated")
    premium = o.get("internet_premium", "Not yet curated")
    quality = o.get("internet_quality", "Not yet curated")
    return f"| {i:03d} | {cell(country)} | {cca2} / {cca3} | {cell(wifi)} | {cell(cellular)} | {cell(wimax)} | {cell(entry)} | {cell(standard)} | {cell(premium)} | {cell(quality)} |"


def build_matrix(countries, overrides):
    lines = [BEGIN, "", "| # | Country | ISO | Wi-Fi standards | Cellular technology | WiMAX / FWA | Entry Internet tier | Standard Internet tier | Premium Internet tier | Quality |", "|---:|---|---|---|---|---|---|---|---|---|"]
    for i, (name, cca2, cca3) in enumerate(countries, 1):
        lines.append(row(i, name, cca2, cca3, overrides))
    lines += ["", END]
    return "\n".join(lines)


def replace_generated(text, generated):
    if BEGIN in text and END in text:
        before = text.split(BEGIN, 1)[0].rstrip()
        after = text.split(END, 1)[1].lstrip()
        return before + "\n\n" + generated + ("\n\n" + after if after else "\n")
    return text.rstrip() + "\n\n" + generated + "\n"


def main():
    countries = load_countries()
    if len(countries) < 300:
        raise RuntimeError(f"Country dataset unexpectedly contains only {len(countries)} entries")
    overrides = load_overrides()
    existing = OUT.read_text(encoding="utf-8") if OUT.exists() else "# TEELTAILS.md.ms.max\n"
    generated = build_matrix(countries, overrides)
    OUT.write_text(replace_generated(existing, generated), encoding="utf-8")
    print(f"Generated TEELTAILS country matrix: {len(countries)} entries")


if __name__ == "__main__":
    main()
