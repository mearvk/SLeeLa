"""Dependency-free QR code encoder (byte mode) with SVG output.

Implements enough of the QR spec to encode the ledger's short identity strings:
byte-mode data, Reed-Solomon error correction, the standard function patterns,
all 8 data masks with penalty scoring, and format-info. Renders the result as a
scalable SVG. No third-party libraries (works offline).

Public entry point: qr_svg(text, ec="M", module_px=4, quiet=4) -> str (SVG).
"""
from __future__ import annotations

from typing import List, Tuple

# ---- Galois field GF(256) for Reed-Solomon ---------------------------------
_EXP = [0] * 512
_LOG = [0] * 256
_x = 1
for _i in range(255):
    _EXP[_i] = _x
    _LOG[_x] = _i
    _x <<= 1
    if _x & 0x100:
        _x ^= 0x11D
for _i in range(255, 512):
    _EXP[_i] = _EXP[_i - 255]


def _gf_mul(a: int, b: int) -> int:
    if a == 0 or b == 0:
        return 0
    return _EXP[_LOG[a] + _LOG[b]]


def _rs_generator(n: int) -> List[int]:
    # Generator polynomial of degree n -> n+1 coefficients, leading coeff 1.
    g = [1]
    for i in range(n):
        g2 = [0] * (len(g) + 1)
        for j in range(len(g)):
            g2[j] ^= g[j]                       # * 1
            g2[j + 1] ^= _gf_mul(g[j], _EXP[i])  # * alpha^i
        g = g2
    return g  # length n+1


def _rs_encode(data: List[int], n: int) -> List[int]:
    # Polynomial long division remainder over GF(256); returns n ECC codewords.
    gen = _rs_generator(n)          # length n+1
    res = [0] * n                   # remainder register (n symbols)
    for d in data:
        factor = d ^ res[0]
        res = res[1:] + [0]         # shift left by one
        # subtract factor * gen[1..n]  (gen[0] == 1 handled by the shift)
        for i in range(n):
            res[i] ^= _gf_mul(gen[i + 1], factor)
    return res


# ---- Version/EC capacity tables (versions 1..10, all four EC levels) --------
# For each version: (ec_codewords_per_block, num_blocks_group1, data_cw_group1,
# num_blocks_group2, data_cw_group2) per EC level L,M,Q,H.
# Source: QR spec Table 9 (subset, versions 1-10).
_EC_TABLE = {
    #        L                         M                         Q                         H
    1:  {"L": (7, 1, 19, 0, 0),   "M": (10, 1, 16, 0, 0),  "Q": (13, 1, 13, 0, 0),  "H": (17, 1, 9, 0, 0)},
    2:  {"L": (10, 1, 34, 0, 0),  "M": (16, 1, 28, 0, 0),  "Q": (22, 1, 22, 0, 0),  "H": (28, 1, 16, 0, 0)},
    3:  {"L": (15, 1, 55, 0, 0),  "M": (26, 1, 44, 0, 0),  "Q": (18, 2, 17, 0, 0),  "H": (22, 2, 13, 0, 0)},
    4:  {"L": (20, 1, 80, 0, 0),  "M": (18, 2, 32, 0, 0),  "Q": (26, 2, 24, 0, 0),  "H": (16, 4, 9, 0, 0)},
    5:  {"L": (26, 1, 108, 0, 0), "M": (24, 2, 43, 0, 0),  "Q": (18, 2, 15, 2, 16), "H": (22, 2, 11, 2, 12)},
    6:  {"L": (18, 2, 68, 0, 0),  "M": (16, 4, 27, 0, 0),  "Q": (24, 4, 19, 0, 0),  "H": (28, 4, 15, 0, 0)},
    7:  {"L": (20, 2, 78, 0, 0),  "M": (18, 4, 31, 0, 0),  "Q": (18, 2, 14, 4, 15), "H": (26, 4, 13, 1, 14)},
    8:  {"L": (24, 2, 97, 0, 0),  "M": (22, 2, 38, 2, 39), "Q": (22, 4, 18, 2, 19), "H": (26, 4, 14, 2, 15)},
    9:  {"L": (30, 2, 116, 0, 0), "M": (22, 3, 36, 2, 37), "Q": (20, 4, 16, 4, 17), "H": (24, 4, 12, 4, 13)},
    10: {"L": (18, 2, 68, 2, 69), "M": (26, 4, 43, 1, 44), "Q": (24, 6, 19, 2, 20), "H": (28, 6, 15, 2, 16)},
}

_ALIGN_POS = {
    1: [], 2: [6, 18], 3: [6, 22], 4: [6, 26], 5: [6, 30],
    6: [6, 34], 7: [6, 22, 38], 8: [6, 24, 42], 9: [6, 26, 46], 10: [6, 28, 50],
}


def _total_data_codewords(ver: int, ec: str) -> int:
    ecc, b1, d1, b2, d2 = _EC_TABLE[ver][ec]
    return b1 * d1 + b2 * d2


def _choose_version(nbytes: int, ec: str) -> int:
    # byte mode overhead: 4 (mode) + 8 or 16 (count) bits -> we add in bits later
    for ver in range(1, 11):
        count_bits = 8 if ver <= 9 else 16
        cap_bits = _total_data_codewords(ver, ec) * 8
        need_bits = 4 + count_bits + nbytes * 8
        if need_bits <= cap_bits:
            return ver
    raise ValueError("data too large for supported QR versions (1-10)")


def _bits_to_codewords(text: bytes, ver: int, ec: str) -> List[int]:
    count_bits = 8 if ver <= 9 else 16
    bits: List[int] = []

    def put(val, n):
        for i in range(n - 1, -1, -1):
            bits.append((val >> i) & 1)

    put(0b0100, 4)               # byte mode
    put(len(text), count_bits)   # char count
    for b in text:
        put(b, 8)
    cap = _total_data_codewords(ver, ec) * 8
    # terminator
    for _ in range(min(4, cap - len(bits))):
        bits.append(0)
    # pad to byte boundary
    while len(bits) % 8 != 0:
        bits.append(0)
    codewords = [int("".join(str(b) for b in bits[i:i + 8]), 2) for i in range(0, len(bits), 8)]
    # pad bytes
    pads = [0xEC, 0x11]
    k = 0
    total = _total_data_codewords(ver, ec)
    while len(codewords) < total:
        codewords.append(pads[k % 2])
        k += 1
    return codewords


def _interleave(codewords: List[int], ver: int, ec: str) -> List[int]:
    ecc, b1, d1, b2, d2 = _EC_TABLE[ver][ec]
    blocks = []
    idx = 0
    for _ in range(b1):
        data = codewords[idx:idx + d1]; idx += d1
        blocks.append((data, _rs_encode(data, ecc)))
    for _ in range(b2):
        data = codewords[idx:idx + d2]; idx += d2
        blocks.append((data, _rs_encode(data, ecc)))
    result = []
    maxd = max(len(b[0]) for b in blocks)
    for i in range(maxd):
        for data, _ in blocks:
            if i < len(data):
                result.append(data[i])
    for i in range(ecc):
        for _, e in blocks:
            result.append(e[i])
    return result


# ---- Matrix construction ----------------------------------------------------
def _new_matrix(size: int):
    return [[None] * size for _ in range(size)]


def _place_finder(m, r, c):
    for dr in range(-1, 8):
        for dc in range(-1, 8):
            rr, cc = r + dr, c + dc
            if 0 <= rr < len(m) and 0 <= cc < len(m):
                if 0 <= dr <= 6 and 0 <= dc <= 6:
                    v = 1 if (dr in (0, 6) or dc in (0, 6) or (2 <= dr <= 4 and 2 <= dc <= 4)) else 0
                    m[rr][cc] = v
                else:
                    m[rr][cc] = 0


def _place_function_patterns(m, ver):
    size = len(m)
    _place_finder(m, 0, 0)
    _place_finder(m, 0, size - 7)
    _place_finder(m, size - 7, 0)
    # timing
    for i in range(size):
        if m[6][i] is None:
            m[6][i] = 1 if i % 2 == 0 else 0
        if m[i][6] is None:
            m[i][6] = 1 if i % 2 == 0 else 0
    # alignment
    pos = _ALIGN_POS[ver]
    for r in pos:
        for c in pos:
            if m[r][c] is not None:
                continue
            for dr in range(-2, 3):
                for dc in range(-2, 3):
                    v = 1 if (abs(dr) == 2 or abs(dc) == 2 or (dr == 0 and dc == 0)) else 0
                    m[r + dr][c + dc] = v
    # dark module
    m[size - 8][8] = 1
    # reserve format areas (set later); mark as 0 placeholder handled in mask step


def _reserve_format(m):
    size = len(m)
    for i in range(9):
        if m[8][i] is None:
            m[8][i] = 0
        if m[i][8] is None:
            m[i][8] = 0
    for i in range(8):
        if m[8][size - 1 - i] is None:
            m[8][size - 1 - i] = 0
        if m[size - 1 - i][8] is None:
            m[size - 1 - i][8] = 0


def _data_positions(m):
    size = len(m)
    col = size - 1
    positions = []
    upward = True
    while col > 0:
        if col == 6:
            col -= 1
        rng = range(size - 1, -1, -1) if upward else range(size)
        for r in rng:
            for c in (col, col - 1):
                if m[r][c] is None:
                    positions.append((r, c))
        upward = not upward
        col -= 2
    return positions


def _mask_fn(k):
    return [
        lambda r, c: (r + c) % 2 == 0,
        lambda r, c: r % 2 == 0,
        lambda r, c: c % 3 == 0,
        lambda r, c: (r + c) % 3 == 0,
        lambda r, c: (r // 2 + c // 3) % 2 == 0,
        lambda r, c: (r * c) % 2 + (r * c) % 3 == 0,
        lambda r, c: ((r * c) % 2 + (r * c) % 3) % 2 == 0,
        lambda r, c: ((r + c) % 2 + (r * c) % 3) % 2 == 0,
    ][k]


def _penalty(m):
    size = len(m)
    score = 0
    # rule 1: runs of 5+
    for line in (m, [list(col) for col in zip(*m)]):
        for row in line:
            run = 1
            for i in range(1, size):
                if row[i] == row[i - 1]:
                    run += 1
                else:
                    if run >= 5:
                        score += 3 + (run - 5)
                    run = 1
            if run >= 5:
                score += 3 + (run - 5)
    # rule 2: 2x2 blocks
    for r in range(size - 1):
        for c in range(size - 1):
            if m[r][c] == m[r][c + 1] == m[r + 1][c] == m[r + 1][c + 1]:
                score += 3
    # rule 3: finder-like pattern
    pat1 = [1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0]
    pat2 = [0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1]
    for row in (m + [list(col) for col in zip(*m)]):
        for i in range(size - 11 + 1):
            seg = row[i:i + 11]
            if seg == pat1 or seg == pat2:
                score += 40
    # rule 4: dark ratio
    dark = sum(sum(1 for v in row if v) for row in m)
    ratio = dark * 100 // (size * size)
    score += 10 * (abs(ratio - 50) // 5)
    return score


_FORMAT_EC = {"L": 0b01, "M": 0b00, "Q": 0b11, "H": 0b10}


def _format_bits(ec, mask):
    data = (_FORMAT_EC[ec] << 3) | mask
    v = data << 10
    gen = 0b10100110111
    for i in range(14, 9, -1):
        if (v >> i) & 1:
            v ^= gen << (i - 10)
    fmt = ((data << 10) | v) ^ 0b101010000010010
    return fmt


def _apply_format(m, ec, mask):
    size = len(m)
    fmt = _format_bits(ec, mask)
    bits = [(fmt >> i) & 1 for i in range(15)]
    # positions around top-left
    coords1 = [(8, 0), (8, 1), (8, 2), (8, 3), (8, 4), (8, 5), (8, 7), (8, 8),
               (7, 8), (5, 8), (4, 8), (3, 8), (2, 8), (1, 8), (0, 8)]
    for b, (r, c) in zip(bits, coords1):
        m[r][c] = b
    # around top-right and bottom-left
    coords2 = [(size - 1, 8), (size - 2, 8), (size - 3, 8), (size - 4, 8),
               (size - 5, 8), (size - 6, 8), (size - 7, 8),
               (8, size - 8), (8, size - 7), (8, size - 6), (8, size - 5),
               (8, size - 4), (8, size - 3), (8, size - 2), (8, size - 1)]
    for b, (r, c) in zip(bits, coords2):
        m[r][c] = b


def _build(text: bytes, ec: str):
    ver = _choose_version(len(text), ec)
    data = _bits_to_codewords(text, ver, ec)
    final = _interleave(data, ver, ec)
    size = 21 + (ver - 1) * 4
    m = _new_matrix(size)
    _place_function_patterns(m, ver)
    _reserve_format(m)
    # snapshot which cells are function (non-None now)
    reserved = [[m[r][c] is not None for c in range(size)] for r in range(size)]
    positions = _data_positions(m)
    bitstream = []
    for cw in final:
        for i in range(7, -1, -1):
            bitstream.append((cw >> i) & 1)
    for (r, c), bit in zip(positions, bitstream):
        m[r][c] = bit
    # Any data positions beyond the bitstream are remainder bits -> 0.
    for (r, c) in positions[len(bitstream):]:
        if m[r][c] is None:
            m[r][c] = 0
    # try all masks
    best = None
    for k in range(8):
        cand = [row[:] for row in m]
        fn = _mask_fn(k)
        for (r, c) in positions:
            if fn(r, c):
                cand[r][c] ^= 1
        _apply_format(cand, ec, k)
        # fill any leftover None (shouldn't be) with 0
        for r in range(size):
            for c in range(size):
                if cand[r][c] is None:
                    cand[r][c] = 0
        p = _penalty(cand)
        if best is None or p < best[0]:
            best = (p, cand)
    return best[1]


def qr_svg(text: str, ec: str = "M", module_px: int = 4, quiet: int = 4) -> str:
    """Encode `text` as a QR code and return an SVG string."""
    m = _build(text.encode("utf-8"), ec)
    size = len(m)
    dim = (size + quiet * 2) * module_px
    parts = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{dim}" height="{dim}" '
        f'viewBox="0 0 {dim} {dim}" shape-rendering="crispEdges">',
        f'<rect width="{dim}" height="{dim}" fill="#ffffff"/>',
    ]
    for r in range(size):
        for c in range(size):
            if m[r][c]:
                x = (c + quiet) * module_px
                y = (r + quiet) * module_px
                parts.append(f'<rect x="{x}" y="{y}" width="{module_px}" height="{module_px}" fill="#000000"/>')
    parts.append("</svg>")
    return "".join(parts)


if __name__ == "__main__":
    import sys
    print(qr_svg(sys.argv[1] if len(sys.argv) > 1 else "sleela-ledger:test"))
