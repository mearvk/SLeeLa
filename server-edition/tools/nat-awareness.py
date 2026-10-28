#!/usr/bin/env python3
"""Explicit, read-only STUN mapping probe. Never changes router/firewall settings."""
from __future__ import annotations

import argparse
import ipaddress
import secrets
import socket
import struct
import sys

MAGIC_COOKIE = 0x2112A442
BINDING_REQUEST = 0x0001
BINDING_SUCCESS = 0x0101
BINDING_ERROR = 0x0111
ATTR_MAPPED_ADDRESS = 0x0001
ATTR_XOR_MAPPED_ADDRESS = 0x0020


def build_request(transaction_id: bytes) -> bytes:
    if len(transaction_id) != 12:
        raise ValueError("STUN transaction ID must be 12 bytes")
    return struct.pack("!HHI", BINDING_REQUEST, 0, MAGIC_COOKIE) + transaction_id


def parse_response(data: bytes, transaction_id: bytes) -> tuple[str, int]:
    if len(data) < 20:
        raise ValueError("STUN response is shorter than its header")
    message_type, length, cookie = struct.unpack("!HHI", data[:8])
    if message_type not in (BINDING_SUCCESS, BINDING_ERROR):
        raise ValueError("unexpected STUN message type")
    if cookie != MAGIC_COOKIE or data[8:20] != transaction_id:
        raise ValueError("STUN transaction/cookie mismatch")
    if 20 + length > len(data) or length % 4:
        raise ValueError("invalid STUN attribute length")
    if message_type == BINDING_ERROR:
        raise ValueError("STUN server returned a binding error")

    offset = 20
    end = 20 + length
    mapped = None
    while offset + 4 <= end:
        attr_type, attr_length = struct.unpack("!HH", data[offset:offset + 4])
        start = offset + 4
        stop = start + attr_length
        if stop > end:
            raise ValueError("truncated STUN attribute")
        value = data[start:stop]
        if attr_type in (ATTR_XOR_MAPPED_ADDRESS, ATTR_MAPPED_ADDRESS):
            if len(value) < 8 or value[0] != 0:
                raise ValueError("unsupported STUN address attribute")
            family = value[1]
            port = struct.unpack("!H", value[2:4])[0]
            raw = value[4:]
            if attr_type == ATTR_XOR_MAPPED_ADDRESS:
                port ^= MAGIC_COOKIE >> 16
                mask = struct.pack("!I", MAGIC_COOKIE) + transaction_id
                raw = bytes(a ^ b for a, b in zip(raw, mask))
            if family == 0x01 and len(raw) == 4:
                mapped = (str(ipaddress.IPv4Address(raw)), port)
            elif family == 0x02 and len(raw) == 16:
                mapped = (str(ipaddress.IPv6Address(raw)), port)
        offset = start + ((attr_length + 3) // 4) * 4
    if mapped is None:
        raise ValueError("STUN response has no supported mapped-address attribute")
    return mapped


def discover(server: str, port: int, timeout: float) -> tuple[str, int, str]:
    transaction_id = secrets.token_bytes(12)
    request = build_request(transaction_id)
    last_error = None
    for family, socktype, proto, _, sockaddr in socket.getaddrinfo(
        server, port, type=socket.SOCK_DGRAM
    ):
        try:
            with socket.socket(family, socktype, proto) as sock:
                sock.settimeout(timeout)
                sock.connect(sockaddr)
                local_ip = sock.getsockname()[0]
                sock.send(request)
                response = sock.recv(2048)
                external_ip, external_port = parse_response(response, transaction_id)
                return local_ip, external_port, external_ip
        except (OSError, ValueError) as exc:
            last_error = exc
    raise RuntimeError(f"STUN discovery failed: {last_error}")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Read-only STUN mapping check; does not configure NAT or port forwarding."
    )
    parser.add_argument("--server", default="stun.l.google.com",
                        help="STUN server hostname (default: stun.l.google.com)")
    parser.add_argument("--port", type=int, default=19302)
    parser.add_argument("--timeout", type=float, default=3.0)
    args = parser.parse_args()
    if not 1 <= args.port <= 65535 or not 0 < args.timeout <= 30:
        parser.error("--port must be 1..65535 and --timeout must be in (0, 30]")
    try:
        local_ip, external_port, external_ip = discover(args.server, args.port, args.timeout)
        local = ipaddress.ip_address(local_ip)
        external = ipaddress.ip_address(external_ip)
        print(f"local_source_address={local_ip}")
        print(f"stun_mapped_address={external_ip}:{external_port}")
        if local != external:
            print("assessment=address_translation_or_upstream_mapping_likely")
        elif not local.is_global:
            print("assessment=non_global_local_address; upstream NAT may still be present")
        else:
            print("assessment=no_address_translation_observed_by_this_probe")
        print("note=STUN is an observation only; it does not prove inbound reachability or identify every NAT type.")
        print("note=no router settings, firewall rules, or port forwarding were changed.")
        return 0
    except (OSError, RuntimeError, ValueError) as exc:
        print(f"nat-awareness: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
