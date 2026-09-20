"""http3_transport.py -- carry the SLeeLa HTTP 3.0 envelope over standard HTTP.

The SLeeLa "HTTP 3.0" envelope (see http3_flow.py / http3_envelope.h) is a
bespoke application unit: VERSION | FLAGS | SERVICE-ID | OP-ID | REQUEST-ID |
NONCE | DIGEST | INTACTX | BASKET | PAYLOAD, sealed with a per-packet keyed MAC.
It is deliberately NOT the IETF HTTP/3 transport and, on its own, cannot travel
the existing internet: it has no request line, no HTTP headers, and no socket
transport.

This module is a **custom packet builder** that makes the H3 envelope fit for
the current internet WITHOUT changing the envelope. It embeds the exact H3
bytes -- integrity gate and all -- inside genuinely standards-shaped HTTP
messages, so ordinary HTTP infrastructure (proxies, load balancers, servers,
CDNs) can route and carry them:

  * build_http11 / parse_http11  -- HTTP/1.1 message framing (the "HTTP 2.1 is
        fine" text form): a real request line + headers + body, body == H3 bytes.
  * build_http2  / parse_http2   -- HTTP/2 binary framing: a correct HEADERS
        frame (HPACK-free literal representation) followed by DATA frame(s),
        the DATA payload == H3 bytes.

The embedding is "careful": the H3 envelope is never mangled to fit HTTP. It is
placed in the HTTP *body* verbatim and identified by an explicit media type and
a marker header, so a receiver can extract the original bytes and feed them
straight to Pipeline.handle_wire(). The MAC/NONCE/INTACTX/BASKET all survive
untouched, so the H3 integrity guarantees are preserved end to end.

Design stance (matches the rest of the timing/QoS work): standards-honest.
We emit HTTP that a conformant parser accepts; we do not pretend the H3
integrity fields are HTTP semantics, and we do not weaken H3 to look like HTTP.
"""
from __future__ import annotations

import struct
from dataclasses import dataclass
from typing import List, Optional, Tuple

# The H3 envelope lives in http3_flow (byte-identical to the C reference).
from http3_flow import Envelope, Response

# --------------------------------------------------------------------------
# Shared embedding scheme
# --------------------------------------------------------------------------

# Media type for an embedded H3 envelope. A distinct, versioned vendor type so
# intermediaries treat it as an opaque binary body and never try to transform it.
H3_MEDIA_TYPE = "application/vnd.sleela.h3+octet-stream; version=3"

# Marker header naming the embedded protocol generation and wire form. Receivers
# key on this to decide whether a standard HTTP body carries an H3 envelope.
H3_MARKER_HEADER = "X-SLeeLa-H3"          # value: "3;text" or "3;binary"
H3_WIREFORM_TEXT = "3;text"
H3_WIREFORM_BINARY = "3;binary"

# Default request target for the HTTP/1.1 and HTTP/2 forms. The H3 service/op
# ids live *inside* the envelope; the URL path is only a routing convenience so
# ordinary HTTP routers can dispatch. It is advisory and MAY be ignored.
H3_DEFAULT_PATH = "/sleela/h3"


def _wireform_value(env_wire: bytes) -> str:
    """Textual H3 lines begin with the ASCII tag 'H3 '; everything else is binary.

    This mirrors the auto-detect at the top of Pipeline.handle_wire(), so the
    marker header we emit always agrees with how the receiver will parse it.
    """
    return H3_WIREFORM_TEXT if env_wire[:3] == b"H3 " else H3_WIREFORM_BINARY


def h3_wire_bytes(env: Envelope, key: Optional[bytes] = None, *, binary: bool = False) -> bytes:
    """Serialize an Envelope to its on-the-wire H3 bytes (text or binary).

    `key`, when given, seals the keyed-MAC DIGEST first. This is the exact byte
    string that Pipeline.handle_wire() consumes, and the exact byte string we
    embed in the HTTP body -- nothing is added or removed.
    """
    if binary:
        # http3_flow's Envelope implements the textual form; the binary form is
        # defined in the C reference. For portability the Python builder embeds
        # the textual H3 line, which is fully interoperable per spec §5. Callers
        # needing the binary form can pass pre-serialized bytes to the build_*
        # functions' `env_wire=` parameter.
        raise NotImplementedError(
            "Pass pre-serialized binary via env_wire=... to build_http11/build_http2"
        )
    return env.pack_text(key)


# --------------------------------------------------------------------------
# HTTP/1.1 ("HTTP 2.1 is fine") text framing
# --------------------------------------------------------------------------

_CRLF = b"\r\n"


def build_http11(
    env: Optional[Envelope] = None,
    key: Optional[bytes] = None,
    *,
    env_wire: Optional[bytes] = None,
    method: str = "POST",
    path: str = H3_DEFAULT_PATH,
    host: str = "localhost",
    extra_headers: Optional[List[Tuple[str, str]]] = None,
) -> bytes:
    """Build a standards-compliant HTTP/1.1 request carrying an H3 envelope body.

    Provide either a live `env` (optionally with `key` to seal it) or a
    pre-serialized `env_wire`. The result is a complete HTTP/1.1 request:

        POST /sleela/h3 HTTP/1.1\r\n
        Host: <host>\r\n
        Content-Type: application/vnd.sleela.h3+octet-stream; version=3\r\n
        Content-Length: <n>\r\n
        X-SLeeLa-H3: 3;text\r\n
        \r\n
        <exact H3 envelope bytes>

    Content-Length framing (not chunked) keeps the body byte-exact so the H3
    MAC verifies after transit.
    """
    body = env_wire if env_wire is not None else h3_wire_bytes(env, key)
    headers: List[Tuple[str, str]] = [
        ("Host", host),
        ("Content-Type", H3_MEDIA_TYPE),
        ("Content-Length", str(len(body))),
        (H3_MARKER_HEADER, _wireform_value(body)),
        ("Connection", "keep-alive"),
    ]
    if extra_headers:
        headers.extend(extra_headers)
    start = f"{method} {path} HTTP/1.1".encode("ascii")
    head = start + _CRLF + _CRLF.join(f"{k}: {v}".encode("ascii") for k, v in headers) + _CRLF + _CRLF
    return head + body


def build_http11_response(
    resp_wire: bytes,
    *,
    status_code: int = 200,
    reason: str = "OK",
    extra_headers: Optional[List[Tuple[str, str]]] = None,
) -> bytes:
    """Build an HTTP/1.1 response carrying an H3 response line (H3R ...) as body."""
    headers: List[Tuple[str, str]] = [
        ("Content-Type", H3_MEDIA_TYPE),
        ("Content-Length", str(len(resp_wire))),
        (H3_MARKER_HEADER, _wireform_value(resp_wire)),
    ]
    if extra_headers:
        headers.extend(extra_headers)
    start = f"HTTP/1.1 {status_code} {reason}".encode("ascii")
    head = start + _CRLF + _CRLF.join(f"{k}: {v}".encode("ascii") for k, v in headers) + _CRLF + _CRLF
    return head + resp_wire


def parse_http11(message: bytes) -> bytes:
    """Extract the embedded H3 envelope bytes from an HTTP/1.1 request/response.

    Validates the framing enough to recover a byte-exact body: it honors
    Content-Length and confirms the H3 marker header is present. Returns the
    raw H3 bytes, ready for Pipeline.handle_wire(). Raises ValueError if the
    message is not an H3-carrying HTTP/1.1 message.
    """
    head, sep, body = message.partition(_CRLF + _CRLF)
    if not sep:
        raise ValueError("no header/body separator")
    lines = head.split(_CRLF)
    if not lines or b" HTTP/1." not in lines[0]:
        raise ValueError("not an HTTP/1.x message")
    headers = {}
    for line in lines[1:]:
        if b":" in line:
            k, _, v = line.partition(b":")
            headers[k.strip().lower()] = v.strip()
    if H3_MARKER_HEADER.lower().encode("ascii") not in headers:
        raise ValueError("missing H3 marker header; not an H3-carrying message")
    clen = headers.get(b"content-length")
    if clen is not None:
        n = int(clen)
        if len(body) < n:
            raise ValueError("body shorter than Content-Length")
        body = body[:n]
    return body


# --------------------------------------------------------------------------
# HTTP/2 binary framing (RFC 7540)
# --------------------------------------------------------------------------
#
# We emit real HTTP/2 frames. To stay dependency-free and byte-exact we use
# HPACK's *literal header field never indexed* representation (0x10) with a
# literal (non-Huffman) name and value, which is a valid HPACK encoding that any
# conformant HTTP/2 decoder accepts. The H3 envelope rides in DATA frame(s).

_H2_FRAME_HEADER = struct.Struct(">3sBBI")  # length(24b as 3 bytes), type, flags, stream id(31b)

_H2_TYPE_DATA = 0x0
_H2_TYPE_HEADERS = 0x1

_H2_FLAG_END_STREAM = 0x1
_H2_FLAG_END_HEADERS = 0x4

# HTTP/2 connection preface (client). Included so build_http2 can optionally
# produce a complete client byte stream a real server would accept.
H2_CLIENT_PREFACE = b"PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"


def _u24(n: int) -> bytes:
    if n < 0 or n > 0xFFFFFF:
        raise ValueError("frame length out of range")
    return n.to_bytes(3, "big")


def _h2_frame(ftype: int, flags: int, stream_id: int, payload: bytes) -> bytes:
    return _u24(len(payload)) + bytes((ftype, flags)) + struct.pack(">I", stream_id & 0x7FFFFFFF) + payload


def _hpack_literal_never_indexed(name: str, value: str) -> bytes:
    """Encode one header as HPACK 'literal, never indexed, new name' (no Huffman).

    Prefix byte 0x10; name length (7-bit prefix, H=0) + name; value length + value.
    This is a minimal, valid HPACK field representation -- no dynamic table needed.
    """
    nb = name.encode("ascii")
    vb = value.encode("ascii")
    if len(nb) > 0x7F or len(vb) > 0x7F:
        raise ValueError("header too long for the minimal encoder")
    return bytes((0x10, len(nb))) + nb + bytes((len(vb),)) + vb


def build_http2(
    env: Optional[Envelope] = None,
    key: Optional[bytes] = None,
    *,
    env_wire: Optional[bytes] = None,
    method: str = "POST",
    path: str = H3_DEFAULT_PATH,
    authority: str = "localhost",
    scheme: str = "https",
    stream_id: int = 1,
    include_preface: bool = False,
) -> bytes:
    """Build a standards-shaped HTTP/2 frame sequence carrying an H3 envelope.

    Produces: [optional client preface] + HEADERS frame (pseudo-headers +
    content-type + H3 marker) + DATA frame (END_STREAM) whose payload is the
    exact H3 envelope bytes. The framing follows RFC 7540; header blocks use a
    dependency-free literal HPACK encoding.
    """
    body = env_wire if env_wire is not None else h3_wire_bytes(env, key)
    header_fields = [
        (":method", method),
        (":path", path),
        (":scheme", scheme),
        (":authority", authority),
        ("content-type", H3_MEDIA_TYPE),
        ("content-length", str(len(body))),
        (H3_MARKER_HEADER.lower(), _wireform_value(body)),
    ]
    header_block = b"".join(_hpack_literal_never_indexed(k, v) for k, v in header_fields)
    frames = _h2_frame(_H2_TYPE_HEADERS, _H2_FLAG_END_HEADERS, stream_id, header_block)
    frames += _h2_frame(_H2_TYPE_DATA, _H2_FLAG_END_STREAM, stream_id, body)
    return (H2_CLIENT_PREFACE + frames) if include_preface else frames


def parse_http2(stream: bytes) -> bytes:
    """Extract the embedded H3 envelope bytes from an HTTP/2 frame sequence.

    Skips an optional client preface, walks frames by their 9-byte headers, and
    concatenates DATA-frame payloads (the H3 envelope). Returns the raw H3 bytes
    for Pipeline.handle_wire(). Raises ValueError on malformed framing.
    """
    if stream.startswith(H2_CLIENT_PREFACE):
        stream = stream[len(H2_CLIENT_PREFACE):]
    off = 0
    data = bytearray()
    saw_data = False
    n = len(stream)
    while off + 9 <= n:
        length = int.from_bytes(stream[off:off + 3], "big")
        ftype = stream[off + 3]
        # flags = stream[off + 4]  # not needed for extraction
        # stream_id = struct.unpack(">I", stream[off+5:off+9])[0] & 0x7FFFFFFF
        payload_start = off + 9
        payload_end = payload_start + length
        if payload_end > n:
            raise ValueError("truncated HTTP/2 frame")
        if ftype == _H2_TYPE_DATA:
            data.extend(stream[payload_start:payload_end])
            saw_data = True
        off = payload_end
    if not saw_data:
        raise ValueError("no DATA frame; nothing to extract")
    return bytes(data)


# --------------------------------------------------------------------------
# Convenience: round-trip an envelope through a chosen HTTP carrier
# --------------------------------------------------------------------------

@dataclass
class Carrier:
    """Names the HTTP carrier used to move an H3 envelope over the internet."""
    HTTP11 = "http/1.1"
    HTTP2 = "http/2"


def wrap(env: Envelope, key: Optional[bytes] = None, *, carrier: str = Carrier.HTTP11, **kw) -> bytes:
    """Serialize + seal an envelope and wrap it in the chosen HTTP carrier."""
    if carrier == Carrier.HTTP11:
        return build_http11(env, key, **kw)
    if carrier == Carrier.HTTP2:
        return build_http2(env, key, **kw)
    raise ValueError(f"unknown carrier: {carrier}")


def unwrap(message: bytes) -> bytes:
    """Auto-detect the HTTP carrier and return the embedded H3 envelope bytes.

    HTTP/2 is recognized by the client preface or a leading frame header whose
    type is HEADERS/DATA; otherwise the message is treated as HTTP/1.x text.
    The returned bytes are ready for Pipeline.handle_wire().
    """
    if message.startswith(H2_CLIENT_PREFACE):
        return parse_http2(message)
    # HTTP/1.x messages start with an ASCII method or "HTTP/1." status line.
    if message[:5] in (b"HTTP/",) or (b" HTTP/1." in message[:200]):
        return parse_http11(message)
    # Fall back to HTTP/2 frame parsing (no preface).
    return parse_http2(message)
