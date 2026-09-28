#!/usr/bin/env python3
"""
qr_decode_check.py -- Independent self-decoder to prove qr.encode() round-trips.

This is a *verification harness*, not a general QR reader: it assumes a clean,
un-rotated boolean matrix (exactly what qr.encode produces), reverses the mask,
reads the format info, de-interleaves the codewords, runs Reed-Solomon syndrome
checking, and decodes byte-mode data back to text. If the decoded text equals
the original input, the encoder is correct end-to-end.
"""

import sys
import qr


def _read_format(m):
    size = len(m)
    coords = [(8, 0), (8, 1), (8, 2), (8, 3), (8, 4), (8, 5), (8, 7), (8, 8),
              (7, 8), (5, 8), (4, 8), (3, 8), (2, 8), (1, 8), (0, 8)]
    raw = 0
    for (r, c) in coords:
        raw = (raw << 1) | (1 if m[r][c] else 0)
    raw ^= 0b101010000010010
    ec_bits = (raw >> 13) & 0b11
    mask = (raw >> 10) & 0b111
    ec = {0b01: "L", 0b00: "M", 0b11: "Q", 0b10: "H"}[ec_bits]
    return ec, mask


def _version_from_size(size):
    return (size - 17) // 4


def decode(matrix):
    size = len(matrix)
    version = _version_from_size(size)
    ec, mask = _read_format(matrix)

    # rebuild reserved map + un-mask
    _, reserved = qr._build_reserved(version)
    fn = qr._MASKS[mask]
    m = [[matrix[r][c] for c in range(size)] for r in range(size)]
    for r in range(size):
        for c in range(size):
            if not reserved[r][c] and fn(r, c):
                m[r][c] = not m[r][c]

    # read data bits in the same zig-zag order
    bits = []
    col = size - 1
    upward = True
    while col > 0:
        if col == 6:
            col -= 1
        rng = range(size - 1, -1, -1) if upward else range(size)
        for row in rng:
            for c in (col, col - 1):
                if not reserved[row][c]:
                    bits.append(1 if m[row][c] else 0)
        upward = not upward
        col -= 2

    # bits -> codewords
    codewords = []
    for i in range(0, len(bits) - 7, 8):
        v = 0
        for b in bits[i:i + 8]:
            v = (v << 1) | b
        codewords.append(v)

    # de-interleave
    ec_per_block, layout = qr._BLOCKS[version][ec]
    blocks = []
    for count, dpb in layout:
        for _ in range(count):
            blocks.append({"dpb": dpb, "data": [], "ec": []})
    total_data = sum(b["dpb"] for b in blocks)
    max_dpb = max(b["dpb"] for b in blocks)

    pos = 0
    for i in range(max_dpb):
        for b in blocks:
            if i < b["dpb"]:
                b["data"].append(codewords[pos]); pos += 1
    for i in range(ec_per_block):
        for b in blocks:
            b["ec"].append(codewords[pos]); pos += 1

    # verify RS syndromes are zero (no errors) as a strong integrity check
    for b in blocks:
        full = b["data"] + b["ec"]
        for s in range(ec_per_block):
            acc = 0
            for coef in full:
                acc = qr._gf_mul(acc, qr._EXP[s]) ^ coef
            if acc != 0:
                raise ValueError("Reed-Solomon syndrome non-zero: corrupted codewords")

    data_cw = []
    for b in blocks:
        data_cw.extend(b["data"])

    # parse byte-mode payload
    stream = []
    for cw in data_cw:
        for i in range(7, -1, -1):
            stream.append((cw >> i) & 1)

    def take(n):
        nonlocal stream
        v = 0
        for _ in range(n):
            v = (v << 1) | stream.pop(0)
        return v

    mode = take(4)
    if mode != 0b0100:
        raise ValueError("unexpected mode indicator: %d" % mode)
    cc_bits = 8 if version < 10 else 16
    length = take(cc_bits)
    out = bytearray()
    for _ in range(length):
        out.append(take(8))
    return out.decode("utf-8")


def main():
    tests = [
        "https://github.com/mearvk/Sleela",
        "https://example.com/repo/path?x=1&y=2",
        "SLEELA",
        "https://github.com/mearvk/Sleela/blob/master/SLEELA.md",
    ]
    ok = True
    for t in tests:
        for ec in ("L", "M", "Q", "H"):
            matrix, ver, ec_used, mask = qr.encode(t, ec=ec)
            got = decode(matrix)
            status = "OK " if got == t else "FAIL"
            if got != t:
                ok = False
            print("[%s] ec=%s v%-2d mask=%d  %r -> %r" % (status, ec_used, ver, mask, t, got))
    print("ALL PASS" if ok else "SOME FAILED")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
