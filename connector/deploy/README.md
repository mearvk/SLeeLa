# SLeeLa HTTP 3.0 over standard HTTP — deployment

This directory carries the SLeeLa **HTTP 3.0 envelope** across the existing
internet by embedding it, byte-exact, inside standards-compliant HTTP messages.
Nothing in the H3 envelope changes: the keyed-MAC **DIGEST**, monotonic
**NONCE**, host **INTACTX**, and fixed **BASKET** all ride untouched in the HTTP
body, so the H3 integrity gate still verifies after transit.

## Pieces

| File | Role |
|---|---|
| [`../../http-3.0/http3_transport.py`](../../http-3.0/http3_transport.py) | **Custom packet builder** — wraps an H3 envelope in HTTP/1.1 (text framing; "HTTP 2.1" style) or **HTTP/2** binary frames, and extracts it back out. |
| [`../java/com/mearvk/sleela/connector/http/SleelaH3Servlet.java`](../java/com/mearvk/sleela/connector/http/SleelaH3Servlet.java) | **HTTP module** — a Jakarta `HttpServlet` that Tomcat serves over **HTTP/2+**, extracts the embedded H3 envelope from the request body, runs it through the pipeline, and returns the H3 response body. |
| [`../java/com/mearvk/sleela/connector/http/SleeuumLedger.java`](../java/com/mearvk/sleela/connector/http/SleeuumLedger.java) | **Server-side Sleeuum™ ledger** — records every packet the servlet handles (dates + numbers), served as JSON at `GET /sleela/h3/ledger`. Conforms to [`../../http-3.0/sleeuum.schema.json`](../../http-3.0/sleeuum.schema.json). |
| [`web.xml`](web.xml) | Servlet mapping at `/sleela/h3`. |
| [`tomcat-server.xml.sample`](tomcat-server.xml.sample) | Tomcat connector config enabling **HTTP/2** (h2 over TLS, and h2c cleartext). |
| [`apache-h3.conf`](apache-h3.conf) | Apache `httpd` front that terminates HTTP/2 (`mod_http2`) and reverse-proxies (`mod_proxy_http2`) to Tomcat's h2c connector. |
| [`../../scripts/install-linux-h3.sh`](../../scripts/install-linux-h3.sh) | **Linux installer** for the whole stack. |

## Embedding scheme (wire contract)

- Request/response **body** = the exact H3 envelope / response bytes.
- `Content-Type: application/vnd.sleela.h3+octet-stream; version=3`
- `X-SLeeLa-H3: 3;text` or `3;binary` — names the embedded H3 wire form.
- Default routing path `/sleela/h3` (advisory; the real service/op ids live
  inside the envelope).

Because the H3 body is opaque to HTTP intermediaries and framed by
`Content-Length` (HTTP/1.1) or a `DATA` frame (HTTP/2), proxies, load balancers,
and CDNs route it without transforming it — and a byte flip anywhere in transit
makes the H3 MAC fail closed (`BAD_DIGEST`).

## Quick start (Linux)

```bash
# 1. Install the packet builder + deploy config (+ build the WAR if you have a
#    Jakarta Servlet API jar and a JDK).
SERVLET_API_JAR=/path/to/jakarta.servlet-api-6.0.0.jar \
CATALINA_HOME=/opt/tomcat \
  ./scripts/install-linux-h3.sh

# 2. Enable HTTP/2 in Tomcat: merge the <Connector> blocks from
#    deploy/tomcat-server.xml.sample into $CATALINA_HOME/conf/server.xml, restart.

# 3. (Optional) Front with Apache for internet-facing HTTP/2:
sudo a2enmod http2 proxy proxy_http proxy_http2 ssl headers
sudo cp connector/deploy/apache-h3.conf /etc/apache2/conf-available/
sudo apachectl -k graceful
```

## Client example (Python packet builder)

```python
import http3_flow as f, http3_transport as t

key = b"\x11" * f.MAC_KEY_BYTES
env = f.Envelope(service_id=sid, op_id=oid, request_id=1, payload=b"hello", nonce=1)
env.seal(key)

http2_bytes = t.build_http2(env, key, include_preface=True)   # send over HTTP/2
# ... or ...
http11_bytes = t.build_http11(env, key)                       # send over HTTP/1.1

# On receipt, the server (or t.unwrap) extracts the H3 envelope and dispatches:
h3_wire = t.unwrap(http2_bytes)
response = pipeline.handle_wire(h3_wire)                       # -> b"H3R 0 ..."
```

## HTTP/2+ note

HTTP/2 is negotiated by the **container/front server** (Tomcat's
`Http2Protocol` upgrade + ALPN, or Apache `mod_http2`). `SleelaH3Servlet` is
transport-agnostic — it behaves identically whether the request arrived over
HTTP/1.1 or HTTP/2, because the H3 envelope is carried in the message body in
both cases. IETF HTTP/3 (QUIC) can be added the same way by fronting with an
HTTP/3-capable proxy; no code change is needed since the body contract is
transport-independent.
