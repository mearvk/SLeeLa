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


MIN_BYTES = 2 * 1024**3   # 2 GiB
MAX_BYTES = 16 * 1024**3  # 16 GiB


def tier_for_bytes(b):
    if b == 0:
        return "unknown"
    if b < 3 * 1024**3:
        return "compact"
    if b < 6 * 1024**3:
        return "standard"
    if b < 12 * 1024**3:
        return "capable"
    return "large"


def validate_capacity(path, schema_path):
    """Schema + advisory-envelope + tier_hint agreement for user capacity profiles."""
    where = str(path)
    schema = load_json(schema_path) if (schema_path and schema_path.exists()) else None
    try:
        with open(path, encoding="utf-8") as fh:
            lines = fh.readlines()
    except OSError as e:
        err(where, f"cannot read: {e}")
        return
    ids = set()
    for lineno, line in enumerate(lines, 1):
        line = line.strip()
        if not line:
            continue
        loc = f"{where}:{lineno}"
        try:
            rec = json.loads(line)
        except json.JSONDecodeError as e:
            err(loc, f"invalid JSON: {e}")
            continue
        if schema is not None:
            schema_check(rec, schema, loc)
        mid = rec.get("model_id", "")
        if mid in ids:
            err(loc, f"duplicate model_id '{mid}'")
        ids.add(mid)
        wb = rec.get("weight_bytes", 0)
        # The envelope is advisory: a value outside 2..16 GB is a WARNING, not a hard
        # failure (the user owns the model choice). Record it as an issue only when
        # strictly outside, so examples in-envelope stay clean.
        if isinstance(wb, int) and wb > 0:
            if wb < MIN_BYTES:
                err(loc, f"[{mid}] weight_bytes {wb} below 2 GB envelope (advisory)")
            elif wb > MAX_BYTES:
                err(loc, f"[{mid}] weight_bytes {wb} above 16 GB envelope (advisory)")
            hint = rec.get("tier_hint")
            if hint and hint != tier_for_bytes(wb):
                err(loc, f"[{mid}] tier_hint '{hint}' disagrees with derived tier '{tier_for_bytes(wb)}'")


CODE_LANGS = {"c", "fortran", "pascal", "python", "shell", "sleela", "other"}
CODE_TASKS = {"summarize", "dry-run", "refactor", "translate", "debug", "complete"}
CODE_CHECKS = {"none", "compile", "test", "trace"}


def validate_code_models(path, schema_path):
    """Schema + consistency checks for user-owned Code LLM profiles."""
    where = str(path)
    schema = load_json(schema_path) if (schema_path and schema_path.exists()) else None
    try:
        with open(path, encoding="utf-8") as fh:
            lines = fh.readlines()
    except OSError as e:
        err(where, f"cannot read: {e}")
        return
    ids = set()
    for lineno, line in enumerate(lines, 1):
        line = line.strip()
        if not line:
            continue
        loc = f"{where}:{lineno}"
        try:
            rec = json.loads(line)
        except json.JSONDecodeError as e:
            err(loc, f"invalid JSON: {e}")
            continue
        if schema is not None:
            schema_check(rec, schema, loc)
        mid = rec.get("model_id", "")
        if mid in ids:
            err(loc, f"duplicate model_id '{mid}'")
        ids.add(mid)
        lang = rec.get("primary_language")
        if lang not in CODE_LANGS:
            err(loc, f"[{mid}] invalid primary_language '{lang}'")
        if lang == "other" and not rec.get("other_language"):
            err(loc, f"[{mid}] primary_language 'other' requires other_language")
        tasks = rec.get("tasks", [])
        if not tasks:
            err(loc, f"[{mid}] tasks must be non-empty")
        for t in tasks:
            if t not in CODE_TASKS:
                err(loc, f"[{mid}] invalid task '{t}'")
        cm = rec.get("check_mode")
        if cm is not None and cm not in CODE_CHECKS:
            err(loc, f"[{mid}] invalid check_mode '{cm}'")
        # Honesty: dry-run/debug claim behavior; with check_mode 'none' that behavior
        # is unverifiable. This is allowed, but the note should say so (advisory).
        if cm == "none" and ("dry-run" in tasks or "debug" in tasks):
            note = (rec.get("note") or "").lower()
            if "unverif" not in note and "unverified" not in note:
                err(loc, f"[{mid}] check_mode 'none' with dry-run/debug: note should flag that behavior claims are unverified")


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

    # Model-capacity profiles (user-owned; advisory 2..16 GB envelope)
    models_dir = root / "models"
    cap_schema = models_dir / "capacity-profile.schema.json"
    cap_files = sorted(models_dir.glob("capacity-profile*.jsonl")) if models_dir.exists() else []
    for f in cap_files:
        validate_capacity(f, cap_schema)

    # Code LLM profiles (user-owned code-specialized models)
    code_schema = models_dir / "code-model.schema.json"
    code_files = sorted(models_dir.glob("code-model*.jsonl")) if models_dir.exists() else []
    for f in code_files:
        validate_code_models(f, code_schema)

    # SLeeLa-specialized Code LLM profile (single JSON object)
    sleela_dir = root / "sleela"
    sleela_profile = sleela_dir / "sleela-language-profile.json"
    sleela_ok = 0
    if sleela_profile.exists():
        sleela_ok = 1
        where = str(sleela_profile)
        try:
            prof = load_json(sleela_profile)
            if prof.get("primary_language") != "sleela":
                err(where, "SLeeLa profile must set primary_language 'sleela'")
            lk = prof.get("language_knowledge", {})
            rw = lk.get("reserved_words", [])
            for must in ("record", "native", "enum", "class", "struct"):
                if must not in rw:
                    err(where, f"reserved_words should list '{must}'")
            if lk.get("limits", {}).get("max_class_or_struct_fields") != 64:
                err(where, "max_class_or_struct_fields must be 64 (SL_MAX_STRUCT_FIELDS)")
            tasks = prof.get("tasks", [])
            for t in tasks:
                if t not in CODE_TASKS:
                    err(where, f"invalid task '{t}'")
        except (OSError, json.JSONDecodeError) as e:
            err(where, f"cannot read/parse: {e}")

    # Parametry: known-models registry + binding examples
    parametry_dir = root / "parametry"
    parametry_ok = 0
    known_names = {}
    if parametry_dir.exists():
        km = parametry_dir / "known-models.json"
        if km.exists():
            parametry_ok = 1
            where = str(km)
            try:
                reg = load_json(km)
                for i, m in enumerate(reg.get("models", [])):
                    nm = m.get("name", "")
                    kind = m.get("kind")
                    if not nm:
                        err(where, f"models[{i}] missing name")
                    if kind not in ("llm", "neural-net"):
                        err(where, f"models[{i}] '{nm}' invalid kind '{kind}'")
                    known_names[nm] = kind
                    if kind == "llm":
                        wb = m.get("weight_bytes", 0)
                        if not (MIN_BYTES <= wb <= MAX_BYTES):
                            err(where, f"models[{i}] '{nm}' llm weight_bytes {wb} outside 2..16 GB envelope")
                    elif kind == "neural-net":
                        ls = m.get("layer_sizes", [])
                        if len(ls) < 2:
                            err(where, f"models[{i}] '{nm}' neural-net needs >=2 layer_sizes")
            except (OSError, json.JSONDecodeError) as e:
                err(where, f"cannot read/parse: {e}")
        bind_schema = parametry_dir / "binding.schema.json"
        for bf in sorted(parametry_dir.glob("bindings*.jsonl")):
            where = str(bf)
            schema = load_json(bind_schema) if bind_schema.exists() else None
            try:
                blines = open(bf, encoding="utf-8").read().splitlines()
            except OSError as e:
                err(where, f"cannot read: {e}"); continue
            for lineno, line in enumerate(blines, 1):
                line = line.strip()
                if not line:
                    continue
                loc = f"{where}:{lineno}"
                try:
                    rec = json.loads(line)
                except json.JSONDecodeError as e:
                    err(loc, f"invalid JSON: {e}"); continue
                if schema is not None:
                    schema_check(rec, schema, loc)
                nm = rec.get("name")
                path = rec.get("local_path")
                if not nm and not path:
                    err(loc, "binding needs a name or a local_path")
                # Known name must exist in the registry and match kind.
                if nm and known_names:
                    if nm not in known_names:
                        err(loc, f"name '{nm}' is not in known-models.json")
                    elif known_names[nm] != rec.get("kind"):
                        err(loc, f"name '{nm}' kind '{known_names[nm]}' != binding kind '{rec.get('kind')}'")
                # A LOCAL neural net must carry a shape.
                if rec.get("kind") == "neural-net" and not nm and not rec.get("layer_sizes"):
                    err(loc, "local neural-net binding requires layer_sizes")

    if errors:
        print(f"validate: FAILED with {len(errors)} issue(s):", file=sys.stderr)
        for e in errors:
            print(f"  - {e}", file=sys.stderr)
        return 1
    scope = (f"base + {len(corpus_files)} corpus file(s) + {len(cap_files)} capacity + "
             f"{len(code_files)} code-model + {sleela_ok} SLeeLa + {parametry_ok} parametry")
    print(f"validate: OK — {scope} pass schema, ID, licensing, capacity, code, SLeeLa, and parametry checks"
          + ("" if HAVE_JSONSCHEMA else " (built-in schema subset; install jsonschema for full checks)"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
