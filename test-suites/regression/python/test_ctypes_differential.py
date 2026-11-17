"""Differential tests: the C core (loaded through ctypes) against Python's own
implementations of the same specifications.

Python is the witness here: `bytes.decode('utf-8')` is a strict Table 3-7
decoder, `str.encode('utf-16-le')` defines the code units, `datetime` defines
the calendar, `struct` defines the codec wire format, `re` defines the shapes.
Java's String.hashCode()/compareTo() are re-implemented from their published
definitions.  None of this shares code with the C under test.
"""
import ctypes
import random
import re
import struct
import unittest
from datetime import datetime, timedelta, timezone
from pathlib import Path

import regression_support as rs

LIB = rs.BUILD / "lib" / "libsleela_core_regression.so"
_lib = None


def load():
    """Compile the code under test into one shared object, once."""
    global _lib
    if _lib is not None:
        return _lib
    LIB.parent.mkdir(parents=True, exist_ok=True)
    objs = []
    for name in ("utf16", "java_string", "java_string_codec", "regex", "path", "time"):
        obj = rs.BUILD / "obj" / f"{name}.pic.o"
        rs.compile_object(rs.SOURCES[name], obj, flags=[*rs.LIB_C, "-fPIC"])
        objs.append(obj)
    import subprocess
    r = subprocess.run([rs.CC, "-shared", *map(str, objs), "-o", str(LIB)], capture_output=True, text=True)
    if r.returncode != 0:
        raise rs.BuildError(r.stderr)
    _lib = ctypes.CDLL(str(LIB))
    declare(_lib)
    return _lib


class JavaString(ctypes.Structure):
    _fields_ = [("units", ctypes.POINTER(ctypes.c_uint16)), ("length", ctypes.c_size_t)]


class Regex(ctypes.Structure):
    _fields_ = [("matched", ctypes.c_int), ("start", ctypes.c_size_t), ("end", ctypes.c_size_t),
                ("length", ctypes.c_size_t), ("level", ctypes.c_int)]


def declare(L):
    u16p = ctypes.POINTER(ctypes.c_uint16)
    L.sleela_utf8_utf16_length.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.POINTER(ctypes.c_int64)]
    L.sleela_utf8_to_utf16.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.POINTER(u16p), ctypes.POINTER(ctypes.c_size_t)]
    L.sleela_utf8_utf16_char_at.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_int64, u16p]
    L.sljava_string_init_empty.argtypes = [ctypes.POINTER(JavaString)]
    L.sljava_string_destroy.argtypes = [ctypes.POINTER(JavaString)]
    L.sljava_string_from_utf16.argtypes = [ctypes.POINTER(JavaString), u16p, ctypes.c_size_t]
    L.sljava_string_hash_code.argtypes = [ctypes.POINTER(JavaString), ctypes.POINTER(ctypes.c_int32)]
    L.sljava_string_compare_to.argtypes = [ctypes.POINTER(JavaString), ctypes.POINTER(JavaString), ctypes.POINTER(ctypes.c_int32)]
    L.sljava_string_encode_record.argtypes = [ctypes.POINTER(JavaString), ctypes.POINTER(ctypes.POINTER(ctypes.c_uint8)), ctypes.POINTER(ctypes.c_size_t)]
    L.sljava_string_decode_record.argtypes = [ctypes.POINTER(JavaString), ctypes.c_char_p, ctypes.c_size_t]
    L.sleela_regex_match.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(Regex)]
    L.slpath_normalize.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p]
    L.slpath_join.argtypes = [ctypes.c_char_p, ctypes.c_size_t, ctypes.c_char_p, ctypes.c_char_p]
    L.sltime_format_iso8601.argtypes = [ctypes.c_int64, ctypes.c_char_p, ctypes.c_size_t]
    L.sltime_http_date.argtypes = [ctypes.c_int64, ctypes.c_char_p, ctypes.c_size_t]
    libc = ctypes.CDLL(None)
    libc.free.argtypes = [ctypes.c_void_p]
    L._free = libc.free


# --------------------------------------------------------------- python oracles

def units_of(text: str):
    raw = text.encode("utf-16-le", "surrogatepass")
    return list(struct.unpack("<%dH" % (len(raw) // 2), raw))


def java_hash(units) -> int:
    h = 0
    for u in units:
        h = (h * 31 + u) & 0xFFFFFFFF
    return h - (1 << 32) if h >= 1 << 31 else h


def java_compare(a, b) -> int:
    for x, y in zip(a, b):
        if x != y:
            return x - y
    return len(a) - len(b)


def sign(v: int) -> int:
    return (v > 0) - (v < 0)


def make_js(L, units) -> JavaString:
    js = JavaString()
    L.sljava_string_init_empty(ctypes.byref(js))
    arr = (ctypes.c_uint16 * max(len(units), 1))(*units)
    assert L.sljava_string_from_utf16(ctypes.byref(js), arr, len(units)) == 0
    return js


def random_text(rng: random.Random, max_len: int = 24) -> str:
    out = []
    for _ in range(rng.randint(0, max_len)):
        plane = rng.choice(("ascii", "latin", "bmp", "astral", "edge"))
        if plane == "ascii":
            cp = rng.randint(0, 0x7F)
        elif plane == "latin":
            cp = rng.randint(0x80, 0x7FF)
        elif plane == "bmp":
            cp = rng.choice([c for c in (rng.randint(0x800, 0xFFFF),) if not 0xD800 <= c <= 0xDFFF] or [0x20AC])
        elif plane == "astral":
            cp = rng.randint(0x10000, 0x10FFFF)
        else:
            cp = rng.choice((0, 0x7F, 0x80, 0x7FF, 0x800, 0xD7FF, 0xE000, 0xFFFD, 0xFFFE, 0xFFFF, 0x10000, 0x10FFFF))
        out.append(chr(cp))
    return "".join(out)


class Utf16Differential(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.L = load()

    def decode(self, data: bytes):
        L = self.L
        n = ctypes.c_int64(-1)
        rc = L.sleela_utf8_utf16_length(data, len(data), ctypes.byref(n))
        return rc, n.value

    def to_units(self, data: bytes):
        L = self.L
        p = ctypes.POINTER(ctypes.c_uint16)()
        k = ctypes.c_size_t()
        rc = L.sleela_utf8_to_utf16(data, len(data), ctypes.byref(p), ctypes.byref(k))
        units = [p[i] for i in range(k.value)] if rc == 0 and p else []
        if p:
            L._free(ctypes.cast(p, ctypes.c_void_p))
        return rc, units

    def test_valid_text_matches_python_utf16(self):
        rng = random.Random(0x0DD5EED)
        for _ in range(6000):
            text = random_text(rng)
            data = text.encode("utf-8")
            want = units_of(text)
            rc, n = self.decode(data)
            self.assertEqual((rc, n), (0, len(want)), repr(text))
            rc, got = self.to_units(data)
            self.assertEqual((rc, got), (0, want), repr(text))
            for i in rng.sample(range(len(want)), min(len(want), 4)):
                c = ctypes.c_uint16()
                self.assertEqual(self.L.sleela_utf8_utf16_char_at(data, len(data), i, ctypes.byref(c)), 0)
                self.assertEqual(c.value, want[i])

    def accepts(self, data: bytes) -> bool:
        return self.decode(data)[0] == 0

    @staticmethod
    def python_accepts(data: bytes) -> bool:
        try:
            data.decode("utf-8")      # strict: rejects overlongs, surrogates, > U+10FFFF
            return True
        except UnicodeDecodeError:
            return False

    def test_all_two_byte_inputs_classified_like_python(self):
        for a in range(256):
            for b in range(256):
                data = bytes((a, b))
                self.assertEqual(self.accepts(data), self.python_accepts(data), data.hex())

    def test_all_three_byte_inputs_with_a_three_byte_lead(self):
        for a in range(0xE0, 0xF0):
            for b in range(256):
                for c in range(256):
                    data = bytes((a, b, c))
                    if self.accepts(data) != self.python_accepts(data):
                        self.fail(f"disagreement on {data.hex()}")

    def test_four_byte_leads_with_boundary_continuations(self):
        edge = (0x00, 0x7F, 0x80, 0x8F, 0x90, 0xBF, 0xC0)
        for a in range(0xF0, 0xF9):
            for b in range(256):
                for c in edge:
                    for d in edge:
                        data = bytes((a, b, c, d))
                        if self.accepts(data) != self.python_accepts(data):
                            self.fail(f"disagreement on {data.hex()}")

    def test_random_byte_soup(self):
        rng = random.Random(0xB17E50)
        pieces = [b"\xC3", b"\xA9", b"\xE2\x82", b"\xAC", b"\xF0\x9F", b"\x98\x80", b"\xED\xA0", b"\x80", b"A", b"\x00", b"\xFF"]
        for _ in range(20000):
            data = b"".join(rng.choice(pieces) for _ in range(rng.randint(0, 8)))
            self.assertEqual(self.accepts(data), self.python_accepts(data), data.hex())
            if self.python_accepts(data):
                rc, got = self.to_units(data)
                self.assertEqual(got, units_of(data.decode("utf-8")), data.hex())


class JavaStringDifferential(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.L = load()

    def test_hash_code_matches_java_definition(self):
        rng = random.Random(31)
        L = self.L
        golden = {"": 0, "hello": 99162322, "Aa": 2112, "BB": 2112, "Hello, World!": 1498789909,
                  "polygenelubricants": -2147483648}
        for text, h in golden.items():
            self.assertEqual(java_hash(units_of(text)), h)      # the oracle itself matches the JDK
        for _ in range(4000):
            u = units_of(random_text(rng))
            if rng.random() < 0.2 and u:
                u[rng.randrange(len(u))] = rng.choice((0xD800, 0xDC00, 0xDFFF, 0xFFFF))   # lone surrogates
            js = make_js(L, u)
            h = ctypes.c_int32()
            self.assertEqual(L.sljava_string_hash_code(ctypes.byref(js), ctypes.byref(h)), 0)
            self.assertEqual(h.value, java_hash(u))
            L.sljava_string_destroy(ctypes.byref(js))

    def test_compare_to_matches_java_definition(self):
        rng = random.Random(77)
        L = self.L
        for _ in range(4000):
            a = units_of(random_text(rng, 8))
            b = units_of(random_text(rng, 8)) if rng.random() < 0.6 else list(a) + units_of(random_text(rng, 3))
            ja, jb = make_js(L, a), make_js(L, b)
            o = ctypes.c_int32()
            self.assertEqual(L.sljava_string_compare_to(ctypes.byref(ja), ctypes.byref(jb), ctypes.byref(o)), 0)
            self.assertEqual(o.value, java_compare(a, b))
            L.sljava_string_destroy(ctypes.byref(ja))
            L.sljava_string_destroy(ctypes.byref(jb))

    def test_codec_wire_format_is_byte_identical(self):
        rng = random.Random(404)
        L = self.L
        for _ in range(3000):
            u = units_of(random_text(rng, 20))
            expected = b"SLJS" + bytes((1, 0, 0, 0)) + struct.pack("<I", len(u)) + struct.pack("<%dH" % len(u), *u)
            js = make_js(L, u)
            out = ctypes.POINTER(ctypes.c_uint8)()
            size = ctypes.c_size_t()
            self.assertEqual(L.sljava_string_encode_record(ctypes.byref(js), ctypes.byref(out), ctypes.byref(size)), 0)
            got = bytes(out[: size.value])
            L._free(ctypes.cast(out, ctypes.c_void_p))
            self.assertEqual(got, expected)
            back = JavaString()
            L.sljava_string_init_empty(ctypes.byref(back))
            self.assertEqual(L.sljava_string_decode_record(ctypes.byref(back), expected, len(expected)), 0)
            self.assertEqual([back.units[i] for i in range(back.length)], u)
            L.sljava_string_destroy(ctypes.byref(back))
            L.sljava_string_destroy(ctypes.byref(js))

    def test_codec_rejects_every_single_bit_flip_of_the_fixed_header(self):
        L = self.L
        good = b"SLJS" + bytes((1, 0, 0, 0)) + struct.pack("<I", 2) + struct.pack("<2H", 0x41, 0x42)
        for byte in range(8):
            for bit in range(8):
                bad = bytearray(good)
                bad[byte] ^= 1 << bit
                back = JavaString()
                L.sljava_string_init_empty(ctypes.byref(back))
                self.assertNotEqual(L.sljava_string_decode_record(ctypes.byref(back), bytes(bad), len(bad)), 0,
                                    f"byte {byte} bit {bit}")


class RegexPathDifferential(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.L = load()

    CLASSES = {"digit": r"[0-9]", "word": r"[A-Za-z0-9_]", "space": r"[ \t\n\v\f\r]", "any": r"[\s\S]"}
    QUANT = [("", "{1}"), ("+", "+"), ("*", "*"), ("{2}", "{2}"), ("{1,3}", "{1,3}"), ("{2,}", "{2,}"), ("{0}", "{0}")]

    def match(self, text: str, pattern: str, level: int):
        r = Regex()
        rc = self.L.sleela_regex_match(text.encode(), pattern.encode(), level, ctypes.byref(r))
        return rc, r

    def test_shapes_match_python_re(self):
        rng = random.Random(9)
        alphabet = "09aZ_ \t-.\n"
        for cls, rx in self.CLASSES.items():
            for sq, pq in self.QUANT:
                compiled = re.compile(rx + pq)
                for _ in range(150):
                    text = "".join(rng.choice(alphabet) for _ in range(rng.randint(0, 6)))
                    rc, r = self.match(text, f"exact {cls}{sq}", 2)
                    self.assertEqual(rc, 0)
                    self.assertEqual(bool(r.matched), compiled.fullmatch(text) is not None, (cls, sq, text))

    def test_find_family_matches_python_str(self):
        rng = random.Random(10)
        for _ in range(6000):
            text = "".join(rng.choice("ab 1") for _ in range(rng.randint(0, 9)))
            lit = "".join(rng.choice("ab 1") for _ in range(rng.randint(0, 3)))
            for kind, fn in (("contains", lambda t, l: t.find(l)),
                             ("starts", lambda t, l: 0 if t.startswith(l) else -1),
                             ("ends", lambda t, l: len(t) - len(l) if t.endswith(l) else -1)):
                rc, r = self.match(text, f"{kind} {lit}", 1)
                self.assertEqual(rc, 0)
                want = fn(text, lit)
                self.assertEqual(bool(r.matched), want >= 0, (kind, text, lit))
                if want >= 0:
                    self.assertEqual((r.start, r.end), (want, want + len(lit)))

    def test_path_normalize_matches_python_model(self):
        L = self.L
        rng = random.Random(11)
        for _ in range(8000):
            p = "".join(rng.choice("ab./\\ ") for _ in range(rng.randint(1, 14)))
            want = re.sub(r"/+", "/", p.replace("\\", "/"))
            if len(want) > 1 and want.endswith("/"):
                want = want.rstrip("/") or "/"
            buf = ctypes.create_string_buffer(64)
            self.assertEqual(L.slpath_normalize(buf, 64, p.encode()), 0)
            self.assertEqual(buf.value.decode(), want, repr(p))


class TimeDifferential(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.L = load()

    EPOCH = datetime(1970, 1, 1, tzinfo=timezone.utc)

    def fmt(self, ms: int):
        d = self.EPOCH + timedelta(milliseconds=ms)
        iso = d.strftime("%Y-%m-%dT%H:%M:%S.") + "%03dZ" % (ms % 1000)
        return iso, d.strftime("%a, %d %b %Y %H:%M:%S GMT")

    def c_fmt(self, ms: int):
        a, b = ctypes.create_string_buffer(64), ctypes.create_string_buffer(64)
        self.assertEqual(self.L.sltime_format_iso8601(ms, a, 64), 0)
        self.assertEqual(self.L.sltime_http_date(ms, b, 64), 0)
        return a.value.decode(), b.value.decode()

    def test_non_negative_instants_match_python_datetime(self):
        rng = random.Random(1970)
        for _ in range(20000):
            ms = rng.randrange(0, 4_102_444_800_000 * 2)              # 1970 .. 2270
            self.assertEqual(self.c_fmt(ms), self.fmt(ms), ms)
        for ms in (0, 999, 1000, 86_399_999, 951_782_400_000, 951_868_799_999, 4_102_444_800_000, 253_402_300_799_999):
            self.assertEqual(self.c_fmt(ms), self.fmt(ms), ms)

    def test_whole_second_instants_before_1970(self):
        rng = random.Random(1969)
        for _ in range(5000):
            ms = -1000 * rng.randrange(1, 2_208_988_800)               # back to 1900
            self.assertEqual(self.c_fmt(ms), self.fmt(ms), ms)

    def test_known_issue_fractional_instants_before_1970(self):
        """KNOWN ISSUE TIME-NEG-FLOOR: seconds truncate toward zero while the
        millisecond field is floored, so -1 ms prints as 1970-01-01T00:00:00.999Z."""
        bad = [ms for ms in (-1, -999, -1001, -86_400_001, -1_700_000_000_123) if self.c_fmt(ms) != self.fmt(ms)]
        rs.record_known_issue("TIME-NEG-FLOOR", "format_iso8601/http_date mis-format negative non-whole-second instants "
                              f"(e.g. {bad[:2]})" if bad else "appears fixed", still_open=bool(bad),
                              source="test_ctypes_differential")


if __name__ == "__main__":
    unittest.main()
