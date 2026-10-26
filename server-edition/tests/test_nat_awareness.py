import importlib.util
import pathlib
import struct
import unittest

SCRIPT = pathlib.Path(__file__).resolve().parents[1] / "tools" / "nat-awareness.py"
SPEC = importlib.util.spec_from_file_location("nat_awareness", SCRIPT)
nat = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(nat)


class NatAwarenessTests(unittest.TestCase):
    def test_request_has_expected_stun_header(self):
        transaction = bytes(range(12))
        request = nat.build_request(transaction)
        self.assertEqual(len(request), 20)
        self.assertEqual(struct.unpack("!HHI", request[:8]),
                         (nat.BINDING_REQUEST, 0, nat.MAGIC_COOKIE))
        self.assertEqual(request[8:], transaction)

    def test_parse_xor_mapped_ipv4(self):
        transaction = bytes(range(12))
        ip = bytes([198, 51, 100, 7])
        port = 54321
        xor_port = port ^ (nat.MAGIC_COOKIE >> 16)
        mask = struct.pack("!I", nat.MAGIC_COOKIE) + transaction
        xor_ip = bytes(a ^ b for a, b in zip(ip, mask))
        value = bytes([0, 1]) + struct.pack("!H", xor_port) + xor_ip
        attribute = struct.pack("!HH", nat.ATTR_XOR_MAPPED_ADDRESS, len(value)) + value
        response = struct.pack("!HHI", nat.BINDING_SUCCESS, len(attribute),
                               nat.MAGIC_COOKIE) + transaction + attribute
        self.assertEqual(nat.parse_response(response, transaction),
                         ("198.51.100.7", 54321))

    def test_rejects_wrong_transaction(self):
        transaction = bytes(range(12))
        response = struct.pack("!HHI", nat.BINDING_SUCCESS, 0,
                               nat.MAGIC_COOKIE) + bytes(reversed(transaction))
        with self.assertRaisesRegex(ValueError, "transaction"):
            nat.parse_response(response, transaction)

    def test_rejects_truncated_response(self):
        with self.assertRaisesRegex(ValueError, "shorter"):
            nat.parse_response(b"short", bytes(12))


if __name__ == "__main__":
    unittest.main()
