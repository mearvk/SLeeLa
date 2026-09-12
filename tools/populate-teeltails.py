#!/usr/bin/env python3
"""Populate TEELTAILS.md.ms.max from free public sources.

Sources:
- ITU 2025 ICT Price Basket Excel workbook: fixed broadband 5GB,
  data-only mobile 5GB, and mobile high-consumption 140 min + 20 SMS + 5GB.
- World Bank World Development Indicators (ITU-sourced): Internet users and
  fixed-broadband subscriptions per 100 people.
- Mledoze public country dataset for ISO-2/ISO-3 name resolution.

The undocumented ITU DataHub API is deliberately NOT used.  This keeps the
GitHub Actions job dependent only on public, downloadable data sources.

The 391-entry BYPASS taxonomy remains authoritative.  Historical/project-only
entries are retained as N/A where a current commercial service observation is
not meaningful.
"""

from pathlib import Path
import json
import re
import tempfile
import urllib.parse
import urllib.request

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "http/spec/TEELTAILS.md.ms.max"
BYPASS = ROOT / "http/spec/BYPASS.md"
COUNTRIES_URL = "https://raw.githubusercontent.com/mledoze/countries/master/countries.json"
ITU_XLSX = "https://www.itu.int/en/ITU-D/Statistics/Documents/ICT_Prices/ITU_ICTPriceBaskets_2008-2025.xlsx"
WB_API = "https://api.worldbank.org/v2"
BEGIN = "<!-- BEGIN GENERATED TEELTAILS COUNTRY MATRIX -->"
END = "<!-- END GENERATED TEELTAILS COUNTRY MATRIX -->"
EXPECTED_ENTRIES = 391


def get_bytes(url, timeout=120):
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-TEELTAILS/3.0"})
    with urllib.request.urlopen(req, timeout=timeout) as response:
        return response.read()


def get_json(url, timeout=120):
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-TEELTAILS/3.0"})
    with urllib.request.urlopen(req, timeout=timeout) as response:
        return json.load(response)


def normalize_name(value):
    value = str(value).casefold().replace("&", "and")
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
        "cape verde": "cabo verde", "czech republic": "czechia",
        "the gambia": "gambia", "turkey": "turkiye", "vatican city": "holy see",
        "russia": "russia", "south korea": "south korea", "north korea": "north korea",
        "iran": "iran", "laos": "laos", "moldova": "moldova", "syria": "syria",
    }
    for source, target in aliases.items():
        if normalize_name(target) in by_name:
            by_name[normalize_name(source)] = by_name[normalize_name(target)]
    return by_name


def clean_text(value):
    return normalize_name(value) if value is not None else ""


def find_header_row(rows):
    for i, row in enumerate(rows[:40]):
        cells = [clean_text(x) for x in row]
        joined = " | ".join(cells)
        if any(x in joined for x in ("economy", "country")) and any(x in joined for x in ("usd", "price")):
            return i
    return None


def sheet_kind(title):
    n = clean_text(title)
    if "fixed" in n and "broadband" in n:
        return "fixed"
    if "data only" in n or "data only mobile" in n:
        return "data_only"
    if "high" in n and "consumption" in n:
        return "mobile_high"
    if "mobile" in n and "5 gb" in n:
        return "data_only"
    return None


def numeric(value):
    if value is None:
        return None
    if isinstance(value, (int, float)) and not isinstance(value, bool):
        return float(value)
    text = str(value).replace(",", "").strip()
    text = re.sub(r"[^0-9.\-]", "", text)
    try:
        return float(text)
    except ValueError:
        return None


def find_column(headers, candidates):
    normalized = [clean_text(x) for x in headers]
    for i, h in enumerate(normalized):
        if any(candidate in h for candidate in candidates):
            return i
    return None


def parse_itu_workbook(path):
    try:
        import openpyxl
    except ImportError as exc:
        raise RuntimeError("openpyxl is required; install it in the workflow") from exc

    wb = openpyxl.load_workbook(path, read_only=True, data_only=True)
    result = {"data_only": {}, "fixed": {}, "mobile_high": {}}
    used_sheets = {}

    for ws in wb.worksheets:
        kind = sheet_kind(ws.title)
        if not kind:
            continue
        rows = list(ws.iter_rows(values_only=True))
        header_i = find_header_row(rows)
        if header_i is None:
            continue
        headers = list(rows[header_i])
        country_col = find_column(headers, ["economy", "country"])
        iso3_col = find_column(headers, ["iso3", "iso 3", "iso alpha 3"])
        usd_col = find_column(headers, ["price usd", "usd", "us dollar", "price"])
        year_col = find_column(headers, ["year", "data year", "collection year"])
        if usd_col is None:
            continue
        used_sheets[kind] = ws.title

        for row in rows[header_i + 1:]:
            if not row:
                continue
            iso3 = str(row[iso3_col]).upper().strip() if iso3_col is not None and row[iso3_col] else ""
            country = str(row[country_col]).strip() if country_col is not None and row[country_col] else ""
            price = numeric(row[usd_col])
            if not price or price < 0 or (not iso3 and not country):
                continue
            year = numeric(row[year_col]) if year_col is not None else 2025
            if year is None:
                year = 2025
            key = iso3 or normalize_name(country)
            old = result[kind].get(key)
            if old is None or year >= old[1]:
                result[kind][key] = (price, int(year), country)

    wb.close()
    if not any(result.values()):
        raise RuntimeError("ITU workbook downloaded but no recognized price sheets/rows were found")
    return result, used_sheets


def wb_series(indicator):
    params = urllib.parse.urlencode({
        "format": "json", "per_page": 20000, "date": "2020:2025"
    })
    raw = get_json(f"{WB_API}/country/all/indicator/{indicator}?{params}")
    if not isinstance(raw, list) or len(raw) < 2:
        return {}
    out = {}
    for record in raw[1]:
        iso = record.get("countryiso3code")
        value = record.get("value")
        year = record.get("date")
        if not iso or value is None:
            continue
        try:
            year_i, value_f = int(year), float(value)
        except (TypeError, ValueError):
            continue
        if iso not in out or year_i > out[iso][0]:
            out[iso] = (year_i, value_f)
    return out


def money(value):
    if value is None:
        return "Not available"
    return f"${value:.2f}/mo" if value < 10 else f"${value:.1f}/mo"


def quality_grade(internet, fixed):
    # Transparent project scale: Internet use is the primary broad-access
    # measure; fixed broadband density is a secondary infrastructure measure.
    if internet is None and fixed is None:
        return "N/A"
    if internet is None:
        score = min(100.0, fixed * 2.0)
    elif fixed is None:
        score = internet
    else:
        score = internet * 0.70 + min(100.0, fixed * 2.0) * 0.30
    if score >= 85:
        return "A"
    if score >= 70:
        return "B"
    if score >= 50:
        return "C"
    if score >= 30:
        return "D"
    return "E"


def wifi_value():
    return "Wi-Fi 4/5/6 deployed; 6 GHz/6E and Wi-Fi 7 depend on local spectrum rules"


def cellular_value(price_available):
    if price_available:
        return "3G+ mobile broadband (ITU 5GB basket); 4G/5G not separately asserted"
    return "Mobile broadband service data unavailable"


def wimax_value(fixed_available):
    return "Fixed wireless/FWA may coexist; WiMAX not separately reported" if fixed_available else "FWA/WiMAX not separately reported"


def lookup_price(table, iso3, country_name, aliases=None):
    aliases = aliases or {}
    item = table.get(iso3)
    if item:
        return item
    key = normalize_name(country_name)
    item = table.get(key)
    if item:
        return item
    for alt in aliases.get(key, []):
        item = table.get(normalize_name(alt))
        if item:
            return item
    return None


def build_rows(entries, iso_by_name, prices, internet, fixed):
    rows = []
    for number, country in entries:
        pair = iso_by_name.get(normalize_name(country))
        if not pair:
            rows.append(f"| {number:03d} | {country} | PROJECT / N/A | N/A | N/A | N/A | N/A | N/A | N/A | N/A |")
            continue
        a2, a3 = pair
        data_only = prices["data_only"].get(a3)
        fixed_p = prices["fixed"].get(a3)
        high = prices["mobile_high"].get(a3)
        internet_item = internet.get(a3)
        fixed_item = fixed.get(a3)
        internet_v = internet_item[1] if internet_item else None
        fixed_v = fixed_item[1] if fixed_item else None
        q = quality_grade(internet_v, fixed_v)
        entry = f"{money(data_only[0])} (ITU {data_only[1]}; 5 GB mobile data)" if data_only else "Not available from ITU 2025 basket"
        standard = f"{money(fixed_p[0])} (ITU {fixed_p[1]}; 5 GB fixed broadband)" if fixed_p else "Not available from ITU 2025 basket"
        premium = f"{money(high[0])} (ITU {high[1]}; 140 min + 20 SMS + 5 GB mobile)" if high else "Not available from ITU 2025 basket"
        rows.append(f"| {number:03d} | {country} | {a2} / {a3} | {wifi_value()} | {cellular_value(bool(data_only))} | {wimax_value(bool(fixed_p))} | {entry} | {standard} | {premium} | {q} |")
    return rows


def replace_generated(text, rows, sheets):
    header = [
        BEGIN,
        "",
        "**Generated from `http/spec/BYPASS.md` using free, public data sources.**",
        "",
        "Price tiers are standardized ITU baskets: Entry = 5 GB data-only mobile; Standard = 5 GB fixed broadband; Premium = 140 minutes + 20 SMS + 5 GB mobile high-consumption. Prices are nominal USD/month for the ITU 2025 collection where available. Quality grades are project-derived from World Bank WDI indicators sourced from ITU; they are not official ITU rankings.",
        "",
        "| # | Country / World Entry | ISO | Wi-Fi standards | Cellular technology | WiMAX / FWA | Entry Internet tier | Standard Internet tier | Premium Internet tier | Quality |",
        "|---:|---|---|---|---|---|---|---|---|---|",
    ]
    generated = "\n".join(header + rows + ["", f"**Taxonomy entries:** {len(rows)}", "**Public sources:** ITU 2025 ICT Price Basket workbook; World Bank World Development Indicators (ITU-sourced).", f"**ITU sheets used:** `{json.dumps(sheets, sort_keys=True)}`", "", END])
    if BEGIN in text and END in text:
        before = text.split(BEGIN, 1)[0].rstrip()
        after = text.split(END, 1)[1].lstrip()
        return before + "\n\n" + generated + ("\n\n" + after if after else "\n")
    return text.rstrip() + "\n\n" + generated + "\n"


def main():
    entries = load_bypass_entries()
    iso_by_name = load_iso_dataset()

    with tempfile.NamedTemporaryFile(suffix=".xlsx", delete=False) as tmp:
        workbook_path = Path(tmp.name)
        tmp.write(get_bytes(ITU_XLSX))

    try:
        prices_raw, sheets = parse_itu_workbook(workbook_path)
    finally:
        workbook_path.unlink(missing_ok=True)

    # Normalize workbook keys that are country names rather than ISO3 codes.
    prices = {"data_only": {}, "fixed": {}, "mobile_high": {}}
    iso_reverse = {normalize_name(name): pair[1] for name, pair in iso_by_name.items()}
    for kind, table in prices_raw.items():
        for key, value in table.items():
            iso3 = key.upper() if re.fullmatch(r"[A-Z]{3}", str(key).upper()) else iso_reverse.get(normalize_name(key))
            if iso3:
                prices[kind][iso3] = value

    internet = wb_series("IT.NET.USER.ZS")
    fixed = wb_series("IT.NET.BBND.P2")

    existing = OUT.read_text(encoding="utf-8") if OUT.exists() else "# TEELTAILS.md.ms.max\n"
    rows = build_rows(entries, iso_by_name, prices, internet, fixed)
    OUT.write_text(replace_generated(existing, rows, sheets), encoding="utf-8")
    populated = sum("PROJECT / N/A" not in row for row in rows)
    priced = sum("ITU 2025" in row for row in rows)
    print(f"Generated TEELTAILS: {len(rows)} taxonomy entries; {populated} ISO-resolved rows; {priced} rows with ITU pricing; sheets={sheets}")


if __name__ == "__main__":
    main()
