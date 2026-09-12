#!/usr/bin/env python3
"""Generate the canonical 391-entry banking registry from SLeeLa country data.

The project already maintains its country universe in
http/spec/COUNTRY_NETWORK_ENCLOSURE.md. This document identifies BYPASS.md as
its companion country taxonomy and contains the generated country/network table.
This script reads that existing project data; it does not invent or import a
second 391-country universe.

Primary source:
    http/spec/COUNTRY_NETWORK_ENCLOSURE.md
Fallback source:
    http/spec/BYPASS.md

The hard 391-entry invariant remains enforced. If the source does not contain
exactly 391 distinct country/geographic entries, generation fails rather than
padding or silently changing the project taxonomy.
"""

import argparse
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
DEFAULT_SOURCE = ROOT / "http" / "spec" / "COUNTRY_NETWORK_ENCLOSURE.md"
DEFAULT_FALLBACK = ROOT / "http" / "spec" / "BYPASS.md"
DEFAULT_OUTPUT = ROOT / "data" / "banking" / "registry.json"

EXPECTED = 391


def repo_path(path):
    """Resolve a CLI path without assuming it is already absolute."""
    candidate = Path(path)
    if not candidate.is_absolute():
        candidate = ROOT / candidate
    return candidate.resolve()


def repo_label(path):
    """Return a stable repository-relative path for logging and metadata."""
    resolved = Path(path).resolve()
    try:
        return resolved.relative_to(ROOT).as_posix()
    except ValueError:
        return str(resolved)


def parse_enclosure(path):
    """Read the generated country table from COUNTRY_NETWORK_ENCLOSURE.md."""
    text = path.read_text(encoding="utf-8")
    marker = "<!-- BEGIN GENERATED COUNTRY NETWORK TABLE -->"
    if marker not in text:
        raise ValueError(f"generated country table marker not found in {path}")

    section = text.split(marker, 1)[1]
    rows = []
    in_table = False
    for line in section.splitlines():
        stripped = line.strip()
        if stripped.startswith("| Country | ISO |"):
            in_table = True
            continue
        if not in_table:
            continue
        if stripped.startswith("|---"):
            continue
        if not stripped.startswith("|"):
            if rows:
                break
            continue
        cells = [cell.strip() for cell in stripped.strip("|").split("|")]
        if len(cells) < 9:
            continue
        country, iso, geodata = cells[0], cells[1], cells[2]
        if not country or country == "Country":
            continue
        rows.append({
            "country_name": country,
            "iso_alpha2": iso if re.fullmatch(r"[A-Z]{2}", iso) else None,
            "geodata": geodata,
        })
    return rows


def parse_bypass(path):
    """Fallback: extract the numbered country headings from BYPASS.md."""
    text = path.read_text(encoding="utf-8")
    pattern = re.compile(r"^###\s+(\d{3})\.\s+(.+?)\s*$", re.MULTILINE)
    rows = []
    for number, name in pattern.findall(text):
        rows.append({
            "country_name": name.strip(),
            "iso_alpha2": None,
            "geodata": None,
        })
    return rows


def validate(rows, source):
    if len(rows) != EXPECTED:
        raise SystemExit(
            f"ERROR: {source} produced {len(rows)} country entries; "
            f"SLeeLa requires exactly {EXPECTED}."
        )

    names = [row["country_name"] for row in rows]
    duplicates = sorted({name for name in names if names.count(name) > 1})
    if duplicates:
        raise SystemExit(
            "ERROR: duplicate country names in canonical source: "
            + ", ".join(duplicates[:20])
        )

    return rows


def build_registry(rows, source_label):
    countries = []
    for index, row in enumerate(rows, 1):
        record = {
            "registry_id": f"{index:03d}",
            "country_name": row["country_name"],
            "iso_alpha2": row.get("iso_alpha2"),
            "sources": [
                source_label,
                "http/spec/BYPASS.md",
            ],
        }
        if row.get("geodata"):
            record["geodata"] = row["geodata"]
        countries.append(record)
    return countries


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", default=str(DEFAULT_SOURCE))
    parser.add_argument("--fallback", default=str(DEFAULT_FALLBACK))
    parser.add_argument("--output", default=str(DEFAULT_OUTPUT))
    args = parser.parse_args()

    source = repo_path(args.source)
    fallback = repo_path(args.fallback)

    rows = []
    source_label = repo_label(source)
    if source.exists():
        try:
            rows = parse_enclosure(source)
        except (OSError, UnicodeError, ValueError) as exc:
            print(f"Primary source unavailable: {exc}")

    if len(rows) != EXPECTED:
        if not fallback.exists():
            raise SystemExit(
                f"ERROR: primary source did not yield {EXPECTED} entries and "
                f"fallback source is missing: {fallback}"
            )
        rows = parse_bypass(fallback)
        source_label = repo_label(fallback)

    rows = validate(rows, source_label)
    countries = build_registry(rows, source_label)

    output = repo_path(args.output)
    output.parent.mkdir(parents=True, exist_ok=True)
    payload = {
        "registry_version": "1.1",
        "expected_count": EXPECTED,
        "source": "SLeeLa canonical country/jurisdiction registry",
        "source_document": source_label,
        "status": "GENERATED_FROM_REPO_SOURCE",
        "countries": countries,
    }
    output.write_text(
        json.dumps(payload, indent=2, ensure_ascii=False) + "\n",
        encoding="utf-8",
    )

    print(f"Generated canonical registry: {len(countries)}/{EXPECTED}")
    print(f"Source: {source_label}")
    print(f"Output: {output}")


if __name__ == "__main__":
    main()
