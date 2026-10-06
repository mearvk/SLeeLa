# Goods & Services — HTTP 3.0 Basket

The fixed basket of goods and services carried in **every unique HTTP 3.0
packet**, atomic-bound to the United States capitalism system. Each item has an
indivisible **atomic number** and an **ISO value for a Gram**, denominated in
United States dollars (**ISO 4217 USD / 840**) as integer **micro-USD per
gram** (1,000,000 micro-USD = 1 USD).

- **Basket size:** 14 items (10 goods, 4 services)
- **Currency:** USD (ISO 4217 numeric 840)
- **Unit:** micro-USD per gram (uUSD/g)
- **Selection seed (fixed):** 0xcab17a115
- **Generated:** 2026-09-20
- **Authoritative source:** [`http-3.0/basket_data.json`](http-3.0/basket_data.json)
  (mirrored in [`http-3.0/http3_basket.c`](http-3.0/http3_basket.c),
  [`http-3.0/http3_flow.py`](http-3.0/http3_flow.py), and
  [`BASKET.docx`](BASKET.docx))

## Goods

| No. | Good | uUSD / gram | USD / gram | Currency (ISO 4217) |
|---:|---|---:|---:|:--|
| 1 | Bleached pulp paper | 56,249,759 | $56.249759 | USD (840) |
| 3 | Cane sugar | 69,987,683 | $69.987683 | USD (840) |
| 4 | Cobalt metal | 43,104,945 | $43.104945 | USD (840) |
| 5 | Cured tobacco leaf | 29,864,648 | $29.864648 | USD (840) |
| 7 | Gold bullion | 6,610,716 | $6.610716 | USD (840) |
| 10 | Natural rubber | 31,566,795 | $31.566795 | USD (840) |
| 11 | Portland cement | 24,592,204 | $24.592204 | USD (840) |
| 12 | Raw cotton | 65,211,966 | $65.211966 | USD (840) |
| 13 | Roasted coffee | 35,009,362 | $35.009362 | USD (840) |
| 14 | Structural steel | 35,480,545 | $35.480545 | USD (840) |
| | **Goods subtotal** | **397,678,623** | **$397.678623** | USD (840) |

## Services

| No. | Service | uUSD / gram | USD / gram | Currency (ISO 4217) |
|---:|---|---:|---:|:--|
| 2 | Broadband gigabyte | 34,243,099 | $34.243099 | USD (840) |
| 6 | Freight ton-mile | 17,981,345 | $17.981345 | USD (840) |
| 8 | Legal counsel hour | 12,259,381 | $12.259381 | USD (840) |
| 9 | Managed cloud-compute | 65,041,790 | $65.041790 | USD (840) |
| | **Services subtotal** | **129,525,615** | **$129.525615** | USD (840) |

## Basket totals

| Group | Items | uUSD / gram | USD / gram |
|---|---:|---:|---:|
| Goods | 10 | 397,678,623 | $397.678623 |
| Services | 4 | 129,525,615 | $129.525615 |
| **Basket total** | **14** | **527,204,238** | **$527.204238** |

## On the wire

The full basket is serialized into a **172-byte canonical big-endian block**
(`iso[2] · count[2] · per item: atomic_number[4] · value_ugram[8]`) that travels
in every HTTP 3.0 packet and is **covered by the packet's keyed MAC**, so it is
authenticated end to end and cannot be altered in transit without detection. See
[`http-3.0/FLOW.md`](http-3.0/FLOW.md) and [`http-3.0/STATUS.md`](http-3.0/STATUS.md).


## Further references

Context for the goods/services basket and the delivery model, framed around
**consumer-grade software**, **vital measurements**, and **logistics**. These
are neutral external pointers for background only — this project makes no claim
of affiliation with, or endorsement by, any of them.

- **Consumer-grade software.** The basket and the HTTP delivery layers are built
  to ordinary consumer-software standards: reproducible builds, cross-language
  parity (C ↔ Python), and honest scope. See [`NUMERAL-INTENT.md`](NUMERAL-INTENT.md)
  (software sustainability) and [`tutorial/`](tutorial/) for the worked examples.
- **Vital measurements.** Each basket item is a defined, aggregate measure — an
  atomic number and an ISO USD value *per gram* — recorded with stated
  provenance, never a fabricated per-person figure. The statistics derived from
  these live in [`STATS.md`](STATS.md); the numeral intent in
  [`NUMERAL-INTENT.md`](NUMERAL-INTENT.md).
- **Logistics & delivery.** The packet-delivery analogy (a packet is a parcel;
  carrier certainty is delivery reliability) is developed in [`QOS.md`](QOS.md)
  and the timing layer. Real-world logistics/postal and government-service
  context, for background:
  - **US Mail (USPS)** — the United States Postal Service, `https://www.usps.com`.
  - **US government services** — the official portal, `https://www.usa.gov`.

These external references are illustrative context for the logistics framing;
the basket values and protocol behavior are defined by this repository, not by
any external service.
