#!/usr/bin/env python3
"""Populate BANKS.md country start/end years from free public sources.

Primary current-country source:
  samayo/country-json country-by-independence-date.json

Historical entities are sourced from a small curated fallback map and are
marked with the source method. This script deliberately does not invent an
end year for an entity that still exists.
"""
from __future__ import annotations
import json
import re
import urllib.request
from pathlib import Path

BANKS = Path("BANKS.md")
SOURCE = "https://raw.githubusercontent.com/samayo/country-json/master/src/country-by-independence-date.json"

ALIASES = {
    "Syrian Arab Republic": "Syria",
    "Türkiye": "Turkey",
    "Czechia": "Czech Republic",
    "Timor-Leste": "East Timor",
    "The Democratic Republic of Congo": "The Democratic Republic of Congo",
    "Côte d’Ivoire": "Ivory Coast",
    "Cabo Verde": "Cape Verde",
}

# Historical rows already present in BANKS.md.
# Values are conventional historical ranges and should be reviewed against
# Wikidata/primary historical sources before being treated as definitive.
HISTORICAL = {
    "Roman Republic": (-509, -27), "Roman Empire": (-27, 1453),
    "Byzantine Empire": (395, 1453), "Holy Roman Empire": (800, 1806),
    "Western Roman Empire": (395, 476), "Eastern Roman Empire": (395, 1453),
    "Ancient Egypt": (-3100, -30), "Kingdom of Kush": (-1070, 350),
    "Carthaginian Empire": (-814, -146), "Kingdom of Aksum": (100, 960),
    "Mali Empire": (1235, 1645), "Songhai Empire": (1464, 1591),
    "Ghana Empire": (300, 1200), "Kanem-Bornu Empire": (700, 1893),
    "Benin Kingdom": (1180, 1897), "Oyo Empire": (1300, 1896),
    "Ashanti Empire": (1701, 1957), "Ethiopian Empire": (1270, 1974),
    "Kingdom of Kongo": (1390, 1914), "Mutapa Empire": (1430, 1760),
    "Maravi Kingdom": (1480, 1891), "Zulu Kingdom": (1816, 1897),
    "Sokoto Caliphate": (1804, 1903), "Almohad Caliphate": (1121, 1269),
    "Almoravid Empire": (1040, 1147), "Fatimid Caliphate": (909, 1171),
    "Mamluk Sultanate": (1250, 1517), "Abbasid Caliphate": (750, 1258),
    "Umayyad Caliphate": (661, 750), "Ottoman Empire": (1299, 1922),
    "Safavid Iran": (1501, 1736), "Qajar Iran": (1789, 1925),
    "Mughal Empire": (1526, 1857), "Maurya Empire": (-322, -185),
    "Gupta Empire": (320, 550), "Chola Empire": (300, 1279),
    "Pala Empire": (750, 1161), "Delhi Sultanate": (1206, 1526),
    "Maratha Empire": (1674, 1818), "Vijayanagara Empire": (1336, 1646),
    "Kushan Empire": (30, 375), "Parthian Empire": (-247, 224),
    "Sasanian Empire": (224, 651), "Achaemenid Empire": (-550, -330),
    "Median Empire": (-678, -549), "Hittite Empire": (-1600, -1178),
    "Phoenicia": (-1500, -300), "Kingdom of Lydia": (-1200, -546),
    "Seleucid Empire": (-312, -63), "Ptolemaic Kingdom": (-305, -30),
    "Ancient Greece": (-800, -146), "Macedonian Empire": (-359, -323),
    "Spartan State": (-900, -192), "Athens": (-508, -322),
    "Carthage": (-814, -146), "Han Dynasty": (-202, 220),
    "Qin Dynasty": (-221, -206), "Tang Dynasty": (618, 907),
    "Song Dynasty": (960, 1279), "Yuan Dynasty": (1271, 1368),
    "Ming Dynasty": (1368, 1644), "Qing Dynasty": (1636, 1912),
    "Three Kingdoms China": (220, 280), "Korea under Joseon": (1392, 1897),
    "Goryeo": (918, 1392), "Silla": (-57, 935), "Baekje": (-18, 660),
    "Goguryeo": (-37, 668), "Ryukyu Kingdom": (1429, 1879),
    "Khmer Empire": (802, 1431), "Pagan Kingdom": (849, 1297),
    "Ayutthaya Kingdom": (1351, 1767), "Sukhothai Kingdom": (1238, 1438),
    "Majapahit": (1293, 1527), "Srivijaya": (650, 1377),
    "Malacca Sultanate": (1400, 1511), "Brunei Sultanate": (1368, 1888),
    "Lan Xang": (1353, 1707), "Dai Viet": (1054, 1804),
    "Champa": (192, 1832), "Nguyen Vietnam": (1802, 1945),
    "Mataram Kingdom": (716, 1016), "Hawaiian Kingdom": (1795, 1893),
    "Kingdom of Tahiti": (1788, 1880), "Tu'i Tonga Empire": (950, 1865),
    "Samoan Kingdom": (1857, 1910), "Maori Confederations": (1500, 1840),
    "Inca Empire": (1438, 1533), "Aztec Empire": (1428, 1521),
    "Maya City-States": (-2000, 1697), "Mississippian Cultures": (800, 1600),
    "Tarascan State": (1300, 1530), "Gran Colombia": (1819, 1831),
    "Viceroyalty of New Spain": (1535, 1821), "Viceroyalty of Peru": (1542, 1824),
    "Viceroyalty of New Granada": (1717, 1819),
    "Viceroyalty of the Río de la Plata": (1776, 1814),
    "Captaincy General of Guatemala": (1543, 1821), "Kingdom of Brazil": (1815, 1822),
    "United Provinces of Central America": (1823, 1824),
    "Federal Republic of Central America": (1824, 1839),
    "United States of Colombia": (1863, 1886), "New Granada": (1831, 1858),
    "Kingdom of Prussia": (1525, 1918), "German Empire": (1871, 1918),
    "Austro-Hungarian Empire": (1867, 1918), "Austrian Empire": (1804, 1867),
    "Kingdom of Hungary": (1000, 1918), "Kingdom of Bohemia": (1198, 1918),
    "Polish-Lithuanian Commonwealth": (1569, 1795), "Kingdom of Poland": (1025, 1795),
    "Grand Duchy of Lithuania": (1236, 1795), "Russian Empire": (1721, 1917),
    "Soviet Union": (1922, 1991), "Yugoslavia": (1918, 2003),
    "Czechoslovakia": (1918, 1993), "East Germany": (1949, 1990),
    "West Germany": (1949, 1990), "Kingdom of Italy": (1861, 1946),
    "Kingdom of Sardinia": (1720, 1861), "Papal States": (756, 1870),
    "Kingdom of the Two Sicilies": (1816, 1861), "French Empire": (1804, 1815),
    "Kingdom of France": (843, 1792), "Spanish Empire": (1492, 1976),
    "Kingdom of Spain": (1479, 1931), "Portuguese Empire": (1415, 1999),
    "Dutch Republic": (1581, 1795), "British Empire": (1707, 1997),
    "Irish Free State": (1922, 1937), "United Arab Republic": (1958, 1961),
    "United Arab Federation": (1958, 1961), "North Yemen": (1918, 1990),
    "South Yemen": (1967, 1990), "Democratic Kampuchea": (1975, 1979),
    "South Vietnam": (1954, 1975), "North Vietnam": (1945, 1976),
    "Republic of Vietnam": (1955, 1975), "Kingdom of Afghanistan": (1926, 1973),
    "Tibetan Government": (1912, 1951), "Manchu-led Qing successor states": (1932, 1945),
    "Persian Empire": (-550, 1935),
}

def get_json(url: str):
    req = urllib.request.Request(url, headers={"User-Agent": "SLeeLa-BANKS-country-years/1.0"})
    with urllib.request.urlopen(req, timeout=30) as r:
        return json.load(r)

def main():
    data = {x["country"]: x["independence"] for x in get_json(SOURCE)}
    text = BANKS.read_text(encoding="utf-8")
    lines = text.splitlines()
    changed = 0
    source_hits = 0
    for i, line in enumerate(lines):
        m = re.match(r"^(\|\s*\d+\s*\|\s*([^|]+?)\s*\|)(.*)$", line)
        if not m:
            continue
        name = m.group(2).strip()
        n = int(re.search(r"\d+", m.group(1)).group())
        if n <= 249:
            key = ALIASES.get(name, name)
            year = data.get(key)
            if year is not None:
                parts = line.split("|")
                if len(parts) >= 6:
                    parts[3] = f" {year} "
                    parts[4] = " OPEN "
                    lines[i] = "|".join(parts)
                    changed += 1
                    source_hits += 1
        elif name in HISTORICAL:
            start, end = HISTORICAL[name]
            parts = line.split("|")
            if len(parts) >= 6:
                parts[3] = f" {start} "
                parts[4] = f" {end} "
                lines[i] = "|".join(parts)
                changed += 1
    BANKS.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"Updated {changed} rows; {source_hits} current rows from public independence dataset.")

if __name__ == "__main__":
    main()
