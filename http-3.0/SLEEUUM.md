# Sleeuum™ — packet ledger client for SLeeLa executables

**Sleeuum™** ([`http-3.0/sleeuum.py`](sleeuum.py)) is the client-side module a
SLeeLa executable uses to **keep track of the HTTP 3.0 / HTTP 2.0+ packets** it
sends and receives — along with their **dates** and their **numbers**. It rides
alongside the packet builder ([`http3_transport.py`](http3_transport.py)) and the
H3 pipeline ([`http3_flow.py`](http3_flow.py)); it observes bytes those layers
already produce and never alters a packet or an integrity field.

## What it tracks

| Category | Fields |
|---|---|
| **Packets** | carrier (`http/1.1`, `http/2`, `h3-raw`), direction (`sent`/`received`), kind (`request`/`response`), wire form (`text`/`binary`), service-id, op-id |
| **Dates** | ISO-8601 UTC timestamp per packet (`at`), session `first_seen`/`last_seen`, and `duration_seconds` |
| **Numbers** | Sleeuum `seq` (monotonic), H3 `nonce`, `digest`, `intactx`, `request_id`, `status`, per-status tallies, payload/wire byte counts, and the **basket numerals** (atomic-number item ids, per [`../NUMERAL.md`](../NUMERAL.md)) carried in each packet |

## Usage

```python
import http3_flow as f, http3_transport as t
from sleeuum import Sleeuum

key = b"\x11" * f.MAC_KEY_BYTES
led = Sleeuum(name="my-exe")

env = f.Envelope(service_id=sid, op_id=oid, request_id=1, payload=b"hi", nonce=1)
env.seal(key)

wire = t.build_http2(env, key, include_preface=True)   # HTTP/2 carrier
led.track_sent(env, carrier=t.Carrier.HTTP2, wire=wire)

response_wire = pipeline.handle_wire(t.unwrap(wire))
led.track_received_wire(response_wire, carrier="http/2")

print(led.report())         # one-line summary (renders the Sleeuum™ mark)
open("ledger.json", "w").write(led.to_json())   # full dates + numbers + packets
```

## JSON Schema

The ledger JSON output has a formal schema:
[`sleeuum.schema.json`](sleeuum.schema.json) (JSON Schema draft 2020-12). It
pins the `Sleeuum™` trademark constants, the permitted carriers
(`http/1.1`, `http/2`, `h3-raw`), the H3 status-name enum, and — via `if/then`
rules — the fields required per packet `kind` (request packets carry
service/op/request-id + NONCE + DIGEST; response packets carry status +
status_name). Both the Python client ledger and the Java server ledger emit
JSON that validates against it.

## Server-side ledger (Java)

The HTTP module keeps its own ledger too:
[`SleeuumLedger.java`](../connector/java/com/mearvk/sleela/connector/http/SleeuumLedger.java),
wired into [`SleelaH3Servlet`](../connector/java/com/mearvk/sleela/connector/http/SleelaH3Servlet.java).
Every packet the servlet handles is recorded — the received request and the
sent response — with the same dates and numbers as the client module, parsed
directly from the H3 wire bytes (service/op ids, request-id, NONCE, DIGEST,
INTACTX, status, and the basket numerals). Its JSON conforms to the same
`sleeuum.schema.json`.

Retrieve the server ledger over HTTP:

```
GET /sleela/h3/ledger      -> application/json  (the Sleeuum™ ledger)
GET /sleela/h3             -> text/plain health probe (includes tracked count)
```

## Notes

- **Carriers:** HTTP 3.0 envelopes carried over HTTP/1.1 or HTTP/2 (2.0+), or
  observed as raw H3. The ledger extracts the embedded H3 bytes automatically
  when handed a full HTTP message.
- **Honest by design:** the H3 status is recorded exactly as the pipeline
  returned it (`OK`, `BAD_DIGEST`, `TAMPERED`, `REPLAYED`, …); Sleeuum™ measures,
  it does not adjudicate.
- **Trademark:** the module renders its name as `Sleeuum™` (unicode) with an
  ASCII fallback `Sleeuum(TM)`.

Run the built-in self-demo (no network required):

```bash
cd http-3.0 && python3 sleeuum.py
```
