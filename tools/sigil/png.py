"""
png.py -- Minimal 1-bit/8-bit grayscale PNG writer using only the standard
library (zlib + struct). No third-party imaging dependency.
"""

import struct
import zlib
from typing import List


def _chunk(tag: bytes, data: bytes) -> bytes:
    return (
        struct.pack(">I", len(data))
        + tag
        + data
        + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)
    )


def write_gray(path: str, rows: List[List[int]]) -> None:
    """Write an 8-bit grayscale PNG. rows[y][x] in 0..255."""
    height = len(rows)
    width = len(rows[0]) if height else 0
    raw = bytearray()
    for row in rows:
        raw.append(0)  # filter type 0 (none)
        raw.extend(bytes(row))
    ihdr = struct.pack(">IIBBBBB", width, height, 8, 0, 0, 0, 0)  # 8-bit grayscale
    png = b"\x89PNG\r\n\x1a\n"
    png += _chunk(b"IHDR", ihdr)
    png += _chunk(b"IDAT", zlib.compress(bytes(raw), 9))
    png += _chunk(b"IEND", b"")
    with open(path, "wb") as f:
        f.write(png)


def write_bitmap(path: str, matrix: List[List[bool]], scale: int = 8, quiet: int = 4) -> None:
    """Render a boolean matrix (True=dark) to an upscaled grayscale PNG with a
    quiet zone. Dark=0, light=255."""
    n = len(matrix)
    dim = (n + 2 * quiet) * scale
    rows: List[List[int]] = []
    for y in range(dim):
        my = y // scale - quiet
        row = [255] * dim
        if 0 <= my < n:
            for x in range(dim):
                mx = x // scale - quiet
                if 0 <= mx < n and matrix[my][mx]:
                    row[x] = 0
        rows.append(row)
    write_gray(path, rows)
