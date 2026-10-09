# SLeeLa Character Catalogue Scanner

The runtime tooling includes a read-only source-folder scanner:

```sh
python3 tools/sleela-character-scan.py utf-4088
python3 tools/sleela-character-scan.py /path/to/character-library --json
```

It recognizes two library strategies and their combination:

- **literal** — characters are enumerated in data files and known before lookup;
- **procedural** — source code calculates or generates a character from a supplied input;
- **hybrid** — both literal records and a procedural generator are present;
- **unknown** — the scanner cannot infer either strategy from file types.

A library may include `SLEELA-CHARSET.conf` at its root to state the strategy explicitly. This file is a plain UTF-8 `key=value` manifest:

```ini
mode=procedural
capacity=1048576
generator=src/generator.cpp
catalogue=data/characters.json
```

Supported `mode` values are `auto`, `literal`, `procedural`, and `hybrid`. The `capacity` must be an integer from 1 through 1,048,576 and becomes the source signature `C<n>`; the default is `C1048576`. The `generator` and `catalogue` fields are descriptive paths for maintainers, not commands the scanner executes.

Auto-detection treats common source extensions as procedural evidence and JSON/CSV/TSV/JSONL/text files as literal-data evidence. Build output and hidden directories are skipped. A reported literal-record count is an estimate, not a validated registry population. JSON arrays and common object arrays are counted directly; CSV/TSV treats the first non-comment row as a header. The scanner never imports, compiles, or executes discovered source code, and never invokes a procedural generator. Use the library's own validated registry API when an authoritative population count or an actual input-to-character result is required.

The scanner is an inventory and dispatch-planning aid, not a Unicode conformance validator. In particular, the experimental `utf-4088/` folder remains separate from Unicode compatibility claims.
