#!/usr/bin/env python3
"""Generate per-airline website builders under lib/website/<airline>/.

Each known modern airline gets its own subdirectory under the `website`
package (nested subdirectories stay part of the first-level `website` package;
they do not add a package or a facade). Inside, one `.sleela` Master Class
encodes the airline's brand identity (name, palette, tagline, hubs) and builds a
complete, self-contained HTML website from the website model: a design
(brand/base/accent + an energy dial) plus an ordered list of semantic signs
(nav, hero, feature, testimonial, cta, footer). This mirrors
lib/website/website.sleela — pure String concatenation on OP_ADD plus
integer-to-text, no new VM opcode.

File naming is coherent to the airline: directory `<slug>-airlines/` (or the
airline's own suffix, e.g. `-airways`, `-air`) and class `<CamelName>Website`.

Usage:
  python3 lib/website/generate-airlines.py            # write all files
  python3 lib/website/generate-airlines.py --count    # print counts only
"""
import argparse
import re
import sys
from pathlib import Path

SLEELA_VERSION = "#sleela 1.3"

# (directory slug, ClassBaseName, Display Name, brand, base, accent,
#  tagline, hubs/region).
# brand = primary brand color; base = dark page background; accent = CTA color.
AIRLINES = [
    # --- North America ---
    ("piedmont-airlines", "Piedmont", "Piedmont Airlines", "#004b8d", "#0a1526", "#e4002b",
     "Regional wings of the American Eagle network.", "Salisbury, Charlotte, Philadelphia"),
    ("delta-air-lines", "Delta", "Delta Air Lines", "#003366", "#0b1220", "#e01933",
     "Keep climbing.", "Atlanta, Minneapolis, Detroit, Salt Lake City"),
    ("american-airlines", "American", "American Airlines", "#0078d2", "#0a1420", "#d1232a",
     "Going for great.", "Dallas/Fort Worth, Charlotte, Phoenix, Miami"),
    ("united-airlines", "United", "United Airlines", "#002244", "#0a1424", "#1414aa",
     "Good leads the way.", "Chicago O'Hare, Denver, Houston, Newark"),
    ("southwest-airlines", "Southwest", "Southwest Airlines", "#304cb2", "#10152a", "#f9b612",
     "Low fares. Nothing to hide.", "Dallas Love, Chicago Midway, Las Vegas"),
    ("alaska-airlines", "Alaska", "Alaska Airlines", "#01426a", "#0a1522", "#44c8f5",
     "Fly smart. Land happy.", "Seattle/Tacoma, Portland, Anchorage"),
    ("jetblue-airways", "JetBlue", "JetBlue Airways", "#003876", "#0a1424", "#e4780b",
     "You above all.", "New York JFK, Boston, Fort Lauderdale"),
    ("spirit-airlines", "Spirit", "Spirit Airlines", "#ffeb00", "#111000", "#000000",
     "Less money. More go.", "Fort Lauderdale, Orlando, Las Vegas"),
    ("frontier-airlines", "Frontier", "Frontier Airlines", "#00703c", "#06160e", "#d50032",
     "Low fares done right.", "Denver"),
    ("hawaiian-airlines", "Hawaiian", "Hawaiian Airlines", "#4b0082", "#140a1e", "#e6008a",
     "The spirit of aloha, in the air.", "Honolulu, Kahului"),
    ("air-canada", "AirCanada", "Air Canada", "#d22630", "#1a0a0c", "#000000",
     "Fly the flag.", "Toronto, Montreal, Vancouver"),
    ("westjet", "WestJet", "WestJet", "#0f1b63", "#0a0f24", "#00a3e0",
     "Owners care.", "Calgary, Toronto"),
    ("aeromexico", "Aeromexico", "Aeromexico", "#0b2265", "#0a0f24", "#e4002b",
     "The heart of Mexico takes flight.", "Mexico City"),

    # --- Europe ---
    ("british-airways", "BritishAirways", "British Airways", "#2e5c99", "#0a1322", "#eb2226",
     "To fly. To serve.", "London Heathrow, London Gatwick"),
    ("lufthansa", "Lufthansa", "Lufthansa", "#05164d", "#080e26", "#ffb81c",
     "Say yes to the world.", "Frankfurt, Munich"),
    ("air-france", "AirFrance", "Air France", "#002157", "#0a0f24", "#e2000f",
     "France is in the air.", "Paris Charles de Gaulle"),
    ("klm", "KLM", "KLM Royal Dutch Airlines", "#00a1de", "#031726", "#f36f21",
     "Journeys of inspiration.", "Amsterdam Schiphol"),
    ("ryanair", "Ryanair", "Ryanair", "#073590", "#0a1024", "#f1c933",
     "Low fares. Made simple.", "Dublin, London Stansted"),
    ("easyjet", "EasyJet", "easyJet", "#ff6600", "#1a0d00", "#003a5d",
     "This is generation easyJet.", "London Gatwick, Luton"),
    ("iberia", "Iberia", "Iberia", "#d7192d", "#1a0a0c", "#f5a800",
     "Spain in the sky.", "Madrid"),
    ("alitalia-ita-airways", "ItaAirways", "ITA Airways", "#004a99", "#0a1322", "#008c45",
     "Born to be sustainable.", "Rome Fiumicino, Milan"),
    ("swiss", "Swiss", "SWISS International Air Lines", "#cb0300", "#1a0505", "#ffffff",
     "Switzerland's airline.", "Zurich, Geneva"),
    ("sas", "SAS", "Scandinavian Airlines", "#003d6b", "#0a1322", "#e8ba00",
     "We are travelers.", "Copenhagen, Stockholm, Oslo"),
    ("turkish-airlines", "TurkishAirlines", "Turkish Airlines", "#c70a2c", "#1a060a", "#ffffff",
     "Widen your world.", "Istanbul"),
    ("aer-lingus", "AerLingus", "Aer Lingus", "#006272", "#04161a", "#8dc63f",
     "Smart flies Aer Lingus.", "Dublin"),

    # --- Middle East ---
    ("emirates", "Emirates", "Emirates", "#d71921", "#1a0608", "#ffffff",
     "Fly better.", "Dubai"),
    ("qatar-airways", "QatarAirways", "Qatar Airways", "#5c0632", "#1a0611", "#8a1538",
     "Going places together.", "Doha"),
    ("etihad-airways", "Etihad", "Etihad Airways", "#bd8b13", "#1a1405", "#4c4c4c",
     "Choose well.", "Abu Dhabi"),

    # --- Asia-Pacific ---
    ("singapore-airlines", "SingaporeAirlines", "Singapore Airlines", "#f99f1c", "#1a1004", "#003c71",
     "A great way to fly.", "Singapore Changi"),
    ("cathay-pacific", "CathayPacific", "Cathay Pacific", "#005c63", "#04161a", "#8ec63f",
     "Move beyond.", "Hong Kong"),
    ("qantas", "Qantas", "Qantas", "#e40000", "#1a0505", "#ffffff",
     "The spirit of Australia.", "Sydney, Melbourne"),
    ("air-new-zealand", "AirNewZealand", "Air New Zealand", "#00205b", "#0a0f24", "#000000",
     "Fly with confidence.", "Auckland"),
    ("japan-airlines", "JapanAirlines", "Japan Airlines", "#c8102e", "#1a060a", "#ffffff",
     "Dream skyward.", "Tokyo Haneda, Tokyo Narita"),
    ("all-nippon-airways", "AllNipponAirways", "All Nippon Airways", "#1c3f94", "#0a1024", "#00a0e9",
     "Inspiration of Japan.", "Tokyo Haneda, Tokyo Narita"),
    ("korean-air", "KoreanAir", "Korean Air", "#0f4c99", "#0a1322", "#e4002b",
     "Excellence in flight.", "Seoul Incheon"),
    ("china-southern-airlines", "ChinaSouthern", "China Southern Airlines", "#004b8d", "#0a1526", "#e4002b",
     "Fly into your dreams.", "Guangzhou, Beijing"),
    ("air-india", "AirIndia", "Air India", "#c8102e", "#1a060a", "#d4a017",
     "The new age of flying.", "Delhi, Mumbai"),
    ("indigo", "IndiGo", "IndiGo", "#09377f", "#0a1024", "#1b3f8b",
     "On time, every time.", "Delhi, Mumbai, Bengaluru"),
    ("thai-airways", "ThaiAirways", "Thai Airways", "#4b276d", "#120a1a", "#d4a017",
     "Smooth as silk.", "Bangkok Suvarnabhumi"),

    # --- Latin America / Africa ---
    ("latam-airlines", "Latam", "LATAM Airlines", "#1b0088", "#0a0520", "#ed1651",
     "We are LATAM.", "Santiago, Sao Paulo, Lima"),
    ("avianca", "Avianca", "Avianca", "#e8120c", "#1a0505", "#ffffff",
     "Here, you are the pilot.", "Bogota"),
    ("copa-airlines", "Copa", "Copa Airlines", "#003da5", "#0a1322", "#e4002b",
     "The route that unites the Americas.", "Panama City"),
    ("ethiopian-airlines", "Ethiopian", "Ethiopian Airlines", "#5c9a1b", "#0a160a", "#e4a500",
     "The new spirit of Africa.", "Addis Ababa"),
    ("south-african-airways", "SouthAfricanAirways", "South African Airways", "#0033a0", "#0a1026", "#ffb81c",
     "Bringing the world to Africa.", "Johannesburg"),
    ("kenya-airways", "KenyaAirways", "Kenya Airways", "#c8102e", "#1a060a", "#ffffff",
     "The pride of Africa.", "Nairobi"),
]


def repo_root() -> Path:
    # lib/website -> repo root
    return Path(__file__).resolve().parent.parent.parent


def header(rel_path: str, definition: str) -> str:
    return (
        "/*\n"
        f" * {rel_path}\n"
        " * SLeeLa Standard Library Definition.\n"
        f" * Definition: {definition}\n"
        " */\n"
        f"{SLEELA_VERSION}\n"
    )


def esc(s: str) -> str:
    """Escape a Python string for embedding inside a SLeeLa double-quoted literal."""
    return s.replace("\\", "\\\\").replace('"', '\\"')


def airline_class(slug, base_name, display, brand, bg, accent, tagline, hubs) -> str:
    cls = f"{base_name}Website"
    rel = f"lib/website/{slug}/{cls}.sleela"
    definition = (
        f"Defines {cls} — the SLeeLa Website Generator build for {display}. It "
        f"composes the airline's brand design (brand {brand}, accent {accent}) "
        f"and an ordered list of semantic signs (nav, hero, feature, "
        f"testimonial, cta, footer) into a complete, self-contained HTML site. "
        f"Mirrors lib/website/website.sleela: String concatenation on OP_ADD "
        f"plus integer-to-text, no new VM opcode."
    )
    d = {k: esc(v) for k, v in dict(
        display=display, brand=brand, bg=bg, accent=accent,
        tagline=tagline, hubs=hubs).items()}
    body = f"""{header(rel, definition)}// SLeeLa website/{slug} front-end builder — one airline website per directory.
// Family: website. {d['display']} brand site, built from the website model: a
// design (brand/base/accent + an energy dial) and an ordered list of semantic
// signs. The same sign model as lib/website/website.sleela; only the brand
// identity and copy change. Self-contained and runnable (print the HTML).
class {cls} {{
  // Brand identity for {d['display']}.
  String brandName() {{ return "{d['display']}"; }}
  String brand() {{ return "{d['brand']}"; }}
  String base() {{ return "{d['bg']}"; }}
  String accent() {{ return "{d['accent']}"; }}
  String tagline() {{ return "{d['tagline']}"; }}
  String hubs() {{ return "{d['hubs']}"; }}
  // Airline sites read as confident and vivid: a bold animated hero gradient.
  int energy() {{ return 85; }}
  String font() {{ return "'Inter', system-ui, sans-serif"; }}

  String intToStr(int v) {{ return "" + v; }}

  // The <style> block, derived entirely from the brand design.
  String style() {{
    String css = "<style>\\n";
    css = css + ":root{{--brand:" + brand() + ";--base:" + base() + ";--accent:" + accent()
              + ";--radius:14px;--font:" + font() + ";}}\\n";
    css = css + "*{{box-sizing:border-box;margin:0;padding:0}}";
    css = css + "body{{font-family:var(--font);background:var(--base);color:#e5e7eb;line-height:1.55}}";
    css = css + "section{{padding:72px 24px}}.wrap{{max-width:1080px;margin:0 auto}}";
    css = css + "nav{{position:sticky;top:0;display:flex;justify-content:space-between;align-items:center;padding:16px 24px;background:rgba(0,0,0,.35)}}";
    css = css + "nav .brand{{font-weight:800;color:#fff}}";
    css = css + ".hero{{background:linear-gradient(120deg,var(--brand),var(--accent),var(--brand));background-size:200% 200%;animation:flow 12s ease infinite;border-radius:var(--radius);padding:120px 32px;text-align:center;color:#fff}}";
    css = css + "@keyframes flow{{0%{{background-position:0% 50%}}50%{{background-position:100% 50%}}100%{{background-position:0% 50%}}}}";
    css = css + ".hero h1{{font-size:clamp(2.2rem,6vw,4rem);letter-spacing:-.02em}}";
    css = css + ".hero p{{opacity:.9;margin:16px auto 28px;max-width:46ch}}";
    css = css + ".btn{{display:inline-block;background:var(--accent);color:#001;font-weight:700;padding:14px 26px;border-radius:var(--radius);text-decoration:none}}";
    css = css + ".grid{{display:grid;gap:20px;grid-template-columns:repeat(auto-fit,minmax(220px,1fr))}}";
    css = css + ".card{{background:rgba(255,255,255,.05);border:1px solid rgba(255,255,255,.08);border-radius:var(--radius);padding:24px}}";
    css = css + ".cta{{background:var(--accent);color:#001;text-align:center;border-radius:var(--radius)}}";
    css = css + "blockquote{{font-size:1.5rem;max-width:40ch;margin:0 auto;text-align:center;color:#fff}}";
    css = css + "footer{{padding:48px 24px;background:rgba(0,0,0,.4)}}";
    css = css + "\\n</style>";
    return css;
  }}

  // The composed page body: the airline's ordered semantic signs.
  String body() {{
    String b = "";
    b = b + "<nav><span class=\\"brand\\">" + brandName() + "</span><span>Book &nbsp; Flights &nbsp; Destinations &nbsp; Loyalty</span></nav>";
    b = b + "<section class=\\"hero\\"><div class=\\"wrap\\"><h1>" + brandName() + "</h1><p>" + tagline() + "</p><a class=\\"btn\\" href=\\"#book\\">Book a flight</a></div></section>";
    String cards = "";
    cards = cards + "<div class=\\"card\\"><h3>Destinations</h3><p>Fly our global network with one trusted brand.</p></div>";
    cards = cards + "<div class=\\"card\\"><h3>Loyalty</h3><p>Earn and redeem on every journey.</p></div>";
    cards = cards + "<div class=\\"card\\"><h3>Hubs</h3><p>" + hubs() + "</p></div>";
    b = b + "<section><div class=\\"wrap\\"><h2>Why fly with us</h2><div class=\\"grid\\">" + cards + "</div></div></section>";
    b = b + "<section><blockquote>" + tagline() + "<br><small>- " + brandName() + "</small></blockquote></section>";
    b = b + "<section class=\\"cta\\"><div class=\\"wrap\\"><h2>Ready to travel?</h2><a class=\\"btn\\" href=\\"#book\\">Search fares</a></div></section>";
    b = b + "<footer><div class=\\"wrap\\"><strong>" + brandName() + "</strong><div>Generated by the SLeeLa Website Generator</div></footer>";
    return b;
  }}

  // The finished, self-contained HTML document for this airline.
  String build() {{
    String html = "<!doctype html>\\n<html lang=\\"en\\">\\n<head>\\n";
    html = html + "<meta charset=\\"utf-8\\">\\n<meta name=\\"viewport\\" content=\\"width=device-width,initial-scale=1\\">\\n";
    html = html + "<title>" + brandName() + " - generated by SLeeLa</title>\\n" + style() + "\\n</head>\\n<body>\\n" + body() + "\\n</body>\\n</html>\\n";
    return html;
  }}

  void main() {{
    print(build());
  }}
}}
"""
    return rel, body


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--count", action="store_true", help="print counts and exit")
    args = ap.parse_args()
    root = repo_root()
    dry = args.count

    # Guard: slugs and class names must be unique.
    slugs = [a[0] for a in AIRLINES]
    classes = [a[1] for a in AIRLINES]
    assert len(slugs) == len(set(slugs)), "duplicate slug"
    assert len(classes) == len(set(classes)), "duplicate class base name"
    for s in slugs:
        assert re.fullmatch(r"[a-z0-9-]+", s), f"bad slug {s}"

    written = []
    for row in AIRLINES:
        rel, text = airline_class(*row)
        if not dry:
            p = root / rel
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(text, encoding="utf-8")
        written.append(rel)

    print(f"airlines: {len(written)} website builders across {len(written)} subdirectories")
    if dry:
        for r in written[:6]:
            print(f"  {r}")
        print("  ...")
    return 0


if __name__ == "__main__":
    sys.exit(main())
