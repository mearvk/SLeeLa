#!/usr/bin/env python3
"""
build_vocabulary.py — corpus -> tokenizer bridge for the SLeeLa LLM subsystem.

Takes the seed base vocabulary (tokenizers/base-vocabulary.json) and a lexical
corpus (vocabulary/corpus/*.jsonl), and produces a built tokenizer vocabulary
plus a build report. It is the ONLY sanctioned path from lexical records to
token IDs, and it enforces the subsystem's standing rules:

  * The base vocabulary is laid down FIRST, so special tokens keep their fixed
    low IDs and base tokens keep their declared IDs. New corpus-derived tokens
    are appended with the next free IDs; a token already present keeps its ID.
  * A token ID is NEVER taken from a dictionary headword. Headwords (and their
    synonyms/antonyms) are run through the tokenizer algorithm; the resulting
    token strings are what get IDs. (For the 'whitespace' algorithm a headword
    is lowercased, NFC-normalized, and split on whitespace.)
  * A reference_only record is treated as metadata only. Its definition text is
    never read into the vocabulary, and the builder fails if asked to import
    one as content (--strict).
  * Only token STRINGS cross the bridge. Definitions never become tokens.

Stdlib only. Deterministic output (stable ordering).
"""
import argparse, json, sys, unicodedata
from pathlib import Path

ADDABLE = {"definition_imported", "sleela_authored"}


def normalize_ws(text):
    """Whitespace-algorithm normalization: NFC + lowercase, split on whitespace."""
    text = unicodedata.normalize("NFC", text).lower()
    return [t for t in text.split() if t]


def load_base(path):
    data = json.loads(Path(path).read_text(encoding="utf-8"))
    order = []  # token strings in ID order
    seen = {}
    for s in data.get("specials", []):
        tok = s["token"]
        if tok in seen:
            raise ValueError(f"duplicate special token '{tok}' in base set")
        seen[tok] = len(order)
        order.append(tok)
    for t in data.get("tokens", []):
        tok = t["token"]
        if tok in seen:
            raise ValueError(f"duplicate token '{tok}' in base set")
        seen[tok] = len(order)
        order.append(tok)
    return data.get("vocabulary_id", "unknown"), order, seen


def iter_corpus(paths):
    for p in paths:
        with open(p, encoding="utf-8") as fh:
            for lineno, line in enumerate(fh, 1):
                line = line.strip()
                if not line:
                    continue
                try:
                    rec = json.loads(line)
                except json.JSONDecodeError as e:
                    raise ValueError(f"{p}:{lineno}: invalid JSON: {e}") from None
                yield p, lineno, rec


def main(argv=None):
    ap = argparse.ArgumentParser(description="Build a tokenizer vocabulary from the base set + lexical corpus.")
    ap.add_argument("--base", required=True, help="tokenizers/base-vocabulary.json")
    ap.add_argument("--corpus", nargs="*", default=[], help="vocabulary/corpus/*.jsonl lexical records")
    ap.add_argument("--out", help="write built vocabulary (token-per-line, ID = line number) here")
    ap.add_argument("--report", help="write a JSON build report here")
    ap.add_argument("--strict", action="store_true", help="fail if a reference_only record exposes sense content")
    args = ap.parse_args(argv)

    try:
        vocab_id, order, index = load_base(args.base)
    except (OSError, ValueError, json.JSONDecodeError) as e:
        print(f"build: base load failed: {e}", file=sys.stderr)
        return 2

    base_count = len(order)
    added, skipped_ref, provenance = [], 0, {}

    try:
        for p, lineno, rec in iter_corpus(args.corpus):
            status = rec.get("record_status")
            headword = rec.get("headword", "")
            # Rule: reference-only records contribute NO content, and must not carry senses.
            if status == "reference_only":
                if rec.get("senses"):
                    msg = f"{p}:{lineno}: reference_only record '{rec.get('record_id')}' carries sense content"
                    if args.strict:
                        print(f"build: {msg}", file=sys.stderr)
                        return 3
                skipped_ref += 1
                continue
            if status not in ADDABLE:
                # unknown status: skip defensively (the gate validates status separately)
                continue
            # Collect candidate strings: the headword and any synonym/antonym surface forms.
            surfaces = [headword]
            for sense in rec.get("senses", []):
                surfaces += sense.get("synonyms", [])
                surfaces += sense.get("antonyms", [])
            for surface in surfaces:
                for tok in normalize_ws(surface):
                    if tok not in index:
                        index[tok] = len(order)
                        order.append(tok)
                        added.append(tok)
                    provenance.setdefault(tok, []).append(rec.get("record_id", "?"))
    except (OSError, ValueError) as e:
        print(f"build: corpus read failed: {e}", file=sys.stderr)
        return 2

    if args.out:
        Path(args.out).write_text("\n".join(order) + "\n", encoding="utf-8")

    report = {
        "base_vocabulary_id": vocab_id,
        "base_token_count": base_count,
        "corpus_files": [str(x) for x in args.corpus],
        "tokens_added": len(added),
        "reference_only_skipped": skipped_ref,
        "final_token_count": len(order),
        "added_tokens": sorted(set(added)),
    }
    if args.report:
        Path(args.report).write_text(json.dumps(report, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    print(f"build: base '{vocab_id}' {base_count} tokens; "
          f"+{len(added)} from corpus; {skipped_ref} reference-only skipped; "
          f"{len(order)} total.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
