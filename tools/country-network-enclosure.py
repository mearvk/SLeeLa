#!/usr/bin/env python3
"""Build the country-level network/geodata appendix for SLeeLa.

Sources:
- REST Countries v3.1 for country identity, geography and basic political metadata.
- ipverse/country-ip-blocks for daily RIR-derived country IP prefixes.

The generator intentionally records aggregate prefixes only; it does not enumerate
individual hosts, customer endpoints, private addresses, or exposed services.
Curated ISP, e-mail, water-price, and U.S.-interoperability fields can be supplied
through http/spec/country_network_overrides.json and are preserved across runs.
"""
from __future__ import annotations

import json
import re
import urllib.request
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DOC = ROOT / "http/spec/COUNTRY_NETWORK_ENCLOSURE.md"
OVERRIDES = ROOT / "http/spec/country_network_overrides.json"
COUNTRIES_URL = "https://restcountries.com/v3.1/all?fields=name,cca2,cca3,capital,latlng,region,subregion,area,borders,landlocked,tld,timezones,currencies"
IP_URL = "https://raw.githubusercontent.com/ipverse/country-ip-blocks/master/country/{code}/aggregated.json"


def get_json(url: str):
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-country-network-workflow/1.1"})
    with urllib.request.urlopen(req, timeout=60) as response:
        return json.load(response)


def clean(value):
    if value is None:
        return "—"
    if isinstance(value, list):
        return ", ".join(str(x) for x in value if x not in (None, "")) or "—"
    return str(value).replace("|", "\\|").replace("\n", " ").strip() or "—"


def money(value):
    if isinstance(value, (int, float)):
        return f"${value:,.4f}"
    return clean(value)


def ip_summary(code: str):
    try:
        data = get_json(IP_URL.format(code=code.lower()))
        if not isinstance(data, dict):
            return f"Not retrieved (unexpected {type(data).__name__} response)"
        prefixes = data.get("prefixes", {})
        if not isinstance(prefixes, dict):
            return "Not reliably geolocated (invalid prefix data)"
        v4 = prefixes.get("ipv4", []) or []
        v6 = prefixes.get("ipv6", []) or []
        if not isinstance(v4, list) or not isinstance(v6, list):
            return "Not reliably geolocated (invalid prefix lists)"
        # Keep the document useful and bounded: counts plus a representative set.
        sample4 = v4[:12]
        sample6 = v6[:8]
        sample = ", ".join(sample4 + sample6)
        if len(v4) > 12 or len(v6) > 8:
            sample += ", …"
        return f"{sample or 'Not reliably geolocated'} (IPv4 {len(v4)}, IPv6 {len(v6)})"
    except Exception as exc:
        return f"Not retrieved ({type(exc).__name__})"


def load_overrides():
    if not OVERRIDES.exists():
        return {}
    try:
        data = json.loads(OVERRIDES.read_text(encoding="utf-8"))
        return data if isinstance(data, dict) else {}
    except Exception:
        return {}


def normalize_countries(data):
    """Normalize the REST Countries response and fail clearly on bad API data."""
    if isinstance(data, list):
        return [item for item in data if isinstance(item, dict)]
    if isinstance(data, dict):
        # Some API/proxy layers may wrap the country list in a conventional key.
        for key in ("countries", "data", "results"):
            value = data.get(key)
            if isinstance(value, list):
                return [item for item in value if isinstance(item, dict)]
    raise RuntimeError(
        "REST Countries returned an unexpected response type: "
        f"{type(data).__name__}. Expected a list of country objects."
    )


def replace_generated_section(text: str, section: str) -> str:
    start = "<!-- BEGIN GENERATED COUNTRY NETWORK TABLE -->"
    end = "<!-- END GENERATED COUNTRY NETWORK TABLE -->"
    block = f"{start}\n{section.rstrip()}\n{end}"
    if start in text and end in text:
        pattern = re.compile(re.escape(start) + r".*?" + re.escape(end), re.S)
        return pattern.sub(block, text, count=1)
    return text.rstrip() + "\n\n" + block + "\n"


def main():
    countries = normalize_countries(get_json(COUNTRIES_URL))
    overrides = load_overrides()
    countries = sorted(countries, key=lambda x: (x.get("name", {}).get("common", "")))
    generated = []
    generated.append("## Generated Country Network & Geodata Table")
    generated.append("")
    generated.append(f"**Generated:** {datetime.now(timezone.utc).strftime('%Y-%m-%d %H:%M UTC')}  ")
    generated.append("**Country source:** REST Countries 3.1  ")
    generated.append("**IP source:** ipverse country-ip-blocks (RIR-derived, daily-updated)  ")
    generated.append("")
    generated.append("> IP values below are aggregate country-associated network prefixes, not individual hosts. A prefix is an administrative/geographic association and does not prove that every address is physically inside a country's perimeter.")
    generated.append("")
    generated.append("| Country | ISO | Geodata | President / Ministers | Water USD/gal | Known ISPs | Known E-mail Companies | Known Country IP Prefixes | U.S. HTTP / Internet Interoperability |")
    generated.append("|---|---|---|---|---:|---|---|---|---|")

    for c in countries:
        name = c.get("name", {}).get("common", "Unknown")
        code = c.get("cca2", "").upper()
        o = overrides.get(code, {})
        latlng = c.get("latlng") or []
        geo = f"{latlng[0]:.5f}, {latlng[1]:.5f}" if len(latlng) >= 2 else "Not available"
        borders = ", ".join(c.get("borders") or []) or "None listed"
        geo += f"; area {c.get('area', '—')} km²; borders {borders}"
        leaders = o.get("leaders", "Not yet curated")
        water = o.get("water_usd_per_gallon", "Not reliably standardized")
        isps = o.get("isps", "Not yet curated")
        email = o.get("email_companies", "Not yet curated")
        interoperability = o.get("us_interoperability", "GET, POST, PUT, PATCH, DELETE, HEAD, OPTIONS, HTTPS/TLS, DNS, SMTP/IMAP where locally supported")
        ips = ip_summary(code) if code else "Not reliably geolocated"
        generated.append("| " + " | ".join([
            clean(name), clean(code), clean(geo), clean(leaders), money(water), clean(isps), clean(email), clean(ips), clean(interoperability)
        ]) + " |")

    current = DOC.read_text(encoding="utf-8")
    DOC.write_text(replace_generated_section(current, "\n".join(generated)), encoding="utf-8")
    print(f"Generated {len(countries)} country rows into {DOC}")


if __name__ == "__main__":
    main()
