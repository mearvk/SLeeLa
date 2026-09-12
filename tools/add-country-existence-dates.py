#!/usr/bin/env python3
"""Add country creation/end years to the generated TEELTAILS matrix.

Dates are resolved from Wikidata using ISO 3166-1 alpha-3 codes where
available, with an exact English-label fallback for project/historical
entries that do not have a current ISO code.

Wikidata's inception (P571) is used for the beginning of an entity and
its dissolved/abolished/demolished date (P576) for an applicable end.
Unknown dates remain explicitly unknown; a current entity without a
recorded dissolution date is shown as Ongoing.
"""

from pathlib import Path
import re
import urllib.parse
import urllib.request
import json

ROOT = Path(__file__).resolve().parents[1]
DOC = ROOT / "http/spec/TEELTAILS.md.ms.max"
BEGIN = "<!-- BEGIN GENERATED TEELTAILS COUNTRY MATRIX -->"
END = "<!-- END GENERATED TEELTAILS COUNTRY MATRIX -->"
WIKIDATA = "https://query.wikidata.org/sparql"


def get_json(url):
    req = urllib.request.Request(
        url,
        headers={
            "User-Agent": "SLeeLa-TEELTAILS-country-dates/1.0 (public research)"
        },
    )
    with urllib.request.urlopen(req, timeout=120) as response:
        return json.load(response)


def year_from_value(value):
    if not value:
        return None
    match = re.search(r"([+-]?\d+)-", value)
    return int(match.group(1)) if match else None


def choose_date(statements):
    """Prefer Wikidata preferred-rank statements, then normal-rank ones."""
    if not statements:
        return None
    ranked = [s for s in statements if s.get("value")]
    if not ranked:
        return None
    max_rank = max(int(s.get("rank", 0)) for s in ranked)
    candidates = [s for s in ranked if int(s.get("rank", 0)) == max_rank]
    years = [year_from_value(s["value"]) for s in candidates]
    years = [y for y in years if y is not None]
    return min(years) if years else None


def query_wikidata(values, field, by_iso=True):
    if not values:
        return {}
    escaped = []
    for value in values:
        value = value.replace('\\', '\\\\').replace('"', '\\"')
        escaped.append(f'"{value}"')
    values_block = " ".join(escaped)
    if by_iso:
        lookup = "?item wdt:P298 ?key ."
        key_filter = ""
    else:
        lookup = "?item rdfs:label ?key ."
        key_filter = 'FILTER(LANG(?key) = "en")'
    query = f"""
PREFIX wd: <http://www.wikidata.org/entity/>
PREFIX wdt: <http://www.wikidata.org/prop/direct/>
PREFIX p: <http://www.wikidata.org/prop/>
PREFIX ps: <http://www.wikidata.org/prop/statement/>
PREFIX wikibase: <http://wikiba.se/ontology#>
PREFIX rdfs: <http://www.w3.org/2000/01/rdf-schema#>
SELECT ?key ?item ?itemLabel ?inception ?inceptionRank ?dissolved ?dissolvedRank WHERE {{
  VALUES ?key {{ {values_block} }}
  {lookup}
  {key_filter}
  OPTIONAL {{
    ?item p:P571 ?insStmt .
    ?insStmt ps:P571 ?inception ; wikibase:rank ?inceptionRank .
    FILTER(?inceptionRank != wikibase:DeprecatedRank)
  }}
  OPTIONAL {{
    ?item p:P576 ?endStmt .
    ?endStmt ps:P576 ?dissolved ; wikibase:rank ?dissolvedRank .
    FILTER(?dissolvedRank != wikibase:DeprecatedRank)
  }}
  SERVICE wikibase:label {{ bd:serviceParam wikibase:language "en". }}
}}
"""
    url = WIKIDATA + "?" + urllib.parse.urlencode({"query": query, "format": "json"})
    raw = get_json(url)
    result = {}
    for binding in raw.get("results", {}).get("bindings", []):
        key = binding.get("key", {}).get("value")
        if not key:
            continue
        item = result.setdefault(key, {"inception": [], "dissolved": [], "item": None, "label": None})
        item["item"] = binding.get("item", {}).get("value")
        item["label"] = binding.get("itemLabel", {}).get("value")
        if "inception" in binding:
            item["inception"].append({
                "value": binding["inception"].get("value"),
                "rank": binding.get("inceptionRank", {}).get("value", "0"),
            })
        if "dissolved" in binding:
            item["dissolved"].append({
                "value": binding["dissolved"].get("value"),
                "rank": binding.get("dissolvedRank", {}).get("value", "0"),
            })
    return {
        key: {
            "creation": choose_date(value["inception"]),
            "end": choose_date(value["dissolved"]),
            "item": value["item"],
            "label": value["label"],
        }
        for key, value in result.items()
    }


def split_row(line):
    if not line.startswith("|"):
        return None
    cells = [c.strip() for c in line.strip().strip("|").split("|")]
    return cells if len(cells) >= 10 else None


def main():
    text = DOC.read_text(encoding="utf-8")
    if BEGIN not in text or END not in text:
        raise RuntimeError("TEELTAILS generated matrix markers not found")

    before, rest = text.split(BEGIN, 1)
    generated, after = rest.split(END, 1)
    lines = generated.splitlines()

    rows = []
    iso_values = []
    names = []
    for line in lines:
        cells = split_row(line)
        if not cells or not cells[0].isdigit():
            continue
        rows.append((line, cells))
        iso = cells[2]
        if re.fullmatch(r"[A-Z]{2} / [A-Z]{3}", iso):
            iso_values.append(iso.split("/")[1].strip())
        else:
            names.append(cells[1])

    iso_data = query_wikidata(sorted(set(iso_values)), "P298", by_iso=True)
    name_data = query_wikidata(sorted(set(names)), "rdfs:label", by_iso=False)

    replacements = {}
    resolved = 0
    for original, cells in rows:
        iso = cells[2]
        data = None
        if re.fullmatch(r"[A-Z]{2} / [A-Z]{3}", iso):
            data = iso_data.get(iso.split("/")[1].strip())
        if data is None:
            data = name_data.get(cells[1])
        creation = str(data["creation"]) if data and data.get("creation") is not None else "Unknown"
        if data and data.get("end") is not None:
            ending = str(data["end"])
        elif data and data.get("item"):
            ending = "Ongoing"
        else:
            ending = "Unknown"
        # New columns follow the country/ISO fields.
        new_cells = cells[:3] + [creation, ending] + cells[3:]
        replacements[original] = "| " + " | ".join(new_cells) + " |"
        if creation != "Unknown":
            resolved += 1

    out_lines = []
    for line in lines:
        if line.startswith("| # | Country / World Entry | ISO |"):
            out_lines.append("| # | Country / World Entry | ISO | Creation | End | Wi-Fi standards | Cellular technology | WiMAX / FWA | Entry Internet tier | Standard Internet tier | Premium Internet tier | Quality |")
        elif line.startswith("|---:") and "Country / World Entry" not in line:
            out_lines.append("|---:|---|---|---:|---:|---|---|---|---|---|---|---|")
        else:
            out_lines.append(replacements.get(line, line))

    generated_new = "\n".join(out_lines)
    generated_new = generated_new.replace(
        "**Generated from `http/spec/BYPASS.md` using free, public data sources.**",
        "**Generated from `http/spec/BYPASS.md` using free, public data sources.**\n\n**Country creation/end years:** resolved from Wikidata where an ISO 3166-1 code or exact project-entry label identifies an entity. `Creation` uses Wikidata inception (P571); `End` uses dissolved/abolished/demolished (P576). `Ongoing` means no dissolution date is recorded for the identified current entity; `Unknown` means the entity could not be resolved reliably."
    )

    # Add a concise methodology section immediately before the generated matrix.
    marker_section = "## 15. Country Creation and End Dates"
    if marker_section not in before:
        before = before.rstrip() + "\n\n" + marker_section + "\n\n"
        before += (
            "Each country/world entry now carries a **Creation** year and an **End** year where applicable. "
            "For an entity that still exists, the End field is `Ongoing`; for an unresolved or insufficiently documented entry it is `Unknown`. "
            "The project uses Wikidata's **inception (P571)** as the creation/formation field and **dissolved, abolished or demolished (P576)** as the end field. "
            "These are entity-existence dates, not necessarily the date of the first civilization, first settlement, first dynasty, first administrative predecessor, or first appearance of the geographic region. "
            "Historical entries may therefore have both a creation and an end year. Date precision is retained at the year level in the comparison matrix."
        )
        before += "\n\n"

    DOC.write_text(before.rstrip() + "\n\n" + BEGIN + "\n\n" + generated_new.rstrip() + "\n" + END + after, encoding="utf-8")
    print(f"Resolved creation dates for {resolved} of {len(rows)} matrix entries")


if __name__ == "__main__":
    main()
