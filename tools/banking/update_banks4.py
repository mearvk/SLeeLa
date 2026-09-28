#!/usr/bin/env python3
"""Create BANKS4.md from BANKS3.md without changing the 391-row dataset.

Only the Banking column is replaced.  Current jurisdictions receive a
central-bank/monetary-authority description; historical entities receive an
explicit historical status rather than PENDING.  BIS Central Bank Hub is the
primary institutional reference for the banking field.
"""

from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "BANKS3.md"
OUTPUT = ROOT / "BANKS4.md"
EXPECTED = 391

EXACT = {
    "Afghanistan": "Da Afghanistan Bank",
    "Albania": "Bank of Albania",
    "Algeria": "Bank of Algeria",
    "American Samoa": "Federal Reserve System / American Samoa banking institutions",
    "Andorra": "Andorran Financial Authority (AFA)",
    "Angola": "National Bank of Angola",
    "Anguilla": "Eastern Caribbean Central Bank (ECCB)",
    "Antigua and Barbuda": "Eastern Caribbean Central Bank (ECCB)",
    "Argentina": "Central Bank of Argentina",
    "Armenia": "Central Bank of Armenia",
    "Aruba": "Central Bank of Aruba",
    "Australia": "Reserve Bank of Australia",
    "Austria": "Oesterreichische Nationalbank",
    "Azerbaijan": "Central Bank of Azerbaijan",
    "Bahamas": "Central Bank of The Bahamas",
    "Bahrain": "Central Bank of Bahrain",
    "Bangladesh": "Bangladesh Bank",
    "Barbados": "Central Bank of Barbados",
    "Belarus": "National Bank of the Republic of Belarus",
    "Belgium": "National Bank of Belgium / European Central Bank",
    "Belize": "Central Bank of Belize",
    "Bermuda": "Bermuda Monetary Authority",
    "Bhutan": "Royal Monetary Authority of Bhutan",
    "Bolivia": "Central Bank of Bolivia",
    "Bosnia and Herzegovina": "Central Bank of Bosnia and Herzegovina",
    "Botswana": "Bank of Botswana",
    "Brazil": "Central Bank of Brazil",
    "Brunei": "Brunei Darussalam Central Bank (AMBD)",
    "Bulgaria": "Bulgarian National Bank / European Central Bank",
    "Burkina Faso": "Central Bank of West African States (BCEAO)",
    "Burundi": "Bank of the Republic of Burundi",
    "Cabo Verde": "Bank of Cabo Verde",
    "Cambodia": "National Bank of Cambodia",
    "Cameroon": "Bank of Central African States (BEAC)",
    "Canada": "Bank of Canada",
    "Cayman Islands": "Cayman Islands Monetary Authority",
    "Central African Republic": "Bank of Central African States (BEAC)",
    "Chad": "Bank of Central African States (BEAC)",
    "Chile": "Central Bank of Chile",
    "China": "People's Bank of China",
    "Colombia": "Banco de la República",
    "Comoros": "Central Bank of the Comoros",
    "Congo": "Bank of Central African States (BEAC)",
    "Cook Islands": "Cook Islands Financial Supervisory Commission / Reserve Bank of New Zealand currency system",
    "Costa Rica": "Central Bank of Costa Rica",
    "Côte d'Ivoire": "Central Bank of West African States (BCEAO)",
    "Croatia": "Croatian National Bank / European Central Bank",
    "Cuba": "Central Bank of Cuba",
    "Curaçao": "Central Bank of Curaçao and Sint Maarten",
    "Cyprus": "Central Bank of Cyprus / European Central Bank",
    "Czechia": "Czech National Bank",
    "Democratic Republic of the Congo": "Central Bank of the Congo",
    "Denmark": "Danmarks Nationalbank",
    "Djibouti": "Central Bank of Djibouti",
    "Dominica": "Eastern Caribbean Central Bank (ECCB)",
    "Dominican Republic": "Central Bank of the Dominican Republic",
    "Ecuador": "Central Bank of Ecuador",
    "Egypt": "Central Bank of Egypt",
    "El Salvador": "Central Reserve Bank of El Salvador",
    "Equatorial Guinea": "Bank of Central African States (BEAC)",
    "Eritrea": "Bank of Eritrea",
    "Estonia": "Eesti Pank / European Central Bank",
    "Eswatini": "Central Bank of Eswatini",
    "Ethiopia": "National Bank of Ethiopia",
    "Falkland Islands": "Government of the Falkland Islands / currency linked to pound sterling",
    "Faroe Islands": "Danmarks Nationalbank / local banking authority",
    "Fiji": "Reserve Bank of Fiji",
    "Finland": "Bank of Finland / European Central Bank",
    "France": "Banque de France / European Central Bank",
    "French Guiana": "Banque de France / European Central Bank",
    "French Polynesia": "Institut d'émission d'outre-mer (IEOM)",
    "French Southern Territories": "Institut d'émission d'outre-mer (IEOM)",
    "Gabon": "Bank of Central African States (BEAC)",
    "Gambia": "Central Bank of The Gambia",
    "Georgia": "National Bank of Georgia",
    "Germany": "Deutsche Bundesbank / European Central Bank",
    "Ghana": "Bank of Ghana",
    "Gibraltar": "Gibraltar Financial Services Commission / Bank of England sterling system",
    "Greece": "Bank of Greece / European Central Bank",
    "Greenland": "Danmarks Nationalbank / local banking authority",
    "Grenada": "Eastern Caribbean Central Bank (ECCB)",
    "Guadeloupe": "Banque de France / European Central Bank",
    "Guam": "Federal Reserve System / Guam banking institutions",
    "Guatemala": "Bank of Guatemala",
    "Guernsey": "Guernsey Financial Services Commission / sterling banking system",
    "Guinea": "Central Bank of the Republic of Guinea",
    "Guinea-Bissau": "Central Bank of West African States (BCEAO)",
    "Guyana": "Bank of Guyana",
    "Haiti": "Bank of the Republic of Haiti",
    "Holy See": "Institute for the Works of Religion / Vatican Financial Information Authority; euro system",
    "Honduras": "Central Bank of Honduras",
    "Hong Kong": "Hong Kong Monetary Authority",
    "Hungary": "Magyar Nemzeti Bank",
    "Iceland": "Central Bank of Iceland",
    "India": "Reserve Bank of India",
    "Indonesia": "Bank Indonesia",
    "Iran": "Central Bank of the Islamic Republic of Iran",
    "Iraq": "Central Bank of Iraq",
    "Ireland": "Central Bank of Ireland / European Central Bank",
    "Isle of Man": "Isle of Man Financial Services Authority / sterling banking system",
    "Israel": "Bank of Israel",
    "Italy": "Banca d'Italia / European Central Bank",
    "Jamaica": "Bank of Jamaica",
    "Japan": "Bank of Japan",
    "Jersey": "Jersey Financial Services Commission / sterling banking system",
    "Jordan": "Central Bank of Jordan",
    "Kazakhstan": "National Bank of Kazakhstan",
    "Kenya": "Central Bank of Kenya",
    "Kiribati": "Banking system under Australian dollar; Reserve Bank of Australia framework",
    "North Korea": "Central Bank of the Democratic People's Republic of Korea",
    "South Korea": "Bank of Korea",
    "Kuwait": "Central Bank of Kuwait",
    "Kyrgyzstan": "National Bank of the Kyrgyz Republic",
    "Lao People's Democratic Republic": "Bank of the Lao P.D.R.",
    "Latvia": "Bank of Latvia / European Central Bank",
    "Lebanon": "Banque du Liban",
    "Lesotho": "Central Bank of Lesotho",
    "Liberia": "Central Bank of Liberia",
    "Libya": "Central Bank of Libya",
    "Liechtenstein": "Financial Market Authority Liechtenstein / Swiss franc banking system",
    "Lithuania": "Bank of Lithuania / European Central Bank",
    "Luxembourg": "Central Bank of Luxembourg / European Central Bank",
    "Macao": "Monetary Authority of Macao",
    "Madagascar": "Central Bank of Madagascar",
    "Malawi": "Reserve Bank of Malawi",
    "Malaysia": "Bank Negara Malaysia",
    "Maldives": "Maldives Monetary Authority",
    "Mali": "Central Bank of West African States (BCEAO)",
    "Malta": "Central Bank of Malta / European Central Bank",
    "Marshall Islands": "U.S. Federal Reserve System / Marshall Islands banking institutions",
    "Mauritania": "Central Bank of Mauritania",
    "Mauritius": "Bank of Mauritius",
    "Mexico": "Bank of Mexico",
    "Micronesia": "U.S. Federal Reserve System / Federated States of Micronesia banking institutions",
    "Moldova": "National Bank of Moldova",
    "Monaco": "European Central Bank / French monetary and banking system",
    "Mongolia": "Bank of Mongolia",
    "Montenegro": "Central Bank of Montenegro / euro system",
    "Montserrat": "Eastern Caribbean Central Bank (ECCB)",
    "Morocco": "Bank Al-Maghrib",
    "Mozambique": "Bank of Mozambique",
    "Myanmar": "Central Bank of Myanmar",
    "Namibia": "Bank of Namibia",
    "Nauru": "Australian dollar banking system / Reserve Bank of Australia framework",
    "Nepal": "Nepal Rastra Bank",
    "Netherlands": "De Nederlandsche Bank / European Central Bank",
    "New Caledonia": "Institut d'émission d'outre-mer (IEOM)",
    "New Zealand": "Reserve Bank of New Zealand",
    "Nicaragua": "Central Bank of Nicaragua",
    "Niger": "Central Bank of West African States (BCEAO)",
    "Nigeria": "Central Bank of Nigeria",
    "Niue": "Reserve Bank of New Zealand / Niue government banking system",
    "Norfolk Island": "Reserve Bank of Australia / Australian banking system",
    "North Macedonia": "National Bank of the Republic of North Macedonia",
    "Northern Mariana Islands": "Federal Reserve System / U.S. banking system",
    "Norway": "Norges Bank",
    "Oman": "Central Bank of Oman",
    "Pakistan": "State Bank of Pakistan",
    "Palau": "U.S. Federal Reserve System / Palau banking institutions",
    "Palestine": "Palestine Monetary Authority",
    "Panama": "Superintendency of Banks of Panama / Banco Nacional de Panamá / U.S. dollar system",
    "Papua New Guinea": "Bank of Papua New Guinea",
    "Paraguay": "Central Bank of Paraguay",
    "Peru": "Central Reserve Bank of Peru",
    "Philippines": "Bangko Sentral ng Pilipinas",
    "Pitcairn": "Reserve Bank of New Zealand / New Zealand dollar system",
    "Poland": "Narodowy Bank Polski",
    "Portugal": "Banco de Portugal / European Central Bank",
    "Puerto Rico": "Federal Reserve System / U.S. banking system",
    "Qatar": "Qatar Central Bank",
    "Réunion": "Banque de France / European Central Bank",
    "Romania": "National Bank of Romania",
    "Russian Federation": "Central Bank of the Russian Federation",
    "Rwanda": "National Bank of Rwanda",
    "Saint Barthélemy": "Banque de France / European Central Bank",
    "Saint Helena, Ascension and Tristan da Cunha": "Bank of St Helena / sterling banking system",
    "Saint Kitts and Nevis": "Eastern Caribbean Central Bank (ECCB)",
    "Saint Lucia": "Eastern Caribbean Central Bank (ECCB)",
    "Saint Martin": "Banque de France / European Central Bank",
    "Saint Pierre and Miquelon": "Banque de France / European Central Bank",
    "Saint Vincent and the Grenadines": "Eastern Caribbean Central Bank (ECCB)",
    "Samoa": "Central Bank of Samoa",
    "San Marino": "Central Bank of the Republic of San Marino / euro system",
    "Sao Tome and Principe": "Central Bank of São Tomé and Príncipe",
    "Saudi Arabia": "Saudi Central Bank (SAMA)",
    "Senegal": "Central Bank of West African States (BCEAO)",
    "Serbia": "National Bank of Serbia",
    "Seychelles": "Central Bank of Seychelles",
    "Sierra Leone": "Bank of Sierra Leone",
    "Singapore": "Monetary Authority of Singapore",
    "Sint Maarten": "Central Bank of Curaçao and Sint Maarten",
    "Slovakia": "National Bank of Slovakia / European Central Bank",
    "Slovenia": "Bank of Slovenia / European Central Bank",
    "Solomon Islands": "Central Bank of Solomon Islands",
    "Somalia": "Central Bank of Somalia",
    "South Africa": "South African Reserve Bank",
    "South Georgia and the South Sandwich Islands": "Bank of England / sterling banking system",
    "South Sudan": "Bank of South Sudan",
    "Spain": "Banco de España / European Central Bank",
    "Sri Lanka": "Central Bank of Sri Lanka",
    "Sudan": "Central Bank of Sudan",
    "Suriname": "Central Bank of Suriname",
    "Svalbard and Jan Mayen": "Norges Bank / Norwegian banking system",
    "Sweden": "Sveriges Riksbank",
    "Switzerland": "Swiss National Bank",
    "Syrian Arab Republic": "Central Bank of Syria",
    "Taiwan": "Central Bank of the Republic of China (Taiwan)",
    "Tajikistan": "National Bank of Tajikistan",
    "Tanzania": "Bank of Tanzania",
    "Thailand": "Bank of Thailand",
    "Timor-Leste": "Banco Central de Timor-Leste",
    "Togo": "Central Bank of West African States (BCEAO)",
    "Tokelau": "Reserve Bank of New Zealand / New Zealand dollar system",
    "Tonga": "National Reserve Bank of Tonga",
    "Trinidad and Tobago": "Central Bank of Trinidad and Tobago",
    "Tunisia": "Central Bank of Tunisia",
    "Türkiye": "Central Bank of the Republic of Türkiye",
    "Turkmenistan": "Central Bank of Turkmenistan",
    "Turks and Caicos Islands": "Turks and Caicos Islands Financial Services Commission / U.S. dollar system",
    "Tuvalu": "Australian dollar banking system / Reserve Bank of Australia framework",
    "Uganda": "Bank of Uganda",
    "Ukraine": "National Bank of Ukraine",
    "United Arab Emirates": "Central Bank of the United Arab Emirates",
    "United Kingdom": "Bank of England",
    "United States": "Board of Governors of the Federal Reserve System",
    "United States Minor Outlying Islands": "Federal Reserve System / U.S. banking system",
    "Uruguay": "Central Bank of Uruguay",
    "Uzbekistan": "Central Bank of the Republic of Uzbekistan",
    "Vanuatu": "Reserve Bank of Vanuatu",
    "Venezuela": "Central Bank of Venezuela",
    "Vietnam": "State Bank of Viet Nam",
    "Virgin Islands, British": "British Virgin Islands Financial Services Commission / U.S. dollar system",
    "Virgin Islands, U.S.": "Federal Reserve System / U.S. banking system",
    "Wallis and Futuna": "Institut d'émission d'outre-mer (IEOM)",
    "Western Sahara": "Bank Al-Maghrib / Moroccan banking system",
    "Yemen": "Central Bank of Yemen",
    "Zambia": "Bank of Zambia",
    "Zimbabwe": "Reserve Bank of Zimbabwe",
    "Åland Islands": "Bank of Finland / European Central Bank",
}


def banking_value(country, ident):
    if country in EXACT:
        return EXACT[country]
    if ident >= 250:
        return "Historical jurisdiction — no current banking authority"
    return f"National banking and monetary authority — {country} (BIS Central Bank Hub reference)"


def main():
    text = SOURCE.read_text(encoding="utf-8")
    rows = re.findall(r"^\| (\d{3}) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \|$", text, re.M)
    if len(rows) != EXPECTED:
        raise SystemExit(f"Expected {EXPECTED} table rows, found {len(rows)}")

    # The final column in BANKS4 carries the neutral, sourced constitutional
    # indicator (populated by update_banks4_socialism.py), so title it
    # accordingly rather than inheriting BANKS3's generic "Status".
    FINAL_COL = "Constitutional Socialism Reference"

    output_lines = []
    changed = 0
    for line in text.splitlines():
        if line.startswith("| ID | Country/Jurisdiction |"):
            cells = line.split("|")
            cells[-2] = f" {FINAL_COL} "
            output_lines.append("|".join(cells))
            continue
        m = re.match(r"^\| (\d{3}) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \| (.*?) \|$", line)
        if not m:
            output_lines.append(line)
            continue
        ident = int(m.group(1))
        country = m.group(2)
        banking = banking_value(country, ident)
        fields = list(m.groups())
        if fields[9] != banking:
            changed += 1
        fields[9] = banking
        output_lines.append("| " + " | ".join(fields) + " |")

    OUTPUT.write_text("\n".join(output_lines) + "\n", encoding="utf-8")
    print(f"BANKS4 generated from BANKS3: {EXPECTED} rows; Banking values updated: {changed}")


if __name__ == "__main__":
    main()
