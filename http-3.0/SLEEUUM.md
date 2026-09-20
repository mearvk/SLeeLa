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
