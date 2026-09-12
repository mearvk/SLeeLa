#!/usr/bin/env python3
"""Populate TEELTAILS.md.ms.max with live, sourced country data.

The 391-entry BYPASS taxonomy remains authoritative. For current economies,
this generator enriches the matrix from the public ITU DataHub API:
- standardized 2025 ICT price baskets (USD)
- mobile-network population coverage by technology
- Internet-use and fixed-broadband indicators used to derive a transparent
  project quality grade

The generator does not invent operator tariffs or Wi-Fi spectrum assignments.
Wi-Fi is represented as a conservative technology baseline, while 6 GHz/6E
and Wi-Fi 7 are explicitly marked as regulatory/device-dependent. Historical
or project-only entries are retained as N/A rather than being silently dropped.
"""

from pathlib import Path
import json
import re
import urllib.request

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "http/spec/TEELTAILS.md.ms.max"
BYPASS = ROOT / "http/spec/BYPASS.md"
COUNTRIES_URL = "https://raw.githubusercontent.com/mledoze/countries/master/countries.json"
ITU_API = "https://api.datahub.itu.int/v2"
BEGIN = "<!-- BEGIN GENERATED TEELTAILS COUNTRY MATRIX -->"
END = "<!-- END GENERATED TEELTAILS COUNTRY MATRIX -->"
EXPECTED_ENTRIES = 391


def get_json(url, timeout=60):
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-TEELTAILS/2.0"})
    with urllib.request.urlopen(req, timeout=timeout) as response:
        return json.load(response)


def normalize_name(value):
    value = value.casefold().replace("&", "and")
    value = re.sub(r"[^a-z0-9]+", " ", value)
    return " ".join(value.split())


def load_bypass_entries():
    pattern = re.compile(r"^###\s+(\d+)\.\s+(.+?)\s*$", re.MULTILINE)
    entries = [(int(n), name.strip()) for n, name in pattern.findall(BYPASS.read_text(encoding="utf-8"))]
    entries.sort()
    if len(entries) != EXPECTED_ENTRIES or [n for n, _ in entries] != list(range(1, EXPECTED_ENTRIES + 1)):
        raise RuntimeError(f"BYPASS taxonomy must contain numbered entries 1-{EXPECTED_ENTRIES}")
    return entries


def load_iso_dataset():
    data = get_json(COUNTRIES_URL)
    by_name = {}
    for item in data:
        if not isinstance(item, dict):
            continue
        name = item.get("name", {}).get("common")
        a2, a3 = item.get("cca2"), item.get("cca3")
        if name and a2 and a3:
            pair = (a2, a3)
            by_name[normalize_name(name)] = pair
            official = item.get("name", {}).get("official")
            if official:
                by_name.setdefault(normalize_name(official), pair)
    aliases = {
        "cape verde": "cabo verde", "czech republic": "czechia", "iran": "iran",
        "laos": "laos", "moldova": "moldova", "north korea": "north korea",
        "russia": "russia", "south korea": "south korea", "syria": "syria",
        "the gambia": "gambia", "turkey": "turkey", "vatican city": "vatican city",
    }
    for source, target in aliases.items():
        if normalize_name(target) in by_name:
            by_name[normalize_name(source)] = by_name[normalize_name(target)]
    return by_name


def flatten_indicator_dictionary(node):
    found = []
    if isinstance(node, dict):
        if "codeID" in node and ("label" in node or "name" in node):
            found.append({"codeID": node.get("codeID"), "label": node.get("label") or node.get("name")})
        for value in node.values():
            found.extend(flatten_indicator_dictionary(value))
    elif isinstance(node, list):
        for value in node:
            found.extend(flatten_indicator_dictionary(value))
    return found


def discover_indicators():
    raw = get_json(f"{ITU_API}/dictionaries/getcategories")
    items = flatten_indicator_dictionary(raw)
    labels = {}
    for item in items:
        code_id = item.get("codeID")
        label = str(item.get("label") or "")
        if code_id is not None and label:
            labels[int(code_id)] = label

    wanted = {
        "data_only": ["Data-only mobile broadband basket 5 GB"],
        "fixed": ["Fixed-broadband Internet 5GB", "Fixed-broadband Internet 5 GB"],
        "mobile_low": ["Mobile broadband data and voice low-consumption basket total"],
        "mobile_high": ["Mobile broadband data and voice high-consumption basket total"],
        "internet_users": ["Individuals using the Internet"],
        "fixed_subs": ["Fixed-broadband subscriptions"],
        "coverage": ["Population coverage, by mobile network technology"],
    }
    result = {}
    for key, candidates in wanted.items():
        match = None
        for needle in candidates:
            for code_id, label in labels.items():
                if normalize_name(needle) == normalize_name(label) or normalize_name(needle) in normalize_name(label):
                    match = (code_id, label)
                    break
            if match:
                break
        result[key] = match
    return result


def indicator_series(code_id):
    if not code_id:
        return []
    data = get_json(f"{ITU_API}/data/bycode/{code_id}")
    return data if isinstance(data, list) else []


def latest_values(series, year=2025):
    out = {}
    for record in series:
        iso = record.get("isoCode")
        if not iso:
            continue
        try:
            yr = int(record.get("dataYear"))
        except (TypeError, ValueError):
            continue
        if yr > year:
            continue
        values = []
        for answer in record.get("answer") or []:
            try:
                values.append(float(answer.get("value")))
            except (TypeError, ValueError, AttributeError):
                pass
        if values and (iso not in out or yr >= out[iso][0]):
            out[iso] = (yr, values, record)
    return out


def parse_coverage(series):
    latest = latest_values(series, 2025)
    out = {}
    for iso, (year, values, record) in latest.items():
        labels = ["2G", "3G", "LTE/WiMAX", "5G"]
        if len(values) >= 4:
            cov = dict(zip(labels, values[:4]))
        elif len(values) == 1:
            cov = {"LTE/WiMAX": values[0]}
        else:
            cov = {labels[i]: v for i, v in enumerate(values[:4])}
        out[iso] = (year, cov)
    return out


def money(value):
    if value is None:
        return "Not available"
    return f"${value:.2f}/mo" if value < 10 else f"${value:.1f}/mo"


def pick_value(latest, iso):
    item = latest.get(iso)
    if not item:
        return None, None
    year, values, _ = item
    return (values[0] if values else None), year


def quality_grade(internet, fixed, coverage):
    score = 0.0
    weight = 0.0
    if internet is not None:
        score += max(0, min(100, internet)) * 0.40
        weight += 0.40
    if fixed is not None:
        score += max(0, min(50, fixed)) * 1.2
        weight += 0.60
    if coverage:
        lte = coverage.get("LTE/WiMAX")
        g5 = coverage.get("5G")
        if lte is not None:
            score += max(0, min(100, lte)) * 0.35
            weight += 0.35
        if g5 is not None:
            score += max(0, min(100, g5)) * 0.20
            weight += 0.20
    if weight == 0:
        return "N/A"
    normalized = score / weight
    if normalized >= 80:
        return "A"
    if normalized >= 65:
        return "B"
    if normalized >= 45:
        return "C"
    if normalized >= 25:
        return "D"
    return "E"


def wifi_value():
    return "Wi-Fi 4/5/6 widely deployed; 6 GHz/Wi-Fi 6E and Wi-Fi 7 subject to local spectrum/device rules"


def cellular_value(cov):
    if not cov:
        return "Coverage data unavailable"
    parts = []
    for label in ["2G", "3G", "LTE/WiMAX", "5G"]:
        if label in cov:
            parts.append(f"{label} {cov[label]:.0f}% pop.")
    return "; ".join(parts) if parts else "Coverage data unavailable"


def wimax_value(cov):
    lte = cov.get("LTE/WiMAX") if cov else None
    return f"LTE/WiMAX coverage {lte:.0f}% pop.; FWA availability varies by operator" if lte is not None else "LTE/FWA; WiMAX status not separately reported"


def tier(value, year, label):
    return f"{money(value)} ({label}; ITU {year})" if value is not None else "Not available from ITU 2025 basket"


def build_rows(entries, iso_by_name, prices, internet, fixed, coverage):
    rows = []
    for number, country in entries:
        pair = iso_by_name.get(normalize_name(country))
        if not pair:
            rows.append(f"| {number:03d} | {country} | PROJECT / N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A |")
            continue
        a2, a3 = pair
        p = prices.get(a3, {})
        cov = coverage.get(a3, (None, {}))[1]
        internet_v, _ = pick_value(internet, a3)
        fixed_v, _ = pick_value(fixed, a3)
        q = quality_grade(internet_v, fixed_v, cov)
        entry = tier(p.get("data_only"), p.get("year"), "5 GB mobile data")
        standard = tier(p.get("fixed"), p.get("year"), "5 GB fixed broadband")
        premium = tier(p.get("mobile_high"), p.get("year"), "140 min + 20 SMS + 5 GB mobile")
        rows.append(f"| {number:03d} | {country} | {a2} / {a3} | {wifi_value()} | {cellular_value(cov)} | {wimax_value(cov)} | {entry} | {standard} | {premium} | {q} |")
    return rows


def replace_generated(text, rows, indicator_info):
    header = [
        BEGIN,
        "",
        "**Generated from `http/spec/BYPASS.md` and live ITU DataHub observations.**",
        "",
        "Price tiers are standardized ITU baskets rather than invented operator packages: Entry = 5 GB mobile data-only basket; Standard = 5 GB fixed-broadband basket; Premium = 140 minutes + 20 SMS + 5 GB mobile high-consumption basket. Prices are nominal USD/month for the 2025 collection where available. Quality grades are project-derived from sourced Internet-use, fixed-broadband and mobile-coverage indicators; they are not official ITU rankings.",
        "",
        "| # | Country / World Entry | ISO | Wi-Fi standards | Cellular technology | WiMAX / FWA | Entry Internet tier | Standard Internet tier | Premium Internet tier | Quality |",
        "|---:|---|---|---|---|---|---|---|---|---|",
    ]
    indicator_text = json.dumps(indicator_info, sort_keys=True)
    generated = "\n".join(header + rows + ["", f"**Taxonomy entries:** {len(rows)}", "**Data source:** ITU DataHub (price baskets, mobile-network coverage, Internet-use and fixed-broadband indicators).", f"**Indicator discovery:** `{indicator_text}`", "", END])
    if BEGIN in text and END in text:
        before = text.split(BEGIN, 1)[0].rstrip()
        after = text.split(END, 1)[1].lstrip()
        return before + "\n\n" + generated + ("\n\n" + after if after else "\n")
    return text.rstrip() + "\n\n" + generated + "\n"


def main():
    entries = load_bypass_entries()
    iso_by_name = load_iso_dataset()
    indicators = discover_indicators()
    series = {k: indicator_series(v[0]) for k, v in indicators.items() if v}
    latest = {k: latest_values(v) for k, v in series.items()}
    coverage = parse_coverage(series.get("coverage", []))

    prices = {}
    for key in ["data_only", "fixed", "mobile_high"]:
        for iso, (year, values, _) in latest.get(key, {}).items():
            prices.setdefault(iso, {})[key] = values[0] if values else None
            prices[iso]["year"] = year

    existing = OUT.read_text(encoding="utf-8") if OUT.exists() else "# TEELTAILS.md.ms.max\n"
    rows = build_rows(entries, iso_by_name, prices, latest.get("internet_users", {}), latest.get("fixed_subs", {}), coverage)
    OUT.write_text(replace_generated(existing, rows, indicators), encoding="utf-8")
    populated = sum("PROJECT / N/A" not in row for row in rows)
    print(f"Generated TEELTAILS country matrix: {len(rows)} entries; current ISO rows: {populated}; ITU indicators discovered: {sum(bool(v) for v in indicators.values())}/{len(indicators)}")


if __name__ == "__main__":
    main()
