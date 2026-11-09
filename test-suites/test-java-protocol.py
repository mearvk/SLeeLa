#!/usr/bin/env python3
"""Black-box SLJIP v1 endpoint tests. Run with the connector classes directory."""
from __future__ import annotations

import os
import struct
import subprocess
import sys
import tempfile
import unittest
import zipfile
from pathlib import Path

MAGIC = b"SLJV"
VERSION = 1
MAX_FRAME = 1_048_576
PING, RESOLVE, CONSTRUCT, INVOKE_INSTANCE, INVOKE_STATIC, RELEASE, CLOSE, GET_FIELD, GET_STATIC_FIELD, SET_FIELD, SET_STATIC_FIELD = range(1, 12)


def u16(value: int) -> bytes:
    return struct.pack(">H", value)


def u32(value: int) -> bytes:
    return struct.pack(">I", value)


def u64(value: int) -> bytes:
    return struct.pack(">Q", value)


def string(value: str) -> bytes:
    raw = value.encode("utf-8")
    return u32(len(raw)) + raw


def types(*values: str) -> bytes:
    return u16(len(values)) + b"".join(string(v) for v in values)


def values(*items) -> bytes:
    encoded = bytearray(u16(len(items)))
    for item in items:
        if item is None:
            encoded += b"\x00"
        elif item is False:
            encoded += b"\x01"
        elif item is True:
            encoded += b"\x02"
        elif isinstance(item, int) and -(2**31) <= item < 2**31:
            encoded += b"\x03" + struct.pack(">i", item)
        elif isinstance(item, int):
            encoded += b"\x04" + struct.pack(">q", item)
        elif isinstance(item, float):
            encoded += b"\x05" + struct.pack(">d", item)
        elif isinstance(item, str):
            encoded += b"\x06" + string(item)
        elif isinstance(item, tuple) and item[0] == "handle":
            encoded += b"\x07" + u64(item[1])
        else:
            raise TypeError(item)
    return bytes(encoded)


def frame(request_id: int, opcode: int, payload: bytes = b"") -> bytes:
    if len(payload) > MAX_FRAME:
        raise ValueError("payload too large")
    return MAGIC + u16(VERSION) + u32(request_id) + bytes([opcode]) + u32(len(payload)) + payload


def read_string(data: bytes, offset: int):
    length = struct.unpack_from(">I", data, offset)[0]
    offset += 4
    value = data[offset:offset + length].decode("utf-8")
    return value, offset + length


CLASSES_DIR = sys.argv.pop(1) if len(sys.argv) > 1 else None
if not CLASSES_DIR:
    raise RuntimeError("usage: test-java-protocol.py CLASSES_DIR")


class EndpointTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.classes = CLASSES_DIR
        cls.external = tempfile.TemporaryDirectory(prefix="sleela-java-roots-")
        root = Path(cls.external.name)
        source_dir = root / "source"
        class_dir = root / "classes"
        package_dir = source_dir / "interop" / "fixture"
        package_dir.mkdir(parents=True)
        class_dir.mkdir()
        (package_dir / "ExternalDir.java").write_text(
            "package interop.fixture; public final class ExternalDir { "
            "public static int answer() { return 42; } }", encoding="utf-8"
        )
        (package_dir / "ExternalJar.java").write_text(
            'package interop.fixture; public final class ExternalJar { '
            'public static String answer() { return "from-jar"; } }', encoding="utf-8"
        )
        subprocess.run(
            [os.environ.get("JAVAC", "javac"), "-d", str(class_dir),
             str(package_dir / "ExternalDir.java"), str(package_dir / "ExternalJar.java")],
            check=True,
        )
        jar_path = root / "external-api.jar"
        jar_class = class_dir / "interop" / "fixture" / "ExternalJar.class"
        with zipfile.ZipFile(jar_path, "w", compression=zipfile.ZIP_STORED) as archive:
            archive.write(jar_class, "interop/fixture/ExternalJar.class")
        jar_class.unlink()
        cls.class_root = class_dir
        cls.jar_root = jar_path

    @classmethod
    def tearDownClass(cls):
        cls.external.cleanup()

    def setUp(self):
        env = os.environ.copy()
        env["SLEELA_JAVA_ALLOW_CLASSES"] = (
            "java.lang.String,java.lang.Object,java.lang.Integer,java.util.ArrayList,"
            "com.mearvk.sleela.connector.SleelaJdkProtocolFixture,"
            "interop.fixture.ExternalDir,interop.fixture.ExternalJar"
        )
        env["SLEELA_JAVA_CLASS_PATHS"] = os.pathsep.join(
            [str(self.class_root), str(self.jar_root)]
        )
        self.proc = subprocess.Popen(
            [os.environ.get("JAVA", "java"), "-cp", self.classes,
             "com.mearvk.sleela.connector.SleelaJdkProtocolServer"],
            stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            env=env,
        )
        self.next_id = 1

    def tearDown(self):
        if self.proc.poll() is None:
            self.proc.kill()
        self.proc.wait(timeout=5)

    def request(self, opcode, payload=b""):
        request_id = self.next_id
        self.next_id += 1
        self.proc.stdin.write(frame(request_id, opcode, payload))
        self.proc.stdin.flush()
        header = self.proc.stdout.read(15)
        self.assertEqual(len(header), 15, self.proc.stderr.read().decode(errors="replace"))
        self.assertEqual(header[:4], MAGIC)
        version, returned_id, returned_op, length = struct.unpack(">H I B I", header[4:])
        self.assertEqual(version, VERSION)
        self.assertEqual(returned_id, request_id)
        self.assertEqual(returned_op, opcode | 0x80)
        self.assertLessEqual(length, MAX_FRAME)
        payload = self.proc.stdout.read(length)
        self.assertEqual(len(payload), length)
        return payload

    def success(self, opcode, payload=b""):
        response = self.request(opcode, payload)
        self.assertEqual(response[0], 0)
        return response[1:]

    def error(self, opcode, payload=b""):
        response = self.request(opcode, payload)
        self.assertEqual(response[0], 1)
        class_name, offset = read_string(response, 1)
        message, offset = read_string(response, offset)
        self.assertEqual(offset, len(response))
        self.assertTrue(class_name)
        return class_name, message

    def test_ping_resolve_construct_invoke_and_release(self):
        self.assertEqual(self.success(PING), b"\x00")
        resolved = self.success(RESOLVE, string("java.lang.String"))
        self.assertEqual(resolved, b"\x06" + string("java.lang.String"))

        static_result = self.success(
            INVOKE_STATIC,
            string("java.lang.String") + string("valueOf")
            + types("int") + values(37),
        )
        self.assertEqual(static_result, b"\x06" + string("37"))

        created = self.success(
            CONSTRUCT, string("java.util.ArrayList") + types() + values()
        )
        self.assertEqual(created[0], 7)
        handle = struct.unpack_from(">Q", created, 1)[0]
        self.assertNotEqual(handle, 0)

        added = self.success(
            INVOKE_INSTANCE, u64(handle) + string("add")
            + types("java.lang.Object") + values("SLeeLa"),
        )
        self.assertEqual(added, b"\x02")

        rendered = self.success(
            INVOKE_INSTANCE, u64(handle) + string("toString") + types() + values()
        )
        self.assertEqual(rendered, b"\x06" + string("[SLeeLa]"))

        self.assertEqual(self.success(RELEASE, u64(handle)), b"\x00")
        self.assertEqual(self.error(
            INVOKE_INSTANCE, u64(handle) + string("toString") + types() + values()
        )[0], "java.lang.IllegalArgumentException")

    def test_public_instance_and_static_fields(self):
        owner = "com.mearvk.sleela.connector.SleelaJdkProtocolFixture"
        created = self.success(CONSTRUCT, string(owner) + types() + values())
        handle = struct.unpack_from(">Q", created, 1)[0]
        self.assertEqual(
            self.success(GET_FIELD, u64(handle) + string("counter")),
            b"\x03" + struct.pack(">i", 3),
        )
        self.assertEqual(
            self.success(SET_FIELD, u64(handle) + string("counter") + b"\x03" + struct.pack(">i", 41)),
            b"\x00",
        )
        self.assertEqual(
            self.success(GET_FIELD, u64(handle) + string("counter")),
            b"\x03" + struct.pack(">i", 41),
        )
        self.assertEqual(
            self.success(SET_STATIC_FIELD, string(owner) + string("sharedCounter")
                         + b"\x03" + struct.pack(">i", 99)),
            b"\x00",
        )
        self.assertEqual(
            self.success(GET_STATIC_FIELD, string(owner) + string("sharedCounter")),
            b"\x03" + struct.pack(">i", 99),
        )
        self.assertEqual(self.success(RELEASE, u64(handle)), b"\x00")

    def test_primitive_argument_conversion(self):
        owner = "com.mearvk.sleela.connector.SleelaJdkProtocolFixture"
        widened = self.success(
            INVOKE_STATIC, string(owner) + string("plusOne")
            + types("long") + values(7)
        )
        self.assertEqual(widened, b"\x04" + struct.pack(">q", 8))
        scaled = self.success(
            INVOKE_STATIC, string(owner) + string("scale")
            + types("float") + values(1.25)
        )
        self.assertEqual(scaled, b"\x05" + struct.pack(">d", 2.5))
        character = self.success(
            INVOKE_STATIC, string(owner) + string("echo")
            + types("char") + values(65)
        )
        self.assertEqual(character, b"\x03" + struct.pack(">i", 65))
        self.assertEqual(self.error(
            INVOKE_STATIC, string(owner) + string("echo")
            + types("byte") + values(300)
        )[0], "java.lang.IllegalArgumentException")

    def test_external_class_directory_and_jar_roots(self):
        directory_result = self.success(
            INVOKE_STATIC, string("interop.fixture.ExternalDir") + string("answer")
            + types() + values()
        )
        self.assertEqual(directory_result, b"\x03" + struct.pack(">i", 42))
        jar_result = self.success(
            INVOKE_STATIC, string("interop.fixture.ExternalJar") + string("answer")
            + types() + values()
        )
        self.assertEqual(jar_result, b"\x06" + string("from-jar"))

    def test_allowlist_and_structured_java_exception(self):
        self.assertEqual(self.error(RESOLVE, string("java.lang.Runtime"))[0],
                         "java.lang.ClassNotFoundException")
        self.assertEqual(self.error(
            INVOKE_STATIC, string("java.lang.Integer") + string("parseInt")
            + types("java.lang.String") + values("not-an-integer")
        )[0], "java.lang.NumberFormatException")

    def test_malformed_payload_terminates_session(self):
        self.proc.stdin.write(frame(self.next_id, PING, b"\x00"))
        self.proc.stdin.flush()
        self.proc.stdin.close()
        self.assertEqual(self.proc.stdout.read(), b"")
        self.assertNotEqual(self.proc.wait(timeout=5), 0)

    def test_close_ends_session(self):
        self.assertEqual(self.success(CLOSE), b"\x00")
        self.assertEqual(self.proc.wait(timeout=5), 0)


if __name__ == "__main__":
    unittest.main(verbosity=2)
