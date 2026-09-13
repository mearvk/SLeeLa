#!/usr/bin/env python3
"""Apply the four-value Socialism score to BANKS4.md.

The score is a registry heuristic, not a moral judgment:
YES = no identified present/legacy socialist institutional basis in this registry;
NO = historical jurisdiction predating modern study of socialism;
CONVINCED = direct socialist/communist institutional inheritance;
INDEED = current socialist/constitutional-socialist or selected modern social-democratic institutional model.

Only the final Status cell is changed. All 391 rows and every other column are preserved.
"""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[2]
PATH = ROOT / "BANKS4.md"

INDEED = {
    "Bangladesh", "China", "Cuba", "India", "Laos", "Nepal", "North Korea",
    "Sri Lanka", "Venezuela", "Vietnam",
    "Denmark", "Finland", "Iceland", "Norway", "Sweden",
}

CONVINCED = {
    "Albania", "Armenia", "Azerbaijan", "Belarus", "Bosnia and Herzegovina",
    "Bulgaria", "Cambodia", "Croatia", "Czechia", "Estonia", "Georgia", "Germany",
    "Hungary", "Kazakhstan", "Kyrgyzstan", "Latvia", "Lithuania", "Moldova",
    "Mongolia", "Montenegro", "North Macedonia", "Poland", "Romania",
    "Russian Federation", "Serbia", "Slovakia", "Slovenia", "Tajikistan",
    "Tanzania", "Turkmenistan", "Ukraine", "Uzbekistan",
    "Czechoslovakia", "Democratic Kampuchea", "East Germany", "North Vietnam",
    "South Vietnam", "South Yemen", "Soviet Union", "Yugoslavia",
}


def score(country: str, ident: int) -> str:
    if country in INDEED:
        return "INDEED"
    if country in CONVINCED:
        return "CONVINCED"
    if ident >= 250:
        return "NO"
    return "YES"


def main() -> None:
    text = PATH.read_text(encoding="utf-8")
    lines = []
    rows = 0
    for line in text.splitlines():
        if re.match(r"^\| \d{3} \|", line):
            cells = line.split("|")
            ident = int(cells[1].strip())
            country = cells[2].strip()
            if not 1 <= ident <= 391:
                raise ValueError(f"Unexpected ID: {ident}")
            # Final table cell is Status; preserve every preceding cell byte-for-byte.
            cells[-2] = f" {score(country, ident)} "
            line = "|".join(cells)
            rows += 1
        lines.append(line)
    if rows != 391:
        raise ValueError(f"Expected 391 rows, found {rows}")
    PATH.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"BANKS4 Socialism score: updated {rows} rows")


if __name__ == "__main__":
    main()
