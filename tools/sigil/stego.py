"""
stego.py -- Deterministic, idempotent steganographic frame generator.

A "Sigil frame" is a fixed-size bitmap, by default 248 bits wide x 48 bits
tall (11,904 bits = 1,488 bytes). It looks like stochastic noise, but it is a
pure deterministic function of its input: the same input always yields the
byte-for-byte identical frame (idempotent), and the frame is unique to the
input -> output mapping.

How it works
------------
1. A seed is derived: seed = SHA-256(DOMAIN || 0x00 || input_bytes).
2. An HMAC-DRBG (HMAC-SHA-256, NIST SP 800-90A style) is instantiated from the
   seed. It produces the entire noise field -- so the "randomness" is a keyed,
   reproducible stream, not os entropy.
3. A small header is *steganographically woven* into the noise so the mapping
   is preserved and later verifiable:
       magic "SLSG" (32b) | version (8b) | width (16b) | height (16b) |
       digest = first 128 bits of SHA-256(input)
   The header bits are XORed into carrier bits at DRBG-selected positions
   (a keyed permutation), so they are indistinguishable from noise without the
   input, yet fully recoverable *with* the input.

Because both the noise and the embedding positions come from the same keyed
DRBG, verify(input, frame) can regenerate the reference frame and confirm an
exact match (idempotency check) and independently recover the embedded digest.
"""

import hashlib
import hmac
from typing import List, Tuple

DOMAIN = b"SLEELA-SIGIL-v1"
MAGIC = b"SLSG"
FORMAT_VERSION = 1
DEFAULT_WIDTH = 248
DEFAULT_HEIGHT = 48


class HmacDrbg:
    """HMAC-SHA-256 deterministic random bit generator (SP 800-90A shape)."""

    def __init__(self, seed: bytes):
        self._k = b"\x00" * 32
        self._v = b"\x01" * 32
        self._update(seed)

    def _update(self, data: bytes) -> None:
        self._k = hmac.new(self._k, self._v + b"\x00" + data, hashlib.sha256).digest()
        self._v = hmac.new(self._k, self._v, hashlib.sha256).digest()
        if data:
            self._k = hmac.new(self._k, self._v + b"\x01" + data, hashlib.sha256).digest()
            self._v = hmac.new(self._k, self._v, hashlib.sha256).digest()

    def generate(self, n: int) -> bytes:
        out = b""
        while len(out) < n:
            self._v = hmac.new(self._k, self._v, hashlib.sha256).digest()
            out += self._v
        self._update(b"")
        return out[:n]


def _seed(input_text: str) -> bytes:
    return hashlib.sha256(DOMAIN + b"\x00" + input_text.encode("utf-8")).digest()


def _bits_from_bytes(data: bytes, n_bits: int) -> List[int]:
    bits = []
    for b in data:
        for i in range(7, -1, -1):
            bits.append((b >> i) & 1)
            if len(bits) == n_bits:
                return bits
    while len(bits) < n_bits:
        bits.append(0)
    return bits


def _header_bits(input_text: str, width: int, height: int) -> List[int]:
    digest = hashlib.sha256(input_text.encode("utf-8")).digest()[:16]  # 128 bits
    header = bytearray()
    header += MAGIC                                   # 32 bits
    header.append(FORMAT_VERSION & 0xFF)              # 8 bits
    header += width.to_bytes(2, "big")                # 16 bits
    header += height.to_bytes(2, "big")               # 16 bits
    header += digest                                  # 128 bits
    return [b for byte in header for b in _bits_from_bytes(bytes([byte]), 8)]


def _permutation(drbg: HmacDrbg, total: int, k: int) -> List[int]:
    """Pick k distinct carrier indices in [0,total) via keyed Fisher-Yates draw."""
    chosen = []
    seen = set()
    while len(chosen) < k:
        raw = drbg.generate(4)
        idx = int.from_bytes(raw, "big") % total
        # linear probe to keep it deterministic and collision-free
        while idx in seen:
            idx = (idx + 1) % total
        seen.add(idx)
        chosen.append(idx)
    return chosen


def generate(input_text: str, width: int = DEFAULT_WIDTH, height: int = DEFAULT_HEIGHT) -> List[List[int]]:
    """Return the frame as a height x width grid of 0/1 ints."""
    total = width * height
    drbg = HmacDrbg(_seed(input_text))

    # 1. Noise field.
    noise = drbg.generate((total + 7) // 8)
    flat = _bits_from_bytes(noise, total)

    # 2. Header woven into keyed carrier positions (XOR so it stays noise-like).
    hbits = _header_bits(input_text, width, height)
    positions = _permutation(drbg, total, len(hbits))
    for bit, pos in zip(hbits, positions):
        flat[pos] ^= bit

    return [flat[r * width:(r + 1) * width] for r in range(height)]


def recover(input_text: str, frame: List[List[int]]) -> dict:
    """Recover the embedded header from a frame using the input as key."""
    height = len(frame)
    width = len(frame[0]) if height else 0
    total = width * height
    flat = [frame[r][c] for r in range(height) for c in range(width)]

    drbg = HmacDrbg(_seed(input_text))
    _ = drbg.generate((total + 7) // 8)  # advance past the noise field
    hlen = len(_header_bits(input_text, width, height))
    positions = _permutation(drbg, total, hlen)

    # Regenerate the noise the header was XORed onto, to subtract it back out.
    noise_drbg = HmacDrbg(_seed(input_text))
    noise = noise_drbg.generate((total + 7) // 8)
    noise_bits = _bits_from_bytes(noise, total)

    recovered_bits = [flat[pos] ^ noise_bits[pos] for pos in positions]
    by = bytearray()
    for i in range(0, len(recovered_bits), 8):
        v = 0
        for b in recovered_bits[i:i + 8]:
            v = (v << 1) | b
        by.append(v)
    magic = bytes(by[0:4])
    version = by[4]
    w = int.from_bytes(bytes(by[5:7]), "big")
    h = int.from_bytes(bytes(by[7:9]), "big")
    digest = bytes(by[9:25])
    return {
        "magic": magic,
        "version": version,
        "width": w,
        "height": h,
        "digest": digest.hex(),
        "magic_ok": magic == MAGIC,
    }


def verify(input_text: str, frame: List[List[int]]) -> Tuple[bool, dict]:
    """True iff `frame` is exactly the frame `generate(input_text)` produces
    (idempotency) and the embedded digest matches the input."""
    ref = generate(input_text, len(frame[0]) if frame else 0, len(frame))
    identical = ref == frame
    info = recover(input_text, frame)
    expected_digest = hashlib.sha256(input_text.encode("utf-8")).digest()[:16].hex()
    info["digest_ok"] = info["digest"] == expected_digest
    info["identical"] = identical
    return (identical and info["magic_ok"] and info["digest_ok"]), info


def to_ascii(frame: List[List[int]], on: str = "#", off: str = ".") -> str:
    return "\n".join("".join(on if v else off for v in row) for row in frame)
