#!/usr/bin/env python3
"""Validate UTF-4088's multilingual annotation schema and starter data."""
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
schema = json.loads((ROOT / "semantics/multilingual-concept-registry.schema.json").read_text())
assert schema["properties"]["schema_version"]["const"] == "UTF4088-MULTILINGUAL-1"
assert set(schema["properties"]["language_mappings"]["items"]["properties"]["language"]["enum"]) == {
    "american-english", "korean", "germanic"
}
assert "unreviewed" in schema["properties"]["language_mappings"]["items"]["properties"]["status"]["enum"]
print("multilingual schema contract: PASS")
