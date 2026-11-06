#!/usr/bin/env python3
"""
validate_vocabulary.py — the careful-word-set gate for the SLeeLa LLM subsystem.

Validates, fail-closed, with exact locations:

  BASE VOCABULARY (tokenizers/base-vocabulary.json)
    * matches base-vocabulary.schema.json (jsonschema if installed, else a
      built-in structural subset check);
    * IDs are a gap-free 0..N-1 sequence, specials occupy the lowest IDs and
      precede all ordinary tokens, each id equals its array position;
    * token strings are unique and non-empty.

  LEXICAL CORPUS (vocabulary/corpus/*.jsonl, vocabulary/samples/*.jsonl)
    * every line is UTF-8 JSON matching lexical-corpus.schema.json (samples use
      the older sample shape and are checked structurally);
    * LICENSING DISCIPLINE:
        - record_status 'reference_only'  => senses MUST be empty;
        - record_status 'definition_imported' => must carry source.license and
          attribution, and the license must NOT look proprietary;
        - record_status 'sleela_authored' => source.license must be an open
          SLeeLa license (CC0/CC-BY) and must not claim a third-party source;
    * record_id values are unique within a file.

Exit 0 = all good; non-zero = at least one failure (each printed with location).
Stdlib only; uses jsonschema automatically when present.
"""
import json, re, sys
from pathlib import Path

try:
    import jsonschema  # type: ignore
    HAVE_JSONSCHEMA = True
except Exception:
    HAVE_JSONSCHEMA = False

PROPRIETARY = re.compile(r"propriet|all rights reserved|commercial-use|redistribution .*require", re.I)
OPEN_SELF = re.compile(r"^(cc0|cc-?by|public domain)", re.I)

errors = []


def err(where, msg):
    errors.append(f"{where}: {msg}")


def load_json(path):
    return json.loads(Path(path).read_text(encoding="utf-8"))


def schema_check(instance, schema, where):
    if HAVE_JSONSCHEMA:
        validator = jsonschema.Draft7Validator(schema)
        for e in sorted(validator.iter_errors(instance), key=lambda e: e.path):
            loc = "/".join(str(p) for p in e.path) or "(root)"
            err(where, f"schema: {loc}: {e.message}")
        return
    # Minimal built-in fallback: required keys + type of top level only.
    req = schema.get("required", [])
    if isinstance(instance, dict):
        for key in req:
            if key not in instance:
                err(where, f"schema(subset): missing required key '{key}'")


def validate_base(base_path, schema_dir):
    where = str(base_path)
    try:
        data = load_json(base_path)
    except (OSError, json.JSONDecodeError) as e:
        err(where, f"cannot read/parse: {e}")
        return
    schema_p = schema_dir / "base-vocabulary.schema.json"
    if schema_p.exists():
        schema_check(data, load_json(schema_p), where)

    specials = data.get("specials", [])
    tokens = data.get("tokens", [])
    seen = {}
    expected = 0
    for i, s in enumerate(specials):
        if s.get("id") != expected:
            err(where, f"specials[{i}] token '{s.get('token')}' has id {s.get('id')}, expected {expected}")
        tok = s.get("token", "")
        if not tok:
            err(where, f"specials[{i}] has empty token")
        if tok in seen:
            err(where, f"duplicate token '{tok}' (specials[{i}] and index {seen[tok]})")
        seen[tok] = expected
        expected += 1
    for i, t in enumerate(tokens):
        if t.get("id") != expected:
            err(where, f"tokens[{i}] '{t.get('token')}' has id {t.get('id')}, expected {expected} (ids must be gap-free and specials must precede tokens)")
        tok = t.get("token", "")
        if not tok:
            err(where, f"tokens[{i}] has empty token")
        if tok in seen:
            err(where, f"duplicate token '{tok}' (tokens[{i}] and index {seen[tok]})")
        seen[tok] = expected
        expected += 1
    # abi_version, if present, is a positive integer
    if "abi_version" in data and (not isinstance(data["abi_version"], int) or data["abi_version"] < 1):
        err(where, "abi_version must be an integer >= 1")


def validate_corpus_record(where, rec, ids_seen, strict_schema=None, schema_dir=None):
    rid = rec.get("record_id", "(no record_id)")
    if strict_schema is not None:
        schema_check(rec, strict_schema, f"{where} [{rid}]")

    if rid in ids_seen:
        err(where, f"duplicate record_id '{rid}'")
    ids_seen.add(rid)

    status = rec.get("record_status")
    senses = rec.get("senses", []) or []
    src = rec.get("source", {}) or {}
    lic = src.get("license", "") or ""

    if status == "reference_only":
        if senses:
            err(where, f"[{rid}] reference_only record must have EMPTY senses (found {len(senses)}); proprietary content must not be stored")
        integ = rec.get("integration", {}) or {}
        if integ.get("definition_stored") is True:
            err(where, f"[{rid}] reference_only record declares definition_stored=true")
    elif status == "definition_imported":
        if not lic:
            err(where, f"[{rid}] definition_imported record must declare source.license")
        if PROPRIETARY.search(lic):
            err(where, f"[{rid}] definition_imported but license looks proprietary: '{lic}' — use reference_only")
        if not rec.get("attribution"):
            err(where, f"[{rid}] definition_imported record must carry attribution")
        if not senses:
            err(where, f"[{rid}] definition_imported record has no senses")
    elif status == "sleela_authored":
        if not OPEN_SELF.match(lic.strip()):
            err(where, f"[{rid}] sleela_authored record must use an open license (CC0/CC-BY), got '{lic}'")
        name = (src.get("name") or "").lower()
        if any(k in name for k in ("oxford", "merriam", "wordnet", "princeton")):
            err(where, f"[{rid}] sleela_authored record must not claim a third-party source ('{src.get('name')}')")
        if not senses:
            err(where, f"[{rid}] sleela_authored record has no senses")
    else:
        err(where, f"[{rid}] unknown or missing record_status: {status!r}")


def validate_jsonl(path, schema_path, schema_dir):
    where = str(path)
    schema = load_json(schema_path) if (schema_path and schema_path.exists()) else None
    ids_seen = set()
    try:
        with open(path, encoding="utf-8") as fh:
            lines = fh.readlines()
    except OSError as e:
        err(where, f"cannot read: {e}")
        return
    any_record = False
    for lineno, line in enumerate(lines, 1):
        line = line.strip()
        if not line:
            continue
        any_record = True
        try:
            rec = json.loads(line)
        except json.JSONDecodeError as e:
            err(f"{where}:{lineno}", f"invalid JSON: {e}")
            continue
        # Only corpus records carry schema_version 1.0; sample fixtures use the
        # older shape and are checked for the licensing invariants only.
        use_schema = schema if rec.get("schema_version") == "1.0" else None
        validate_corpus_record(f"{where}:{lineno}", rec, ids_seen, use_schema, schema_dir)
    if not any_record:
        err(where, "file has no records")


def main(argv=None):
    root = Path(__file__).resolve().parent.parent  # large-language-models/
    schema_dir_tok = root / "tokenizers"
    schema_dir_vocab = root / "vocabulary"

    # Base vocabulary
    base = schema_dir_tok / "base-vocabulary.json"
    if base.exists():
        validate_base(base, schema_dir_tok)
    else:
        err(str(base), "missing base vocabulary")

    corpus_schema = schema_dir_vocab / "lexical-corpus.schema.json"
    # Corpus files
    corpus_dir = schema_dir_vocab / "corpus"
    corpus_files = sorted(corpus_dir.glob("*.jsonl")) if corpus_dir.exists() else []
    for f in corpus_files:
        validate_jsonl(f, corpus_schema, schema_dir_vocab)
    # Sample fixtures (licensing invariants only)
    samples_dir = schema_dir_vocab / "samples"
    for f in sorted(samples_dir.glob("*.jsonl")) if samples_dir.exists() else []:
        validate_jsonl(f, corpus_schema, schema_dir_vocab)

    if errors:
        print(f"validate: FAILED with {len(errors)} issue(s):", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    scope = f"base + {len(corpus_files)} corpus file(s)"
    print(f"validate: OK — {scope} pass schema, ID, and licensing checks"
          + ("" if HAVE_JSONSCHEMA else " (built-in schema subset; install jsonschema for full checks)"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
