#!/usr/bin/env python3
"""Build and validate FRUCHST v1 binary lexical dictionaries."""

from __future__ import annotations

import argparse
import hashlib
import json
import math
import re
import struct
import os
import sys
import tempfile
import unicodedata
from pathlib import Path

MAGIC = b"FRUCHST\0"
VERSION_MAJOR = 1
VERSION_MINOR = 0
HEADER = struct.Struct(">8sHHHHIHHQQ32s")
ENTRY_PREFIX = struct.Struct(">HdI")
U32 = struct.Struct(">I")
EXPECTED_ENTRIES = 1000
LANG_TAG = re.compile(r"^[A-Za-z]{2,3}(?:-[A-Za-z0-9]{2,8})*$")


class FruchstError(ValueError):
    pass


def nfc_text(value: object, field: str) -> bytes:
    if not isinstance(value, str):
        raise FruchstError(f"{field} must be a string")
    normalized = unicodedata.normalize("NFC", value)
    if not normalized or normalized != normalized.strip():
        raise FruchstError(f"{field} must be non-empty and have no outer whitespace")
    try:
        return normalized.encode("utf-8", errors="strict")
    except UnicodeEncodeError as exc:
        raise FruchstError(f"{field} is not valid Unicode") from exc


def pack_string(value: object, field: str) -> bytes:
    encoded = nfc_text(value, field)
    return U32.pack(len(encoded)) + encoded


def load_input(path: Path) -> dict:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as exc:
        raise FruchstError(f"cannot read input JSON: {exc}") from exc
    if not isinstance(data, dict):
        raise FruchstError("input root must be an object")
    return data


def build(data: dict) -> bytes:
    language_tag = data.get("language_tag")
    source_id = data.get("source_id")
    if not isinstance(language_tag, str) or not LANG_TAG.fullmatch(language_tag):
        raise FruchstError("language_tag must be a valid-looking BCP 47 tag")
    lang_bytes = nfc_text(language_tag, "language_tag")
    source_bytes = nfc_text(source_id, "source_id")
    if len(lang_bytes) > 65535 or len(source_bytes) > 65535:
        raise FruchstError("language_tag or source_id is too long")

    entries = data.get("entries")
    if not isinstance(entries, list) or len(entries) != EXPECTED_ENTRIES:
        raise FruchstError(f"production input must contain exactly {EXPECTED_ENTRIES} entries")

    payload = bytearray(lang_bytes + source_bytes)
    seen = set()
    for expected_rank, entry in enumerate(entries, 1):
        if not isinstance(entry, dict):
            raise FruchstError(f"entry {expected_rank}: entry must be an object")
        rank = entry.get("rank")
        if isinstance(rank, bool) or not isinstance(rank, int) or rank != expected_rank:
            raise FruchstError(f"entry {expected_rank}: ranks must be contiguous integers in order")
        word_value = entry.get("word")
        word_bytes = nfc_text(word_value, f"entry {expected_rank} word")
        word = word_bytes.decode("utf-8")
        if word in seen:
            raise FruchstError(f"entry {expected_rank}: duplicate normalized headword {word!r}")
        seen.add(word)
        frequency = entry.get("frequency", 0.0)
        if isinstance(frequency, bool) or not isinstance(frequency, (int, float)):
            raise FruchstError(f"entry {expected_rank}: frequency must be numeric")
        frequency = float(frequency)
        if not math.isfinite(frequency) or frequency < 0:
            raise FruchstError(f"entry {expected_rank}: frequency must be finite and non-negative")
        definitions = entry.get("definitions")
        if not isinstance(definitions, dict):
            raise FruchstError(f"entry {expected_rank}: definitions object is required")
        payload.extend(ENTRY_PREFIX.pack(expected_rank, frequency, len(word_bytes)))
        payload.extend(word_bytes)
        for code in ("en", "fr", "es"):
            payload.extend(pack_string(definitions.get(code), f"entry {expected_rank} definition {code}"))

    digest = hashlib.sha256(payload).digest()
    header = HEADER.pack(
        MAGIC, VERSION_MAJOR, VERSION_MINOR, HEADER.size, 1,
        EXPECTED_ENTRIES, len(lang_bytes), len(source_bytes),
        0, len(payload), digest
    )
    return header + payload


def read_u32(data: bytes, offset: int) -> tuple[int, int]:
    if offset + 4 > len(data):
        raise FruchstError("truncated string length")
    return U32.unpack_from(data, offset)[0], offset + 4


def read_string(data: bytes, offset: int, length: int, field: str) -> tuple[str, int]:
    end = offset + length
    if end > len(data):
        raise FruchstError(f"truncated {field}")
    try:
        value = data[offset:end].decode("utf-8", errors="strict")
    except UnicodeDecodeError as exc:
        raise FruchstError(f"{field} is not valid UTF-8") from exc
    if not value or unicodedata.normalize("NFC", value) != value:
        raise FruchstError(f"{field} is empty or not NFC-normalized")
    return value, end


def validate(blob: bytes) -> dict:
    if len(blob) < HEADER.size:
        raise FruchstError("file is shorter than the fixed header")
    (magic, major, minor, header_size, flags, count, lang_len, source_len,
     generated_epoch, payload_len, digest) = HEADER.unpack_from(blob)
    if magic != MAGIC:
        raise FruchstError("bad FRUCHST magic")
    if major != VERSION_MAJOR or minor > VERSION_MINOR:
        raise FruchstError(f"unsupported FRUCHST version {major}.{minor}")
    if header_size != HEADER.size or flags & ~1:
        raise FruchstError("invalid header size or unknown required flags")
    if count != EXPECTED_ENTRIES:
        raise FruchstError(f"expected {EXPECTED_ENTRIES} entries, found {count}")
    payload = blob[HEADER.size:]
    if len(payload) != payload_len:
        raise FruchstError("payload length mismatch")
    if hashlib.sha256(payload).digest() != digest:
        raise FruchstError("payload SHA-256 mismatch")

    offset = 0
    language_tag, offset = read_string(payload, offset, lang_len, "language tag")
    source_id, offset = read_string(payload, offset, source_len, "source id")
    if not LANG_TAG.fullmatch(language_tag):
        raise FruchstError("invalid BCP 47 language tag")

    seen = set()
    for rank in range(1, count + 1):
        if offset + ENTRY_PREFIX.size > len(payload):
            raise FruchstError("truncated entry prefix")
        actual_rank, frequency, word_len = ENTRY_PREFIX.unpack_from(payload, offset)
        offset += ENTRY_PREFIX.size
        if actual_rank != rank:
            raise FruchstError(f"rank sequence error at entry {rank}")
        if not math.isfinite(frequency) or frequency < 0:
            raise FruchstError(f"invalid frequency at rank {rank}")
        word, offset = read_string(payload, offset, word_len, f"headword {rank}")
        if word in seen:
            raise FruchstError(f"duplicate headword at rank {rank}")
        seen.add(word)
        for code in ("en", "fr", "es"):
            length, offset = read_u32(payload, offset)
            _, offset = read_string(payload, offset, length, f"{code} definition at rank {rank}")
    if offset != len(payload):
        raise FruchstError("unexpected trailing bytes")
    return {
        "format": "FRUCHST",
        "version": f"{major}.{minor}",
        "language_tag": language_tag,
        "source_id": source_id,
        "entry_count": count,
        "payload_sha256": digest.hex(),
        "file_sha256": hashlib.sha256(blob).hexdigest(),
        "generated_epoch": generated_epoch,
        "valid": True,
    }



def validate_production_input(data: dict) -> None:
    provenance = data.get("provenance")
    if not isinstance(provenance, dict):
        raise FruchstError("production provenance object is required")
    for field in ("frequency_source", "attribution", "license_review"):
        if not provenance.get(field):
            raise FruchstError(f"provenance.{field} is required")
    sources = provenance.get("definition_sources")
    if not isinstance(sources, dict):
        raise FruchstError("provenance.definition_sources is required")
    for code in ("en", "fr", "es"):
        source = sources.get(code)
        if not isinstance(source, dict):
            raise FruchstError(f"provenance.definition_sources.{code} is required")
        for field in ("url", "dump_revision", "sha256", "license"):
            if not isinstance(source.get(field), str) or not source[field].strip():
                raise FruchstError(f"definition source {code} requires {field}")
        if not re.fullmatch(r"[0-9a-f]{64}", source["sha256"]):
            raise FruchstError(f"definition source {code} has invalid SHA-256")
    entries = data.get("entries")
    if not isinstance(entries, list) or len(entries) != EXPECTED_ENTRIES:
        raise FruchstError(f"production input must contain exactly {EXPECTED_ENTRIES} entries")
    for rank, entry in enumerate(entries, 1):
        if not isinstance(entry, dict) or entry.get("review_status") != "approved":
            raise FruchstError(f"entry {rank}: human approval is required")
        selected = entry.get("selected_sense")
        if not isinstance(selected, dict):
            raise FruchstError(f"entry {rank}: selected_sense evidence is required")
        for field in ("sense_id", "evidence", "reviewer", "approved_at_utc"):
            if not isinstance(selected.get(field), str) or not selected[field].strip():
                raise FruchstError(f"entry {rank}: selected_sense.{field} is required")


def write_atomic(path: Path, content: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    fd, temporary = tempfile.mkstemp(prefix=f".{path.name}.", dir=str(path.parent))
    try:
        with os.fdopen(fd, "wb") as stream:
            stream.write(content)
            stream.flush()
            os.fsync(stream.fileno())
        os.replace(temporary, path)
    except Exception:
        try:
            os.unlink(temporary)
        except OSError:
            pass
        raise


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    build_parser = sub.add_parser("build")
    build_parser.add_argument("input", type=Path)
    build_parser.add_argument("output", type=Path)
    build_parser.add_argument("--manifest", type=Path, required=True)
    validate_parser = sub.add_parser("validate")
    validate_parser.add_argument("binary", type=Path)
    validate_parser.add_argument("--manifest", type=Path)
    args = parser.parse_args()

    try:
        if args.command == "build":
            data = load_input(args.input)
            validate_production_input(data)
            blob = build(data)
            result = validate(blob)
            manifest_out = {
                "format": "FRUCHST",
                "format_version": "1.0",
                "language_tag": data["language_tag"],
                "source_id": data["source_id"],
                "frequency_unit": data.get("frequency_unit"),
                "entry_count": EXPECTED_ENTRIES,
                "binary_file": args.output.name,
                "binary_sha256": result["file_sha256"],
                "provenance": data["provenance"],
            }
            manifest_bytes = (json.dumps(manifest_out, ensure_ascii=False, indent=2) + "\n").encode("utf-8")
            write_atomic(args.output, blob)
            write_atomic(args.manifest, manifest_bytes)
            print(json.dumps(result, ensure_ascii=False, indent=2))
        else:
            result = validate(args.binary.read_bytes())
            if args.manifest:
                manifest = json.loads(args.manifest.read_text(encoding="utf-8"))
                if manifest.get("format") != "FRUCHST" or manifest.get("binary_sha256") != result["file_sha256"]:
                    raise FruchstError("manifest does not match the validated binary")
                if manifest.get("language_tag") != result["language_tag"] or manifest.get("source_id") != result["source_id"]:
                    raise FruchstError("manifest identity does not match the validated binary")
            print(json.dumps(result, ensure_ascii=False, indent=2))
    except (OSError, FruchstError, KeyError) as exc:
        print(f"fruchst: error: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
