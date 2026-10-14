#!/usr/bin/env python3
"""Minimal P6 PPM -> PNG converter (stdlib only: zlib, struct)."""
import sys, zlib, struct

def read_ppm(path):
    with open(path, "rb") as f:
        data = f.read()
    assert data[:2] == b"P6", "not a P6 PPM"
    # parse header: magic, width, height, maxval (whitespace separated)
    idx = 2
    vals = []
    while len(vals) < 3:
        while idx < len(data) and data[idx] in b" \t\r\n":
            idx += 1
        if data[idx:idx+1] == b"#":
            while data[idx] not in b"\r\n":
                idx += 1
            continue
        start = idx
        while data[idx] not in b" \t\r\n":
            idx += 1
        vals.append(int(data[start:idx]))
    w, h, _maxv = vals
    idx += 1  # single whitespace after maxval
    pix = data[idx:idx + w * h * 3]
    return w, h, pix

def write_png(path, w, h, rgb):
    def chunk(typ, body):
        c = typ + body
        return struct.pack(">I", len(body)) + c + struct.pack(">I", zlib.crc32(c) & 0xffffffff)
    raw = bytearray()
    stride = w * 3
    for y in range(h):
        raw.append(0)  # filter type 0
        raw += rgb[y*stride:(y+1)*stride]
    png = b"\x89PNG\r\n\x1a\n"
    png += chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(bytes(raw), 9))
    png += chunk(b"IEND", b"")
    with open(path, "wb") as f:
        f.write(png)

if __name__ == "__main__":
    src = sys.argv[1]
    dst = sys.argv[2] if len(sys.argv) > 2 else src.rsplit(".", 1)[0] + ".png"
    w, h, pix = read_ppm(src)
    write_png(dst, w, h, pix)
    print(f"wrote {dst} ({w}x{h})")
