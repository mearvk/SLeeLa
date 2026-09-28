#!/usr/bin/env python3
"""
sigil.py -- Sleela Sigil generator (dependency-free, pure standard library).

Produces two artifacts from a single configurable input URL:

  1. A QR code encoding the URL (PNG + ASCII), configurable to point anywhere.
     Default target: the GitHub repository the source was developed on.
  2. A deterministic 248 x 48 steganographic "Sigil frame" (PNG + ASCII) that is
     idempotent: the same URL always yields the byte-for-byte identical frame,
     and the frame embeds a recoverable digest of the URL (the input->output
     mapping is preserved and verifiable).

Configuration precedence for the URL (highest first):
  1. --url CLI argument
  2. SLEELA_QR_URL environment variable
  3. "url" field in a config file (--config, default: sigil.config.json next to
     this script, if present)
  4. Built-in default: https://github.com/mearvk/Sleela

Usage:
  python3 sigil.py                      # generate with the resolved URL
  python3 sigil.py --url https://...    # override the target
  python3 sigil.py --out-dir build      # choose output directory
  python3 sigil.py --verify             # regenerate + verify idempotency
  python3 sigil.py --print-url          # just print the resolved URL
"""

import argparse
import json
import os
import sys

import qr
import png
import stego

DEFAULT_URL = "https://github.com/mearvk/Sleela"
HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_CONFIG = os.path.join(HERE, "sigil.config.json")


def resolve_url(cli_url, config_path):
    if cli_url:
        return cli_url, "cli"
    env = os.environ.get("SLEELA_QR_URL")
    if env:
        return env, "env:SLEELA_QR_URL"
    path = config_path or DEFAULT_CONFIG
    if path and os.path.exists(path):
        try:
            with open(path, "r", encoding="utf-8") as f:
                cfg = json.load(f)
            if isinstance(cfg, dict) and cfg.get("url"):
                return cfg["url"], "config:%s" % path
        except (ValueError, OSError):
            pass
    return DEFAULT_URL, "default"


def gen_qr(url, out_dir, ec, scale):
    matrix, version, ec_used, mask = qr.encode(url, ec=ec)
    png_path = os.path.join(out_dir, "sleela-qr.png")
    txt_path = os.path.join(out_dir, "sleela-qr.txt")
    png.write_bitmap(png_path, matrix, scale=scale, quiet=4)
    with open(txt_path, "w", encoding="utf-8") as f:
        for row in matrix:
            f.write("".join("##" if c else "  " for c in row) + "\n")
    return {
        "png": png_path, "txt": txt_path, "version": version,
        "ec": ec_used, "mask": mask, "modules": len(matrix),
    }


def gen_frame(url, out_dir, width, height, scale):
    frame = stego.generate(url, width, height)
    png_path = os.path.join(out_dir, "sleela-sigil-frame.png")
    txt_path = os.path.join(out_dir, "sleela-sigil-frame.txt")
    rows = []
    for _ in range(height * scale):
        rows.append([0] * (width * scale))
    for y in range(height):
        for x in range(width):
            val = 0 if frame[y][x] else 255
            for dy in range(scale):
                for dx in range(scale):
                    rows[y * scale + dy][x * scale + dx] = val
    png.write_gray(png_path, rows)
    with open(txt_path, "w", encoding="utf-8") as f:
        f.write(stego.to_ascii(frame) + "\n")
    ok, info = stego.verify(url, frame)
    return {"png": png_path, "txt": txt_path, "width": width, "height": height,
            "verified": ok, "digest": info["digest"]}


def main(argv=None):
    p = argparse.ArgumentParser(description="Sleela Sigil generator (QR + stego frame).")
    p.add_argument("--url", help="target URL to encode (overrides env/config/default)")
    p.add_argument("--config", help="path to a JSON config file with a 'url' field")
    p.add_argument("--out-dir", default=os.path.join(HERE, "out"), help="output directory")
    p.add_argument("--ec", default="M", choices=["L", "M", "Q", "H"], help="QR error correction")
    p.add_argument("--qr-scale", type=int, default=8, help="QR pixel scale")
    p.add_argument("--frame-width", type=int, default=stego.DEFAULT_WIDTH, help="stego frame width in bits")
    p.add_argument("--frame-height", type=int, default=stego.DEFAULT_HEIGHT, help="stego frame height in bits")
    p.add_argument("--frame-scale", type=int, default=4, help="stego frame pixel scale")
    p.add_argument("--print-url", action="store_true", help="print the resolved URL and exit")
    p.add_argument("--verify", action="store_true", help="regenerate the frame and verify idempotency")
    args = p.parse_args(argv)

    url, source = resolve_url(args.url, args.config)

    if args.print_url:
        print(url)
        return 0

    if args.verify:
        frame = stego.generate(url, args.frame_width, args.frame_height)
        again = stego.generate(url, args.frame_width, args.frame_height)
        ok, info = stego.verify(url, frame)
        idem = frame == again
        print("url:        %s (%s)" % (url, source))
        print("idempotent: %s" % idem)
        print("verified:   %s" % ok)
        print("recovered:  magic=%s version=%s size=%dx%d digest=%s"
              % (info["magic"].decode("latin1"), info["version"],
                 info["width"], info["height"], info["digest"]))
        return 0 if (ok and idem) else 1

    os.makedirs(args.out_dir, exist_ok=True)
    q = gen_qr(url, args.out_dir, args.ec, args.qr_scale)
    fr = gen_frame(url, args.out_dir, args.frame_width, args.frame_height, args.frame_scale)

    print("Sleela Sigil")
    print("  url:    %s  (source: %s)" % (url, source))
    print("  QR:     %s  (v%d, ec=%s, mask=%d, %dx%d modules)"
          % (q["png"], q["version"], q["ec"], q["mask"], q["modules"], q["modules"]))
    print("          %s" % q["txt"])
    print("  Frame:  %s  (%dx%d bits, verified=%s)"
          % (fr["png"], fr["width"], fr["height"], fr["verified"]))
    print("          %s" % fr["txt"])
    print("  digest: %s" % fr["digest"])
    return 0


if __name__ == "__main__":
    sys.exit(main())
