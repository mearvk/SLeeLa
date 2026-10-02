#!/usr/bin/env python3
"""Refresh preferred-routers.csv/json from current PeeringDB country records.

This script is deliberately opt-in: the checked-in catalog remains usable offline.
It queries PeeringDB for networks, exchanges and facilities by ISO country code,
then selects up to four records per country by routing role. No OS route table
changes are performed.
"""
import csv, json, sys, urllib.parse, urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent
CSV_PATH = ROOT / "preferred-routers.csv"
JSON_PATH = ROOT / "preferred-routers.json"
API = "https://www.peeringdb.com/api"

ROLES = [
    ("national_backbone", "net", 5),
    ("national_ixp", "ix", 5),
    ("regional_carrier", "net", 4),
    ("international_dc", "fac", 4),
]

def get(kind, country):
    q = urllib.parse.urlencode({"country": country, "limit": 20, "depth": 1})
    with urllib.request.urlopen(f"{API}/{kind}?{q}", timeout=20) as r:
        return json.load(r).get("data", [])

def choose(items, fallback):
    if not items:
        return fallback
    def key(x):
        return (-int(x.get("net_count", 0) or 0),
                -int(x.get("carrier_count", 0) or 0),
                str(x.get("name", "")).lower())
    return sorted(items, key=key)[0].get("name") or fallback

def main():
    if not CSV_PATH.exists() or not JSON_PATH.exists():
        raise SystemExit("preferred-routers.csv/json must exist before refresh")
    with CSV_PATH.open(newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))
    countries = []
    seen = set()
    for row in rows:
        if row["country_code"] not in seen:
            seen.add(row["country_code"])
            countries.append((row["country_code"], row["country"]))

    out = []
    for n, (code, name) in enumerate(countries, 1):
        try:
            nets = get("net", code)
            ixs = get("ix", code)
            facs = get("fac", code)
        except Exception as exc:
            print(f"[{n}/{len(countries)}] {code}: refresh failed: {exc}", file=sys.stderr)
            out.extend([r for r in rows if r["country_code"] == code])
            continue

        candidates = {
            "national_backbone": choose(nets, f"{name} — national backbone / primary ISP anchor"),
            "national_ixp": choose(ixs, f"{name} — national or primary Internet Exchange anchor"),
            "regional_carrier": choose(nets[1:] if len(nets) > 1 else nets, f"{name} — regional carrier / upstream ISP anchor"),
            "international_dc": choose(facs, f"{name} — international carrier / data-center interconnect anchor"),
        }
        for r in rows:
            if r["country_code"] != code:
                continue
            role = r["role"]
            if candidates[role] and "— " not in candidates[role]:
                r["candidate"] = candidates[role]
                r["selection_mode"] = "peeringdb-current"
                r["source"] = "PeeringDB API"
                r["source_url"] = "https://www.peeringdb.com/"
                r["notes"] = "Routing preference tier, not a performance or safety guarantee."
            out.append(r)
        print(f"[{n}/{len(countries)}] {code}", file=sys.stderr)

    with CSV_PATH.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=out[0].keys())
        w.writeheader()
        w.writerows(out)
    with JSON_PATH.open("r", encoding="utf-8") as f:
        doc = json.load(f)
    doc["generated"] = __import__("datetime").date.today().isoformat()
    doc["entries"] = out
    doc["data_status"] = "refreshed from PeeringDB where records were available"
    with JSON_PATH.open("w", encoding="utf-8") as f:
        json.dump(doc, f, ensure_ascii=False, indent=2)
        f.write("\n")

if __name__ == "__main__":
    main()
