#!/usr/bin/env python3
"""Local mTLS integration test. Uses temporary certificates and loopback only."""
from __future__ import annotations

import argparse
import os
import pathlib
import socket
import ssl
import struct
import subprocess
import tempfile
import time


def run(*args: str, cwd: pathlib.Path) -> None:
    subprocess.run(args, cwd=cwd, check=True, stdout=subprocess.DEVNULL,
                   stderr=subprocess.DEVNULL)


def make_certificates(root: pathlib.Path) -> dict[str, pathlib.Path]:
    paths = {name: root / name for name in (
        "ca.key", "ca.crt", "server.key", "server.csr", "server.crt",
        "client.key", "client.csr", "client.crt", "other-client.key",
        "other-client.csr", "other-client.crt", "other-ca.key",
        "other-ca.crt", "bad-client.key", "bad-client.csr", "bad-client.crt",
        "server-ext.cnf", "client-ext.cnf",
    )}
    run("openssl", "req", "-x509", "-newkey", "rsa:2048", "-nodes",
        "-keyout", str(paths["ca.key"]), "-out", str(paths["ca.crt"]),
        "-days", "2", "-subj", "/CN=SLeeLa-Test-CA", cwd=root)
    run("openssl", "req", "-newkey", "rsa:2048", "-nodes",
        "-keyout", str(paths["server.key"]), "-out", str(paths["server.csr"]),
        "-subj", "/CN=localhost", cwd=root)
    paths["server-ext.cnf"].write_text(
        "[server]\nsubjectAltName=DNS:localhost,IP:127.0.0.1\nextendedKeyUsage=serverAuth\n",
        encoding="utf-8")
    run("openssl", "x509", "-req", "-in", str(paths["server.csr"]),
        "-CA", str(paths["ca.crt"]), "-CAkey", str(paths["ca.key"]),
        "-CAcreateserial", "-out", str(paths["server.crt"]), "-days", "2",
        "-extfile", str(paths["server-ext.cnf"]), "-extensions", "server", cwd=root)
    run("openssl", "req", "-newkey", "rsa:2048", "-nodes",
        "-keyout", str(paths["client.key"]), "-out", str(paths["client.csr"]),
        "-subj", "/CN=integration-client", cwd=root)
    paths["client-ext.cnf"].write_text("[client]\nextendedKeyUsage=clientAuth\n", encoding="utf-8")
    run("openssl", "x509", "-req", "-in", str(paths["client.csr"]),
        "-CA", str(paths["ca.crt"]), "-CAkey", str(paths["ca.key"]),
        "-CAcreateserial", "-out", str(paths["client.crt"]), "-days", "2",
        "-extfile", str(paths["client-ext.cnf"]), "-extensions", "client", cwd=root)
    run("openssl", "req", "-newkey", "rsa:2048", "-nodes",
        "-keyout", str(paths["other-client.key"]), "-out", str(paths["other-client.csr"]),
        "-subj", "/CN=not-allowlisted", cwd=root)
    run("openssl", "x509", "-req", "-in", str(paths["other-client.csr"]),
        "-CA", str(paths["ca.crt"]), "-CAkey", str(paths["ca.key"]),
        "-CAcreateserial", "-out", str(paths["other-client.crt"]), "-days", "2",
        "-extfile", str(paths["client-ext.cnf"]), "-extensions", "client", cwd=root)
    run("openssl", "req", "-x509", "-newkey", "rsa:2048", "-nodes",
        "-keyout", str(paths["other-ca.key"]), "-out", str(paths["other-ca.crt"]),
        "-days", "2", "-subj", "/CN=Untrusted-Test-CA", cwd=root)
    run("openssl", "req", "-newkey", "rsa:2048", "-nodes",
        "-keyout", str(paths["bad-client.key"]), "-out", str(paths["bad-client.csr"]),
        "-subj", "/CN=integration-client", cwd=root)
    run("openssl", "x509", "-req", "-in", str(paths["bad-client.csr"]),
        "-CA", str(paths["other-ca.crt"]), "-CAkey", str(paths["other-ca.key"]),
        "-CAcreateserial", "-out", str(paths["bad-client.crt"]), "-days", "2",
        "-extfile", str(paths["client-ext.cnf"]), "-extensions", "client", cwd=root)
    return paths


def free_port() -> int:
    with socket.socket() as sock:
        sock.bind(("127.0.0.1", 0))
        return sock.getsockname()[1]


def context(ca: pathlib.Path, cert: pathlib.Path | None = None,
            key: pathlib.Path | None = None) -> ssl.SSLContext:
    ctx = ssl.create_default_context(ssl.Purpose.SERVER_AUTH, cafile=str(ca))
    ctx.minimum_version = ssl.TLSVersion.TLSv1_3
    ctx.maximum_version = ssl.TLSVersion.TLSv1_3
    if cert and key:
        ctx.load_cert_chain(str(cert), str(key))
    return ctx


def send_denied_request(sock: ssl.SSLSocket) -> str:
    headers = b"PROTOCOL-GRADE=1\nSERVICE-ID=health\nOP-ID=ping\nREQUEST-ID=42\n"
    fixed = struct.pack("!4sBBHIIQQQ", b"SLPK", 1, 1, 0, len(headers), 0, 1, 42, 1)
    sock.sendall(fixed + headers)
    return sock.recv(256).decode("ascii", errors="replace")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("server_binary", help="path to built sleelas executable")
    args = parser.parse_args()
    binary = pathlib.Path(args.server_binary).resolve()
    if not binary.exists():
        raise SystemExit(f"server binary does not exist: {binary}")
    with tempfile.TemporaryDirectory(prefix="sleela-mtls-test-") as temp:
        root = pathlib.Path(temp)
        cert = make_certificates(root)
        port = free_port()
        proc = subprocess.Popen([
            str(binary), "--bind", "127.0.0.1", "--port", str(port),
            "--tls-cert", str(cert["server.crt"]), "--tls-key", str(cert["server.key"]),
            "--tls-client-ca", str(cert["ca.crt"]), "--allow-peer", "integration-client",
            "--authorize", "integration-client:health:other",
        ], cwd=root, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        try:
            good = context(cert["ca.crt"], cert["client.crt"], cert["client.key"])
            response = None
            last_error = None
            for _ in range(50):
                if proc.poll() is not None:
                    raise RuntimeError(f"server exited early with code {proc.returncode}")
                try:
                    with socket.create_connection(("127.0.0.1", port), timeout=0.5) as raw:
                        with good.wrap_socket(raw, server_hostname="localhost") as tls:
                            response = send_denied_request(tls)
                    break
                except (OSError, ssl.SSLError) as exc:
                    last_error = exc
                    time.sleep(0.1)
            if response is None:
                raise RuntimeError(f"server TLS startup/handshake failed: {last_error}")
            if not response.startswith("SLPK/1 403 identity_not_authorized"):
                raise AssertionError(f"expected authorization denial, got {response!r}")

            # Offer only X25519: a successful TLS 1.3 handshake proves the server
            # supports this explicitly configured ephemeral DH group.
            group_probe = subprocess.run([
                "openssl", "s_client", "-connect", f"127.0.0.1:{port}",
                "-servername", "localhost", "-tls1_3", "-groups", "X25519",
                "-CAfile", str(cert["ca.crt"]), "-cert", str(cert["client.crt"]),
                "-key", str(cert["client.key"]), "-brief",
            ], cwd=root, input="", text=True, capture_output=True, timeout=5)
            group_output = group_probe.stdout + group_probe.stderr
            if group_probe.returncode != 0 or "TLSv1.3" not in group_output:
                raise AssertionError(f"TLS 1.3 X25519-only handshake failed: {group_output}")

            def assert_rejected(client_context: ssl.SSLContext, description: str) -> None:
                if proc.poll() is not None:
                    raise RuntimeError(f"server exited before rejection test: {proc.returncode}")
                try:
                    with socket.create_connection(("127.0.0.1", port), timeout=2) as raw:
                        with client_context.wrap_socket(raw, server_hostname="localhost") as tls:
                            tls.settimeout(2)
                            try:
                                data = tls.recv(1)
                            except ssl.SSLError:
                                return
                            except socket.timeout as exc:
                                raise AssertionError(f"server did not reject {description} after TLS handshake") from exc
                            if data:
                                raise AssertionError(f"server accepted {description}")
                except (OSError, ssl.SSLError):
                    return

            assert_rejected(context(cert["ca.crt"]), "a client without a certificate")
            assert_rejected(context(cert["ca.crt"], cert["other-client.crt"], cert["other-client.key"]), "a trusted but non-allowlisted identity")
            bad = context(cert["ca.crt"], cert["bad-client.crt"], cert["bad-client.key"])
            assert_rejected(bad, "a certificate from an untrusted CA")
        finally:
            proc.terminate()
            try:
                proc.wait(timeout=5)
            except subprocess.TimeoutExpired:
                proc.kill()
                proc.wait(timeout=5)
    print("TLS integration: PASS (TLS 1.3 X25519, trusted client, unauthorized operation denied, missing/non-allowlisted/untrusted client certificates rejected)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
