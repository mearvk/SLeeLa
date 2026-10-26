#!/usr/bin/env python3
"""Generate the deterministic, compact UTF-4088 experimental catalogue.

This is a project-specific experimental symbol catalogue, not a Unicode encoder.
The 32-bit input word stores x (3 bits), y (4 bits), pressure-1 (6 bits),
and normalized voltage (8 bits). Glyphs are deterministic 8x12 one-bit images.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
from pathlib import Path

CAPACITY = 1_048_576
MAX_BYTES = 20_000_000
SEED = 0x5554463430383801
MAGIC = b"UTF4088\0"
VERSION = 1
HEADER = struct.Struct("<8sIIIQ")  # 28 bytes
RECORD = struct.Struct("<I12s")   # packed input + 96-bit glyph
WIDTH = 8
HEIGHT = 12
PRESSURE_MIN = 1
PRESSURE_MAX = 50
VOLTAGE_MIN = 0
VOLTAGE_MAX = 255


def pack_input(x: int, y: int, pressure: int, voltage: int) -> int:
    if not 0 <= x <= 7:
        raise ValueError("x must be in 0..7")
    if not 0 <= y <= 11:
        raise ValueError("y must be in 0..11")
    if not PRESSURE_MIN <= pressure <= PRESSURE_MAX:
        raise ValueError("pressure must be in 1..50")
    if not VOLTAGE_MIN <= voltage <= VOLTAGE_MAX:
        raise ValueError("normalized voltage must be in 0..255")
    return x | (y << 3) | ((pressure - 1) << 7) | (voltage << 13)


def input_states(count: int):
    """Canonical spatial traversal: z-layer, y, x, then pressure and voltage.

    Version 1 populates z=0. The full x/y/pressure/voltage input domain has
    8*12*50*256 = 1,228,800 combinations; count is a deterministic prefix.
    """
    if not 0 <= count <= CAPACITY:
        raise ValueError(f"count must be in 0..{CAPACITY}")
    emitted = 0
    for y in range(12):
        for x in range(8):
            for pressure in range(1, 51):
                for voltage in range(256):
                    yield x, y, pressure, voltage
                    emitted += 1
                    if emitted >= count:
                        return


def make_glyph(packed_input: int, ordinal: int) -> bytes:
    payload = (b"SLeeLa/UTF-4088/catalogue/v1\0"
               + SEED.to_bytes(8, "little")
               + ordinal.to_bytes(4, "little")
               + packed_input.to_bytes(4, "little"))
    return hashlib.sha256(payload).digest()[:12]


def generate(output: Path, summary_path: Path | None, count: int,
             max_bytes: int) -> dict:
    if not 1 <= count <= CAPACITY:
        raise ValueError(f"count must be in 1..{CAPACITY}")
    expected_size = HEADER.size + count * RECORD.size
    if expected_size >= max_bytes:
        raise ValueError(
            f"expected output {expected_size} bytes must be below limit {max_bytes}"
        )
    output.parent.mkdir(parents=True, exist_ok=True)
    digest = hashlib.sha256()
    seen_glyphs: set[bytes] = set()
    with output.open("wb") as stream:
        header = HEADER.pack(MAGIC, VERSION, count, RECORD.size, SEED)
        stream.write(header)
        digest.update(header)
        for ordinal, (x, y, pressure, voltage) in enumerate(input_states(count)):
            packed = pack_input(x, y, pressure, voltage)
            glyph = make_glyph(packed, ordinal)
            if glyph in seen_glyphs:
                raise RuntimeError(f"duplicate glyph detected at entry {ordinal}")
            seen_glyphs.add(glyph)
            record = RECORD.pack(packed, glyph)
            stream.write(record)
            digest.update(record)
        if len(seen_glyphs) != count:
            raise RuntimeError(f"generated {len(seen_glyphs)} unique glyphs; expected {count}")
    result = {
        "format": "UTF-4088 compact catalogue",
        "format_version": VERSION,
        "experimental_not_unicode": True,
        "entry_count": count,
        "capacity": CAPACITY,
        "record_bytes": RECORD.size,
        "header_bytes": HEADER.size,
        "file_bytes": output.stat().st_size,
        "file_limit_bytes_exclusive": max_bytes,
        "under_size_limit": output.stat().st_size < max_bytes,
        "glyph_dimensions": [WIDTH, HEIGHT],
        "glyph_encoding": "96-bit row-major bitmap, 12 bytes per glyph",
        "input_fields": {
            "x": "0..7",
            "y": "0..11",
            "z_layer": 0,
            "pressure": "1..50",
            "normalized_voltage": "0..255"
        },
        "traversal": "z-layer, y, x, pressure, normalized_voltage",
        "seed": f"0x{SEED:016X}",
        "sha256": digest.hexdigest(),
        "unique_glyph_count": len(seen_glyphs)
    }
    if output.stat().st_size != expected_size:
        raise RuntimeError("file size does not match declared format")
    if not result["under_size_limit"]:
        raise RuntimeError("catalogue exceeded the configured size limit")
    if summary_path:
        summary_path.parent.mkdir(parents=True, exist_ok=True)
        summary_path.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--count", type=int, default=CAPACITY)
    parser.add_argument("--output", type=Path, default=Path("utf4088-million.bin"))
    parser.add_argument("--summary", type=Path, default=Path("utf4088-million.summary.json"))
    parser.add_argument("--max-bytes", type=int, default=MAX_BYTES)
    args = parser.parse_args()
    result = generate(args.output, args.summary, args.count, args.max_bytes)
    print(json.dumps(result, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
